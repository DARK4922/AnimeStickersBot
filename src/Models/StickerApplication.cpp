#include "StickerApplication.h"
#include "Core.h"

StickerApplication::StickerApplication(
	int64_t creatorId,
	std::string creatorName
)
	: creatorInfo_(creatorId, creatorName)
{}

StickerApplication::StickerApplication(int64_t creatorId)
	: creatorInfo_(creatorId)
{}

const UserInfo& StickerApplication::creatorInfo() const { return creatorInfo_; }
const std::string& StickerApplication::category() const { return category_; }
const std::unordered_set<std::string>& StickerApplication::links() const { return links_; }

void StickerApplication::setCategory(std::string newCategory)
{ category_ = std::move(newCategory); }

bool StickerApplication::addLink(const std::string& link)
{
	if (links_.contains(link)) { return false; }
	links_.insert(std::move(link));
	return true;
}
void StickerApplication::removeLink(const std::string& link)
{
	if (!links_.erase(link))
	{
		Core::logError("Попытка удалить несуществующую ссылку: " + link);
	}
}
bool StickerApplication::exists(const std::string& link) const
{
	return links_.contains(link);
}