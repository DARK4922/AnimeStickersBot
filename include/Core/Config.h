#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
	// Данные БД
	struct Database
	{
		std::string host{};
		std::string port{};
		std::string name{};
		std::string user{};
		std::string password{};
	};

	// Данные бота
	struct Bot
	{
		std::string token{};
		std::string administratorID{};
	};

	// Данные программы
	struct Program
	{
		enum class UpdateSource
		{
			File,
			Database
		};

		std::string version{};
		bool automaticDataUpdate{};
		UpdateSource updateSource{};
	};

private:
	std::string filePath_{};	// Путь к файлу конфигурации

	Database databaseData_;
	Bot botData_;
	Program programData_;

public:
	Config(const std::string& filePath);

	bool load();

	// Геттеры
	const Database& database() const;
	const Bot& bot() const;
	const Program& program() const;
};

#endif // !CONFIG_H
