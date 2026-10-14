#ifndef UI_H
#define UI_H

#include "UserUI.h"
#include "ModeratorUI.h"
#include "AdministratorUI.h"
#include "ApplicationUI.h"

struct UI
{
	UserUI& user;
	ModeratorUI& moderator;
	AdministratorUI& administrator;
	ApplicationUI& application;
};

#endif // !UI_H
