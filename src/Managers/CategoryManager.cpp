#include "CategoryManager.h"

#include <algorithm>
#include <locale>
#include <mutex>

#include "Core.h"

void CategoryManager::addCategory(std::string category)
{
	std::unique_lock<std::shared_mutex> lock(categoriesMutex_);

	if (std::find(categories_.begin(), categories_.end(), category)
		!= categories_.end())
	{
		Core::logError("Попытка добавить существующую категорию");
		return;
	}
	
	categories_.push_back(std::move(category));
	sortCategory();
}

bool CategoryManager::exists(const std::string& category) const
{
	std::shared_lock<std::shared_mutex> lock(categoriesMutex_);
	return std::find(categories_.begin(), categories_.end(), category)
		!= categories_.end();
}

void CategoryManager::sortCategory()
{
	static const std::locale russianLocale("ru_RU.UTF-8");

	const auto& collate =
		std::use_facet<std::collate<char>>(russianLocale);

	auto russianCompare = [&collate](
		const std::string& a,
		const std::string& b)
		{
			return collate.compare(
				a.data(), a.data() + a.size(),
				b.data(), b.data() + b.size()
			) < 0;
		};

	std::sort(categories_.begin(), categories_.end(), russianCompare);
}

std::vector<std::string> CategoryManager::categories() const
{
	std::shared_lock<std::shared_mutex> lock(categoriesMutex_);
	return categories_;
}

void CategoryManager::clear()
{
	std::unique_lock<std::shared_mutex> lock(categoriesMutex_);
	categories_.clear();
}