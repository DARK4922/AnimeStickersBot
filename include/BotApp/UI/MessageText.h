#ifndef MESSAGETEXT_H
#define MESSAGETEXT_H

#include <string>
#include <vector>
#include <unordered_set>
#include "User.h"

namespace MessageText
{
	namespace User
	{
        inline std::string start(const std::string& firstName)
        {
            std::string text = "<b>✨ Добро пожаловать в мир аниме-стикеров,</b> "
                + firstName + "! ✨";
            text += R"(
<i>Ты только что активировал режим "уютного чата" (◕‿◕✿)</i>

🌸 <b>Что ждёт тебя внутри?</b>
• <i>Стикеры на любой вкус</i> — от <i>трогательных</i> до <i>эпичных</i> (а иногда и странных, как Дандадан) (￣ω￣;)
• Любимые аниме, включая:

<b>✧ Клинок, рассекающий демонов</b> <i>(и слезы зрителей...)</i>
<b>✧ Берсерк</b> <i>(тьма, но с Гатсом в стикер-формате!)</i>
<b>✧ Магическая битва</b> <i>(Гоё: "Я бы одобрил")</i>
<b>✧ У Коми проблемы с общением</b> <i>(зато стикеры говорят за неё!) ♡</i>
<i>...и ещё 100+ миров, где слизь милее котиков, а фарфоровые куклы — романтичнее диснеевских принцесс</i>
(´｡• ᵕ •｡`)

🚀 Это только начало! Новые стикеры прилетают, как атака титанов на кухню Саши.
Выбирай, куда отправимся дальше? 🌙
)";

            return text;
        }

        inline std::string animeList(const std::vector<std::string>& category)
        {
            std::string animeList{};
            animeList += "✨ Полный список всех аниме в боте (А-Я) ✨\n\nА-И\n";

            for (size_t index{}; index < category.size(); ++index)
            {
                animeList += "- " + category[index] + "\n";
                // Проверка является ли элемент последним
                if (index != category.size() - 1)
                {
                    if (category[index + 1].substr(0, 2) == "К"
                        && category[index].substr(0, 2) != "К")
                    {
                        animeList += "\n<b>К-С</b>\n";
                    }
                    else if (category[index + 1].substr(0, 2) == "Т"
                        && category[index].substr(0, 2) != "Т")
                    {
                        animeList += "\n<b>Т-Я</b>\n";
                    }
                }
            }

            animeList += "\n📌 <b>Важно:</b>\n-Все названия проверены и соответствуют стикерам в боте.\n"
                "-Если не нашел нужное аниме — напиши нам, добавим! <i>Сделать это можно в Тайном меню (/secretMenu)</i>";

            return animeList;
        }

        inline std::string getAnimeByLetter(const std::string& letter)
        {
            std::string text = "<em>Буква</em> <b>«" + letter + "»</b> <i>— вот что у нас есть:</i>"
                "\n\n━━━━━━━━\n<code>Просто нажмите на кнопку с названием</code>";

            return text;
        }

        inline std::string broadcastStickersByAnimeStart(const std::string& name, size_t index)
        {
            std::string text{};
            if (index == 0)
            {
                text =
                    "<b>✨ " + name + "</b>\n<em>Лови волшебные стикеры!</em> (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧";
            }
            else
            {
                text =
                    "<i>Загружаем волшебный стикер-пак</i>👇";
            }

            return text;
        }

        inline std::string broadcastStickersByAnimeEnd1(
            const std::string& name,
            int numberStickersAtTime,
            size_t index,
            size_t size
        )
        {
            std::string text{};
            if (index == size)
            {
                text =
                    "<em>Вы посмотрели все стикеры по</em> <b>" + name + "</b> (´･ω･`)";
            }
            else
            {
                text =
                "<b>✨ Ещё больше аниме-стикеров!</b>\n\n"
                    "Ты уже посмотрел(а) <code>" + std::to_string(index + numberStickersAtTime)
                    + "</code> наборов <i>няшных стикеров</i> 💫\n"
                    "Но у меня для тебя припрятано ещё <code>"
                    + std::to_string(size - (index + numberStickersAtTime))
                    + "</code> наборов! (´｡• ᵕ •｡`) ♡\n\n"
                    "Хочешь посмотреть другие стикеры из этого аниме или выбрать новое аниме?\n\n"
                    "<i>Просмотрено: " + std::to_string(index + numberStickersAtTime) + "/" +
                    std::to_string(size - (index + numberStickersAtTime)) + "</i>\n\n";
            }

            return text;
        }

        inline std::string broadcastStickersByAnimeEnd2(const std::string& name)
        {
            std::string text =
                R"(🌟 <b>Вы посмотрели все стикеры по аниме </b>)";
            text += "<i>" + name + "</i> 🌟";
            return text;
        }

        inline std::string addStickersMenu(int stickersSize)
        {
            std::string text =
                "<b>🎨 Добавление стикеров</b>\n\n"
                "<i>Добавь стикеры, которые хочешь отправить на модерацию!</i>\n\n"
                "<b>📎 Как добавить ? </b>\n"
                "Отправь мне <b>сам стикер</b> или <b>ссылку на стикерпак</b>.\n\n"
                "<b>📦 Уже добавлено:</b> " + std::to_string(stickersSize) + " стикеров\n\n"
                "<i>Можешь добавлять их по одному или сразу несколько — когда закончишь, отправь заявку на проверку.\n\n"
                "</i> ━━━━━━━━\n"
                "<b>Выбери действие : </b>";
            return text;
        }

        inline std::string applicationPreview(
            const std::string& name,
            const std::vector<std::string>& stickers
        )
        {
            std::string text =
                "<b>✨ Твоя заявка почти готова!</b>\n\n"
                "<i>Проверь всё ещё раз перед отправкой модераторам~</i>\n\n"
                "<b>🎬 Аниме:</b>\n" + name + "\n\n" +
                "<b>🎨 Стикеры:</b>\n";

            for (const std::string sticker : stickers)
            {
                text += "▸ " + sticker + '\n';
            }

            text +=
                "\n━━━━━━━━\n"
                "<b>📋 Что дальше?</b>"
                "После отправки заявку проверят наши модераторы-тян. Если всё будет хорошо — стикеры появятся в боте! ✨\n\n"
                "<i>Всё верно? Отправляем на проверку? (｡•̀ᴗ-)✧</i>";

            return text;
        }

        inline std::string applicationWasRejected(const std::string& reason)
        {
            std::string text =
                R"(<b>❌ Заявка отклонена</b>

К сожалению, модераторы не смогли одобрить твою заявку.

<b>Причина:</b>
)" + reason + R"(
<i>Пожалуйста, учти это при следующей подаче заявки~</i> 🐾)";

            return text;
        }

        static const std::string handleReactionError =
R"(💔 Ой-ой... реакцию на этот стикерпак поставить не удалось! (｡•́︿•̀｡)

Похоже, стикерпак был удалён или его данные изменились, поэтому я больше не могу найти его актуальную версию...)";

        static const std::string theLinkWasAddedEarlier =
R"(🌸 <b>Ой-ой! Похоже, кто-то уже опередил тебя!</b> 🌸

Эта ссылка на стикеры уже была добавлена в бота другим пользователем ✨

Не переживай 💫
Попробуй добавить другую ссылку и собрать ещё больше крутых стикеров! 🎀)";

        static const std::string start2 =
R"(<b>✨ Полный список аниме-вселенных в стикерах ✨</b>
Чтобы увидеть все доступные стикер-миры, просто нажми: <b>🌸 Все аниме-миры</b>)";

        static const std::string menu =
R"(<b>✨ Выбери букву аниме-вселенной ✨</b>
<i>Куда отправимся сегодня? Найди свой стикер-мир по первой букве~ (◕‿◕✿)</i>)";

        static const std::string extraMenu =
R"(<b>✨ Тайное меню ✨</b>
<i>Ты нашёл скрытую комнату с магическими функциями! (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧</i>

<b>📜 Доступные действия:</b>

<b>🌸 Инфо-канал</b>
<i>Узнавай о новых стикерах первым! Тут бывают эксклюзивные релизы~</i>

<b>🎌 Создатель-сенсей</b>
<i>Познакомься с тем, кто собрал этого аниме-бота на чистой любви к аниме (и литрах кофе) ☕</i>

<b>✉️ Администратор-тян</b>
<i>Напиши нам, если что-то сломалось... или просто хочешь поболтать о аниме!</i>

<b>✨Предложить улучшение</b>
<i>Хочешь, чтобы в боте появились стикеры из «Твоего имени»? Или есть крутая идея? Ждём!</i>

<b>≽ Назад ≼</b>
<i>Вернуться в уютный главный мир</i>)";

        static const std::string channelInfo =
R"(<b>🚀 Dark Shark — твой проводник в мир эксклюзивных стикеров!</b>

Здесь ты найдёшь:
• <b>Ранний доступ</b> к новым стикер-пакам
• <b>Скрытые релизы</b> (которые не попадают в общий бот)
• <b>Анонсы</b> и секретные конкурсы

Подписывайся: 👉 <b>https://t.me/Dark_project64</b>

<em>P.S. Только для избранных отаку. Не распространяй ссылки!</em>)";

        static const std::string developerInfo =
R"(<b>👨💻 Тайная лаборатория Создателя-сенсея</b>

<em>Этот загадочный кудесник:</em>
• Собирал бота между пересмотрами «Берсерка»
• Спорит с ИИ о том, кто лучше рисует — Макото Синкай или нейросети
• Готов обсудить твой любимый тайтл (и почему «Клинок» — это шедевр)

<i>Напиши ему:</i> )";

        static const std::string support =
R"(<b>🛎️ Связь с Администратором-тян</b>
<em>Ты можешь обратиться за помощью:</em>
• Написать <b>напрямую</b> администратору (для срочных вопросов)
• Оставить сообщение в <b>группе техподдержки</b> (если вопрос не срочный)

<u>Что можно обсудить:</u>
• Проблемы со стикерами (´；ω；`)
• Сотрудничество

━━━━━━━━
<i>Выбери способ связи:</i>)";

        static const std::string feedbackGroup =
R"(<b>🌌 Твой голос важен для вселенной бота!</b>

<em>Хочешь:</em>
• Добавить стикеры из любимого аниме?
• Улучшить работу бота?
• Поделиться впечатлениями?

<u>Просто напиши в нашу</u> <b>группу отзывов</b> <u>— мы читаем всё!</u>)";

        static const std::string donate =
R"(<b>🌸 Ваша поддержка согревает моего создателя!</b>

Если вам нравится бот и вы хотите сказать "спасибо" (СБЕР):
<code>2202208125839416</code>

Каждый рубль помогает:
• Добавлять новые крутые стикеры
• Улучшать работу бота
• Делать аниме-сообщество уютнее

<em>Спасибо, что вы с нами! (´｡• ᵕ •｡`)</em>)";

        static const std::string advertising =
R"(<b>✨ Хотите рассказать о своем проекте?</b>

Я могу разместить вашу рекламу в своей рассылке для подписчиков.

<em>Почему это хорошо:</em>
• Теплое аниме-комьюнити
• Честные отзывы и фидбек
• Возможность найти единомышленников

<i>Просто нажмите на кнопку ниже для покупки рекламы или уточнение подробностей👇</i>)";

        static const std::string buyAdvertising =
R"(<b>✨ Хотите разместить рекламу?</b>

Для покупки и обсуждения деталей напишите нашему менеджеру по кнопку ниже👇)";

        static const std::string advertisingRules =
R"(<b>🌸 Условия сотрудничества</b>

<em>Мы рады предложить рекламу в нашем боте, но с некоторыми ограничениями:</em>

<b>✅ Можно:</b>
• Любые легальные проекты и услуги
• Товары и магазины (кроме запрещённых)
• Полезные сервисы и приложения
• Творческие и образовательные проекты

<b>❌ Строго запрещено:</b>
• 18+/эротический контент
• Казино, ставки, криптовалюты
• Обман, скам и мошенничество
• Политика и провокационные темы

<b>📝 Технические требования:</b>
• Максимум 1 пост в 2 часа
• Только текст + 1 фото
• Ссылки через @username

━━━━━━━━
<em>Стоимость: от 300₽ за 1 пост</em>)";

        static const std::string help =
R"(<b>🌸 Основные команды</b>

<b>/start</b>-Перезагрузить бота (если что-то «заглючило»)
<b>/menu</b>-Выбрать аниме по алфавиту ✨
<b>/random</b>-Случайный стикер для настроения 🎲
<b>/feedback</b>-Предложить новое аниме или улучшения 💌

<b>🎌 Как пользоваться?</b>

<b>1. Выбираешь аниме</b> через /menu (например, «К» → «Клинок»)
<b>2. Получаешь стикеры</b> — просто нажимаешь на нужный
<b>3. Сохраняешь в избранное</b> (зажми стикер → «Добавить»)

<b>🛠️ Техподдержка</b>

Если бот «умер» или стикеры не приходят:
• Напиши админу по кнопке ниже
• Или в группу поддержки)";

        static const std::string noAnimeFound =
R"(<b>(ﾉ｀Д´)ﾉ・゜✧・゜</b>

<b>Эта буква</b> <i>осталась без стикеров?!</i>
*Этот провал не может быть реальным...*

━━━━━━━━
<code>Но ты можешь:</code>
• Устроить <b>бунт</b> и предложить аниме в Тайном меню <i>(доступно по команде:/secretMenu)</i>
• Выбрать <b>любую другую букву</b> (например, «К» для «Клинка»)

<code>Админ уже бежит исправлять это безобразие!</code>)";

        static const std::string choosingNewAnime =
R"(<b>✨ Хочешь выбрать другое аниме?</b>

<em>Просто нажми кнопку ниже, чтобы вернуться в меню выбора:</em>)";

        static const std::string notifyAnimeNotFound =
R"(<b>✨ Ой-ой...</b>

<em>Похоже, этого аниме пока нет в нашей коллекции</em> (´･ω･`)

<i>Но вы можете:</i>
• Проверить название (/list — список аниме)
• Предложить его в Тайном меню <i>(/secretMenu)</i>
• Выбрать что-то другое из /menu

<code>Мы активно пополняем стикерпаки!</code>)";

        static const std::string stickerLab =
R"(<b>🎨 Лаборатория стикеров</b>

<i>Ты попал в святая святых — творческую кухню, где рождаются новые стикеры!</i>
(ﾉ◕ヮ◕)ﾉ:･ﾟ✧
Здесь ты можешь:
<b>✨ Добавить стикеры</b> в бота — совершенно неважно, есть такое аниме у нас или ещё нет!

━━━━━━━━
<b>📌 Как это работает?</b>
1. Ты присылаешь стикерпак
2. Наши модераторы-тян проверяют его
3. Если всё ок — стикеры появляются в боте для всех!

<i>P.S. Авторы лучших стикеров получают особую роль в сообществе~</i>

━━━━━━━━
Выбери действие:)";

        static const std::string getStickerUploadRules =
R"(<b>Чтобы твои стикеры приняли:</b>

✧ <b>Качество</b>: Только чёткие PNG без артефактов (как у Къюба в «Мадока☆Магика»)
✧ <b>Контент</b>: Без порно и хейта (даже если это концовка «Атаки титанов» ಥ_ಥ)
✧ <b>Оригинальность</b>: Не копируй чужие стикерпаки — только свой уникальный стиль!
✧ <b>Лимиты</b>: Не больше 3 заявок в день (мы же не Стальная алхимия, чтобы нарушать правила等价交换)

<b>Примеры крутых стикеров:</b>\n
• Персонаж в необычной позе (например, Леви с чаем)
• Мемный момент из аниме («Гоё ест бургер»)
• Кастомные надписи («Когда Monday…» + грустный Макабе)

<i>Нарушители попадают в бесконечное цукуёми (бан). Будь как Усаги — делай всё с любовью! ♡</i>)";

        static const std::string getCategoryName =
R"(<i>Отлично! 🌟 Давай добавим стикеры в нашу коллекцию!</i>

<b>Пришли название аниме</b>, для которого хочешь добавить стикеры.

<b>📌 Если аниме уже есть в боте</b> — стикеры будут добавлены в его коллекцию.
<b>🆕 Если такого аниме ещё нет</b> — мы создадим для него новый раздел.

<b>📌 Важно:</b>
✔ Указывай официальное русское название
✔ Не используй сокращения или фанатские названия
✔ Проверь название перед отправкой

<i>(Сначала выберем аниме, а потом перейдём к добавлению стикеров~)</i>)";

        static const std::string startMultipleLinkSubmission =
R"(<b>📚 Многократная отправка стикеров</b>

<i>Режим многократной отправки включён!</i> ✨

Теперь ты можешь <b>отправлять ссылки на стикерпаки одну за другой</b> — каждый стикерпак будет автоматически добавляться к заявке.

<b>📎 Просто отправляй ссылки</b>, а я сам всё обработаю.

<i>Когда закончишь — нажми кнопку ниже, чтобы прекратить многократную отправку.</i>

━━━━━━━━
<b>Жду ссылки на стикерпаки~</b>)";

        static const std::string stickerAdded =
R"(<b>✅ Стикер добавлен!</b>

<i>Можешь продолжать отправлять стикеры один за другим.</i>)";

        static const std::string stickerAlreadyAdded =
R"(<b>⚠️ Стикер уже добавлен!</b>

<i>Ты уже добавлял этот стикер ранее.</i>

<i>Можешь отправить другой стикер~</i>)";

        static const std::string getSingleSticker =
R"(<b>➕ Добавление стикера</b>

<i>Отправь мне <b>один стикер</b> или <b>ссылку на него</b> — и он будет добавлен к заявке.</i>)";

        static const std::string allStickersHaveBeenRemoved =
R"(<b>🗑️ Больше нечего удалять!</b>
<i>Ты уже удалил все добавленные стикеры.</i>)";

        static const std::string stickerDeletionMenu =
R"(<b>🗑️ Удаление стикеров</b>

<i>Ты можешь удалить любой добавленный стикер.</i>

<b>Просто выбери его ссылку</b> на кнопках ниже.

<i>После выбора стикер будет удалён из заявки~</i>)";

        static const std::string sendApplicationForReview =
R"(Твоя заявка успешно отправлена модераторам! ✨

<b>Что дальше?</b>
<b>⏳ Срок проверки:</b> 1-2 дня (мы не «Блич» — без бесконечных флеров!)
<b>📨 Уведомление:</b> Результат придёт в этот чат, даже если что-то пойдёт не так.
<b>🛠️ Если одобрят:</b> Стикеры появятся в боте через 5 минут после уведомления.

<i>Спасибо, что расширяешь нашу аниме-вселенную!(ﾉ◕ヮ◕)ﾉ*:･ﾟ✧</i>)";

        static const std::string isApplicationUpToDate =
R"(🌀 <b>Ой-ой... Похоже, заявка куда-то исчезла!</b>

Возможно, ты уже отправлял(а) её ранее...
Или же в момент заполнения бот незаметно обновился.

Не мог(ла) бы ты заполнить заявку заново, пожалуйста?
<i>Буду очень ждать~ (´ . ｡•ω•｡)</i>)";

        static const std::string applicationAccepted =
            R"(<b>🎉 Заявка одобрена!</b>

<i>Ура! Твоя заявка успешно прошла проверку модераторов~</i> ✨

<b>🎨 Стикеры уже доступны в боте!</b>

━━━━━━━━
Теперь их смогут находить и использовать другие пользователи~ 💫

<i>Спасибо, что пополняешь нашу коллекцию стикеров! (ﾉ◕ヮ◕)ﾉ:･ﾟ✧</i>)";
	}

    namespace Administrator
    {
        inline std::string showUserSettingsMenu(const ::User& user)
        {
            std::string username;
            for (const char character : user.username())
            {
                switch (character)
                {
                case '&': username += "&amp;"; break;
                case '<': username += "&lt;"; break;
                case '>': username += "&gt;"; break;
                default: username += character; break;
                }
            }
            if (username.empty()) { username = "Не указан"; }

            std::string role;
            switch (user.role())
            {
            case ::User::Role::User: role = "Пользователь"; break;
            case ::User::Role::Moderator: role = "Модератор"; break;
            case ::User::Role::Administrator: role = "Администратор"; break;
            default: role = "Неизвестна"; break;
            }

            return "👤 <b>Управление ролью пользователя</b>\n\n"
                "🆔 <b>ID:</b> <code>" + std::to_string(user.id()) + "</code>\n"
                "👤 <b>Username:</b> " + username + "\n"
                "🏷️ <b>Текущая роль:</b> " + role + "\n\n"
                "Выберите роль, которую хотите назначить данному пользователю.\n\n"
                "🔹 <b>Пользователь</b> — стандартные права доступа.\n"
                "🛡️ <b>Модератор</b> — дополнительные права для модерации.\n"
                "👑 <b>Администратор</b> — расширенные права управления ботом.\n\n"
                "👇 <b>Выберите новую роль с помощью кнопок ниже.</b>";
        }

        inline std::string onUserRoleAssigned(const ::User& user)
        {
            std::string username{};
            for (const char character : user.username())
            {
                switch (character)
                {
                case '&': username += "&amp;"; break;
                case '<': username += "&lt;"; break;
                case '>': username += "&gt;"; break;
                default: username += character; break;
                }
            }
            if (username.empty()) { username = "Не указан"; }

            std::string role;
            switch (user.role())
            {
            case ::User::Role::User: role = "Пользователь"; break;
            case ::User::Role::Moderator: role = "Модератор"; break;
            case ::User::Role::Administrator: role = "Администратор"; break;
            default: role = "Неизвестна"; break;
            }

            return "✅ <b>Новая роль назначена!</b>\n\n"
                "<i>Роль пользователя успешно обновлена~</i> ✨\n\n"
                "🆔 <b>ID:</b> <code>" + std::to_string(user.id()) + "</code>\n"
                "👤 <b>Username:</b> " + username + "\n"
                "🏷️ <b>Новая роль:</b> " + role;
        }

        inline std::string roleAssignmentNotification(::User::Role newRole)
        {
            std::string role{};
            std::string instructions{};
            switch (newRole)
            {
            case ::User::Role::User:
                role = "Пользователь";
                instructions = "Тебе доступны обычные возможности бота: выбирай аниме, "
                    "смотри стикеры и отправляй свои заявки. Дополнительных обязанностей нет~ 🌸";
                break;
            case ::User::Role::Moderator:
                role = "Модератор";
                instructions = "Теперь ты можешь <b>принимать и удалять заявки</b> пользователей.\n"
                    "Проверяй присланные стикеры перед принятием заявки и помогай "
                    "поддерживать порядок в нашей коллекции~ 🛡️";
                break;
            case ::User::Role::Administrator:
                role = "Администратор";
                instructions = "Тебе открыт <b>полный доступ к управлению ботом</b>, "
                    "включая все возможности модератора.\n\n"
                    "🔧 <b>Меню разработчика:</b> /developerMenu\n"
                    "Используй его для управления ботом~ 👑";
                break;
            default:
                role = "Неизвестна";
                instructions = "Не удалось определить возможности этой роли. "
                    "Пожалуйста, обратись к администратору.";
                break;
            }

            return "✨ <b>Тебе назначена новая роль!</b>\n\n"
                "🏷️ <b>Твоя роль:</b> " + role + "\n\n"
                "📋 <b>Что это значит?</b>\n" + instructions;
        }

        inline std::string userWasNotFound(int64_t userId)
        {
            return "❌ <b>Пользователь не найден.</b>\nID: <code>"
                + std::to_string(userId) + "</code>";
        }

		inline std::string advancedStatistics(
            std::string time,
			size_t users,
			size_t stickers,
			size_t categories,
			size_t applications
		)
		{
            std::string text =
                "<b>📊 Расширенная статистика бота</b>\n"
				"<em>На данный момент:</em> " + time + "\n\n"
                "👤 <b>Пользователей:</b> " + std::to_string(users) + "\n"
				"🎬 <b>Аниме:</b> " + std::to_string(categories) + "\n"
                "🎨 <b>Стикеров:</b> " + std::to_string(stickers) + "\n"
                "📋 <b>Заявок на добавление стикеров:</b> " + std::to_string(applications) + "\n";
			return text;
		}

        inline std::string advertisingMenu(bool message, bool photo)
        {
            std::string text =
                R"(📢 <b>Создание рекламной рассылки</b>

Настройте рекламное сообщение перед отправкой пользователям.

Вы можете добавить текст и фотографию, а затем отправить готовую рекламу всем пользователям бота.

<u>Добавленные материалы:</u>
Текст: )";

            text += message ? "✔" : "❌";
            text += "\nФото: ";
            text += photo ? "✔" : "❌";
            text += "\n\n<i>Выберите нужное действие в меню ниже.</i>";

            return text;
        }

        inline std::string numberOfUsers(size_t users)
        {
            std::string text =
                "<b>На данный момент в боте </b>" + std::to_string(users)
                + "<b> пользователей </b>";
            
            return text;
        }

        inline std::string confirmationAction(const std::string& action)
        {
            const auto separatorPos{ action.find('#') };
            const std::string name{ action.substr(0, separatorPos) };
            if (separatorPos != std::string::npos
                && (name == "make_a_user" || name == "make_a_moderator"
                    || name == "make_an_administrator"))
            {
                const std::string role{ name == "make_a_user" ? "Пользователь"
                    : name == "make_a_moderator" ? "Модератор" : "Администратор" };
                return "⚠️ <b>Подтверждение изменения роли</b>\n\n"
                    "Назначить пользователю с ID <code>" + action.substr(separatorPos + 1)
                    + "</code> роль <b>" + role + "</b>?";
            }
            std::string text =
                "<b>⚠️ Внимание!</b>\nВы действительно уверены, что хотите <i>" + action + "</i> 👇";
            return text;
        }

        inline std::string broadcastAdvertisement(size_t sentMessages, size_t deleteUsers)
        {
            std::string text =
                "<b>✔ Рассылка успешно завершена ✨</b>\n\n"
                "📬Количество полученных сообщений: " + std::to_string(sentMessages) + "\n"
                "🚫Количество заблокированных пользователей: " + std::to_string(deleteUsers);
            return text;
        }

        static const std::string accessRestricted =
R"(<b>Вы не являетесь администратором</b>)";

        static const std::string menu =
R"(<b>🔧 Панель управления ботом</b>
<em>Выберите действие из списка ниже</em> ✨)";

        static const std::string mailingListHasStarted =
R"(✔ Рассылка началась)";

        static const std::string saveMenu =
			R"(<b>💾 Меню сохранения данных</b>)";

        static const std::string updateMenu =
			R"(<b>🔄 Меню обновления данных</b>)";

        static const std::string revocationTheAction =
            R"(😒 <b>Действие отменено</b>)";

        static const std::string getAdvertisingMessage =
            R"(📝 <b>Введите текст рекламы</b>

Отправьте сообщение с текстом, который будет использован в рекламной рассылке.

<u>Поддерживается HTML-разметка!</u> Вы можете использовать форматирование текста.)";

        static const std::string receiveAdvertisingPhoto =
            R"(🖼️ <b>Отправка рекламного фото</b>

Отправьте фотографию, которая будет прикреплена ко всей рекламной рассылке.

Фото будет автоматически добавлено к рекламному сообщению при отправке пользователям.)";

		static const std::string deleteAdvertisingPost =
			R"(🗑️ <b>Рекламный пост удалён</b>)";

        static const std::string applicationDisappeared =
            R"(⚠️ <b>Рекламный пост не найден</b>

К сожалению, рекламный пост куда-то пропал. Это могло произойти из-за ошибки на сервере или перезапуска бота.

Попробуйте создать рекламный пост заново.)";

        static const std::string confirmAdvertisement =
            R"(❗<b>Подтвердите отправку рекламы</b>)";

        static const std::string sendAdvertisementToAllUsers =
            R"(<b>Рассылка рекламы начинается</b>)";

        static const std::string showAdminManagementMenu =
            R"(👤 <b>Выбор пользователя</b>

Пожалуйста, отправьте <b>Telegram ID пользователя</b>, над которым вы хотите выполнить операции.

🔎 <b>Пример:</b> `123456789`

После ввода ID откроется меню доступных действий для выбранного пользователя.)";
    }

    namespace Moderator
    {
        static const std::string confirmStickerRemoval =
R"(⚠️ <b>Удаление стикерпака</b>

Ты действительно хочешь удалить этот стикерпак из коллекции бота?

<i>Подтверди удаление или нажми «Отмена», чтобы оставить стикерпак в коллекции.</i>)";

        static const std::string stickerRemoved =
R"(🗑️ <b>Стикерпак удалён</b>

<i>Этот стикерпак больше не доступен в коллекции бота~</i>)";

        inline std::string getApplication(
            int64_t id,
            const std::string& name,
            const std::string& category,
            const std::unordered_set<std::string>& links
        )
        {
            std::string anime{};
            for (const auto& anim : links)
            {
                anime += "- " + anim + '\n';
            }

            std::string text =
R"(<b>📋 Заявка на добавление стикеров</b>

<b>👤 Создатель:</b>
ID: )" + std::to_string(id) + R"(
Username: @)" + name + R"(

<b>🎬 Аниме:</b>
)" + category + "\n\n"
R"(<b>🔗 Ссылки на стикеры:</b>
)" + anime + "\n\n"
R"(━━━━━━━━
<i>Проверь информацию и выбери действие ниже~</i>)";

            return text;
        }

        inline std::string applicationHasBeenRejected(const std::string& reason)
        {
            std::string text =
                R"(<b>✅ Заявка отклонена</b>

Заявка успешно отправлена в отказ.

<b>Причина:</b>
)" + reason + R"(

<i>Пользователь получит уведомление с указанной причиной.</i>
)";

            return text;
        }

        static const std::string messageIsEmpty =
R"(Сообщение не заполнено)";

        static const std::string previewPost =
R"(Подтвердите отправку поста:)";

        static const std::string thereAreNoApplications =
R"(<b>📭 Заявок пока нет</b>

<i>Похоже, здесь пока совсем пусто~</i> (｡•́︿•̀｡)

На данный момент ни одной заявки на добавление стикеров не поступало.

<b>✨ Здесь появятся:</b>
• новые заявки от пользователей
• ссылки на предложенные стикеры
• информация об аниме

━━━━━━━━
<i>Как только кто-нибудь отправит заявку — она появится здесь~</i> 💫)";

        static const std::string applicationWasSuccessfullyAccepted =
R"(<b>✅ Заявка принята!</b>

<i>Ты успешно одобрил(а) заявку~</i> ✨

Стикеры из этой заявки теперь доступны пользователям в боте.

━━━━━━━━
<b>🎨 Спасибо за работу!</b>

<i>Ещё одна заявка прошла модерацию~ (｡•̀ᴗ-)✧</i>)";

        static const std::string applicationCouldNotBeApproved =
R"(<b>❌ Не удалось одобрить/отклонить заявку</b>

<i>При попытке произошла ошибка.</i>

Возможно, бот перезагружался.

━━━━━━━━
<i>Попробуй обновить список заявок и повторить действие~</i> 🔄)";

        static const std::string getRejectionReason =
R"(<b>📝 Нужна причина отказа</b>

Заявку нельзя просто отправить в небытие — нужно объяснить пользователю, что пошло не так.

<i>Напиши причину отказа одним сообщением ниже.</i>

Например: <code>Ссылки ведут на стикеры, которые уже есть в боте.</code>

<i>После этого пользователь получит твою причину и сможет учесть её при повторной подаче~</i> 🐾)";

        static const std::string isModerator =
R"(<b>🚫 Доступ закрыт</b>

Похоже, у тебя пока нет прав для использования этой функции.

<i>Она доступна только модераторам~</i> 🔒)";
    }
}

#endif // !MESSAGETEXT_H
