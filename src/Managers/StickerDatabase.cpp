#include "StickerDatabase.h"

void StickerDatabase::addSticker(StickerPack sticker)
{
	ensureCategory(sticker.category());

	stickers_.addSticker(std::move(sticker));
}

void StickerDatabase::addSticker(
	std::string category,
	std::string link,
	int64_t creatorId,
	std::string creatorName
)
{
	ensureCategory(category);

	stickers_.addSticker(
		std::move(category),
		std::move(link),
		creatorId,
		std::move(creatorName)
	);
}

void StickerDatabase::addCategory(std::string category)
{
	categories_.addCategory(std::move(category));
}

const CategoryManager& StickerDatabase::categories() const { return categories_; }
const StickerPackManager& StickerDatabase::stickers() const { return stickers_; }

void StickerDatabase::addStickers(std::vector<StickerPack> stickers)
{
	for (auto& sticker : stickers)
	{
		if (!categories_.exists(sticker.category()))
			{ categories_.addCategory(sticker.category()); }

		stickers_.addSticker(std::move(sticker));
	}
}

void StickerDatabase::ensureCategory(const std::string& category)
{
	if (!categories_.exists(category))
		{ categories_.addCategory(category); }
}

void StickerDatabase::clearStickers()
{
	categories_.clear();
	stickers_.clearStickers();
}

bool StickerDatabase::removeSticker(int id, Users& users)
{
	if (!stickers_.removeSticker(id)) { return false; }
	users.removeStickerRatings(id);
	return true;
}

void StickerDatabase::ensureNextId(int64_t nextId)
{
	stickers_.ensureNextId(nextId);
}

bool StickerDatabase::changeReaction(
    int stickerId,
    int64_t userId,
    Users& users,
    ReactionType reaction
)
{
    bool stickerFound{};
    bool userFound{};
    bool invalidReactionState{};

    stickers_.withSticker(
        stickerId,
        [&](StickerPack& sticker)
        {
            stickerFound = true;

            users.withUser(
                userId,
                [&](User& user)
                {
                    userFound = true;

                    const bool hasLike{
                        user.stickerRatings().isLike(stickerId)
                    };

                    const bool hasDislike{
                        user.stickerRatings().isDislike(stickerId)
                    };

                    // Проверка целостности состояния
                    if (hasLike && hasDislike)
                    {
                        invalidReactionState = true;
                        return;
                    }

                    // Проверка переданной реакции
                    if (reaction != ReactionType::Like
                        && reaction != ReactionType::Dislike)
                    {
                        invalidReactionState = true;
                        return;
                    }

                    const auto removeLike = [&]
                        {
                            user.removeLike(stickerId);
                            sticker.removeLike();
                        };

                    const auto removeDislike = [&]
                        {
                            user.removeDislike(stickerId);
                            sticker.removeDislike();
                        };

                    const auto addLike = [&]
                        {
                            user.addLike(stickerId);
                            sticker.addLike();
                        };

                    const auto addDislike = [&]
                        {
                            user.addDislike(stickerId);
                            sticker.addDislike();
                        };

                    if (reaction == ReactionType::Like)
                    {
                        if (hasLike)
                        {
                            // Повторное нажатие снимает лайк
                            removeLike();
                        }
                        else
                        {
                            // При необходимости удаляем дизлайк
                            if (hasDislike)
                            {
                                removeDislike();
                            }

                            addLike();
                        }
                    }
                    else
                    {
                        if (hasDislike)
                        {
                            // Повторное нажатие снимает дизлайк
                            removeDislike();
                        }
                        else
                        {
                            // При необходимости удаляем лайк
                            if (hasLike)
                            {
                                removeLike();
                            }

                            addDislike();
                        }
                    }
                }
            );
        }
    );

    if (!stickerFound)
    {
        Core::logError(
            "Попытка изменить реакцию у несуществующего стикерпака"
        );

        return false;
    }

    if (!userFound)
    {
        Core::logError(
            "Попытка изменить реакцию от несуществующего пользователя"
        );

        return false;
    }

    if (invalidReactionState)
    {
        Core::logError(
            "Некорректное состояние реакции пользователя"
        );

        return false;
    }

    return true;
}
