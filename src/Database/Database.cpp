#include "Database.h"

#include <iostream>
#include <chrono>
#include <format>

#include "Core.h"

Database::Database(
    const Config& config,
    Users& users,
    StickerDatabase& stickers
)
    : database_(
        "host=" + config.database().host + " "
        "port=" + config.database().port + " "
        "dbname=" + config.database().name + " "
        "user=" + config.database().user + " "
        "password=" + config.database().password
    )
    , users_(users)
    , stickers_(stickers)
{
    if (database_.is_open())
    {
        std::cout
            << "Имя базы данных: "
            << database_.dbname()
            << std::endl;
    }
    else
    {
        Core::logError("Не удалось открыть базу данных");
    }
}

void Database::createStructure()
{
    std::lock_guard<std::mutex> lock(databaseMutex_);

    try
    {
        pqxx::work transaction{ database_ };

        // Пользователи
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id BIGINT PRIMARY KEY"
            ");"
        );

        // Названия аниме
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS names ("
            "id INTEGER PRIMARY KEY,"
            "name VARCHAR(200) NOT NULL UNIQUE"
            ");"
        );

        // Стикерпаки
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS stickers ("
            "id INTEGER PRIMARY KEY,"
            "link VARCHAR(250) NOT NULL UNIQUE,"
            "anime_title VARCHAR(200) NOT NULL,"
            "date_added TIMESTAMP NOT NULL,"
            "creator_id BIGINT,"
            "creator_username VARCHAR(100) NOT NULL,"
            "like_count INTEGER NOT NULL DEFAULT 0,"
            "dislike_count INTEGER NOT NULL DEFAULT 0,"
            "FOREIGN KEY (creator_id) "
            "REFERENCES users(id) "
            "ON DELETE SET NULL"
            ");"
        );

        // Счётчик ID не уменьшается после удаления стикерпаков.
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS sticker_id_state ("
            "id INTEGER PRIMARY KEY CHECK (id = 1),"
            "next_id BIGINT NOT NULL CHECK (next_id BETWEEN 1 AND 2147483648)"
            ");"
        );
        transaction.exec(
            "INSERT INTO sticker_id_state (id, next_id) "
            "SELECT 1, COALESCE(MAX(id)::BIGINT, 0) + 1 FROM stickers "
            "ON CONFLICT (id) DO UPDATE SET "
            "next_id = GREATEST(sticker_id_state.next_id, EXCLUDED.next_id);"
        );

        // Понравившиеся стикерпаки
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS user_likes ("
            "user_id BIGINT,"
            "sticker_id INTEGER,"
            "PRIMARY KEY (user_id, sticker_id),"
            "FOREIGN KEY (user_id) "
            "REFERENCES users(id) "
            "ON DELETE CASCADE,"
            "FOREIGN KEY (sticker_id) "
            "REFERENCES stickers(id) "
            "ON DELETE CASCADE"
            ");"
        );

        // Непонравившиеся стикерпаки
        transaction.exec(
            "CREATE TABLE IF NOT EXISTS user_dislikes ("
            "user_id BIGINT,"
            "sticker_id INTEGER,"
            "PRIMARY KEY (user_id, sticker_id),"
            "FOREIGN KEY (user_id) "
            "REFERENCES users(id) "
            "ON DELETE CASCADE,"
            "FOREIGN KEY (sticker_id) "
            "REFERENCES stickers(id) "
            "ON DELETE CASCADE"
            ");"
        );

        transaction.commit();

        std::cout
            << "Структура базы данных успешно создана."
            << std::endl;
    }
    catch (const pqxx::sql_error& e)
    {
        Core::logError(
            "Ошибка SQL при создании структуры: "
            + std::string(e.what())
        );
    }
    catch (const std::exception& e)
    {
        Core::logError(
            "Системная ошибка при создании структуры: "
            + std::string(e.what())
        );
    }
}

bool Database::saveUsers()
{
    std::lock_guard<std::mutex> lock(databaseMutex_);

    try
    {
        const auto users{ users_.allUsers() };

        pqxx::work transaction{ database_ };

        for (const auto& user : users)
        {
            transaction.exec(
                "INSERT INTO users (id) "
                "VALUES ($1) "
                "ON CONFLICT (id) DO NOTHING;",
                pqxx::params{
                    user.id()
                }
            );
        }

        transaction.commit();

        return true;
    }
    catch (const pqxx::sql_error& e)
    {
        Core::logError(
            "Ошибка SQL при сохранении пользователей: "
            + std::string(e.what())
        );
    }
    catch (const std::exception& e)
    {
        Core::logError(
            "Системная ошибка при сохранении пользователей: "
            + std::string(e.what())
        );
    }

    return false;
}

bool Database::saveStickers()
{
    std::lock_guard<std::mutex> lock(databaseMutex_);

    try
    {
        const auto stickers{ stickers_.stickers().allPacks() };
        const auto users{ users_.allUsers() };

        pqxx::work transaction{ database_ };

        transaction.exec(
            "INSERT INTO sticker_id_state (id, next_id) VALUES (1, $1) "
            "ON CONFLICT (id) DO UPDATE SET "
            "next_id = GREATEST(sticker_id_state.next_id, EXCLUDED.next_id);",
            pqxx::params{ stickers_.stickers().nextId() }
        );

        // Сохраняем стикерпаки
        for (const auto& sticker : stickers)
        {
            const auto date = std::format(
                "{:%Y-%m-%d %H:%M:%S}",
                sticker.dateAdded()
            );

            transaction.exec(
                "INSERT INTO stickers ("
                "id, link, anime_title, date_added, "
                "creator_id, creator_username, "
                "like_count, dislike_count"
                ") "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8) "
                "ON CONFLICT (id) DO UPDATE SET "
                "link = EXCLUDED.link, "
                "anime_title = EXCLUDED.anime_title, "
                "date_added = EXCLUDED.date_added, "
                "creator_id = EXCLUDED.creator_id, "
                "creator_username = EXCLUDED.creator_username, "
                "like_count = EXCLUDED.like_count, "
                "dislike_count = EXCLUDED.dislike_count;",
                pqxx::params{
                    sticker.id(),
                    sticker.link(),
                    sticker.category(),
                    date,
                    sticker.creatorInfo().id(),
                    sticker.creatorInfo().username(),
                    sticker.like(),
                    sticker.dislike()
                }
            );
        }

        // Полностью очищаем старые реакции
        transaction.exec(
            "DELETE FROM user_likes;"
        );

        transaction.exec(
            "DELETE FROM user_dislikes;"
        );

        // Сохраняем актуальные реакции пользователей
        for (const auto& user : users)
        {
            for (const int stickerId :
            user.stickerRatings().likes())
            {
                transaction.exec(
                    "INSERT INTO user_likes "
                    "(user_id, sticker_id) "
                    "VALUES ($1, $2);",
                    pqxx::params{
                        user.id(),
                        stickerId
                    }
                );
            }

            for (const int stickerId :
            user.stickerRatings().dislikes())
            {
                transaction.exec(
                    "INSERT INTO user_dislikes "
                    "(user_id, sticker_id) "
                    "VALUES ($1, $2);",
                    pqxx::params{
                        user.id(),
                        stickerId
                    }
                );
            }
        }

        // Пересчитываем количество реакций
        for (const auto& sticker : stickers)
        {
            const auto likes = transaction.exec(
                "SELECT COUNT(*) "
                "FROM user_likes "
                "WHERE sticker_id = $1;",
                pqxx::params{
                    sticker.id()
                }
            );

            const auto dislikes = transaction.exec(
                "SELECT COUNT(*) "
                "FROM user_dislikes "
                "WHERE sticker_id = $1;",
                pqxx::params{
                    sticker.id()
                }
            );

            const auto likeCount =
                likes[0][0].as<unsigned int>();

            const auto dislikeCount =
                dislikes[0][0].as<unsigned int>();

            transaction.exec(
                "UPDATE stickers "
                "SET like_count = $1, "
                "dislike_count = $2 "
                "WHERE id = $3;",
                pqxx::params{
                    likeCount,
                    dislikeCount,
                    sticker.id()
                }
            );
        }

        transaction.commit();

        return true;
    }
    catch (const pqxx::sql_error& e)
    {
        Core::logError(
            "Ошибка SQL при сохранении стикерпаков: "
            + std::string(e.what())
        );
    }
    catch (const std::exception& e)
    {
        Core::logError(
            "Системная ошибка при сохранении стикерпаков: "
            + std::string(e.what())
        );
    }

    return false;
}

bool Database::loadUsers()
{
    std::lock_guard<std::mutex> lock(databaseMutex_);

    try
    {
        pqxx::read_transaction transaction{ database_ };

        // Пользователи
        const auto usersResult = transaction.exec(
            "SELECT id "
            "FROM users;"
        );

        for (const auto& row : usersResult)
        {
            users_.addUser(
                row["id"].as<int64_t>()
            );
        }

        // Лайки пользователей
        const auto likesResult = transaction.exec(
            "SELECT user_id, sticker_id "
            "FROM user_likes;"
        );

        for (const auto& row : likesResult)
        {
            const auto userId =
                row["user_id"].as<int64_t>();

            const auto stickerId =
                row["sticker_id"].as<int>();

            users_.withUser(
                userId,
                [stickerId](User& user)
                {
                    user.addLike(stickerId);
                }
            );
        }

        // Дизлайки пользователей
        const auto dislikesResult = transaction.exec(
            "SELECT user_id, sticker_id "
            "FROM user_dislikes;"
        );

        for (const auto& row : dislikesResult)
        {
            const auto userId =
                row["user_id"].as<int64_t>();

            const auto stickerId =
                row["sticker_id"].as<int>();

            users_.withUser(
                userId,
                [stickerId](User& user)
                {
                    user.addDislike(stickerId);
                }
            );
        }

        return true;
    }
    catch (const pqxx::sql_error& e)
    {
        Core::logError(
            "Ошибка SQL при загрузке пользователей: "
            + std::string(e.what())
        );
    }
    catch (const std::exception& e)
    {
        Core::logError(
            "Системная ошибка при загрузке пользователей: "
            + std::string(e.what())
        );
    }

    return false;
}

bool Database::loadStickers()
{
    std::lock_guard<std::mutex> lock(databaseMutex_);

    try
    {
        pqxx::read_transaction transaction{ database_ };

        const auto idState{ transaction.exec(
            "SELECT next_id FROM sticker_id_state WHERE id = 1;"
        ) };
        if (!idState.empty())
        {
            stickers_.ensureNextId(idState[0][0].as<int64_t>());
        }

        const auto result = transaction.exec(
            "SELECT "
            "id, "
            "anime_title, "
            "link, "
            "date_added, "
            "creator_id, "
            "creator_username, "
            "like_count, "
            "dislike_count "
            "FROM stickers;"
        );

        for (const auto& row : result)
        {
            const int id =
                row["id"].as<int>();

            const std::string category =
                row["anime_title"].as<std::string>();

            const std::string link =
                row["link"].as<std::string>();

            const auto dateAdded =
                Core::ConvertStrToDate(
                    row["date_added"].as<std::string>()
                );

            const unsigned int likes =
                row["like_count"].as<unsigned int>();

            const unsigned int dislikes =
                row["dislike_count"].as<unsigned int>();

            const int64_t creatorId =
                row["creator_id"].is_null()
                ? 0
                : row["creator_id"].as<int64_t>();

            const std::string creatorUsername =
                row["creator_username"].as<std::string>();

            StickerPack sticker(
                id,
                category,
                link,
                dateAdded,
                likes,
                dislikes,
                creatorId,
                creatorUsername
            );

            stickers_.addSticker(
                std::move(sticker)
            );
        }

        return true;
    }
    catch (const pqxx::sql_error& e)
    {
        Core::logError(
            "Ошибка SQL при загрузке стикерпаков: "
            + std::string(e.what())
        );
    }
    catch (const std::exception& e)
    {
        Core::logError(
            "Системная ошибка при загрузке стикерпаков: "
            + std::string(e.what())
        );
    }

    return false;
}
