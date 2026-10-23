#ifndef CALLBACKHANDLERS_H
#define CALLBACKHANDLERS_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

#include "Users.h"
#include "Context.h"

class CallbackHandlers
{
private:
	using Handler =
		std::function<void(int64_t, int64_t)>;

	Context& ctx_;
	std::unordered_map<std::string_view, Handler> handlers_;
	void enqueue(std::function<void()> task);

public:
	CallbackHandlers(Context& ctx);

	void load();

	void handle(
		int64_t chatId,
		int64_t messageId,
		const std::string& callbackData);

	// Обработка callback-запросов с сторонней информацией
	void handleExternalCallback(
		int64_t chatId,
		int64_t messageId,
		const std::string& callbackData
	);
};

#endif // !CALLBACKHANDLERS_H
