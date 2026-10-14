#include "ApplicationManager.h"

#include "StickerDatabase.h"

void ApplicationManager::addApplication(StickerApplication application)
{
	std::lock_guard<std::mutex> lock(mutex_);
	applications_.push(std::move(application));
}

std::optional<StickerApplication> ApplicationManager::getApplicationForModerator
(int64_t moderatorId)
{
	std::lock_guard<std::mutex> lock(mutex_);

	// Проверка, если ли у этого модератора активная заявка
	if (activeApplications_.contains(moderatorId))
	{
		return activeApplications_.at(moderatorId);
	}

	if (applications_.empty()) { return std::nullopt; }

	StickerApplication application{ std::move(applications_.front()) };
	applications_.pop();

	activeApplications_.emplace(moderatorId, std::move(application));

	return activeApplications_.at(moderatorId);
}

size_t ApplicationManager::numberApplications() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return applications_.size()
		+ activeApplications_.size();
}

std::optional<int64_t> ApplicationManager::acceptApplication(
	int64_t moderatorId,
	StickerDatabase& stickers
)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (!activeApplications_.contains(moderatorId))
		{ return std::nullopt; }

	StickerApplication application{
		std::move(activeApplications_.at(moderatorId))
	};
	activeApplications_.erase(moderatorId);

	const std::string& category{ application.category() };
	int64_t creatorId{ application.creatorInfo().id() };
	const std::string& creatorName{ application.creatorInfo().username() };

	for (const auto& link : application.links())
	{
		stickers.addSticker(
			category,
			link,
			creatorId,
			creatorName
		);
	}

	return creatorId;
}

std::optional<int64_t> ApplicationManager::rejectApplication(int64_t moderatorId)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (!activeApplications_.contains(moderatorId))
		{ return std::nullopt; }

	int64_t creatorId{
		activeApplications_.at(moderatorId).creatorInfo().id()
	};

	activeApplications_.erase(moderatorId);

	return creatorId;
}