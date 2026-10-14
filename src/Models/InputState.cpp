#include "InputState.h"

void InputState::UserState::resetInputState()
{
	enteringName = false;
	enteringLink = false;
	multipleEnteringLink = false;
}

bool InputState::UserState::isInputInProgress() const
{
	return enteringName
		|| enteringLink
		|| multipleEnteringLink;
}

void InputState::ModeratorState::resetModeratorInputState()
{
	enteringReasonRefusal = false;
}

bool InputState::ModeratorState::isModeratorInputInProgress()
{
	return enteringReasonRefusal;
}

void InputState::AdministratorState::resetAdministratorInputState()
{
	resetModeratorInputState();
	sendingPostText = false;
	sendingPhotoPost = false;
}

bool InputState::AdministratorState::isAdminInputInProgress()
{
	return isModeratorInputInProgress()
		|| sendingPostText
		|| sendingPhotoPost;
}