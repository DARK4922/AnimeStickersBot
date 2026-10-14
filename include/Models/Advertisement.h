#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <string>

#include "TelegramApi.h"
#include "Users.h"

class Advertisement
{
public:
	struct AdvertisingPost
	{
		std::string message{};
		std::string photoId{};
	};

private:
	AdvertisingPost advertisingPost_;
	TelegramApi& tgApi_;
	Users& users_;

public:
	Advertisement(
		TelegramApi& TgApi,
		Users& users
	);

	AdvertisingPost& advertisingPost();

	// Предпоказ поста
	void previewPost(int64_t chatId);

	// Отправка рекламы
	bool sendAdvertisement(int64_t userId);

	// Рассылка рекламы
	void broadcastAdvertisement(int64_t chatId);
};

#endif // !ADVERTISEMENT_H
