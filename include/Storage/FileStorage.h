#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>
#include <cstdint>
#include "User.h"
#include "StickerPack.h"

class FileStorage
{
private:
	int numberMargins_{};
	std::string stickersFile_{};
	std::string usersFile_{};

public:
	FileStorage(
		std::string stickersFile,
		std::string usersFile,
		int numberMargins = 4
	);

	// Методы сохранения
	bool saveStickers(const std::vector<StickerPack>& stickers, int64_t nextId) const;
	bool saveUsers(const std::vector<User>& users) const;

	// Методы обновления
	std::vector<StickerPack> updateStickers(int64_t& nextId) const;
	std::vector<User> updateUsers() const;
};

#endif // !FILESTORAGE_H
