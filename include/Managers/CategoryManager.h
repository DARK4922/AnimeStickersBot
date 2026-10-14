#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <string>
#include <vector>
#include <shared_mutex>

class CategoryManager
{
private:
	std::vector<std::string> categories_;
	mutable std::shared_mutex categoriesMutex_;

	// Сортировка категорий
	void sortCategory();

public:
	// Добавление новой категории
	void addCategory(std::string category);

	// Проверка, существует ли такая категория
	bool exists(const std::string& category) const;

	// Получение всех категорий
	std::vector<std::string> categories() const;

	void clear();
};

#endif // !CATEGORYMANAGER_H
