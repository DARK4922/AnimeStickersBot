#include "UserUI.h"

#include "Core.h"
#include "TelegramApi.h"
#include "MessageText.h"

using ReplyKeyboard = TelegramApi::ReplyKeyboard;
using InlineKeyboard = TelegramApi::InlineKeyboard;
using LinkPreviewOptions = TelegramApi::LinkPreviewOptions;

UserUI::UserUI(TelegramApi& TgApi)
	: tgApi_(TgApi) {}

void UserUI::start(int64_t chatId, std::string firstName)
{
	// Проверка имени на знак '<' или '>'
	if (firstName.empty() ||
		firstName.find('<') != std::string::npos ||
		firstName.find('>') != std::string::npos)
	{
		firstName = "User";
	}

	static const ReplyKeyboard replyKeyboard{
		{
			{"🏮 Войти в аниме-мир"},
			{"🎨 Лаборатория стикеров"},
			{"📜 Тайное меню"}
		}
	};

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"🌸 Все аниме-миры", "list_of_anime"}
			}
		}
	};

	tgApi_.sendRandomPhoto(
		{
			chatId,
			MessageText::User::start(firstName)
		},
		{ replyKeyboard }
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::start2
		},
		{ inlineKeyboard }
	);
}

void UserUI::menu(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"А", "Б", "В", "Г", "Д"},
			{"Е", "Ё", "Ж", "З", "И"},
			{"Й", "К", "Л", "М", "Н"},
			{"О", "П", "Р", "С", "Т"},
			{"У", "Ф", "Х", "Ц", "Ч"},
			{"Ш", "Щ", "Э", "Ю", "Я"},
			{"≽ Назад ≼"}
		}
	};

	tgApi_.sendRandomPhoto(
		{
			chatId,
			MessageText::User::menu
		},
		{ replyKeyboard }
	);
}

void UserUI::extraMenu(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"✨ Реклама", "💖 Донат"},
			{"🎌 Создатель-сенсей", "✉️ Администратор-тян"},
			{"🌸 Инфо-канал (эксклюзивы внутри!)"},
			{"✨Предложить улучшение или оставить отзыв"},
			{"≽ Назад ≼"}
		}
	};

	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::extraMenu
		}
	);
}

void UserUI::channelInfo(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Вернуться в Тайное меню"}
		}
	};

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"🌸 Dark Shark | Эксклюзивы",
				"https://t.me/Dark_project64"}
			}
		}
	};

	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::channelInfo
		},
		{ inlineKeyboard }
	);
}

void UserUI::developerInfo(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Вернуться в Тайное меню"}
		}
	};

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"📩 Визитка сенсея", "https://t.me/dark_586"}
			}
		}
	};
	
	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::developerInfo
		},
		{ inlineKeyboard }
	);
}

void UserUI::support(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Вернуться в Тайное меню"}
		}
	};

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"🛠️ Группа техподдержки", "https://t.me/AnimeStikerHub"}
			},
			{
				{"👨💻 Написать админу (личное сообщение)", "https://t.me/dark_586"}
			}
		}
	};

	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::support
		},
		{ inlineKeyboard }
	);
}

void UserUI::feedbackGroup(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Вернуться в Тайное меню"}
		}
	};

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"✨ Группа отзывов", "https://t.me/+BrYT3jrkfhQwNWE6"}
			}
		}
	};

	tgApi_.sendRandomSticker(
		chatId,
		replyKeyboard
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::feedbackGroup
		},
		{ inlineKeyboard }
	);
}

void UserUI::donate(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Вернуться в Тайное меню"}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::donate
		},
		{ replyKeyboard }
	);
}

void UserUI::advertising(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"✨ Заказать рекламу", "order_an_advertisement"}
			},
			{
				{"⚖️ Рекламный кодекс", "the_advertising_code"}
			}
		}
	};

	if (messageId <= 0)
	{
		static const ReplyKeyboard replyKeyboard{
			{
				{"🌸 Вернуться в Тайное меню"}
			}
		};

		tgApi_.sendRandomSticker(
			chatId,
			replyKeyboard
		);

		tgApi_.sendMessage(
			{
				chatId,
				MessageText::User::advertising
			},
			{ inlineKeyboard }
		);

		return;
	}

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::advertising
		},
		messageId,
		{ inlineKeyboard }
	);
}

void UserUI::buyAdvertising(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"Написать менеджеру", "https://t.me/dark_586"}
			},
			{
				{"<< Назад", "advertising"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::buyAdvertising
		},
		messageId,
		{ inlineKeyboard }
	);
}

void UserUI::advertisingRules(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"<< Назад", "advertising"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::advertisingRules,
		},
		messageId,
		{ inlineKeyboard }
	);
}

void UserUI::help(int64_t chatId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"✨ Написать админу", "https://t.me/dark_586"}
			},
			{
				{"🎌 Написать в группу поддержки", "https://t.me/AnimeStikerHub"}
			},
			{
				{"🎲 Случайный стикер", "random_sticker"}
			}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::help
		},
		{ inlineKeyboard }
	);
}

void UserUI::randomSticker(int64_t chatId)
{
	tgApi_.sendRandomSticker(chatId);
}

void UserUI::animeList(
	int64_t chatId,
	const std::vector<std::string>& category
)
{
	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::animeList(std::move(category))
		}
	);
}

void UserUI::getAnimeByLetter(
	int64_t chatId,
	const std::string& letter,
	const std::vector<std::string>& animes
)
{
	// Создание клавиатуры с названиями доступных аниме
	ReplyKeyboard replyKeyboard;
	for (const auto& anime : animes)
	{
		if (anime.starts_with(letter))
		{
			replyKeyboard.addButtonRow(
				{ Core::animeMarker() + anime + Core::animeMarker() }
			);
		}
	}

	// Если нет аниме на данную букву
	if (replyKeyboard.buttons().empty())
	{
		tgApi_.sendMessage(
			{
				chatId,
				MessageText::User::noAnimeFound
			}
		);

		return;
	}

	replyKeyboard.addButtonRow({ "🔙Назад" });

	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"🌸 Выбрать новое аниме", "menu"}
			}
		}
	};

	tgApi_.sendRandomPhoto(
		{
			chatId,
			MessageText::User::getAnimeByLetter(letter)
		},
		{ replyKeyboard }
	);

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::choosingNewAnime
		},
		{ inlineKeyboard }
	);
}

void UserUI::notifyAnimeNotFound(int64_t chatId)
{
	static const ReplyKeyboard replyKeyboard{
		{
			{"🌸 Выбрать новое аниме"}
		}
	};

	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::notifyAnimeNotFound
		},
		{ replyKeyboard }
	);
}

void UserUI::broadcastStickersByAnime(
	int64_t chatId,
	const User& user,
	const std::vector<StickerPack>& stickers,
	size_t startIndex
)
{
	if (stickers.empty())
	{
		notifyAnimeNotFound(chatId);
		return;
	}

	// Определение индекса окончания
	static const int numberStickersAtTime{ 7 };
	size_t endIndex{};
	if (startIndex + numberStickersAtTime <= stickers.size())
	{
		endIndex = startIndex + numberStickersAtTime;
	}
	else { endIndex = stickers.size(); }

	std::string animeName{ stickers[0].category() };
	// Определение сообщений
	const std::string start =
		MessageText::User::broadcastStickersByAnimeStart(
			animeName,
			startIndex
		);

	InlineKeyboard inlineKeyboard{
		{
			{
				{ "Выбрать новое аниме", "menu_and_delete_message" }
			}
		}
	};

	std::string end{};
	if (endIndex == stickers.size())
	{
		end = MessageText::User::broadcastStickersByAnimeEnd2(animeName);
	}
	else
	{
		end = MessageText::User::broadcastStickersByAnimeEnd1(
				animeName,
				numberStickersAtTime,
				startIndex,
				stickers.size()
		);

		inlineKeyboard.addButtonRow(
			{
				{ "Смотреть ещё стикеры", "more_stickers:"
				+ std::to_string(stickers[endIndex - 1].id())
				+ Core::separationMarker()
				+ std::to_string(endIndex) }
			}
		);
	}

	static const ReplyKeyboard replyKeyboard
	{
		{
			{ "🌸 Выбрать новое аниме" }
		}
	};

	// Отправка сообщений и стикеров
	tgApi_.sendMessage(
		{
			chatId,
			start
		},
		{ replyKeyboard }
	);
	
	for (size_t index{ startIndex }; index < stickers.size()
		&& index != endIndex; ++index)
	{
		const StickerPack& sticker{ stickers[index] };

		sendSticker(chatId, sticker, user);
	}

	tgApi_.sendMessage(
		{
			chatId,
			end
		},
		{ inlineKeyboard }
	);
}

void UserUI::stickerLab(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"📜 Правила загрузки", "get_sticker_upload_rules"}
			}
		}
	};

	if (messageId == 0)
	{
		static const ReplyKeyboard replyKeyboard{
			{
				{"➕ Добавить стикеры"},
				{"≪ На главную"}
			}
		};

		tgApi_.sendRandomSticker(
			chatId,
			replyKeyboard
		);

		tgApi_.sendMessage(
			{
				chatId,
				MessageText::User::stickerLab
			},
			{ inlineKeyboard }
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				MessageText::User::stickerLab
			},
			messageId,
			{ inlineKeyboard }
		);
	}
}

void UserUI::getStickerUploadRules(int64_t chatId, int64_t messageId)
{
	static const InlineKeyboard inlineKeyboard{
		{
			{
				{"≪ Назад", "sticker_lab"}
			}
		}
	};

	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::getStickerUploadRules
		},
		messageId,
		{ inlineKeyboard }
	);
}

void UserUI::applicationAccepted(int64_t chatId)
{
	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::applicationAccepted
		}
	);
}

void UserUI::rejectApplication(
	int64_t chatId,
	const std::string& reason
)
{
	tgApi_.sendMessage(
		{
			chatId,
			MessageText::User::applicationWasRejected(
				reason
			)
		}
	);
}

void UserUI::messageDeletion(int64_t chatId, int64_t messageId)
{
	tgApi_.messageDeletion(chatId, messageId);
}

void UserUI::handleReactionError(int64_t chatId, int64_t messageId)
{
	tgApi_.editMessage(
		{
			chatId,
			MessageText::User::handleReactionError
		},
		messageId
	);
}

void UserUI::sendSticker(
	int64_t chatId,
	const StickerPack& sticker,
	const User& user,
	int64_t messageId
)
{
	static const LinkPreviewOptions linkOptions
	{
		false,
		true,
		false,
		true
	};

	const std::string packId{ std::to_string(sticker.id()) };

	const bool isLiked =
		user.stickerRatings().isLike(sticker.id());
	const bool isDisliked =
		user.stickerRatings().isDislike(sticker.id());

	const std::string likeText =
		std::string(isLiked ? "✔️" : "")
		+ "👍 " + std::to_string(sticker.like());

	const std::string dislikeText =
		std::string(isDisliked ? "✔️" : "")
		+ "👎 " + std::to_string(sticker.dislike());

	const InlineKeyboard buttons{
		{
			{
				{ likeText, "Like:" + packId },
				{ dislikeText, "Dislike:" + packId }
			}
		}
	};

	if (messageId == 0)
	{
		tgApi_.sendMessage(
			{
				chatId,
				"<a href='" + sticker.link() +
				"'>📦 <b>РАСПАКОВАТЬ</b></a>"
			},
			{
				buttons,
				"HTML",
				linkOptions
			}
		);
	}
	else
	{
		tgApi_.editMessage(
			{
				chatId,
				"<a href='" + sticker.link() +
				"'>📦 <b>РАСПАКОВАТЬ</b></a>"
			},
			messageId,
			{
				buttons,
				linkOptions
			}
		);
	}
}