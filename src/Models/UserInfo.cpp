#include "UserInfo.h"

UserInfo::UserInfo(int64_t id, std::string username)
	: id_(id), username_(std::move(username))
{}
UserInfo::UserInfo(int64_t id) : id_(id) {}

int64_t UserInfo::id() const { return id_; }
const std::string& UserInfo::username() const { return username_; }

void UserInfo::setUsername(std::string newName) { username_ = std::move(newName); }

void to_json(nlohmann::json& j, const UserInfo& user)
{
	j =
	{
		{"id", user.id()},
		{"username", user.username()}
	};
}

void from_json(const nlohmann::json& j, UserInfo& user)
{
	user = UserInfo(
		j.at("id").get<int64_t>(),
		j.at("username").get<std::string>()
	);
}