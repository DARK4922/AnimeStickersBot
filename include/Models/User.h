#ifndef USER_H
#define USER_H

#include <string>
#include <variant>
#include <memory>
#include <nlohmann/json.hpp>

#include "UserInfo.h"
#include "InputState.h"
#include "StickerRatings.h"
#include "StickerApplication.h"

class User : public UserInfo
{
public:
	// Роль пользователя
	enum class Role
	{
		User,
		Moderator,
		Administrator
	};

	using RoleOptions =
		std::variant<
			InputState::UserState,
			InputState::ModeratorState,
			InputState::AdministratorState
		>;

private:
	Role role_{};
	StickerRatings stickerRatings_;
	std::shared_ptr<StickerApplication> application_;

	// Возможные состояния ввода
	RoleOptions state_;

	// Определение состояния ввода
	void updateInputState();

public:
	User(
		int64_t id,
		std::string username,
		Role role = Role::User,
		StickerRatings stickerRatings = {}
	);
	User(
		int64_t id,
		Role role = Role::User,
		StickerRatings stickerRatings = {}
	);

	bool operator<(const User& other) const;
	bool operator==(const User& other) const;

	// Геттеры
	Role role() const;
	const StickerRatings& stickerRatings() const;
	StickerApplication* application();
	const StickerApplication* application() const;

	template <typename T>
	T& state()
	{
		return std::visit([](auto& state) -> T&
			{
				using State =
					std::remove_cvref_t<decltype(state)>;

				if constexpr (std::is_base_of_v<T, State>)
					{ return static_cast<T&>(state); }
				else
				{
					throw std::logic_error(
						"Requested state is unavailable"
					);
				}
			}, state_);
	}

	// Сеттеры
	void setRole(Role newRole);

	// Управление заявкой
	void createApplication();
	void removeApplication();
	bool hasApplication() const;

	// Управление реакциями
	void addLike(int sticker);
	void addDislike(int sticker);
	void removeLike(int sticker);
	void removeDislike(int sticker);
};

// Функция хеширования
namespace std
{
	template<>
	struct hash<User>
	{
		size_t operator()(const User& user) const noexcept
		{
			// Использование стандартного хеша для int64_t
			return hash<int64_t>{}(user.id());
		}
	};
}

namespace nlohmann
{
	template<>
	struct adl_serializer<User>
	{
		static User from_json(const json& j)
		{
			return User(
				j.at("id").get<int64_t>(),
				j.at("username").get<std::string>(),
				j.at("role").get<User::Role>(),
				j.at("stickerRatings").get<StickerRatings>()
			);
		}

		static void to_json(json& j, const User& user)
		{
			j = {
				{"id", user.id()},
				{"username", user.username()},
				{"role", static_cast<int>(user.role())},
				{"stickerRatings", user.stickerRatings()}
			};
		}
	};
}

#endif // !USER_H
