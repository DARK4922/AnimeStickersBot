#include "AdministratorUI.h"

#include <vector>

#include "User.h"
#include "StickerPack.h"
#include "MessageText.h"
#include "Core.h"

using ReplyKeyboard = TelegramApi::ReplyKeyboard;
using InlineKeyboard = TelegramApi::InlineKeyboard;

AdministratorUI::AdministratorUI(
	TelegramApi& TgApi,
	Users& users,
	StickerDatabase& stickers,
	FileStorage& fileStorage,
	AdvertisementManager& advertisementManager
)
	: tgApi_(TgApi)
	, users_(users)
	, stickers_(stickers) 
	, fileStorage_(fileStorage)
	, advertisementManager_(advertisementManager)
{}

bool AdministratorUI::accessVerification(int64_t chatId)
{
	User::Role role{};
	users_.withUser(chatId, [&role](User& user)
		{
			role = user.role();
		});
	
	if (role != User::Role::Administrator)
	{
		accessRestricted(chatId);
		return false;
	}
	return true;
}

void AdministratorUI::accessRestricted(int64_t chatId)
{
	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Administrator::accessRestricted
		}
	);
}

void AdministratorUI::menu(int64_t chatId, int64_t messageId)
{
	if (!accessVerification(chatId)) { return; }

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"💾 Меню сохранения", "save_menu"}
			},
			{
				{"🔄 Меню обновления", "update_menu"}
			},
			{
				{"💾 Сохранить всё в БД", "save_all_to_database"},
				{"🔄 Обновить всё из БД", "confirm:update_all_data_from_the_database"}
			},
			{
				{"👨‍💼 Админы", "administrators_menu"},
				{"📊 Статистика", "number_of_users"}
			},
			{
				{"📢 Реклама", "publish_an_advertisement"},
				{"📄 Список заявок", "list_of_applications"}
			},
			{
				{"🚪 Выход", "exit_developers_menu"}
			}
		}
	};

	if (messageId == 0)
	{
		// Эмодзи
		static const std::vector<std::string> emoji{
			"❤", "💝", "💘", "💖", "💞", "🔞", "‼",
			"✔", "😎", "😍", "🤩", "🤐", "🤑", "🥵", "👾"
		};

		static const ReplyKeyboard replyKeyboard{
			{
				{"Выйти из меню разработчика❌"},
				{"Обновить меню разработчика🔄"}
			}
		};

		tgApi_.sendMessage(
			{
				chatId,
				emoji[Core::randomNumber(0, emoji.size())]
			},
			{ replyKeyboard }
		);

		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::menu
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::menu
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void AdministratorUI::numberOfUsers(int64_t chatId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"📊 Расширенная статистика", "advanced_statistics"}
			}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Administrator::numberOfUsers(users_.size())
		},
		{ inlineKeyboard }
	);
}

void AdministratorUI::saveUserToFile(int64_t chatId)
{
	if (!accessVerification(chatId)) { return; }

	std::string status =
		fileStorage_.saveUsers(users_.allUsers()) ?
		"✔ ID сохранены" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::saveStickersToFile(int64_t chatId)
{
	if (!accessVerification(chatId)) { return; }

	const auto stickers{ stickers_.stickers().allPacks() };
	const int64_t nextId{ stickers_.stickers().nextId() };
	std::string status =
		fileStorage_.saveStickers(
			stickers,
			nextId
		) ?
		"✔ Стикеры сохранены" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::updateUserFromFile(int64_t chatId)
{
	if (!accessVerification(chatId)) { return; }

	std::string status{ "✔ Пользователи обновлены" };

	std::vector<User> users{
		fileStorage_.updateUsers()
	};

	if (users.empty()) { status = "❌ Произошла ошибка"; }
	else { users_.clearUsers(); }

	for (User& user : users) { users_.addUser(user); }

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::updateStickersFromFile(int64_t chatId)
{
	if (!accessVerification(chatId)) { return; }

	std::string status{ "✔ Стикеры обновлены" };

	int64_t nextId{};
	std::vector<StickerPack> stickers{
		fileStorage_.updateStickers(nextId)
	};

	if (nextId == 0) { status = "❌ Произошла ошибка"; }
	else
	{
		stickers_.ensureNextId(nextId);
		stickers_.clearStickers();
	}

	for (StickerPack& sticker : stickers)
		{ stickers_.addSticker(sticker); }

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::confirmationAction(
	int64_t chatId,
	const std::string& action,
	int64_t messageId
)
{
	if (!accessVerification(chatId)) { return; }

	const InlineKeyboard inlineKeyboard{
		{
			{
				{"✔️", action},
				{"❌", "cancellation_of_the_action"}
			}
		}
	};

	if (messageId == 0)
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::confirmationAction(action)
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::confirmationAction(action)
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void AdministratorUI::saveMenu(int64_t chatId, int64_t messageId)
{
	if (!accessVerification(chatId)) { return; }

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "📄 Сохранить пользователей в файл", "save_users_to_file" }
			},
			{
				{ "📄 Сохранить стикеры в файл", "save_stickers_to_file" }
			},
			{
				{ "🗄️ Сохранить пользователей в БД", "save_users_to_db" }
			},
			{
				{ "🗄️ Сохранить стикеры в БД", "save_stickers_to_db" }
			},
			{
				{ "🗄️ Сохранение все данные в БД", "save_all_to_database" },
				{ "📄 Сохранение все данные в Файла", "save_all_data_to_file" }
			},
			{
				{ "🚪 Выход", "developers_menu" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::saveMenu
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::updateMenu(int64_t chatId, int64_t messageId)
{
	if (!accessVerification(chatId)) { return; }

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "📄 Обновить пользователей из файла", "confirm:update_users_from_file" }
			},
			{
				{ "📄 Обновить стикеры из файла", "confirm:update_stickers_from_file" }
			},
			{
				{ "🗄️ Обновить пользователей из БД", "confirm:update_users_from_db" }
			},
			{
				{ "🗄️ Обновить стикеры из БД", "confirm:update_stickers_from_db" }
			},
			{
				{ "🗄️ Обновить все данные из БД", "confirm:update_all_data_from_db" },
				{ "📄 Обновить все данные из Файла", "confirm:update_all_data_from_the_file" }
			},
			{
				{ "🚪 Выход", "developers_menu" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::updateMenu
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::saveUserToDB(int64_t chatId, Database& db)
{
	if (!accessVerification(chatId)) { return; }

	std::string status =
		db.saveUsers() ? "✔ Пользователи сохранены в БД" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::saveStickersToDB(int64_t chatId, Database& db)
{
	if (!accessVerification(chatId)) { return; }

	std::string status =
		db.saveStickers() ? "✔ Стикеры сохранены в БД" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::updateUserFromDB(int64_t chatId, Database& db)
{
	if (!accessVerification(chatId)) { return; }

	std::string status = db.loadUsers() ?
		"✔ Пользователи обновлены" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::updateStickersFromDB(int64_t chatId, Database& db)
{
	if (!accessVerification(chatId)) { return; }

	std::string status = db.loadStickers() ?
		"✔ Стикеры обновлены" : "❌ Произошла ошибка";

	tgApi_.sendMessage(
		{
			chatId,
			status
		}
	);
}

void AdministratorUI::saveAllDataToDB(int64_t chatId, Database& db)
{
	saveUserToDB(chatId, db);
	saveStickersToDB(chatId, db);
}

void AdministratorUI::updateAllDataFromDB(int64_t chatId, Database& db)
{
	updateUserFromDB(chatId, db);
	updateStickersFromDB(chatId, db);
}

void AdministratorUI::saveAllDataToFile(int64_t chatId)
{
	saveUserToFile(chatId);
	saveStickersToFile(chatId);
}

void AdministratorUI::updateAllDataFromFile(int64_t chatId)
{
	updateUserFromFile(chatId);
	updateStickersFromFile(chatId);
}

void AdministratorUI::revocationTheAction(int64_t chatId, int64_t messageId)
{
	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::revocationTheAction
		},
		messageId
	);
}

void AdministratorUI::advancedStatistics(
	int64_t chatId,
	size_t applicationsSize,
	int64_t messageId
)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "🔄️ Обновить статистику", "advanced_statistics" }
			}
		}
	};

	if (messageId == 0)
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::advancedStatistics(
					Core::getDate(),
					users_.size(),
					stickers_.stickers().numberStickers(),
					stickers_.stickers().numberSections(),
					applicationsSize
				)
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::advancedStatistics(
					Core::getDate(),
					users_.size(),
					stickers_.stickers().numberSections(),
					stickers_.stickers().numberStickers(),
					applicationsSize
				)
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void AdministratorUI::advertisingMenu(int64_t chatId, int64_t messageId)
{
	// Сброс всех флагов ввода
	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::AdministratorState>().
				resetAdministratorInputState();
		});

	// Создание пустого рекламного поста
	if (!advertisementManager_.hasAdvertisement(chatId))
		{ advertisementManager_.addAdvertisement(chatId); }

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "📝 Добавить текст", "add_advertising_text" },
				{ "🖼 Добавить фото", "add_advertising_photo" }
			},
			{
				{ "👁️ Предпросмотр поста", "post_preview" },
				{ "📢 Отправить рекламу", "send_advertising_mailing" }
			},
			{
				{ "❌ Отмена", "canceling_the_advertising_mailing" }
			}
		}
	};

	const auto advertisement{
		advertisementManager_.getAdvertisement(chatId)
	};

	bool text{ !advertisement.get()->advertisingPost().message.empty() };
	bool photo{ !advertisement.get()->advertisingPost().photoId.empty() };

	if (messageId != 0)
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::advertisingMenu(text, photo)
			},
			messageId,
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::advertisingMenu(text, photo)
			},
			{ inlineKeyboard }
		);
	}
}

void AdministratorUI::getAdvertisingMessage(int64_t chatId, int64_t messageId)
{
	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::AdministratorState>().sendingPostText = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "<< Назад", "publish_an_advertisement" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::getAdvertisingMessage
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::receiveAdvertisingPhoto(int64_t chatId, int64_t messageId)
{
	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::AdministratorState>().sendingPhotoPost = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "<< Назад", "publish_an_advertisement" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::receiveAdvertisingPhoto
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::deleteAdvertisingPost(int64_t chatId, int64_t messageId)
{
	advertisementManager_.removeAdvertisement(chatId);

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "В меню разработчика", "developers_menu" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::deleteAdvertisingPost
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::applicationDisappeared(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "В меню разработчика", "developers_menu" }
			}
		}
	};

	if (messageId == 0)
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::applicationDisappeared
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::applicationDisappeared
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void AdministratorUI::postPreview(int64_t chatId)
{
	const auto advertisement{ advertisementManager_.getAdvertisement(chatId) };
	if (!advertisement)
	{
		applicationDisappeared(chatId);
		return;
	}

	advertisement->previewPost(chatId);
}

void AdministratorUI::confirmAdvertisement(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "✔️", "sending_ads" },
				{ "❌", "publish_an_advertisement" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::confirmAdvertisement
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::sendAdvertisementToAllUsers(int64_t chatId, int64_t messageId)
{
	if (!accessVerification(chatId)) { return; }
	const auto advertisement{ advertisementManager_.getAdvertisement(chatId) };
	if (!advertisement)
	{
		applicationDisappeared(chatId, messageId);
		return;
	}

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::sendAdvertisementToAllUsers
		},
		messageId
	);

	advertisement->broadcastAdvertisement(chatId);
	advertisementManager_.removeAdvertisement(chatId);
}

void AdministratorUI::showAdminManagementMenu(int64_t chatId, int64_t messageId)
{
	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::AdministratorState>().enteringUserId = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "<< Назад", "developers_menu" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::showAdminManagementMenu
		},
		messageId,
		{ inlineKeyboard }
	);
}

void AdministratorUI::showUserSettingsMenu(int64_t chatId, int64_t userId)
{
	if (!accessVerification(chatId)) { return; }

	const std::string& marker{ Core::separationMarker() };

	const InlineKeyboard inlineKeyboard{
		{
			{
				{ "👤 Сделать обычным пользователем",
				"confirm:make_a_user"
				+ marker + std::to_string(userId) }
			},
			{
				{ "🛡️ Сделать модератором",
				"confirm:make_a_moderator"
				+ marker + std::to_string(userId) }
			},
			{
				{ "👑 Сделать администратором",
				"confirm:make_an_administrator"
				+ marker + std::to_string(userId) }
			}
		}
	};

	auto user{ users_.getUser(userId) };
	if (!user.has_value())
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Administrator::userWasNotFound(userId)
			}
		);
		return;
	}

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Administrator::showUserSettingsMenu(user.value())
		},
		{ inlineKeyboard }
	);
}

void AdministratorUI::onUserRoleAssigned(
	int64_t chatId,
	int64_t messageId,
	int64_t userId
)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{ "🏠 На главную", "developers_menu" }
			}
		}
	};

	auto user{ users_.getUser(userId) };
	if (!user.has_value())
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Administrator::userWasNotFound(userId)
			},
			messageId,
			{ inlineKeyboard }
		);
		return;
	}

	// Отправка сообщения пользователю
	tgApi_.sendMessage(
		{
			userId,
			MessageText::Administrator::roleAssignmentNotification(user->role())
		}
	);

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Administrator::onUserRoleAssigned(user.value())
		},
		messageId,
		{ inlineKeyboard }
	);
}
