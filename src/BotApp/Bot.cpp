#include "Bot.h"

#include <tgbot/tgbot.h>

#include "ThreadPool.h"
#include "Users.h"
#include "Context.h"
#include "StickerDatabase.h"
#include "ApplicationManager.h"
#include "AdvertisementManager.h"

#include "UserUI.h"
#include "ApplicationUI.h"
#include "ModeratorUI.h"
#include "AdministratorUI.h"

#include "TextHandlers.h"
#include "CallbackHandlers.h"

#include "Database.h"
#include "FileStorage.h"
#include "TelegramApi.h"
#include "Core.h"

void bot(const Config& config)
{
	// Версия бота
	std::string VERSION{ config.program().version };
	std::cout << "Version: " << VERSION << std::endl;

	FileStorage fileStorage(
		"Data/Storage/Stickers.json",
		"Data/Storage/Users.json"
	);

	Users users;
	StickerDatabase stickerDatabase;
	ApplicationManager applicationManager;

	// Подключение лючени к БД
	Database database(
		config,
		users,
		stickerDatabase
	);
	database.createStructure();

	// Подключение к Telegram
	TgBot::Bot bot(config.bot().token);
	TelegramApi api(bot);
	if (!api.connectionStatus()) { return; }

	// Создание главного админа
	const int64_t ADMINISTRATORS_ID{ 5183420401 };
	const std::string ADMINISTRATORS_USERNAME{ "dark_586" };
	if (!users.doesUserExist(ADMINISTRATORS_ID))
		{ users.addUser(ADMINISTRATORS_ID, ADMINISTRATORS_USERNAME); }
	users.withUser(ADMINISTRATORS_ID, [](User& user)
		{
			user.setRole(User::Role::Administrator);
		});

	AdvertisementManager advertisementManager(api, users);

	// Команды
	UserUI userUI(api);
	ModeratorUI moderatorUI(api, applicationManager);
	ApplicationUI applicationUI(api, users);
	AdministratorUI administratorUI(
		api,
		users,
		stickerDatabase,
		fileStorage,
		advertisementManager
	);

	ThreadPool pool(6); // 6 потоков для обработки задач

	UI ui
	{
		userUI,
		moderatorUI,
		administratorUI,
		applicationUI
	};

	Context ctx
	{
		pool,
		users,
		stickerDatabase,
		applicationManager,
		ui,
		database
	};

	// Автообновление данных
	if (config.program().automaticDataUpdate)
	{
		std::cout << "Автообновление данных..." << std::endl;
		if (config.program().updateSource == Config::Program::UpdateSource::Database)
		{
			administratorUI.updateAllDataFromDB(ADMINISTRATORS_ID, database);

			std::cout << "Данные успешно обновлены из базы данных" << std::endl;
		}
		else if (config.program().updateSource == Config::Program::UpdateSource::File)
		{
			administratorUI.updateAllDataFromFile(ADMINISTRATORS_ID);

			std::cout << "Данные успешно обновлены из файлов" << std::endl;
		}
		else
		{
			Core::logError("Неизвестный источник обновления данных");
		}
	}
	std::cout << "================================" << std::endl;

	// Обработчики ответов
	CallbackHandlers callbackHandlers(ctx);

	// Обработка команд
	// start
	bot.getEvents().onCommand("start", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };
			std::string firstName = message->from ?
				message->from->firstName : "User";

			pool.addTask([&userUI, chatId, firstName]
				{
					userUI.start(chatId, firstName);
				});
		});

	// help
	bot.getEvents().onCommand("help", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId]
				{
					userUI.help(chatId);
				});
		});

	// secretMenu
	bot.getEvents().onCommand("secretMenu", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId]
				{
					userUI.extraMenu(chatId);
				});
		});

	// menu
	bot.getEvents().onCommand("menu", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId]
				{
					userUI.menu(chatId);
				});
		});

	// random
	bot.getEvents().onCommand("random", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId]
				{
					userUI.randomSticker(chatId);
				});
		});

	// list
	bot.getEvents().onCommand("list", [&pool, &userUI, &stickerDatabase]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId, &stickerDatabase]
				{
					userUI.animeList(
						chatId,
						stickerDatabase.categories().categories()
					);
				});
		});

	// feedback
	bot.getEvents().onCommand("feedback", [&pool, &userUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&userUI, chatId]
				{
					userUI.feedbackGroup(chatId);
				});
		});

	// applications
	bot.getEvents().onCommand("applications", [&pool, &moderatorUI, &users]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };

			pool.addTask([&moderatorUI, chatId, &users]
				{
					moderatorUI.getApplication(chatId, users);
				});
		});

	// developerMenu
	bot.getEvents().onCommand("developerMenu", [&pool, &administratorUI]
	(TgBot::Message::Ptr message)
		{
			int64_t chatId{ message->chat->id };
			pool.addTask([&administratorUI, chatId]
				{
					administratorUI.menu(chatId);
				});
		});

	// Обработка сообщений пользователя
	bot.getEvents().onAnyMessage([&pool, &users, &ctx, &advertisementManager]
	(TgBot::Message::Ptr message)
		{
			// Проверка, существует ли этот пользователь
			if (!users.doesUserExist(message->chat->id))
			{
				// Создание нового пользователя
				std::string firstName = message->from ?
					message->from->firstName : "User";

				users.addUser(
					message->chat->id,
					firstName
				);
			}

			auto user{ users.getUser(message->chat->id) };
			if (user.has_value())
			{
				const bool isStaffInputInProgress =
					(user->role() == User::Role::Moderator &&
						user->state<InputState::ModeratorState>().isModeratorInputInProgress())
					||
					(user->role() == User::Role::Administrator &&
						user->state<InputState::AdministratorState>().isAdminInputInProgress());

				const std::string messageText =
					!message->text.empty()
					? message->text
					: message->sticker
					? Core::createStickerLink(message->sticker->setName)
					: !message->photo.empty()
					? message->photo.back()->fileId
					: "";

				if (isStaffInputInProgress)
				{
					TextHandlers::handleStaffInput(
						message->chat->id,
						messageText,
						ctx,
						advertisementManager
					);
					return;
				}

				const auto& state{ user->state<InputState::UserState>() };

				if (state.isInputInProgress())
				{
					TextHandlers::handleUserInput(
						message->chat->id,
						messageText,
						ctx
					);
				}
				else
				{
					TextHandlers::textHandlers(
						message->chat->id,
						messageText,
						ctx
					);
				}
			}
		});

	// Обработка callback кнопок
	bot.getEvents().onCallbackQuery([&pool, &ctx, &bot, &callbackHandlers]
	(TgBot::CallbackQuery::Ptr callback)
		{
			if (!callback) { return; }
			auto callbackId{ callback->id };
			pool.addTask([&bot, callbackId]
				{
					try { bot.getApi().answerCallbackQuery(callbackId); }
					catch (const std::exception& error) { Core::logError(error.what()); }
				});
			// Состояние пользователя хранится по ID личного чата.
			if (!callback->message || !callback->message->chat || !callback->from
				|| callback->message->chat->id != callback->from->id) { return; }
			callbackHandlers.handle(
				callback->message->chat->id,
				callback->message->messageId,
				callback->data
			);
		});

	// Основной цикл обработки обновлений от Telegram
	auto allowedUpdates = std::make_shared<std::vector<std::string>>(
		std::initializer_list<std::string>{
			"message",
			"edited_message",
			"channel_post",
			"edited_channel_post",
			"callback_query"
		}
	);

	TgBot::TgLongPoll longPoll(bot, 100, 60, allowedUpdates);
	while (true) {
		try {
			longPoll.start();   // Блокирующий вызов
		}
		catch (const TgBot::TgException& e)
		{
			Core::logError("Critical error: " + std::string(e.what()));
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		catch (const std::exception& e)
		{
			Core::logError("Critical error: " + std::string(e.what()));
			return;
		}
	}
}
