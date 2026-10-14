#ifndef CONTEXT_H
#define CONTEXT_H

#include "Users.h"
#include "ThreadPool.h"
#include "StickerDatabase.h"
#include "ApplicationManager.h"
#include "UI.h"
#include "Database.h"

struct Context
{
	ThreadPool& pool;
	Users& users;
	StickerDatabase& stickerDatabase;
	ApplicationManager& applicationManager;
	UI& ui;
	Database& db;
};

#endif // !CONTEXT_H
