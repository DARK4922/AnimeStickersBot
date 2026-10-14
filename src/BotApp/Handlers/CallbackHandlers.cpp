#include "CallbackHandlers.h"

#include <charconv>
#include <system_error>

#include "Core.h"

CallbackHandlers::CallbackHandlers(Context& ctx) : ctx_(ctx) { load(); }

void CallbackHandlers::load()
{
    handlers_ =
    {
        // Информация о рекламе
        {
            "advertising",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.advertising(chatId, messageId);
            }
        },

        // Покупка рекламы
        {
            "order_an_advertisement",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.buyAdvertising(chatId, messageId);
            }
        },

        // Правила рекламы
        {
            "the_advertising_code",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.advertisingRules(chatId, messageId);
            }
        },

        // Рандомный стикер
        {
            "random_sticker",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.user.randomSticker(chatId);
            }
        },

        // Список доступных аниме
        {
            "list_of_anime",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.user.animeList(
                    chatId,
                    ctx_.stickerDatabase.categories().categories()
                );
            }
        },

        // Меню выбора
        {
            "menu",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.user.menu(chatId);
            }
        },

        // Правила загрузки стикеров
        {
            "get_sticker_upload_rules",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.getStickerUploadRules(chatId, messageId);
            }
        },

        // Лаборатория стикеров
        {
            "sticker_lab",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.stickerLab(chatId, messageId);
            }
        },
        {
            "sticker_lab_new",
            [this](int64_t chatId, int64_t)
            {
                ctx_.users.withUser(chatId, [](User& user)
                {
                   user.state<InputState::UserState>().resetInputState();
                });
                ctx_.ui.user.stickerLab(chatId);
            }
        },

        // Начало многократной отправки ссылок
        {
            "add_some_stickers_to_the_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.application.startMultipleLinkSubmission
                (chatId, messageId);
            }
        },

        // Прекращение отправки ссылок
        {
            "stop_entering_stickers",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.users.withUser(chatId, [](User& user)
                    {
                       user.state<InputState::UserState>().
                            resetInputState();
                    });
                ctx_.ui.application.addStickersMenu(chatId, messageId);
            }
        },

        // Меню добавления стикеров
        {
            "sticker_addition_menu",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.application.addStickersMenu(chatId, messageId);
            }
        },

        // Получение одного стикера
        {
            "add_a_sticker_pack_to_the_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.application.processSingleSticker
                (chatId, messageId);
            }
        },

        // Показ заявки перед отправкой
        {
            "checking_the_completed_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.application.applicationPreview(chatId, messageId);
            }
        },

        // Главное меню
        {
            "main_menu",
            [this](int64_t chatId, int64_t)
            {
                std::string firstName{};
                ctx_.users.withUser(chatId, [&firstName](User& user)
                    {
                        firstName = user.username();
                    });

                ctx_.ui.user.start(chatId, firstName);
            }
        },

        // Отправка заявки на проверку
        {
            "send_a_request_for_verification",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.application.sendApplicationForReview(
                    chatId,
                    messageId,
                    ctx_.applicationManager 
                );
            }
        },

        // Получение заявки
        {
            "get_the_first_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.moderator.getApplication(
                    chatId,
                    ctx_.users,
                    messageId
                );
            }
        },
        {
            "get_the_first_application_update",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.moderator.getApplication(
					chatId,
					ctx_.users
				);
            }
        },

        // Одобрение заявки
        {
            "accept_the_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.moderator.approveApplication(
                    chatId,
                    messageId,
                    ctx_.stickerDatabase,
                    ctx_.ui.user
                );
            }
        },

        // Вернуться к заявке
        {
            "go_back_to_the_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.users.withUser(chatId, [](User& user)
                    {
                        user.state<InputState::ModeratorState>().
                                enteringReasonRefusal = false;
                    });

                ctx_.ui.moderator.getApplication(
                    chatId,
                    ctx_.users,
                    messageId
                );
            }
        },

        // Получение причины отказа
        {
            "reject_the_application",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.moderator.getRejectionReason(
                    chatId,
                    messageId,
                    ctx_.users
                );
            }
        },

		// Меню сохранения данных
		{
			"save_menu",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.administrator.saveMenu(chatId, messageId);
			}
		},

		// Меню обновления данных
		{
			"update_menu",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.administrator.updateMenu(chatId, messageId);
			}
		},

        // Меню разработчика
        {
            "developers_menu",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.administrator.menu(chatId, messageId);
            }
        },

        // Сохранение данных в БД по отдельности
        {
            "save_users_to_db",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveUserToDB(
                    chatId,
                    ctx_.db
                );
            }
        },
        {
            "save_stickers_to_db",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveStickersToDB(
                    chatId,
                    ctx_.db
                );
            }
        },

        // Сохранение данных в файл по отдельности
        {
            "save_users_to_file",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveUserToFile(chatId);
            }
        },
        {
            "save_stickers_to_file",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveStickersToFile(chatId);
            }
        },

        // Сохранение всех данных в БД/Файл
        {
            "save_all_to_database",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveAllDataToDB(
                    chatId,
                    ctx_.db
                );
            }
        },
        {
            "save_all_data_to_file",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.saveAllDataToFile(chatId);
            }
        },

        // Обновления всех данных из БД/Файла
        {
            "update_all_data_from_the_database",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.messageDeletion(chatId, messageId);

                ctx_.ui.administrator.updateAllDataFromDB(
                    chatId,
                    ctx_.db
                );
            }
        },
        {
            "update_all_data_from_the_file",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.messageDeletion(chatId, messageId);
                ctx_.ui.administrator.updateAllDataFromFile(chatId);
            }
        },

		// Обновление данных из файла по отдельности
		{
			"update_users_from_file",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.user.messageDeletion(chatId, messageId);
				ctx_.ui.administrator.updateUserFromFile(chatId);
			}
		},
		{
			"update_stickers_from_file",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.user.messageDeletion(chatId, messageId);
				ctx_.ui.administrator.updateStickersFromFile(chatId);
			}
		},

        // Отмена действия
        {
            "cancellation_of_the_action",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.administrator.revocationTheAction
                (chatId, messageId);
            }
        },

        // Статистика
        {
            "number_of_users",
			[this](int64_t chatId, int64_t)
			{
				ctx_.ui.administrator.numberOfUsers(chatId);
			}
        },
        {
			"advanced_statistics",
            [this](int64_t chatId, int64_t messageId)
            {
				ctx_.ui.administrator.advancedStatistics(
					chatId,
					ctx_.applicationManager.numberApplications(),
					messageId
				);
            }
        },

		// Список заявок на проверку
		{
			"list_of_applications",
			[this](int64_t chatId, int64_t)
			{
                ctx_.ui.moderator.getApplication
                (chatId, ctx_.users);
			}
		},

        // Выход из меню разработчика
		{
			"exit_developers_menu",
			[this](int64_t chatId, int64_t)
			{
				ctx_.ui.user.menu(chatId);
			}
		},

        // Выбор нового аниме
        {
            "menu_and_delete_message",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.user.messageDeletion(chatId, messageId);
                ctx_.ui.user.menu(chatId);
            }
        },

        // Меню рекламы
        {
            "publish_an_advertisement",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.administrator.advertisingMenu(chatId, messageId);
			}
        },

        // Добавление рекламного текста
        {
            "add_advertising_text",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.administrator.getAdvertisingMessage(chatId, messageId);
			}
        },
        // Добавление рекламного фото
        {
            "add_advertising_photo",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.administrator.receiveAdvertisingPhoto(chatId, messageId);
            }
        },

        // Отмена рекламы
        {
            "canceling_the_advertising_mailing",
			[this](int64_t chatId, int64_t messageId)
			{
				ctx_.ui.administrator.deleteAdvertisingPost(chatId, messageId);
			}
        },

        // Предпросмотр поста
        {
            "post_preview",
            [this](int64_t chatId, int64_t)
            {
                ctx_.ui.administrator.postPreview(chatId);
            }
        },

        // Подтверждение отправки рекламы
        {
            "send_advertising_mailing",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.administrator.confirmAdvertisement(chatId, messageId);
            }
        },

        // Отправка рекламы
        {
            "sending_ads",
            [this](int64_t chatId, int64_t messageId)
            {
                ctx_.ui.administrator.sendAdvertisementToAllUsers(chatId, messageId);
            }
        }
    };
}

void CallbackHandlers::handle(
    int64_t chatId,
    int64_t messageId,
    const std::string& callbackData)
{
    // Проверка существует ли таколь пользователь
    if (!ctx_.users.doesUserExist(chatId))
    {
        ctx_.users.addUser(chatId);
    }

    auto it{ handlers_.find(callbackData) };
    if (it != handlers_.end())
    {
        ctx_.pool.addTask(
            [it = it->second, chatId, messageId]
            {
                it(chatId, messageId);
            });
    }
    else
    {
        handleExternalCallback(
            chatId,
            messageId,
            callbackData
        );
    }
}

void CallbackHandlers::handleExternalCallback(
    int64_t chatId,
    int64_t messageId,
    const std::string& callbackData
)
{
    if (callbackData.starts_with("confirm:"))
    {
		std::string action{ callbackData.substr(8) };

		ctx_.pool.addTask(
			[this, chatId, action]
			{
                ctx_.ui.administrator.confirmationAction(
                    chatId,
                    action
                );
			}
        );
	}
    else if (callbackData.starts_with("more_stickers:"))
    {
        std::string data{ callbackData.substr(14) };

        const std::string separator{ Core::separationMarker() };
        const std::size_t separatorPos{ data.find(separator) };

        if (separatorPos == std::string::npos)
        {
            Core::logError("Ошибка извлечения данных стикера");
            return;
        }

        const int stickerId{ std::stoi(data.substr(0, separatorPos)) };
        const int startIndex{
            std::stoi(data.substr(separatorPos + separator.size()))
        };

        // Получение пользователя
        auto user{ ctx_.users.getUser(chatId) };
        if (!user.has_value())
        {
            Core::logError(
                "Обработка информации для несуществующего пользователя"
            );
            return;
        }

        std::string category{};
        ctx_.stickerDatabase.withSticker(stickerId,
            [&category](const StickerPack& sticker)
            {
                category = sticker.category();
            });

        // Получение стикеров
        const std::vector<StickerPack> stickers{
            ctx_.stickerDatabase.stickers().getStickers(category)
        };

        ctx_.pool.addTask(
            [this, chatId, messageId, startIndex, user, stickers]
            {
                ctx_.ui.user.messageDeletion(chatId, messageId);
                ctx_.ui.user.broadcastStickersByAnime(
                    chatId,
                    user.value(),
                    stickers,
                    startIndex
                );
            }
        );
    }

    // Реакции
    else if (callbackData.starts_with("Like:")
        || callbackData.starts_with("Dislike:"))
    {
        // Получение данных
        const size_t separatorPos{ callbackData.find(':') };

        const std::string_view idString{
            callbackData.data() + separatorPos + 1,
            callbackData.size() - separatorPos - 1
        };

        int stickerId{};

        const auto [ptr, ec] = std::from_chars(
            idString.data(),
            idString.data() + idString.size(),
            stickerId
        );

        if (ec != std::errc{} || ptr != idString.data() + idString.size())
        {
            Core::logError("Некорректный ID стикера");
            return;
        }

        const bool isLike{ callbackData.starts_with("Like:") };

        const bool result{
            ctx_.stickerDatabase.changeReaction(
                stickerId,
                chatId,
                ctx_.users,
                isLike
                    ? StickerDatabase::ReactionType::Like
                    : StickerDatabase::ReactionType::Dislike
            )
        };

        ctx_.pool.addTask(
            [this, chatId, messageId, result, stickerId]
            {
                if (!result)
                {
                    ctx_.ui.user.handleReactionError(chatId, messageId);
                }
                else
                {
                    // Получение стикера
                    const auto sticker{
                        ctx_.stickerDatabase.stickers().getSticker(stickerId)
                    };
                    if (!sticker.has_value())
                    {
                        Core::logError(
                            "Обращение к несуществующему стикеру"
                        );
                        return;
                    }

					// Получение актуального пользователя
                    const auto user{ ctx_.users.getUser(chatId) };
                    if (!user.has_value())
                    {
                        Core::logError(
                            "Обращение к несуществующему пользователю"
                        );
                        return;
                    }

                    // Обновление вывода стикера
                    ctx_.ui.user.sendSticker(
                        chatId,
                        sticker.value(),
                        user.value(),
                        messageId
                    );
                }
            }
        );
    }

    else
    {
		Core::logError(
			"Неизвестный callback-запрос: " + callbackData
		);
    }
}