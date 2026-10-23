#ifndef STICKERPACKMANAGER_H
#define STICKERPACKMANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <optional>
#include <cstdint>
#include <mutex>
#include <algorithm>
#include "StickerPack.h"

class StickerPackManager
{
private:
	// Название категории -> список стикерпаков
	std::unordered_map<std::string, std::vector<StickerPack>> packs_;
	mutable std::shared_mutex packsMutex_;

	std::unordered_set<std::string> links_;
	mutable std::shared_mutex linksMutex_;

	int64_t nextId_{ 1 };

public:
	// Добавление нового стикера
	void addSticker(StickerPack sticker);
	void addSticker(
		std::string category,
		std::string link,
		int64_t creatorId = 0,
		std::string creatorName = {}
	);

	// Удаление стикерпака по ID
	bool removeSticker(int id);

	// Очистка всех стикеров
	void clearStickers();

	// Счётчик ID сохраняется отдельно от оставшихся стикерпаков
	int64_t nextId() const;
	void ensureNextId(int64_t nextId);

	// Статистика
	size_t numberSections() const;
	size_t numberStickers() const;

	// Получение стикеров по названию
	std::vector<StickerPack>
		getStickers(const std::string& name) const;

	// Проверка, существует ли такой стикер
	bool exists(const std::string& link) const;

	// Сортировка вектора с стикерами
	void sortStickers(const std::string& name);

	// Получение всех стикеров
	std::vector<StickerPack> allPacks() const;

	// Получение стикера по id
	std::optional<StickerPack> getSticker(int id) const;

	// Доступ к конкретному стикеру по id
	template<typename Func>
	bool withSticker(int id, Func&& func)
	{
		std::unique_lock<std::shared_mutex> lock(packsMutex_);

		for (auto& [_, vec] : packs_)
		{
			auto it = std::find_if(vec.begin(), vec.end(),
				[id](const StickerPack& sticker)
				{
					return sticker.id() == id;
				});

				if (it != vec.end())
				{
					func(*it);
					return true;
				}
		}

		return false;
	}
};

#endif // !STICKERPACKMANAGER_H
