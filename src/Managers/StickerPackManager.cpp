#include "StickerPackManager.h"

#include <algorithm>
#include <chrono>
#include <mutex>

#include "Core.h"

void StickerPackManager::addSticker(StickerPack sticker)
{
	// Безопасная блокировка двух мьютексов
	std::unique_lock<std::shared_mutex> packsLock(packsMutex_, std::defer_lock);
	std::unique_lock<std::shared_mutex> linksLock(linksMutex_, std::defer_lock);
	std::lock(packsLock, linksLock);

	// Проверка, существует ли такая ссылка
	if (links_.contains(sticker.link()))
		{
			Core::logError("Попытка добавить существующий стикер");
			return;
		}

	nextId_ = std::max(nextId_, sticker.id() + 1);

	links_.insert(sticker.link());

	linksLock.unlock();

	std::string name{ sticker.category() };
	std::vector<StickerPack>& vec{ packs_[name] };

	vec.push_back(std::move(sticker));
	
	// Сортировка этого отдела
	std::sort(vec.begin(), vec.end());
}

void StickerPackManager::addSticker(
	std::string category,
	std::string link,
	int64_t creatorId,
	std::string creatorName
)
{
	// Безопасная блокировка двух мьютексов
	std::unique_lock<std::shared_mutex> packsLock(packsMutex_, std::defer_lock);
	std::unique_lock<std::shared_mutex> linksLock(linksMutex_, std::defer_lock);
	std::lock(packsLock, linksLock);

	// Проверка, существует ли такая ссылка
	if (links_.contains(link))
	{
		Core::logError("Попытка добавить существующий стикер");
		return;
	}

	links_.insert(link);

	linksLock.unlock();

	std::vector<StickerPack>& vec{ packs_[category] };

	vec.emplace_back(
		nextId_++,
		std::move(category),
		std::move(link),
		std::chrono::system_clock::time_point{},
		0,
		0,
		creatorId,
		std::move(creatorName)
	);

	// Сортировка этого отдела
	std::sort(vec.begin(), vec.end());
}

size_t StickerPackManager::numberSections() const
{
	std::shared_lock<std::shared_mutex> lock(packsMutex_);
	return packs_.size();
}

size_t StickerPackManager::numberStickers() const
{
	std::shared_lock<std::shared_mutex> lock(packsMutex_);

	// Подсчёт стикеров в каждой категории
	size_t result{};
	for (const auto& [name, stickers] : packs_)
		{ result += stickers.size(); }

	return result;
}

std::vector<StickerPack>
StickerPackManager::getStickers(const std::string& name) const
{
	std::shared_lock lock(packsMutex_);

	auto it{ packs_.find(name) };
	if (it != packs_.end()) { return it->second; }
	return {};
}

bool StickerPackManager::exists(const std::string& link) const
{
	std::shared_lock<std::shared_mutex> lock(linksMutex_);
	return links_.contains(link);
}

void StickerPackManager::sortStickers(const std::string& name)
{
	std::unique_lock<std::shared_mutex> lock(packsMutex_);
	
	auto it{ packs_.find(name) };
	if (it != packs_.end())
	{
		std::sort(it->second.begin(), it->second.end());
	}
}

std::vector<StickerPack> StickerPackManager::allPacks() const
{
	std::shared_lock<std::shared_mutex> lock(packsMutex_);

	std::vector<StickerPack> result;
	for (const auto& [_, vec] : packs_)
	{
		result.insert(result.end(), vec.begin(), vec.end());
	}

	return result;
}

void StickerPackManager::clearStickers()
{
	std::unique_lock<std::shared_mutex> packsLock(packsMutex_, std::defer_lock);
	std::unique_lock<std::shared_mutex> linksLock(linksMutex_, std::defer_lock);
	std::lock(packsLock, linksLock);

	packs_.clear();
	links_.clear();
	nextId_ = 1;
}

std::optional<StickerPack> StickerPackManager::getSticker(int id) const
{
	std::shared_lock<std::shared_mutex> lock(packsMutex_);

	for (const auto& [_, vec] : packs_)
	{
		const auto it = std::find_if(vec.begin(), vec.end(),
			[id](const StickerPack& sticker)
			{
				return sticker.id() == id;
			}
		);

		if (it != vec.end())
		{
			return *it;
		}
	}

	Core::logError(
		"Обращение к несуществующему стикерпаку"
	);

	return std::nullopt;
}