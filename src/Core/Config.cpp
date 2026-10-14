#include "Config.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

Config::Config(const std::string& filePath) : filePath_(filePath) {}

bool Config::load()
{
	try
	{
		// Чтение данных из файла конфигурации
		boost::property_tree::ptree config;
		boost::property_tree::read_ini(filePath_, config);

		// Запись данных в переменные
		databaseData_.host = config.get<std::string>("database.host");
		databaseData_.port = config.get<std::string>("database.port");
		databaseData_.name = config.get<std::string>("database.name");
		databaseData_.user = config.get<std::string>("database.user");
		databaseData_.password = config.get<std::string>("database.password");

		botData_.token = config.get<std::string>("bot.token");
		botData_.administratorID = config.get<std::string>("bot.administrationId");

		programData_.version = config.get<std::string>("program.version");
		programData_.automaticDataUpdate = config.get<bool>("program.automaticDataUpdate");
		programData_.updateSource = config.get<std::string>("program.updateSource") == "database" ?
			Program::UpdateSource::Database : Program::UpdateSource::File;

		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Configuration reading error" << e.what() << std::endl;
		return false;
	}
}

const Config::Database& Config::database() const { return databaseData_; }
const Config::Bot& Config::bot() const { return botData_; }
const Config::Program& Config::program() const { return programData_; }