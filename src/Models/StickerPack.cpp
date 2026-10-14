#include "StickerPack.h"
#include <iostream>

void test()
{
	std::cout << typeid(nlohmann::json).name() << '\n';
}

StickerPack::StickerPack(int id,
	std::string category,
	std::string link,
	Time time,
	unsigned int like,
	unsigned int dislike,
	int64_t creatorId,
	std::string creatorName)
	: id_(id)
	, category_(std::move(category))
	, link_(std::move(link))
	, dateAdded_(time == Time{} ? std::chrono::system_clock::now() : time)
	, like_(like)
	, dislike_(dislike)
	, creatorInfo_(creatorId, creatorName)
{}

bool StickerPack::operator<(const StickerPack& other) const
{
	// Вычисление средней оценки
	int averageScoreThis =
		static_cast<int>(like_) -
		static_cast<int>(dislike_);

	int averageScoreOther =
		static_cast<int>(other.like_) -
		static_cast<int>(other.dislike_);

	if (averageScoreThis != averageScoreOther) { return averageScoreThis > averageScoreOther; }
	// Если средняя оценка одинаковая, то сравниваем по id
	else { return id_ < other.id_; }
}

bool StickerPack::operator==(const StickerPack& other) const
{
	return id_ == other.id_;
}

int StickerPack::id() const { return id_; }
const std::string& StickerPack::category() const { return category_; }
const std::string& StickerPack::link() const { return link_; }
const StickerPack::Time& StickerPack::dateAdded() const { return dateAdded_; }

unsigned int StickerPack::like() const { return like_; }
unsigned int StickerPack::dislike() const { return dislike_; }

const UserInfo& StickerPack::creatorInfo() const { return creatorInfo_; }

void StickerPack::addLike() { ++like_; }
void StickerPack::addDislike() { ++dislike_; }
void StickerPack::removeLike()
{
	if (like_ > 0) { --like_; }
	else { Core::logError("Попытка уменьшить минимальную оценку"); }
}
void StickerPack::removeDislike()
{
	if (dislike_ > 0) { --dislike_; }
	else { Core::logError("Попытка уменьшить минимальную оценку"); }
}