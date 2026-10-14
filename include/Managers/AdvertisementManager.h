#ifndef ADVERTISEMENTMANAGER_H
#define ADVERTISEMENTMANAGER_H

#include <unordered_map>
#include <shared_mutex>
#include <memory>

#include "Advertisement.h"
#include "TelegramApi.h"
#include "Users.h"

class AdvertisementManager
{
private:
	std::unordered_map<int64_t, std::shared_ptr<Advertisement>> advertisements_;

	TelegramApi& tgApi_;
	Users& users_;

	mutable std::shared_mutex mutex_;

public:
	AdvertisementManager(TelegramApi& telegramApi, Users& users);

	std::shared_ptr<Advertisement> getAdvertisement(int64_t adminId);

	void addAdvertisement(int64_t adminId);
	void removeAdvertisement(int64_t adminId);

	bool hasAdvertisement(int64_t adminId) const;
};

#endif // !ADVERTISEMENTMANAGER_H
