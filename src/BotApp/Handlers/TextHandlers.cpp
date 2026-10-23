#include "TextHandlers.h"

#include <charconv>

#include "Core.h"
#include "User.h"

void TextHandlers::textHandlers(
	int64_t chatId,
	const std::string& messageText,
	Context& ctx
)
{
	// Игнорирование команд
	if (!messageText.empty() && messageText.front() == '/')
		{ return; }

	// Обработка стандартных команд
	// Главное меню
	if (messageText == "🏮 Войти в аниме-мир"
		|| messageText == "🔙Назад"
		|| messageText == "🌸 Выбрать новое аниме")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.menu(chatId);
			});
	}

	// Экран приветствия
	else if (messageText == "≽ Назад ≼"
		|| messageText == "Выйти из меню разработчика❌"
		|| messageText == "🏠 На главную"
		|| messageText == "≪ На главную")
	{
		// Получение username пользователя
		std::string username{};
		ctx.users.withUser(chatId, [&username](User& user)
			{
				username = user.username();
			});

		ctx.pool.addTask([&ctx, chatId, username]
			{
				ctx.ui.user.start(chatId, username);
			});
	}

	// Выход из меню добавления стикеров
	else if (messageText == "❌ Выйти")
	{
		ctx.users.withUser(chatId, [](User& user)
			{
				user.removeApplication();
			});

		ctx.pool.addTask([&ctx, chatId]()
			{
				ctx.ui.user.stickerLab(chatId);
			});

		return;
	}

	// Дополнительное меню
	else if (messageText == "📜 Тайное меню"
		|| messageText == "🌸 Вернуться в Тайное меню")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.extraMenu(chatId);
			});
	}

	// Инфо. канал
	else if (messageText == "🌸 Инфо-канал (эксклюзивы внутри!)")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.channelInfo(chatId);
			});
	}

	// Информация о разработчике
	else if (messageText == "🎌 Создатель-сенсей")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.developerInfo(chatId);
			});
	}

	// Тех поддержка
	else if (messageText == "✉️ Администратор-тян")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.support(chatId);
			});
	}

	// Группа отзывов
	else if (messageText == "✨Предложить улучшение или оставить отзыв")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.feedbackGroup(chatId);
			});
	}

	// Донат
	else if (messageText == "💖 Донат")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.donate(chatId);
			});
	}

	// Информация о рекламе
	else if (messageText == "✨ Реклама")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.advertising(chatId);
			});
	}

	// Лаборатория стикеров
	else if (messageText == "🎨 Лаборатория стикеров"
		|| messageText == "≼≼ Назад")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.user.stickerLab(chatId);
			});
	}

	// Получение названия категории
	else if (messageText == "➕ Добавить стикеры")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.application.getCategoryName(chatId);
			});
	}

	// Меню разработчика
	else if (messageText == "Обновить меню разработчика🔄")
	{
		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.administrator.menu(chatId);
			});
	}

	else if (Core::getAnimeLetters().count(messageText))
	{
		ctx.pool.addTask([&ctx, chatId, messageText]
			{
				ctx.ui.user.getAnimeByLetter(
					chatId,
					messageText,
					ctx.stickerDatabase.categories().categories()
				);
			});
	}

	// Проверка на название аниме
	else if ((messageText.size() >= 3)
		&& messageText.starts_with(Core::animeMarker())
		&& messageText.ends_with(Core::animeMarker()))
		{
			ctx.pool.addTask([&ctx, chatId, messageText]
				{
					const auto marker{ Core::animeMarker() };

					std::string name{
						messageText.substr(
							marker.size(),
							messageText.size() - marker.size() * 2
						)
					};

					const auto user{ ctx.users.getUser(chatId) };
					if (!user) { return; }

					auto stickers{
						ctx.stickerDatabase.stickers().getStickers(name)
					};

					ctx.ui.user.broadcastStickersByAnime(
						chatId,
						*user,
						std::move(stickers)
					);
				});
		}
}

void TextHandlers::handleUserInput(
	int64_t chatId,
	const std::string& messageText,
	Context& ctx
)
{
	// Если пользователь ввёл команду, то сбрасывает весь ввод
	if (!messageText.empty() && messageText.front() == '/')
	{
		ctx.users.withUser(chatId, [](User& user)
			{
				user.state<InputState::UserState>().resetInputState();
				user.removeApplication();
			});
		return;
	}

	if (messageText == "❌ Выйти")
	{
		ctx.users.withUser(chatId, [](User& user)
			{
				user.state<InputState::UserState>().resetInputState();
				user.removeApplication();
			});

		ctx.pool.addTask([&ctx, chatId]()
			{
				ctx.ui.user.stickerLab(chatId);
			});

		return;
	}

	auto user{ ctx.users.getUser(chatId) };
	if (!user.has_value())
	{
		Core::logError(
			"Обращение к несуществующему пользователю"
		);
		return;
	}

	if (!user.value().application())
	{
		Core::logError(
			"Обращение к не созданной заявки"
		);
		return;
	}
	
	auto& state{ user->state<InputState::UserState>() };

	auto addStickerLink = [&](auto onSuccess)
		{
			if (ctx.stickerDatabase.stickers().exists(messageText))
			{
				ctx.pool.addTask([&ctx, chatId]()
					{
						ctx.ui.application.notifyStickerAddedByAnotherUser(chatId);
					});
				return;
			}

			bool added{};
			ctx.users.withUser(chatId, [messageText, &added](User& user)
				{
					added = user.application()->addLink(messageText);
				});

			ctx.pool.addTask([&ctx, chatId, added, onSuccess]()
				{
					if (added)
					{
						onSuccess(chatId);
					}
					else
					{
						ctx.ui.application.notifyStickerAlreadyAdded(chatId);
					}
				});
		};

	// Ввод названия
	if (state.enteringName)
	{
		ctx.users.withUser(chatId, [messageText](User& user)
			{
				user.application()->setCategory(messageText);
				user.state<InputState::UserState>().enteringName = false;
			});

		ctx.pool.addTask([&ctx, chatId]()
			{
				ctx.ui.application.addStickersMenu(chatId);
			});
	}

	// Ввод ссылки
	else if (state.enteringLink)
	{
		addStickerLink([&ctx](int64_t chatId)
			{
				ctx.users.withUser(chatId, [](User& user)
					{
						user.state<InputState::UserState>().enteringLink = false;
					});

				ctx.ui.application.addStickersMenu(chatId);
			});
	}
	// Многократный ввод ссылок
	else if (state.multipleEnteringLink)
	{
		addStickerLink([&ctx](int64_t chatId)
			{
				ctx.ui.application.notifyStickerAdded(chatId);
			});
	}
}

void TextHandlers::handleStaffInput(
	int64_t chatId,
	const std::string& messageText,
	Context& ctx,
	AdvertisementManager& advertisementManager
)
{
	// Если пользователь ввёл команду, то сбрасывает весь ввод
	if (!messageText.empty() && messageText.front() == '/')
	{
		ctx.users.withUser(chatId, [](User& user)
			{
				if (user.role() == User::Role::Moderator)
				{
					user.state<InputState::ModeratorState>().
						enteringReasonRefusal = false;
				}
				else if (user.role() == User::Role::Administrator)
				{
					user.state<InputState::AdministratorState>().
						resetAdministratorInputState();
				}
			});
		return;
	}

	auto user{ ctx.users.getUser(chatId) };
	if (!user.has_value())
	{
		Core::logError(
			"Обращение к несуществующему пользователю"
		);
		return;
	}

	if (user->state<InputState::ModeratorState>().enteringReasonRefusal)
	{
		ctx.pool.addTask([&ctx, chatId, messageText]
			{
				ctx.ui.moderator.applicationRejected(
					chatId,
					ctx.ui.user,
					messageText
				);
			});
	}

	// Ввод ID пользователя администратором
	else if (user->role() == User::Role::Administrator
		&& user->state<InputState::AdministratorState>().enteringUserId)
	{
		int64_t userId{};
		const auto [end, error] = std::from_chars(
			messageText.data(),
			messageText.data() + messageText.size(),
			userId
		);

		if (error != std::errc{}
			|| end != messageText.data() + messageText.size()
			|| userId <= 0)
		{
			Core::logError("Некорректный ID пользователя при вводе администратором");
			return;
		}

		ctx.users.withUser(chatId, [](User& administrator)
			{
				administrator.state<InputState::AdministratorState>().enteringUserId = false;
			});

		ctx.pool.addTask([&ctx, chatId, userId]
			{
				if (!ctx.ui.administrator.accessVerification(chatId)) { return; }
				ctx.ui.administrator.showUserSettingsMenu(chatId, userId);
			});
	}

	// Ввод информации для рекламы
	else if (user->role() == User::Role::Administrator
		&& (user->state<InputState::AdministratorState>().sendingPostText
			|| user->state<InputState::AdministratorState>().sendingPhotoPost))
	{
		if (!advertisementManager.hasAdvertisement(chatId))
		{
			ctx.pool.addTask([&ctx, chatId]
				{
					ctx.ui.administrator.applicationDisappeared(chatId);
				});
			return;
		}

		if (user->state<InputState::AdministratorState>().sendingPostText)
		{
			advertisementManager.getAdvertisement(chatId)->
				advertisingPost().message = messageText;
		}
		else
		{
			advertisementManager.getAdvertisement(chatId)->
				advertisingPost().photoId = messageText;
		}

		ctx.pool.addTask([&ctx, chatId]
			{
				ctx.ui.administrator.advertisingMenu(chatId);
			});
	}
}