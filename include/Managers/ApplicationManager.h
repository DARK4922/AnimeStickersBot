#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <queue>
#include <mutex>
#include <optional>
#include <unordered_map>
#include "StickerApplication.h"

class StickerDatabase;

class ApplicationManager
{
private:
	std::queue<StickerApplication> applications_;
	std::unordered_map<int64_t, StickerApplication> activeApplications_;
	mutable std::mutex mutex_;

public:
	void addApplication(StickerApplication application);

	std::optional<StickerApplication> getApplicationForModerator
	(int64_t moderatorId);

	// Принять/Отклонить заявку
	std::optional<int64_t> acceptApplication(
		int64_t moderatorId,
		StickerDatabase& stickers
	);
	std::optional<int64_t> rejectApplication(int64_t moderatorId);

	size_t numberApplications() const;
};

#endif // !APPLICATIONMANAGER_H
