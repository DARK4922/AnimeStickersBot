#ifndef STICKERPACK_H
#define STICKERPACK_H

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include "UserInfo.h"
#include "Core.h"

class StickerPack
{
public:
	using Time = std::chrono::system_clock::time_point;

private:
	// Данные стикерпака
	int id_{};
	std::string category_{};
	std::string link_{};
	Time dateAdded_{};

	// Оценки пользователей
	unsigned int like_{};
	unsigned int dislike_{};

	// Данные создателя
	UserInfo creatorInfo_;

public:
    StickerPack() = default;

	StickerPack(int id,
		std::string category,
		std::string link,
		Time time = {},
		unsigned int like = 0,
		unsigned int dislike = 0,
		int64_t creatorId = 0,
		std::string creatorName = {});

	bool operator<(const StickerPack& other) const;
	bool operator==(const StickerPack& other) const;

	// Геттеры
	int id() const;
	const std::string& category() const;
	const std::string& link() const;
	const Time& dateAdded() const;

	unsigned int like() const;
	unsigned int dislike() const;

	const UserInfo& creatorInfo() const;

	// Управление реакциями
	void addLike();
	void addDislike();
	void removeLike();
	void removeDislike();
};

namespace nlohmann
{
	template<>
	struct adl_serializer<StickerPack>
	{
		static StickerPack from_json(const json& j)
		{
			return StickerPack(
				j.at("id").get<int>(),
				j.at("category").get<std::string>(),
				j.at("link").get<std::string>(),
				Core::ConvertStrToDate(j.at("time").get<std::string>()),
				j.at("like").get<unsigned int>(),
				j.at("dislike").get<unsigned int>(),
				j.at("creatorInfo").at("id").get<int64_t>(),
				j.at("creatorInfo").at("username").get<std::string>()
			);
		}

		static void to_json(json& j, const StickerPack& pack)
		{
			j = {
				{"id", pack.id()},
				{"category", pack.category()},
				{"link", pack.link()},
				{"time", Core::formatDate(pack.dateAdded())},
				{"like", pack.like()},
				{"dislike", pack.dislike()},
				{"creatorInfo", pack.creatorInfo()}
			};
		}
	};
}

#endif // !STICKERPACK_H