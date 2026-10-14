#ifndef INPUTSTATE_H
#define INPUTSTATE_H

namespace InputState
{
	struct UserState
	{
		bool enteringName{};			// Ввод названия
		bool enteringLink{};			// Ввод ссылки
		bool multipleEnteringLink{};	// Многократный ввод ссылки
		void resetInputState();			// Сброс всех параметров ввода
		bool isInputInProgress() const;	// Осуществляется ли какой-то ввод
	};

	struct ModeratorState : public UserState
	{
		bool enteringReasonRefusal{};		// Ввод причины отказа заявки
		void resetModeratorInputState();	// Сброс всех параметров ввода модератора
		bool isModeratorInputInProgress();	// Осуществляется ли какой-то ввод модератора
	};

	struct AdministratorState : public ModeratorState
	{
		bool sendingPostText{};					// Отправка текста рекламного поста
		bool sendingPhotoPost{};				// Отправка фото рекламного поста
		void resetAdministratorInputState();	// Сброс всех параметров ввода администратора
		bool isAdminInputInProgress();			// Осуществляется ли какой-то ввод администратора
	};
}

#endif // !INPUTSTATE_H
