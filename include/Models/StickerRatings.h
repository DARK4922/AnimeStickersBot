#ifndef STICKERRATINGS_H
#define STICKERRATINGS_H

#include <unordered_set>
#include <nlohmann/json.hpp>

class StickerRatings
{
private:
	std::unordered_set<int> likes_{};
	std::unordered_set<int> dislikes_{};

public:
	StickerRatings(
		std::unordered_set<int> likes = {},
		std::unordered_set<int> dislikes = {}
	);
	
	// Геттеры
	const std::unordered_set<int>& likes() const;
	const std::unordered_set<int>& dislikes() const;

	// Управление оценками
	bool isLike(int sticker) const;
	bool isDislike(int sticker) const;
	void addLike(int sticker);
	void addDislike(int sticker);
	void removeLike(int sticker);
	void removeDislike(int sticker);
};

namespace nlohmann
{
	template<>
	struct adl_serializer<StickerRatings>
	{
		static StickerRatings from_json(const json& j)
		{
			return StickerRatings(
				j.at("likes").get<std::unordered_set<int>>(),
				j.at("dislikes").get<std::unordered_set<int>>()
			);
		}

		static void to_json(json& j, const StickerRatings& ratings)
		{
			j = {
				{"likes", ratings.likes()},
				{"dislikes", ratings.dislikes()}
			};
		}
	};
}

#endif // !STICKERRATINGS_H
