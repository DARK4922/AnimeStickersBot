
cmake_minimum_required(VERSION 3.20)

project(AnimeStickersBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost 1.65.1 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

find_package(TgBot CONFIG REQUIRED)

file(GLOB_RECURSE BOT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(StickersBot ${BOT_SOURCES})

target_include_directories(StickersBot PRIVATE
    include
    include/BotApp
    include/BotApp/Handlers
    include/BotApp/UI
    include/Core
    include/Database
    include/Managers
    include/Models
    include/Storage
    include/Telegram
)

target_link_libraries(StickersBot PRIVATE
    TgBot::TgBot
    PkgConfig::PQXX
    nlohmann_json::nlohmann_json
    Threads::Threads
)
