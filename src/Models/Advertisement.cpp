#include "Advertisement.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

#include "MessageText.h"

using ReplyKeyboard = TelegramApi::ReplyKeyboard;
using InlineKeyboard = TelegramApi::InlineKeyboard;

Advertisement::Advertisement(
	TelegramApi& TgApi,
	Users& users
)
	: tgApi_(TgApi)
	, users_(users)
{}

Advertisement::AdvertisingPost& Advertisement::advertisingPost()
{
	return advertisingPost_;
}

void Advertisement::previewPost(int64_t chatId)
{
	if (advertisingPost_.message.empty())
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Moderator::messageIsEmpty
			}
		);
		return;
	}

	// Отправка с фото
	if (!advertisingPost_.photoId.empty())
	{
		tgApi_.sendPhotoId(
			{
				chatId,
				advertisingPost_.message
			},
			advertisingPost_.photoId
		);
	}
	// Отправка без фото
	else
	{
		tgApi_.sendMessage(
			{
				chatId,
				advertisingPost_.message
			}
		);
	}
}

void Advertisement::broadcastAdvertisement(int64_t chatId)
{
	// Копирование id пользователей
	std::vector<int64_t> usersId{ users_.allIds() };
	std::vector<int64_t> deleteUsersId;

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Administrator::mailingListHasStarted
		}
	);

	// Ограничение количества одновременно работающих потоков
	const size_t MAX_THREADS{ 20 };	// Можно увеличить до 30

	std::vector<std::thread> threads;
	std::mutex usersMutex;

	for (int64_t userId : usersId)
	{
		if (threads.size() >= MAX_THREADS)
		{
			for (auto& t : threads)
			{
				if (t.joinable()) { t.join(); }
			}
			threads.clear();
		}

		threads.emplace_back([this, userId, &deleteUsersId, &usersMutex]()
			{
				bool result = sendAdvertisement(userId);
				if (!result)
				{
					std::lock_guard<std::mutex> lock(usersMutex);
					deleteUsersId.push_back(userId);
				}
			});
	}

	for (auto& t : threads)
	{
		if (t.joinable()) { t.join(); }
	}

	size_t deleteUsers{ deleteUsersId.size() };
	size_t sentMessages{ usersId.size() - deleteUsers };

	for (int64_t id : deleteUsersId) { users_.deleteUser(id); }

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Administrator::broadcastAdvertisement(
				sentMessages,
				deleteUsers
			)
		}
	);
}

bool Advertisement::sendAdvertisement(int64_t userId)
{
	// Отправка с фото
	if (!advertisingPost_.photoId.empty())
	{
		bool result = tgApi_.sendPhotoIdWithBlockCheck(
			{
				userId,
				advertisingPost_.message
			},
			advertisingPost_.photoId
		);

		if (result) { return true; }
	}
	/*
	* Если не фото не добавлено или не удалось отправить с ним,
	* то пробуем отправить без фото
	*/
	bool result = tgApi_.sendMessageWithBlockCheck(
		{
			userId,
			advertisingPost_.message
		}
	);

	return result;
}