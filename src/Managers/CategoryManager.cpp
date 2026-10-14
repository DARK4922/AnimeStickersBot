#include "CategoryManager.h"
#include <algorithm>
#include "Core.h"
#include <locale>

void CategoryManager::addCategory(std::string category)
{
	std::unique_lock<std::shared_mutex> lock(categoriesMutex_);

	if (std::find(categories_.begin(), categories_.end(), category)
		== categories_.end())
	{
		categories_.push_back(std::move(category));
	}
	else { Core::logError("Попытка добавить существующую категорию"); }
}

bool CategoryManager::exists(const std::string& category) const
{
	std::shared_lock<std::shared_mutex> lock(categoriesMutex_);
	return std::find(categories_.begin(), categories_.end(), category)
		!= categories_.end();
}

void CategoryManager::sortCategory()
{
	// Функция сравнения для русских строк
	auto russian_compare = [](const std::string& a, const std::string& b)
		{
			return std::use_facet<std::collate<char>>(std::locale("ru_RU.UTF-8"))
				.compare(&a[0], &a[0] + a.size(), &b[0], &b[0] + b.size()) < 0;
		};

	std::sort(categories_.begin(), categories_.end(), russian_compare);
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