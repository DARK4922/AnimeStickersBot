#ifndef TEXTHANDLERS_H
#define TEXTHANDLERS_H

#include <string>
#include "Context.h"
#include "AdvertisementManager.h"

namespace TextHandlers
{
	// Стандартные команды
	void textHandlers(
		int64_t chatId,
		const std::string& messageText,
		Context& ctx
	);

	// Сторонний вод пользователя
	void handleUserInput(
		int64_t chatId,
		const std::string& messageText,
		Context& ctx
	);

	// Сторонний ввод модератора/администратора
	void handleStaffInput(
		int64_t chatId,
		const std::string& messageText,
		Context& ctx,
		AdvertisementManager& advertisementManager
	);
}

#endif // !TEXTHANDLERS_H
