#ifndef STICKERAPPLICATION_H
#define STICKERAPPLICATION_H

#include <string>
#include <unordered_set>
#include "UserInfo.h"

class StickerApplication
{
private:
	std::string category_{};
	std::unordered_set<std::string> links_;

	UserInfo creatorInfo_;

public:
	StickerApplication(
		int64_t creatorId,
		std::string creatorName
	);
	StickerApplication(int64_t creatorId);

	// Геттеры
	const UserInfo& creatorInfo() const;
	const std::string& category() const;
	const std::unordered_set<std::string>& links() const;

	// Сеттеры
	void setCategory(std::string newCategory);

	// Управление ссылками
	bool addLink(const std::string& link);
	void removeLink(const std::string& link);
	bool exists(const std::string& link) const;
};

#endif // !STICKERAPPLICATION_H
