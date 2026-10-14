#include "AdvertisementManager.h"

AdvertisementManager::AdvertisementManager(TelegramApi& telegramApi, Users& users)
	: tgApi_(telegramApi), users_(users)
{}

std::shared_ptr<Advertisement> AdvertisementManager::getAdvertisement(int64_t adminId)
{
	std::shared_lock<std::shared_mutex> lock(mutex_);
	
	auto it = advertisements_.find(adminId);
	if (it != advertisements_.end())
	{
		return it->second;
	}

	return nullptr;
}

void AdvertisementManager::addAdvertisement(int64_t adminId)
{
	std::unique_lock lock(mutex_);

	if (advertisements_.contains(adminId))
	{
		return;
	}

	advertisements_.emplace(
		adminId,
		std::make_shared<Advertisement>(tgApi_, users_)
	);
}

void AdvertisementManager::removeAdvertisement(int64_t adminId)
{
	std::unique_lock<std::shared_mutex> lock(mutex_);
	advertisements_.erase(adminId);
}

bool AdvertisementManager::hasAdvertisement(int64_t adminId) const
{
	std::shared_lock<std::shared_mutex> lock(mutex_);
	return advertisements_.contains(adminId);
}