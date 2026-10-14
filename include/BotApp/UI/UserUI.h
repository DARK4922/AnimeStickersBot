#ifndef USERUI_H
#define USERUI_H

#include "TelegramApi.h"
#include "StickerPack.h"
#include "StickerDatabase.h"
#include "Users.h"
#include "User.h"

class UserUI
{
private:
	TelegramApi& tgApi_;

public:
	UserUI(TelegramApi& TgApi);

	// Приветствие
	void start(int64_t chatId, std::string firstName);

	// Главное меню
	void menu(int64_t chatId);

	// Дополнительное меню
	void extraMenu(int64_t chatId);

	// Инфо. канал
	void channelInfo(int64_t chatId);

	// Информация о разработчике
	void developerInfo(int64_t chatId);

	// Тех поддержка
	void support(int64_t chatId);

	// Группа отзывов
	void feedbackGroup(int64_t chatId);

	// Донат
	void donate(int64_t chatId);

	// Информация о рекламе
	void advertising(int64_t chatId, int64_t messageId = 0);

	// Покупка рекламы
	void buyAdvertising(int64_t chatId, int64_t messageId);

	// Правила рекламы
	void advertisingRules(int64_t chatId, int64_t messageId);

	// Помощь
	void help(int64_t chatId);

	// Рандомный стикер
	void randomSticker(int64_t chatId);

	// Список доступных аниме
	void animeList(
		int64_t chatId,
		const std::vector<std::string>& category
	);

	// Доступные аниме по букве
	void getAnimeByLetter(
		int64_t chatId,
		const std::string& letter,
		const std::vector<std::string>& animes
	);

	// Стикеры по выбранному аниме
	void broadcastStickersByAnime(
		int64_t chatId,
		const User& user,
		const std::vector<StickerPack>& stickers,
		size_t startIndex = 0
	);

	// Аниме не найдено
	void notifyAnimeNotFound(int64_t chatId);

	// Лаборатория стикеров
	void stickerLab(int64_t chatId, int64_t messageId = 0);

	// Правила загрузки стикеров
	void getStickerUploadRules(int64_t chatId, int64_t messageId);

	// Заявка на добавление принята
	void applicationAccepted(int64_t chatId);

	// Заявка на добавление отклонена
	void rejectApplication(
		int64_t chatId,
		const std::string& reason
	);

	// Удаление сообщения
	void messageDeletion(int64_t chatId, int64_t messageId);

	// Ошибка при попытке поставить реакцию
	void handleReactionError(int64_t chatId, int64_t messageId);

	// Отправка одного стикера
	void sendSticker(
		int64_t chatId,
		const StickerPack& sticker,
		const User& user,
		int64_t messageId = 0
	);
};

#endif // !USERUI_H
