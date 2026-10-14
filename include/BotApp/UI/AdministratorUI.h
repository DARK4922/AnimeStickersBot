#ifndef ADMINISTRATORUI_H
#define ADMINISTRATORUI_H

#include "TelegramApi.h"
#include "Users.h"
#include "StickerDatabase.h"
#include "FileStorage.h"
#include "Database.h"
#include "AdvertisementManager.h"

class AdministratorUI
{
private:
	TelegramApi& tgApi_;
	Users& users_;
	StickerDatabase& stickers_;
	FileStorage& fileStorage_;
	AdvertisementManager& advertisementManager_;

public:
	AdministratorUI(
		TelegramApi& TgApi,
		Users& users,
		StickerDatabase& stickers,
		FileStorage& fileStorage,
		AdvertisementManager& advertisementManager
	);

	// Проверка доступа
	bool accessVerification(int64_t chatId);

	// Доступ ограничен
	void accessRestricted(int64_t chatId);

	// Меню администратора
	void menu(int64_t chatId, int64_t messageId = 0);

	// Количество пользователей
	void numberOfUsers(int64_t chatId);

	// Подробная статистика
	void advancedStatistics(
		int64_t chatId,
		size_t applicationsSize,
		int64_t messageId = 0
	);

	// Работа с файлом
	void saveAllDataToFile(int64_t chatId);
	void saveUserToFile(int64_t chatId);
	void saveStickersToFile(int64_t chatId);

	void updateAllDataFromFile(int64_t chatId);
	void updateUserFromFile(int64_t chatId);
	void updateStickersFromFile(int64_t chatId);

	// Работа с БД
	void saveAllDataToDB(int64_t chatId, Database& db);
	void saveUserToDB(int64_t chatId, Database& db);
	void saveStickersToDB(int64_t chatId, Database& db);

	void updateAllDataFromDB(int64_t chatId, Database& db);
	void updateUserFromDB(int64_t chatId, Database& db);
	void updateStickersFromDB(int64_t chatId, Database& db);

	// Подтверждение действия
	void confirmationAction(
		int64_t chatId,
		const std::string& action,
		int64_t messageId = 0
	);
	// Отмена действия
	void revocationTheAction(int64_t chatId, int64_t messageId);

	// Меню сохранения данных
	void saveMenu(int64_t chatId, int64_t messageId);

	// Меню обновления данных
	void updateMenu(int64_t chatId, int64_t messageId);

	// Меню рекламы
	void advertisingMenu(int64_t chatId, int64_t messageId = 0);

	// Получение рекламного сообщения
	void getAdvertisingMessage(int64_t chatId, int64_t messageId);

	// Получение рекламного фото
	void receiveAdvertisingPhoto(int64_t chatId, int64_t messageId);

	// Удаление рекламного поста
	void deleteAdvertisingPost(int64_t chatId, int64_t messageId);

	// Заявка пропала
	void applicationDisappeared(int64_t chatId, int64_t messageId = 0);

	// Предпросмотр поста
	void postPreview(int64_t chatId);

	// Подтверждение отправки рекламы
	void confirmAdvertisement(int64_t chatId, int64_t messageId);

	// Рассылка рекламы
	void sendAdvertisementToAllUsers(int64_t chatId, int64_t messageId);
};

#endif // !ADMINISTRATOR_H
