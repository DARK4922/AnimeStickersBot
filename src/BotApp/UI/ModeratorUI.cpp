#include "ModeratorUI.h"

#include "MessageText.h"
#include "StickerDatabase.h"
#include "Users.h"
#include "Core.h"

using ReplyKeyboard = TelegramApi::ReplyKeyboard;
using InlineKeyboard = TelegramApi::InlineKeyboard;

ModeratorUI::ModeratorUI(
	TelegramApi& TgApi,
	ApplicationManager& applications
)
	: tgApi_(TgApi)
	, applications_(applications)
{}

void ModeratorUI::confirmStickerRemoval(int64_t chatId, int64_t messageId, int stickerId)
{
	const std::string suffix{ Core::separationMarker() + std::to_string(stickerId) };
	const InlineKeyboard buttons{
		{
			{
				{ "✅ Удалить", "remove_the_sticker_pack" + suffix },
				{ "❌ Отмена", "cancellation_of_the_action" }
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Moderator::confirmStickerRemoval
		},
		messageId,
		{ buttons }
	);
}

void ModeratorUI::onStickerRemoved(int64_t chatId, int64_t messageId)
{
	tgApi_.editMessage(
		{
			chatId, MessageText::Moderator::stickerRemoved
		},
		messageId
	);
}

void ModeratorUI::getApplication(
	int64_t chatId,
	Users& users,
	int64_t messageId
)
{
	if (!isModerator(chatId, users)) { return; }

	auto application{ applications_.getApplicationForModerator(chatId) };
	if (!application.has_value())
	{
		static const InlineKeyboard inlineKeyboard{
			{
				{
					{"🔃 Обновить",
					"get_the_first_application_update"}
				}
			}
		};

		if (messageId == 0)
		{
			tgApi_.sendMessage(
				{
					chatId,
					MessageText::Moderator::thereAreNoApplications
				},
				{ inlineKeyboard }
			);
		}
		else
		{
			tgApi_.editMessage(
				{
					chatId,
					MessageText::Moderator::thereAreNoApplications
				},
				messageId,
				{ inlineKeyboard }
			);
		}

		return;
	}

	const auto& app{ *application };

	int64_t creatorId{ app.creatorInfo().id() };
	const auto& creatorName{ app.creatorInfo().username() };
	const auto& category{ app.category() };
	const auto& links{ app.links() };

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"✅ Принять", "accept_the_application"}
			},
			{
				{"❌ Отклонить", "reject_the_application"}
			}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Moderator::getApplication(
				creatorId,
				creatorName,
				category,
				links
			)
		},
		{ inlineKeyboard }
	);
}

void ModeratorUI::approveApplication(
	int64_t chatId,
	int64_t messageId,
	StickerDatabase& stickers,
	UserUI& userUI
)
{
	auto creatorId = applications_.acceptApplication(
		chatId,
		stickers
	);

	static const InlineKeyboard inlineKeyboard{
			{
				{
					{"➡️ Проверить ещё",
					"get_the_first_application"}
				}
			}
	};

	if (!creatorId.has_value())
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::Moderator::applicationCouldNotBeApproved
			},
			messageId,
			{ inlineKeyboard }
		);

		return;
	}

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Moderator::applicationWasSuccessfullyAccepted
		},
		messageId,
		{ inlineKeyboard }
	);

	// Отправка сообщения пользователю
	userUI.applicationAccepted(creatorId.value());
}

void ModeratorUI::getRejectionReason(
	int64_t chatId,
	int64_t messageId,
	Users& users
)
{
	users.withUser(chatId, [](User& user)
		{
			user.state<InputState::ModeratorState>().enteringReasonRefusal
				= true;
		});

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"⬅️ Назад",
				"go_back_to_the_application"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::Moderator::getRejectionReason
		},
		messageId,
		{ inlineKeyboard }
	);
}

bool ModeratorUI::isModerator(int64_t chatId, Users& users) const
{
	User::Role role{};
	users.withUser(chatId, [&role](User& user)
		{
			role = user.role();
		});

	if (role < User::Role::Moderator)
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Moderator::isModerator
			}
		);
	}

	return role >= User::Role::Moderator;
}

void ModeratorUI::applicationRejected(
	int64_t chatId,
	UserUI& userUI,
	std::string reasonForRefusal
)
{
	auto creatorId{ applications_.rejectApplication(chatId) };

	static const InlineKeyboard inlineKeyboard{
			{
				{
					{"➡️ Проверить ещё",
					"get_the_first_application"}
				}
			}
	};

	if (!creatorId.has_value())
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::Moderator::applicationCouldNotBeApproved
			},
			{ inlineKeyboard }
		);

		return;
	}

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::Moderator::applicationHasBeenRejected(
				reasonForRefusal
			)
		},
		{ inlineKeyboard }
	);

	// Отправка сообщения пользователю
	userUI.rejectApplication(
		creatorId.value(),
		std::move(reasonForRefusal)
	);
}
