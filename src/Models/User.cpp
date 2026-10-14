#include "User.h"

#include "Core.h"

User::User(
	int64_t id,
	std::string username,
	Role role,
	StickerRatings stickerRatings

)
	: UserInfo(id, std::move(username))
	, role_(role)
	, stickerRatings_(std::move(stickerRatings))
{ updateInputState(); }

User::User(
	int64_t id,
	Role role,
	StickerRatings stickerRatings
)
	: UserInfo(id)
	, role_(role)
	, stickerRatings_(std::move(stickerRatings))
{ updateInputState(); }

void User::updateInputState()
{
	switch (role_)
	{
	case Role::User:
		state_ = InputState::UserState{};
		break;

	case Role::Moderator:
		state_ = InputState::ModeratorState{};
		break;

	case Role::Administrator:
		state_ = InputState::AdministratorState{};
		break;
	}
}

bool User::operator<(const User& other) const { return id() < other.id(); }
bool User::operator==(const User& other) const { return id() == other.id(); }

User::Role User::role() const { return role_; }

void User::setRole(User::Role newRole)
{
	role_ = newRole;
	// Обновление состояние ввода
	updateInputState();
}

StickerApplication* User::application() { return application_.get(); }
const StickerApplication* User::application() const { return application_.get(); }

const StickerRatings& User::stickerRatings() const { return stickerRatings_; }

void User::addLike(int sticker) { stickerRatings_.addLike(sticker); }
void User::addDislike(int sticker) { stickerRatings_.addDislike(sticker); }
void User::removeLike(int sticker) { stickerRatings_.removeLike(sticker); }
void User::removeDislike(int sticker) { stickerRatings_.removeDislike(sticker); }

bool User::hasApplication() const { return application_ != nullptr; }
void User::createApplication()
{
	application_ = std::make_shared<StickerApplication>(
		id(),
		username()
	);
}
void User::removeApplication() { application_ = nullptr; }