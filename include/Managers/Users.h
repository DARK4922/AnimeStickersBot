#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <unordered_set>
#include <shared_mutex>
#include <vector>
#include <optional>

#include "User.h"
#include "Core.h"

class Users
{
private:
	std::unordered_set<User> users_;
	mutable std::shared_mutex usersMutex_;

public:
	// Добавление нового пользователя
	void addUser(int64_t id, const std::string& name);
	void addUser(int64_t id);
	void addUser(User&& user);
	void addUser(User& user);

	// Удаление пользователя
	void deleteUser(int64_t id);

	// Очистка оценок удалённого стикерпака
	void removeStickerRatings(int stickerId);

	// Очистка всех пользователей
	void clearUsers();

	// Проверка, существует ли пользователь
	bool doesUserExist(int64_t id) const;

	// Получение определённого пользователя
	std::optional<User> getUser(int64_t id) const;

	// Выполнение метода для конкретного пользователя
	template <typename Func>
	void withUser(int64_t id, Func&& func)
	{
		std::shared_lock<std::shared_mutex> lock(usersMutex_);

		// Поиск данного пользователя
		auto it{ users_.find(User(id)) };
		if (it != users_.end()) { func(const_cast<User&>(*it)); }
		else { Core::logError("Обращение к несуществующему пользователю"); }
	}

	// Получение всех id пользователей
	std::vector<int64_t> allIds() const;

	// Получение всех пользователей
	std::vector<User> allUsers() const;

	// Количество пользователей
	size_t size() const;
};

#endif // !USERS_H
