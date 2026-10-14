#ifndef STICKERDATABASE_H
#define STICKERDATABASE_H

#include <string>
#include <vector>

#include "StickerPack.h"
#include "CategoryManager.h"
#include "StickerPackManager.h"
#include "Users.h"

class StickerDatabase
{
private:
	CategoryManager categories_;
	StickerPackManager stickers_;

public:
	enum class ReactionType
	{
		Like,
		Dislike
	};

	void addSticker(StickerPack sticker);
	void addSticker(
		std::string category,
		std::string link,
		int64_t creatorId = 0,
		std::string creatorName = {}
	);
	void addStickers(std::vector<StickerPack> stickers);
	void addCategory(std::string category);

	void clearStickers();

	const CategoryManager& categories() const;
	const StickerPackManager& stickers() const;

	void ensureCategory(const std::string& category);

	bool changeReaction(
		int stickerId,
		int64_t userId,
		Users& users,
		ReactionType reaction
	);

	// Доступ к стикеру
	template<typename Func>
	bool withSticker(int id, Func&& func)
	{
		return stickers_.withSticker(id, std::forward<Func>(func));
	}
};

#endif // !STICKERDATABASE_H
