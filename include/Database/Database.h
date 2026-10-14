#ifndef DATABASE_H
#define DATABASE_H

#include <mutex>
#include <pqxx/pqxx>

#include "Config.h"
#include "Users.h"
#include "StickerDatabase.h"

class Database
{
private:
    pqxx::connection database_;
    std::mutex databaseMutex_;

    Users& users_;
    StickerDatabase& stickers_;

public:
    explicit Database(
        const Config& config,
        Users& users,
        StickerDatabase& stickers
    );

    // Создание структуры базы данных
    void createStructure();

    // Сохранение данных
    bool saveUsers();
    bool saveStickers();

    // Обновление данных
    bool loadUsers();
    bool loadStickers();
};

#endif // DATABASE_H