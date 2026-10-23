#include "FileStorage.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Core.h"

FileStorage::FileStorage(
	std::string stickersFile,
	std::string usersFile,
	int numberMargins
)
	: stickersFile_(std::move(stickersFile))
	, usersFile_(std::move(usersFile))
	, numberMargins_(numberMargins)
{}

bool FileStorage::saveStickers(const std::vector<StickerPack>& stickers, int64_t nextId) const
{
	std::ofstream file(stickersFile_);
	if (!file.is_open())
	{
		Core::logError("Не удалось открыть файл для сохранения стикеров"
			+ stickersFile_);
		return false;
	}

	nlohmann::json j = {
		{ "nextId", nextId },
		{ "stickers", stickers }
	};
	file << j.dump(numberMargins_);

	if (!file)
	{
		Core::logError("Ошибка записи файла со стикерами");
		return false;
	}

	file.close();
	return true;
}

bool FileStorage::saveUsers(const std::vector<User>& users) const
{
	std::ofstream file(usersFile_);
	if (!file.is_open())
	{
		Core::logError(
			"Не удалось открыть файл для сохранения пользователей "
			+ usersFile_
		);
		return false;
	}

	nlohmann::json data = users;
	file << data.dump(numberMargins_);

	if (!file)
	{
		Core::logError("Ошибка записи файла с пользователями");
		return false;
	}

	file.close();
	return true;
}

std::vector<StickerPack> FileStorage::updateStickers(int64_t& nextId) const
{
	nextId = 0;
	std::ifstream file(stickersFile_);
	if (!file.is_open())
	{
		Core::logError("Не удалось открыть файл для обновления стикеров"
			+ stickersFile_);
		return {};
	}

	try
	{
		nlohmann::json data;
		file >> data;

		// Старые файлы содержат только массив стикерпаков.
		const auto stickers{ (data.is_array() ? data : data.at("stickers"))
			.get<std::vector<StickerPack>>() };
		int64_t savedNextId{ data.is_array() ? 1 : data.at("nextId").get<int64_t>() };
		if (savedNextId < 1
			|| savedNextId > static_cast<int64_t>(std::numeric_limits<int>::max()) + 1)
		{
			Core::logError("Некорректный счётчик ID стикерпаков в файле");
			return {};
		}
		for (const auto& sticker : stickers)
		{
			savedNextId = std::max(savedNextId, static_cast<int64_t>(sticker.id()) + 1);
		}
		nextId = savedNextId;
		return stickers;
	}
	catch (const nlohmann::json::exception& e)
	{
		Core::logError(
			"Ошибка при чтении файла стикеров: "
			+ std::string(e.what())
		);
		return {};
	}
}

std::vector<User> FileStorage::updateUsers() const
{
	std::ifstream file(usersFile_);
	if (!file.is_open())
	{
		Core::logError("Не удалось открыть файл для обновления пользователей"
			+ usersFile_);
		return {};
	}

	try
	{
		nlohmann::json data;
		file >> data;

		return data.get<std::vector<User>>();
	}
	catch (const nlohmann::json::exception& e)
	{
		Core::logError(
			"Ошибка при чтении файла пользователей: "
			+ std::string(e.what())
		);
		return {};
	}
}
