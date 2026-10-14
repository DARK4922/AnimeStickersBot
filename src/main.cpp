#include <iostream>
#include <cstdlib>
#include <clocale>

#include "Config.h"
#include "Core.h"
#include "Bot.h"

#ifdef _WIN32
	#include <Windows.h>
#endif

int main()
{
	// Изменение локали консоли
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#else
	// Попытка устоновки UTF-8 локали
	if (!std::setlocale(LC_ALL, "en_US.UTF-8"))
	{
		// При неудаче используем системную локаль
		std::setlocale(LC_ALL, "");
	}
#endif

	// Чтение конфигурации
	Config config("Data/Config.ini");
	if (!config.load())
	{
		Core::logError("Ошибка чтения конфигурации");
		return ERROR_SUCCESS;
	}
	else { std::cout << "Конфигурация успешно получена" << std::endl; }

	bot(config);

	return EXIT_SUCCESS;
}