#ifndef USERINFO_H
#define USERINFO_H

#include <string>
#include <nlohmann/json.hpp>

class UserInfo
{
private:
	int64_t id_{};
	std::string username_{};

public:
	UserInfo(int64_t id, std::string username);
	UserInfo(int64_t id);

	// Геттеры
	int64_t id() const;
	const std::string& username() const;

	// Сеттеры
	void setUsername(std::string newName);
};

void to_json(nlohmann::json& j, const UserInfo& user);
void from_json(const nlohmann::json& j, UserInfo& user);

#endif // !USERINFO_H
