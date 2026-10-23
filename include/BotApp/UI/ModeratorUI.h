#ifndef MODERATORUI_H
#define MODERATORUI_H

#include "TelegramApi.h"
#include "ApplicationManager.h"
#include "UserUI.h"

class StickerDatabase;
class Users;

class ModeratorUI
{
private:
	TelegramApi& tgApi_;
	ApplicationManager& applications_;

	// Проверка, является ли пользователь модератором
	bool isModerator(int64_t chatId, Users& users) const;

public:
	ModeratorUI(
		TelegramApi& TgApi,
		ApplicationManager& applications
	);

	// Подтверждение удаления стикерпака
	void confirmStickerRemoval(int64_t chatId, int64_t messageId, int stickerId);

	// Стикерпак удалён
	void onStickerRemoved(int64_t chatId, int64_t messageId);

	// Получение заявки
	void getApplication(
		int64_t chatId,
		Users& users,
		int64_t messageId = 0
	);

	// Получение причины отказа
	void getRejectionReason(
		int64_t chatId,
		int64_t messageId,
		Users& users
	);

	// Одобрение заявки
	void approveApplication(
		int64_t chatId,
		int64_t messageId,
		StickerDatabase& stickers,
		UserUI& userUI
	);

	// Отказ заявки
	void applicationRejected(
		int64_t chatId,
		UserUI& userUI,
		std::string reasonForRefusal
	);
};

#endif // !MODERATORUI_H
