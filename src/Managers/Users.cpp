#include "Users.h"

#include <mutex>

void Users::addUser(int64_t id)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	users_.insert(User(id));
}

void Users::addUser(int64_t id, const std::string& name)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	users_.insert(User(id, name));
}

void Users::addUser(User&& user)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	users_.insert(std::move(user));
}

void Users::addUser(User& user)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	users_.insert(std::move(user));
}

void Users::deleteUser(int64_t id)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	auto it{ users_.find(User(id)) };
	if (it != users_.end()) { users_.erase(it); }
}

void Users::removeStickerRatings(int stickerId)
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	for (auto it{ users_.begin() }; it != users_.end();)
	{
		if (!it->stickerRatings().isLike(stickerId)
			&& !it->stickerRatings().isDislike(stickerId))
		{
			++it;
			continue;
		}

		auto node{ users_.extract(it++) };
		node.value().removeLike(stickerId);
		node.value().removeDislike(stickerId);
		users_.insert(std::move(node));
	}
}

bool Users::doesUserExist(int64_t id) const
{
	std::shared_lock<std::shared_mutex> lock(usersMutex_);
	auto it{ users_.find(User(id)) };
	return it != users_.end();
}

std::vector<int64_t> Users::allIds() const
{
	std::shared_lock<std::shared_mutex> lock(usersMutex_);

	std::vector<int64_t> ids;
	ids.reserve(users_.size());

	for (const auto& user : users_) { ids.push_back(user.id()); }

	return ids;
}

std::vector<User> Users::allUsers() const
{
	std::shared_lock<std::shared_mutex> lock(usersMutex_);
	return std::vector<User>(users_.begin(), users_.end());
}

std::optional<User> Users::getUser(int64_t id) const
{
	std::shared_lock<std::shared_mutex> lock(usersMutex_);

	auto it{ users_.find(User(id)) };

	if (it != users_.end())
	{
		return *it;
	}

	Core::logError("Обращение к несуществующему пользователю");
	return {};
}

size_t Users::size() const
{
	std::shared_lock<std::shared_mutex> lock(usersMutex_);
	return users_.size();
}

void Users::clearUsers()
{
	std::unique_lock<std::shared_mutex> lock(usersMutex_);
	users_.clear();
}
