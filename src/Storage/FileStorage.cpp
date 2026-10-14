#include "FileStorage.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
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

bool FileStorage::saveStickers(const std::vector<StickerPack>& stickers) const
{
	std::ofstream file(stickersFile_);
	if (!file.is_open())
	{
		Core::logError("Не удалось открыть файл для сохранения стикеров"
			+ stickersFile_);
		return false;
	}

	nlohmann::json j = stickers;
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

std::vector<StickerPack> FileStorage::updateStickers() const
{
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

		return data.get<std::vector<StickerPack>>();
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