#include "StickerRatings.h"

StickerRatings::StickerRatings(
	std::unordered_set<int> likes,
	std::unordered_set<int> dislikes
)
	: likes_(std::move(likes))
	, dislikes_(std::move(dislikes))
{}

const std::unordered_set<int>& StickerRatings::likes() const
{
	return likes_;
}

const std::unordered_set<int>& StickerRatings::dislikes() const
{
	return dislikes_;
}

bool StickerRatings::isLike(int sticker) const { return likes_.contains(sticker); }

bool StickerRatings::isDislike(int sticker) const { return dislikes_.contains(sticker); }

void StickerRatings::addLike(int sticker)
{
	dislikes_.erase(sticker);
	likes_.insert(sticker);
}

void StickerRatings::addDislike(int sticker)
{
	likes_.erase(sticker);
	dislikes_.insert(sticker);
}

void StickerRatings::removeLike(int sticker)
{
	likes_.erase(sticker);
}

void StickerRatings::removeDislike(int sticker)
{
	dislikes_.erase(sticker);
}