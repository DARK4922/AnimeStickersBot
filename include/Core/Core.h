#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <format>
#include <chrono>
#include <climits>
#include <unordered_set>

namespace Core
{
	// Вывод ошибок
	void logError(const std::string& errorText);

	// Получение даты
	std::string getDate();

	// Преобразование даты в стороку
	std::string formatDate(const std::chrono::system_clock::time_point& time);

	// Преобразование строки в дату
	std::chrono::system_clock::time_point ConvertStrToDate(const std::string& str);

	// Проверка, является ли строка ссылкой
	bool isLink(const std::string& str);

	// Получение случайного фото
	std::string randomPhoto();

	// Получение случайного числа
	int randomNumber(
		int min = INT_MIN,
		int max = INT_MAX
	);

	// Получение фоновых стикеров
	const std::vector<std::string>& getBackgroundStickers();

	// Получение букв аниме
	const std::unordered_set<std::string>& getAnimeLetters();

	// Маркер аниме
	const std::string animeMarker();

	// Маркер разделений
	const std::string& separationMarker();

	// Создание ссылки по стикеру
	std::string createStickerLink(const std::string& stickerName);
}

#endif // !CORE_H
