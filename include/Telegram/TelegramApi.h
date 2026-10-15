#ifndef TELEGRAMAPI_H
#define TELEGRAMAPI_H

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <tgbot/tgbot.h>

class TelegramApi
{
public:
	class InlineKeyboard
	{
	public:
		struct ButtonInfo
		{
			std::string text{};
			std::string callbackData{};
		};

	private:
		std::vector<std::vector<ButtonInfo>> buttons_;

	public:
		InlineKeyboard() = default;
		InlineKeyboard(
			std::vector<std::vector<ButtonInfo>> buttons
		);

		void addButtonRow(const std::vector<ButtonInfo>& button);
		void addButtonRows(
			const std::vector<std::vector<ButtonInfo>>& buttons
		);

		const std::vector<std::vector<ButtonInfo>>& buttons() const;
	};

	class ReplyKeyboard
	{
	private:
		std::vector<std::vector<std::string>> buttons_;

	public:
		ReplyKeyboard() = default;
		ReplyKeyboard(
			std::vector<std::vector<std::string>> buttons
		);

		void addButtonRow(std::vector<std::string> button);
		void addButtonRows(
			std::vector<std::vector<std::string>> buttons
		);

		const std::vector<std::vector<std::string>>& buttons() const;
	};

	using Keyboard = std::variant<
		InlineKeyboard,
		ReplyKeyboard
	>;

	struct Message
	{
		int64_t chatId{};
		std::string text{};

		Message(
			int64_t id,
			std::string text
		);
	};

	struct LinkPreviewOptions
	{
		bool isDisabled{ false };
		bool preferSmallMedia{ false };
		bool preferLargeMedia{ false };
		bool showAboveText{ false };
		std::string url{};
	};

	struct MessageOptions
	{
		std::optional<Keyboard> keyboard{};
		std::string parseMode{ "HTML" };
	};

	struct SendMessageOptions : MessageOptions
	{
		std::optional<LinkPreviewOptions> linkOptions{};
	};

	struct EditMessageOptions
	{
		std::optional<InlineKeyboard> keyboard{};
		std::optional<LinkPreviewOptions> linkOptions{};
		std::string parseMode{ "HTML" };
	};

private:
	TgBot::Bot& bot_;

	// Конвертация в TgBot::GenericReply::Ptr
	TgBot::GenericReply::Ptr toGenericReply(
		const Keyboard& keyboard
	);

	// Конвертация в TgBot::LinkPreviewOptions::Ptr
	TgBot::LinkPreviewOptions::Ptr toLinkPreviewOptions(
		const LinkPreviewOptions& options
	);

	// Создание KeyboardReplyButton
	TgBot::KeyboardButton::Ptr makeReplyButton(const std::string& text);
	TgBot::ReplyKeyboardMarkup::Ptr makeReplyKeyboard(
		const std::vector<std::vector<std::string>>& layout);

	// Создание InlineKeyboardButton
	TgBot::InlineKeyboardButton::Ptr makeInlineButton(
		const InlineKeyboard::ButtonInfo& button
	);
	TgBot::InlineKeyboardMarkup::Ptr makeInlineKeyboard(
		const std::vector<std::vector<InlineKeyboard::ButtonInfo>>&
		layout
	);

public:
	TelegramApi(TgBot::Bot& bot);

	// Проверка на подключение
	bool connectionStatus();

	// Отправка сообщения
	bool sendMessage(
		const Message& message,
		const SendMessageOptions& options =
			SendMessageOptions{
				MessageOptions{ std::nullopt, "HTML" },
				std::nullopt
		}
	);

	// Отправка фото
	bool sendPhoto(
		const Message& message,
		const std::string& photo,
		const MessageOptions& options =
			MessageOptions{ std::nullopt, "HTML" }
	);
	bool sendPhotoId(
		const Message& message,
		const std::string& photo,
		const MessageOptions& options =
			MessageOptions{ std::nullopt, "HTML" }
	);

	// Отправка сообщений с проверкой на блокировку
	bool sendMessageWithBlockCheck(
		const Message& message,
		const SendMessageOptions& options =
			SendMessageOptions{
				MessageOptions{ std::nullopt, "HTML" },
				std::nullopt
		}
	);
	bool sendPhotoIdWithBlockCheck(
		const Message& message,
		const std::string& photo,
		const MessageOptions& options =
			MessageOptions{ std::nullopt, "HTML" }
	);

	// Отправка рандомного фото
	bool sendRandomPhoto(
		const Message& message,
		const MessageOptions& options =
			MessageOptions{ std::nullopt, "HTML" }
	);

	// Отправка стикера
	bool sendSticker(
		int64_t chatId,
		const std::string& sticker,
		const std::optional<Keyboard>& keyboard = {}
	);

	// Отправка рандомного стикера
	bool sendRandomSticker(
		int64_t chatId,
		const std::optional<Keyboard>& keyboard = {}
	);

	// Изменение сообщения
	bool editMessage(
		const Message& message,
		int64_t messageId,
		const EditMessageOptions& options =
			EditMessageOptions{ std::nullopt, std::nullopt, "HTML" }
	);

	// Удаление сообщения
	bool messageDeletion(int64_t chatId, int64_t messageId);
};

#endif // !TELEGRAMAPI_H
