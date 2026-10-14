#ifndef APPLICATIONUI_H
#define APPLICATIONUI_H

#include "TelegramApi.h"
#include "Users.h"
#include "ApplicationManager.h"

class ApplicationUI
{
private:
	TelegramApi& tgApi_;
	Users& users_;

	// Проверка на актуальность заявки
	bool isApplicationUpToDate(int64_t chatId, int64_t messageId);

public:
	ApplicationUI(
		TelegramApi& tgApi,
		Users& users
	);

	// Получение названия категории
	void getCategoryName(int64_t chatId);

	// Меню добавления стикеров
	void addStickersMenu(
		int64_t chatId,
		int64_t messageId = 0
	);

	// Начало многократной отправки ссылок
	void startMultipleLinkSubmission(int64_t chatId, int64_t messageId);

	// Стикер добавлен
	void notifyStickerAdded(int64_t chatId);

	// Стикер был добавлен ранее
	void notifyStickerAlreadyAdded(int64_t chatId);

	// Получение одного стикера
	void processSingleSticker(int64_t chatId, int64_t messageId);

	// Показ заявки перед отправкой
	void applicationPreview(int64_t chatId, int64_t messageId);

	// Отправка заявки на проверку
	void sendApplicationForReview(
		int64_t chatId,
		int64_t messageId,
		ApplicationManager& applications
	);

	// Ссылка была добавлена другим пользователем
	void notifyStickerAddedByAnotherUser(int64_t chatId);
};

#endif // !APPLICATIONUI_H
