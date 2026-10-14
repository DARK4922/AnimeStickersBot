#include "ApplicationUI.h"

#include <vector>
#include <string>
#include <unordered_set>

#include "MessageText.h"
#include "StickerApplication.h"

using ReplyKeyboard = TelegramApi::ReplyKeyboard;
using InlineKeyboard = TelegramApi::InlineKeyboard;

ApplicationUI::ApplicationUI(
	TelegramApi& tgApi,
	Users& users_
)
	: tgApi_(tgApi)
	, users_(users_)
{}

void ApplicationUI::getCategoryName(int64_t chatId)
{
	users_.withUser(chatId, [](User& user)
		{
			user.createApplication();
			user.state<InputState::UserState>().enteringName = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"◀ Назад", "sticker_lab_new"}
			}
		}
	};

	static const ReplyKeyboard replyKeyboard{
		{
			{"❌ Выйти"}
		}
	};

	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::getCategoryName
		},
		{ inlineKeyboard }
	);
}

void ApplicationUI::addStickersMenu(int64_t chatId, int64_t messageId)
{
	if (!isApplicationUpToDate(chatId, messageId)) { return; }

	int stickersSize{};
	users_.withUser(chatId, [&stickersSize](User& user)
		{
			stickersSize = user.application()->links().size();
		});

	InlineKeyboard inlineKeyboard{
		{
			{
				{"➕ Добавить один стикер",
				"add_a_sticker_pack_to_the_application"}
			},
			{
				{"📚 Добавить несколько стикеров",
				"add_some_stickers_to_the_application"}
			}
		}
	};

	if (stickersSize > 0)
	{
		inlineKeyboard.addButtonRow(
			{
				{"📤 Отправить заявку",
				"checking_the_completed_application"}
			}
		);
	}

	if (messageId == 0)
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::User::addStickersMenu(stickersSize)
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::User::addStickersMenu(stickersSize)
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void ApplicationUI::startMultipleLinkSubmission(int64_t chatId, int64_t messageId)
{
	if (!isApplicationUpToDate(chatId, messageId)) { return; }

	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::UserState>().
				multipleEnteringLink = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"⛔ Завершить добавление",
				"stop_entering_stickers"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::startMultipleLinkSubmission
		},
		messageId,
		{ inlineKeyboard }
	);
}

void ApplicationUI::notifyStickerAdded(int64_t chatId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"⛔ Завершить добавление",
				"stop_entering_stickers"}
			}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::stickerAdded
		},
		{ inlineKeyboard }
	);
}

void ApplicationUI::notifyStickerAlreadyAdded(int64_t chatId)
{
	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::stickerAlreadyAdded
		}
	);
}

void ApplicationUI::processSingleSticker(int64_t chatId, int64_t messageId)
{
	if (!isApplicationUpToDate(chatId, messageId)) { return; }

	users_.withUser(chatId, [](User& user)
		{
			user.state<InputState::UserState>().enteringLink = true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"≪ Назад", "stop_entering_stickers"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::getSingleSticker,
		},
		messageId,
		{ inlineKeyboard }
	);
}

void ApplicationUI::applicationPreview(int64_t chatId, int64_t messageId)
{
	if (!isApplicationUpToDate(chatId, messageId)) { return; }

	StickerApplication* application{};
	users_.withUser(chatId, [&application](User& user)
		{
			application = user.application();
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"✅ Отправить",
				"send_a_request_for_verification"}
			},
			{
				{"↩️ Назад", "sticker_addition_menu"}
			}
		}
	};

	std::vector<std::string> stickers(
		application->links().begin(),
		application->links().end()
	);
	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::applicationPreview(
				application->category(),
				stickers
			)
		},
		messageId,
		{ inlineKeyboard }
	);
}

void ApplicationUI::sendApplicationForReview(
	int64_t chatId,
	int64_t messageId,
	ApplicationManager& applications
)
{
	if (!isApplicationUpToDate(chatId, messageId)) { return; }

	StickerApplication* application{};
	users_.withUser(chatId, [&application](User& user)
		{
			application = user.application();
		});

	applications.addApplication(*application);

	// Удаление заявки только у пользователя
	users_.withUser(chatId, [&applications](User& user)
		{
			user.removeApplication();
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"🏠 На главную", "main_menu"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::sendApplicationForReview
		},
		messageId,
		{ inlineKeyboard }
	);
}

bool ApplicationUI::isApplicationUpToDate(int64_t chatId, int64_t messageId)
{
	bool application{};
	users_.withUser(chatId, [&application](User& user)
		{
			application = user.hasApplication();
		});

	if (!application)
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::User::isApplicationUpToDate
			},
			messageId
		);
	}

	return application;
}

void ApplicationUI::notifyStickerAddedByAnotherUser(int64_t chatId)
{
	tgApi_.sendMessage(
	{
		chatId,
		MessageText::User::theLinkWasAddedEarlier
	}
	);
}