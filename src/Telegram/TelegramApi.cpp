#include "TelegramApi.h"
#include <iostream>
#include "Core.h"

TelegramApi::Message::Message(
	int64_t id,
	std::string text
)
	: chatId(id)
	, text(std::move(text))
{}

TelegramApi::TelegramApi(TgBot::Bot& bot)
	: bot_(bot) {}

bool TelegramApi::connectionStatus()
{
	std::cout << "Бот запущен! Проверяем подключение..." << std::endl;
	try
	{
		std::cout << "Имя бота: "
			<< bot_.getApi().getMe()->username << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		Core::logError("Ошибка подключения к Telegram: "
			+ std::string(e.what()));
		return false;
	}
}

TgBot::KeyboardButton::Ptr TelegramApi::makeReplyButton(const std::string& text)
{
	auto button{ std::make_shared<TgBot::KeyboardButton>() };
	button->text = text;
	return button;
}

TgBot::ReplyKeyboardMarkup::Ptr TelegramApi::makeReplyKeyboard(
	const std::vector<std::vector<std::string>>& layout)
{
	auto keyboard{ std::make_shared<TgBot::ReplyKeyboardMarkup>() };
	keyboard->resizeKeyboard = true;

	for (const auto& row : layout)
	{
		std::vector<TgBot::KeyboardButton::Ptr> buttons;
		buttons.reserve(row.size());

		for (const auto& text : row)
		{
			buttons.push_back(makeReplyButton(text));
		}

		keyboard->keyboard.push_back(std::move(buttons));
	}

	return keyboard;
}

TgBot::InlineKeyboardButton::Ptr TelegramApi::makeInlineButton(
	const InlineKeyboard::ButtonInfo& button
)
{
	auto tgBotButton{ std::make_shared<TgBot::InlineKeyboardButton>() };
	tgBotButton->text = button.text;

	// Проверка, является ли строка ссылкой
	if (Core::isLink(button.callbackData))
	{ tgBotButton->url = button.callbackData; }
	else { tgBotButton->callbackData = button.callbackData; }

	return tgBotButton;
}

TgBot::InlineKeyboardMarkup::Ptr TelegramApi::makeInlineKeyboard(
	const std::vector<std::vector<InlineKeyboard::ButtonInfo>>&
	layout
)
{
	auto keyboard{ std::make_shared<TgBot::InlineKeyboardMarkup>() };

	for (const auto& row : layout)
	{
		std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
		buttons.reserve(row.size());

		for (const auto& button : row)
		{
			buttons.push_back(makeInlineButton(button));
		}

		keyboard->inlineKeyboard.push_back(std::move(buttons));
	}

	return keyboard;
}

bool TelegramApi::sendMessage(
	const Message& message,
	const SendMessageOptions& options
)
{
	auto linkPreviewOptions = options.linkOptions
		? toLinkPreviewOptions(*options.linkOptions)
		: nullptr;

	auto keyboard = options.keyboard
		? toGenericReply(*options.keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendMessage(
			message.chatId,
			message.text,
			linkPreviewOptions,
			{},
			keyboard,
			options.parseMode
		);

		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при отправке сообщения: "
			+ std::string(e.what())
		);
	}
	catch (...) { Core::logError("Неизвестная ошибка при отправке сообщения"); }

	return false;
}

bool TelegramApi::sendPhoto(
	const Message& message,
	const std::string& photo,
	const MessageOptions& options
)
{
	auto keyboard = options.keyboard
		? toGenericReply(*options.keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendPhoto(
			message.chatId,
			TgBot::InputFile::fromFile(photo, "image/jpeg"),
			message.text,
			{},
			keyboard,
			options.parseMode
		);

		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при отправке фото: "
			+ std::string(e.what())
		);
	}
	catch (const std::exception& e)
	{
		Core::logError(
			"Системная ошибка при отправке фото: "
			+ std::string(e.what())
		);
	}

	return false;
}

bool TelegramApi::sendPhotoId(
	const Message& message,
	const std::string& photo,
	const MessageOptions& options
)
{
	auto keyboard = options.keyboard
		? toGenericReply(*options.keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendPhoto(
			message.chatId,
			photo,
			message.text,
			{},
			keyboard,
			options.parseMode
		);

		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при отправке фото: "
			+ std::string(e.what())
		);
	}
	catch (const std::exception& e)
	{
		Core::logError(
			"Системная ошибка при отправке фото: "
			+ std::string(e.what())
		);
	}

	return false;
}

bool TelegramApi::sendMessageWithBlockCheck(
	const Message& message,
	const SendMessageOptions& options
)
{
	auto linkPreviewOptions = options.linkOptions
		? toLinkPreviewOptions(*options.linkOptions)
		: nullptr;

	auto keyboard = options.keyboard
		? toGenericReply(*options.keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendMessage(
			message.chatId,
			message.text,
			linkPreviewOptions,
			{},
			keyboard,
			options.parseMode
		);
	}
	catch (const TgBot::TgException& e)
	{
		const std::string error = e.what();

		// Пользователь заблокировал бота
		if (e.errorCode == TgBot::TgException::ErrorCode::Forbidden
			&& error.find("bot was blocked by the user") != std::string::npos
			)
		{
			return false;
		}

		// Остальные ошибки Telegram API
		Core::logError(
			"Ошибка при отправке сообщения: " + error
		);
	}
	catch (const std::exception& e)
	{
		Core::logError(
			"Системная ошибка при отправке сообщения: "
			+ std::string(e.what())
		);
	}
	catch (...)
	{
		Core::logError(
			"Неизвестная ошибка при отправке сообщения"
		);
	}

	return true;
}

bool TelegramApi::sendPhotoIdWithBlockCheck(
	const Message& message,
	const std::string& photo,
	const MessageOptions& options
)
{
	auto keyboard = options.keyboard
		? toGenericReply(*options.keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendPhoto(
			message.chatId,
			photo,
			message.text,
			{},
			keyboard,
			options.parseMode
		);
	}
	catch (const TgBot::TgException& e)
	{
		const std::string error = e.what();

		// Пользователь заблокировал бота
		if (e.errorCode == TgBot::TgException::ErrorCode::Forbidden
			&& error.find("bot was blocked by the user") != std::string::npos)
		{
			return false;
		}

		Core::logError(
			"Ошибка при отправке фото: " + error
		);
	}
	catch (const std::exception& e)
	{
		Core::logError(
			"Системная ошибка при отправке фото: "
			+ std::string(e.what())
		);
	}
	catch (...)
	{
		Core::logError(
			"Неизвестная ошибка при отправке фото"
		);
	}

	return true;
}

bool TelegramApi::sendRandomPhoto(
	const Message& message,
	const MessageOptions& options
)
{
	return sendPhoto(
		message,
		Core::randomPhoto(),
		options
	);
}

bool TelegramApi::sendSticker(
	int64_t chatId,
	const std::string& sticker,
	const std::optional<Keyboard>& keyboard
)
{
	auto tgKeyboard = keyboard
		? toGenericReply(*keyboard)
		: nullptr;

	try
	{
		bot_.getApi().sendSticker(
			chatId,
			sticker,
			{},
			tgKeyboard
		);

		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при отправке стикера: "
			+ std::string(e.what())
		);
	}
	catch (...) { Core::logError("Неизвестная ошибка при отправке стикера"); }

	return false;
}

bool TelegramApi::sendRandomSticker(
	int64_t chatId,
	const std::optional<Keyboard>& keyboard
)
{
	const auto& stickers = Core::getBackgroundStickers();

	return sendSticker(
		chatId,
		stickers[Core::randomNumber(0, stickers.size())],
		keyboard
	);
}

bool TelegramApi::editMessage(
	const Message& message,
	int64_t messageId,
	const EditMessageOptions& options
)
{
	auto linkPreviewOptions = options.linkOptions
		? toLinkPreviewOptions(*options.linkOptions)
		: nullptr;
	auto inlineKeyboard = options.keyboard
		? makeInlineKeyboard(options.keyboard->buttons())
		: nullptr;

	try
	{
		bot_.getApi().editMessageText(
			message.text,
			message.chatId,
			messageId,
			"",
			options.parseMode,
			linkPreviewOptions,
			inlineKeyboard
		);

		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при изменении сообщения: "
			+ std::string(e.what())
		);
	}
	catch (const std::exception& e)
	{
		Core::logError(
			"Неизвестная ошибка при изменении сообщения: "
			+ std::string(e.what())
		);
	}

	return false;
}

TgBot::GenericReply::Ptr TelegramApi::toGenericReply(
	const Keyboard& keyboard
)
{
	TgBot::GenericReply::Ptr tgBotKeyboard;

	if (std::holds_alternative<InlineKeyboard>(keyboard))
	{
		tgBotKeyboard = makeInlineKeyboard(
			std::get<InlineKeyboard>(keyboard).buttons()
		);
	}
	else if (std::holds_alternative<ReplyKeyboard>(keyboard))
	{
		tgBotKeyboard = makeReplyKeyboard(
			std::get<ReplyKeyboard>(keyboard).buttons()
		);
	}

	return tgBotKeyboard;
}

TgBot::LinkPreviewOptions::Ptr TelegramApi::toLinkPreviewOptions(
	const LinkPreviewOptions& options
)
{
	auto linkOptions{ std::make_shared<TgBot::LinkPreviewOptions>() };

	linkOptions->isDisabled = options.isDisabled;
	linkOptions->preferLargeMedia = options.preferLargeMedia;
	linkOptions->preferSmallMedia = options.preferSmallMedia;
	linkOptions->showAboveText = options.showAboveText;
	linkOptions->url = options.url;

	return linkOptions;
}

TelegramApi::InlineKeyboard::InlineKeyboard(
	std::vector<std::vector<ButtonInfo>> buttons
)
	: buttons_(std::move(buttons))
{}

TelegramApi::ReplyKeyboard::ReplyKeyboard(
	std::vector<std::vector<std::string>> buttons
)
	: buttons_(std::move(buttons))
{}

void TelegramApi::InlineKeyboard::addButtonRow(const std::vector<ButtonInfo>& button)
{
	buttons_.push_back(button);
}

void TelegramApi::InlineKeyboard::addButtonRows(
	const std::vector<std::vector<ButtonInfo>>& buttons
)
{
	for (const auto& row : buttons)
	{
		buttons_.push_back(row);
	}
}

const std::vector<std::vector<TelegramApi::InlineKeyboard::ButtonInfo>>&
TelegramApi::InlineKeyboard::buttons() const
{ return buttons_; }

void TelegramApi::ReplyKeyboard::addButtonRow(std::vector<std::string> button)
{
	buttons_.push_back(std::move(button));
}

void TelegramApi::ReplyKeyboard::addButtonRows(
	std::vector<std::vector<std::string>> buttons
)
{
	for (auto& row : buttons)
	{
		buttons_.push_back(std::move(row));
	}
}

const std::vector<std::vector<std::string>>& TelegramApi::ReplyKeyboard::buttons() const
{ return buttons_; }

bool TelegramApi::messageDeletion(int64_t chatId, int64_t messageId)
{
	try
	{
		bot_.getApi().deleteMessage(chatId, messageId);
		return true;
	}
	catch (const TgBot::TgException& e)
	{
		Core::logError(
			"Ошибка при удалении сообщения: "
			+ std::string(e.what())
		);
	}
	catch (...)
	{
		Core::logError(
			"Неизвестная ошибка при удалении сообщения"
		);
	}

	return false;
}