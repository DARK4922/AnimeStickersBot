#include "Core.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <regex>
#include <random>

void Core::logError(const std::string& errorText)
{
	std::string date{ getDate() };

	// Вывод ошибки
	std::cerr << date << " -> " << errorText << std::endl;

	// Запись ошибки в файл
	std::ofstream file("Data/Error.log", std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "Ошибка при открытии файла Error.log" << std::endl;
		return;
	}

	file << date << ": " << errorText << std::endl;

	file.close();
}

std::string Core::getDate()
{
	// Получение текущего времени в системной зоне
	auto now{ std::chrono::system_clock::now() };

	// Преобразование в локальное время с учетом часового пояса
	auto localTime{ std::chrono::zoned_time{
		std::chrono::current_zone(),
		now
	} };

	// Форматирование
	return std::format("{:%Y-%m-%d %H:%M:%S}", localTime);
}

std::string Core::formatDate(const std::chrono::system_clock::time_point& time)
{
	return std::format("{:%Y-%m-%d %H:%M:%S}", time);
}

std::chrono::system_clock::time_point Core::ConvertStrToDate(const std::string& str)
{
	std::chrono::system_clock::time_point tp;

	std::istringstream iss(str);
	iss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", tp);

	if (iss.fail())
		throw std::runtime_error("Invalid date format");

	return tp;
}

bool Core::isLink(const std::string& str)
{
	static const std::regex pattern(
		R"(^(https?://)?(www\.)?[\w\-]+\.[a-z]{2,}.*$)",
		std::regex::icase
	);

	return std::regex_match(str, pattern);
}

std::string Core::randomPhoto()
{
	std::ostringstream oss;
	oss << "Photo/photo" << randomNumber(1, 216) << ".jpg";
	return oss.str();
}

int Core::randomNumber(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist(min, max);

	return dist(gen);
}

const std::vector<std::string>& Core::getBackgroundStickers()
{
	static const std::vector<std::string> stickers =
	{
        "CAACAgIAAxkBAAEOLjhn576kp1KknZZfnz6070s8jXCAIgACXU4AAuCjggfYAxOEoBQOpzYE",
        "CAACAgIAAxkBAAEOMDpn6Ux-2smrxHNqFh826mTjlu5FKgACLVAAAlZAqEmvnLdvIefIFjYE",
        "CAACAgEAAxkBAAEOLeJn5ynTmfNWR0skXJoPcR06FQrQxwACfgEAApl9eUeUqBogIQfx4zYE",
        "CAACAgIAAxkBAAEOLehn5ywaouD_Z_7XvkMngtiDkMo_iAACYl0AAm9r-UlB5TEF0Hg3iTYE",
        "CAACAgQAAxkBAAEOLRFn5tiu2alJw3Wau8PHfeIa_GaPigACJWwAAuOnXQW5PPVIZId7wjYE",
        "CAACAgEAAxkBAAEOMIFn6WIooMQRB42CT6ReWRbug5pKbQACXAMAAhzSOEegnqZhrv1OMzYE",
        "CAACAgEAAxkBAAEOMINn6WIr2W_Le3fVPfWTlCfrxhZoFQACGAMAAsmgOEcH7DgafJdN4zYE",
        "CAACAgEAAxkBAAEOMIVn6WIvFkfgC9IEj3aYZ4LqYBMZsAACQAIAAu8PqEXki2cE8kh0tDYE",
        "CAACAgEAAxkBAAEOMIdn6WI4IBKIjiG8TNPhi6zjnQXE-gACPQIAAjLwqUVm11-6KrDg7DYE",
        "CAACAgIAAxkBAAEOMIln6WJP699pYC4rdvJTi4NwKbacgAAC8gUAAotGbScmv8K2oQi7IjYE",
        "CAACAgIAAxkBAAEOMItn6WJSekh5xXa4os77_hScmT3wbAACFAYAAotGbSdT6oaQdWX6ijYE",
        "CAACAgQAAxkBAAEOMI1n6WJass4f4HEVBMCLEGqhixv14QACC2wAAuOnXQWd1AfpXqdlbzYE",
        "CAACAgQAAxkBAAEOMI9n6WJcEMFcPxfB6d2ZkKGnvJcewQACDGwAAuOnXQVp3yMQ46tZJjYE",
        "CAACAgQAAxkBAAEOMJFn6WJhD6GL-R_7pgW1bkVd8LzEKwACD2wAAuOnXQX4CYVn6t7FnjYE",
        "CAACAgQAAxkBAAEOMJNn6WJjYriB53BtHYxi3HmdpdUNgQACFWwAAuOnXQUTQAxfSMuHrDYE",
        "CAACAgQAAxkBAAEOMJVn6WJlpNgor61JgHnXTvElQ3kFDgACEGwAAuOnXQUcaN6krpCBlDYE",
        "CAACAgQAAxkBAAEOMJdn6WJnT_f3i7awachSglOk2U8GngACJGwAAuOnXQWwXaxzdlPtaTYE",
        "CAACAgQAAxkBAAEOMJln6WJpc5b1cNYJXdDW6MEWNg2atAACHWwAAuOnXQVi7IysEs_jZzYE",
        "CAACAgQAAxkBAAEOMJtn6WJrMCsrFGVmaci9BCpe63WriAACI2wAAuOnXQVpB71IVI8xODYE",
        "CAACAgIAAxkBAAEOMJ1n6WJumIouCfEK0IrViYY1AbXk1QACPCcAAuX3UgNgjWJIEQ5qUTYE",
        "CAACAgQAAxkBAAEOMJ9n6WJ37kLusBAhZJY7VTAzvI9n_AAC6QwAAhgwqgVo2_RpCKODzzYE",
        "CAACAgEAAxkBAAEOMKFn6WKCfo-v8NObp6uxlgLgLs2xvAACZwIAAvK6IEXB-PlD2zqrbDYE",
        "CAACAgEAAxkBAAEOMKNn6WKFpmc8U8MEjNf92BBE3BlDoQAChgIAAvIGKEVVM0vsXUcXLjYE",
        "CAACAgIAAxkBAAEOMKVn6WKQNo6Reqvfjs1vc5mhEJdPzAACPU4AAuCjggfphqdsc5GdoTYE",
        "CAACAgUAAxkBAAEOMKdn6WK_PywTOFzKQHfwZnqgqf9CUwACbAIAAvqY8ShCfIVxg9HciTYE",
        "CAACAgUAAxkBAAEOMKln6WLE9puKDXxm8TTxA_v1LEnH8gACDQMAAvqY8SgrXHePN3g1KzYE",
        "CAACAgUAAxkBAAEOMKtn6WLa-CFJeXpa5q3c53bbT1AaSgACMAADlUgdJwFcGcaTL938NgQ",
        "CAACAgEAAxkBAAEOMK1n6WMEIWskdp6_FbIJid6_nhG-NgACaQMAAvsgmEUp7bf0NYs4GTYE",
        "CAACAgQAAxkBAAEOMK9n6WMOaZYw24BM9ugKmSQ9rqCN3AAC1RMAAr6W4VFgCwMLdzlDRzYE",
        "CAACAgIAAxkBAAEOMLFn6WM0u4jKnhsEFhSQYubGl-IoBAACJQADkdh8C_uNu-KChOV0NgQ",
        "CAACAgIAAxkBAAEOMLNn6WNqKo_ZwOetXyMCm_yDGhD8qgACKAAD2lY6D4_TIDRX3kPDNgQ",
        "CAACAgIAAxkBAAEOMLVn6WNw0QOBDenDNzHmVtjXPxS86QACMQAD2lY6D2C2kuAXGVCCNgQ",
        "CAACAgIAAxkBAAEOMLdn6WN0Vh04EsiDW-RrD16MDOA3lQACWzEAAga6EEk1FxbfQdSoDTYE",
        "CAACAgUAAxkBAAEOMLln6WOS8XHZKUC-vQ5hCUwC-1o4UAAClREAAiEfbCzX7lH8ipzOGDYE",
        "CAACAgUAAxkBAAEOMLtn6WOWGOuRwZ33PDKABEC-nzKqtwACMwIAAoGKiQ51VyKHb4nEyzYE",
        "CAACAgEAAxkBAAEOML1n6WOhm6LPfO04wt1Hmq3GWJfAuAACshoAAkKjYyZfdZs40gHvxDYE",
        "CAACAgEAAxkBAAEOML9n6WOmJrjE-I8BoyKC-EGGaAmA_gACnQADNPQHPfPD5Ydr1cPSNgQ",
        "CAACAgIAAxkBAAEOMMFn6WO1C8kyz94f6p21Uj7z1OZ31gACsRsAAuX3UgMJhoaiGsf1zjYE",
        "CAACAgUAAxkBAAEOMMNn6WPIuiSA3rKN9edRJ4QGPTWUYgACuwIAAuL7mVa-nxkOYKZkCzYE",
        "CAACAgUAAxkBAAEOMMVn6WPJFkvGdfNvOxKKd68k7u7DcgACeQEAAuWDoFbl3zvKQhpfKTYE",
        "CAACAgUAAxkBAAEOMMdn6WPLcIDkOjFwkwds-SfyzsIILgAC3AMAAlDOmFYoewwy1fRE2zYE",
        "CAACAgQAAxkBAAEOMMln6WPVBwrJhk6m26kjhk0aL1KpMAACCAADl2-iBNZApggJQh8UNgQ",
        "CAACAgIAAxkBAAEOMMtn6WP9cSAZrmPIEvuqV5zTlkABSAAC00AAAuCjggf4-h7SRG9jQjYE",
        "CAACAgIAAxkBAAEO_qxof9vUHMRKhz388YGR0PeJ7-g4BAACfV8AAnaksEmMYJx07kZLHDYE",
        "CAACAgIAAxkBAAEO_q5of9vWK-u-GhCAFOrsU3DUe_oAAUgAAiZiAAJYEbBJHPQqHflrsWs2BA",
        "CAACAgIAAxkBAAEO_rBof9wBlg7INxE3KcH62Q6HuYocigACcWoAAqCSsUkD5wPulPtOHjYE",
        "CAACAgIAAxkBAAEO_rJof9wDKoX4UNBTxZH5R7pBYY4gPgACMYkAApXskUo-D4gZ0le0-jYE",
        "CAACAgIAAxkBAAEO_rRof9wJImVgMqWGerkI1RuV_HbYngACdGYAApWRaEm29JKlogtUejYE",
        "CAACAgIAAxkBAAEO_rZof9wbVNXhTfsa9fAxKmr783ewYgACWXAAAuZJoEvnSI1tGulbRTYE",
        "CAACAgIAAxkBAAEO_rhof9wgu0NVqYUQcVToKTiw8o5chQACp0gAAsFfOEiafUdxh3qHLDYE",
        "CAACAgIAAxkBAAEO_rpof9wl3GEzrDBlP7jNwtrjQx2spAAC9lsAAlI7OUhsOBFTY5OtgTYE",
        "CAACAgIAAxkBAAEO_rxof9w8evCp_4l_SK-IFByuAo1zyQAC_AMAArDqcQ5DkzKoyY3wyDYE",
        "CAACAgIAAxkBAAEO_r5of9w_2NouBwSE4fDQdUUeXG-9-QAC_QMAArDqcQ4h4gLmwPjlzjYE",
        "CAACAgIAAxkBAAEO_sBof9xBRYiABys038ilIwp_wGCWIQAC_gMAArDqcQ5un8tZQPUZsDYE",
        "CAACAgEAAxkBAAEO_sJof9xP9-K8BTyvRlU2Voq8-9T7nAAC0QoAAg6_0Ar3gfdfuHXtHDYE",
        "CAACAgIAAxkBAAEO_sRof9xpE31s2lqAOXF0hBcGowQznQACryIAAkiXAAFKOrVcFNrYn8c2BA",
        "CAACAgIAAxkBAAEO_sZof9xsmbt5Ip172gVbEkikQ7ZuSwACyScAAoCm-UnBMRLYS9VEXjYE",
        "CAACAgIAAxkBAAEO_shof9xuE90KlDRTuxF5GhmY1uP8vwACfSsAAjSX-UkPRv8NhHHNczYE",
        "CAACAgIAAxkBAAEO_spof9xwl4RC5M5YlVi6apuUgqS4UQACgiMAAs77AUpNzSXuAQssrjYE",
        "CAACAgIAAxkBAAEO_sxof9xzCiP61BMOOFstI9AqMX8ygAACdSIAAkG3AUq2bac3NdS0vDYE",
        "CAACAgIAAxkBAAEO_s5of9x4TEYSlnL0PFE6GWSm9I1o-QACKCMAAsjtAAFKyokXVp5cE3A2BA",
        "CAACAgIAAxkBAAEO_tBof9x7iHbxtC21Ah2nW0GEzy5liQACjyQAAgu3AAFKbYX0fvu1HhE2BA",
        "CAACAgIAAxkBAAEO_tJof9yGLMrLMgfArXE-GNhQQLwAAXQAAtQpAAKeevlJXL-s8jgLe2g2BA",
        "CAACAgUAAxkBAAEO_tRof9yU9W2AWtuFI8lzICb55rgsRwACjQ4AAqcp0FTb9KUSHLMdYDYE",
        "CAACAgUAAxkBAAEO_tZof9yWSXJ69f7JbiH0CHvLuU5LTQACThEAAulu0FQU8ESvVjPVrzYE",
        "CAACAgUAAxkBAAEO_thof9yf2Rq-GPqIOy9ARigaKsHYXgACxg0AAoa-0VQSVmnbpSwEbDYE",
        "CAACAgUAAxkBAAEO_tpof9yhBbmVZeBTYwulPtcZs_RLJwACzg8AAsq60FTX3RMGwDkpOjYE",
        "CAACAgEAAxkBAAEO_txof9yjraF_BH3V4ragIVErZ3-QiwACsAADg6_ARUxDJYLwavuZNgQ",
        "CAACAgEAAxkBAAEO_t5of9ymPJcUGtBI3s6Jj0P72lFfDAACewEAAoyUwUVFCZ2ea2UX_zYE",
        "CAACAgIAAxkBAAEO_uBof9yoo_8ZwRjnaauK7wTM4ilEDgACKX8AAquOoEj7yXnY7jBsYzYE",
        "CAACAgIAAxkBAAEO_uJof9yrnWs2d38itst3rE3FhUK2JwACXWIAAndniUiIp1jOHmtdiTYE",
        "CAACAgIAAxkBAAEO_uRof9yvU_Tyv5qYeZP-FMs3mQKW1wAC50UAAr1FgEsQc3xb0OqFRDYE",
        "CAACAgIAAxkBAAEO_uZof9yxuNBnqtxhBAm4iNKHTmoy5AACQlYAAityeUvbqureoZIg6zYE",
        "CAACAgQAAxkBAAEO_uhof9zEmBhh8l0V03bnF27vxarakQAC6REAAuIhKVD9_Xm2VCHJTzYE",
        "CAACAgQAAxkBAAEO_upof9zH2xxIKjOjN9IzypGOsV8f1QACTA8AAguIKFC0T4dK2gOstTYE",
        "CAACAgQAAxkBAAEO_uxof9zIIa56dMsDti2CSkvF8l5aTwACgQ8AAioxKVB0e8pGzS0kSTYE",
        "CAACAgQAAxkBAAEO_u5of9zPGW1KtdafJ5kl0FsYbkNMLAACgBIAAjTYKFDJbiLeIoiIYTYE",
        "CAACAgQAAxkBAAEO_vBof9zcDohmVpNkkooXdzwFh_KuUAACaA4AAr0tKVBpcjvkKIni_DYE",
        "CAACAgIAAxkBAAEO_vJof90KQJyN2bOeJrOS1YtXk3-NhQACKRYAAjJRyEqNZhYfhulQoDYE",
        "CAACAgIAAxkBAAEO_vRof90M75WwKtQy5kcJe2aGsyzEoAACAhQAArBqyUp1XEqxIREVlzYE",
        "CAACAgIAAxkBAAEO_vZof90Pmov9CbYNl4DvXd7k5EIftgACbQ8AAg030UqyRb5o46mw2TYE",
        "CAACAgIAAxkBAAEO_vhof90S_FYu9AjxhN7v9qMUtvfc4wACbBUAAtVI0Up-j6LsqVssOjYE",
        "CAACAgIAAxkBAAEO_vpof90VUKtgzVKA4_8-d8eexu03aAAC-h0AAoctkEio29SST-ci5zYE",
        "CAACAgIAAxkBAAEO_vxof90X5D_wiVHuAAFua95_SN_6owsAAgQeAAKhJZBI2R8li44H1KA2BA",
        "CAACAgIAAxkBAAEO_v5of90Y4zKOHvd-ehHwHUvxpOvC1wACoxgAAn5smUijMANorXMJoDYE",
        "CAACAgIAAxkBAAEO_wABaH_dHBzBmMZUCn0fE0bMlTJxRUoAAg0cAALRUZBI1rcCwcPv-5U2BA",
        "CAACAgIAAxkBAAEO_wJof90k9_0euX9JhprbccyU_OiKIAACz3wAAmiECUp9XClDZnBalTYE",
        "CAACAgIAAxkBAAEO_wRof90o9H5vsrtZn_RewzJVdn7yMwAC5XgAAsroEEoHQZrJiNAxGjYE",
        "CAACAgIAAxkBAAEO_wZof90r9QIWkh57X6XP8d8Zke7stwACmHQAAu1zEEqyQlDd0FcHEzYE",
        "CAACAgIAAxkBAAEO_whof90tlhVy844Ulo3K7BBmhTEdQQACv2sAAlDxEUqveRgRKZLhWzYE",
        "CAACAgIAAxkBAAEO_wpof90vF_cAAV8EIoFVqFtXpMwnuq4AAlt5AAK9iRFK_U1tNcJQuNI2BA",
        "CAACAgIAAxkBAAEO_wxof91WN5sGuFRv-jBlZ_HBvrRD1wAC5TcAAkAf8UjGKGSzpDpNIzYE",
        "CAACAgIAAxkBAAEO_w5of91ZV_PjNB533CwPebBJuavj9wACgDcAAqqv6UiAlHz3arE5DzYE",
        "CAACAgIAAxkBAAEO_xBof91dyvtEF2AuCTGTmc4ZMKDdUQAC_zwAApJD8UhblXrykJWRZTYE",
        "CAACAgIAAxkBAAEO_xJof91oN2dUDQRmmh5qvfsO5EqohQACLlUAAkjR6EhpYLK6o3hHSTYE",
        "CAACAgQAAxkBAAEO_xZof918fO07UsSSKFQ8KUGyRAny8gACwkYAAuOnXQVkjcNHGjkD_TYE",
        "CAACAgQAAxkBAAEO_xhof91-lm2zFHvQtlNuh3QI6S9xZwACw0YAAuOnXQUmSdlEBo6YbzYE",
        "CAACAgQAAxkBAAEO_xpof92CDTAZhvjwzKXRGvIwoQPAsAAC40YAAuOnXQV6rus1-5GJsTYE",
        "CAACAgQAAxkBAAEO_xxof92EOPMsavzH3CtYAAGK8lcjX3QAAuhGAALjp10FNQz0QFVTueM2BA",
        "CAACAgIAAxkBAAEO_x5of92IDHmUny7Y6y_YHePSHVen0AAC-g4AAvRPeUnVSSEi41NjUjYE",
        "CAACAgIAAxkBAAEO_yBof92KI5otxpVSCJPNWud7ME9CrgACqBAAAphDeUmUGf3_qtC-pzYE",
        "CAACAgIAAxkBAAEO_yJof92NT9X2qNa41kx9jFz8x8ddPQACDg8AAhoKeEk1l97q3TkoLTYE",
        "CAACAgIAAxkBAAEO_yRof92PhzeWBlZyJcD9tgEmGmer_QACEhoAAuWEeUm89DiS44KvFjYE",
        "CAACAgIAAxkBAAEO_yZof92R8jMJMsgZYrVlU_vq9sSAtQACLA0AAvmCeEmnNUUh3YYvaTYE",
        "CAACAgIAAxkBAAEO_yhof92Xcqsu9BHX6NFJnyPbS8puhQACgB8AAuCjggedCp5e92V7ZzYE",
        "CAACAgIAAxkBAAEO_ypof92ZT_W1P7BrdWwlb7l8joUrjwACgR8AAuCjggfs0tASmhtIrDYE",
        "CAACAgIAAxkBAAEO_yxof92as42sXhxuV95-eqBRv4QXPgACih8AAuCjggc0Uql4DIDnYjYE",
        "CAACAgIAAxkBAAEO_y5of92cMS1-cfBgsZ227li-eC2-ywACix8AAuCjggcMAAGD1_GI_Ts2BA",
        "CAACAgIAAxkBAAEO_zBof92eNWcgia0KPWlpt4SyJE_7NwACjB8AAuCjggfmOVlG76WrmDYE",
        "CAACAgIAAxkBAAEO_zJof92gH0hLSFwNmr-8uLRn87wtPgACjR8AAuCjggdJRoxdFLZymzYE",
        "CAACAgIAAxkBAAEO_zRof92jvb8ydsaD4h-Tog3Heidl2QAClh8AAuCjggc8r5190p1QrzYE",
        "CAACAgIAAxkBAAEO_zZof92kA5QzeXzBu_9UnnXik9PbqQACmR8AAuCjggdkeA-lVL6dEjYE",
        "CAACAgIAAxkBAAEO_zhof92nF-EeZF9SNtfrSQdqiCkgWgACmx8AAuCjggcikbseCNdhFzYE",
        "CAACAgIAAxkBAAEO_zpof92p7FS5a_nc_NGlDTkgHMdNTQACph8AAuCjggfeXkqDXBEi-zYE",
        "CAACAgIAAxkBAAEO_zxof92rETMR9CfsSBr7EltXas205gACox8AAuCjgge1rpfi3LHDATYE",
        "CAACAgIAAxkBAAEO_z5of92xMkE9DIkRSuRO38zMUyIlAQACCE4AAqfUKUjIVxvcdCLGcTYE",
        "CAACAgQAAxkBAAEO_0Bof928Ro_iVMVd-UCOROPkIpSvVgACdAgAAs32aVMZC_8WCLpb_DYE",
        "CAACAgQAAxkBAAEO_0Jof92_DUz8BybTh96DRVvUbTO46AACsggAAiIDcFNP8jdyH_PjwDYE",
        "CAACAgEAAxkBAAEO_0Rof93DumRvKHJrH0ZMilUdUBFFGgACKQUAAlIQuEU9bYi2IykelTYE",
        "CAACAgEAAxkBAAEO_0hof93dXC2F7xOlNJfb1cbHfMB1tgACvwQAAr_HyEVTZEMtfot8rDYE",
        "CAACAgQAAxkBAAEO_0pof93gYHmU1foMp71UbF5XfLrqgQACOw8AApZNEFM9oJVo-P-8VTYE",
        "CAACAgIAAxkBAAEO_0xof93sYn9akdPHpeJty1vroaJd_QAC2xoAAhlzSEmtOCyvF0fbuDYE",
        "CAACAgIAAxkBAAEO_05of93u_lTPwZ43RwyERBF8f-3-AQACkxoAAg1wQUnLp9G6U1-hMTYE",
        "CAACAgIAAxkBAAEO_1Bof93wujjfe8QjVBwZq6LUNY4GeQACWBoAAswVQEng4_sH01Nn3zYE",
        "CAACAgIAAxkBAAEO_1Jof932HWMeQgI6csgmPK-JtNnFZwAC7hgAAvzCQEmxMJTWm2RmVjYE",
        "CAACAgIAAxkBAAEO_1Rof938EELX0Jzh0HC_dKJA8JUyigAC9BsAAnlBQUlpmmxRIPIwmTYE",
        "CAACAgIAAxkBAAEO_1Zof93-qr33KVEPbhVjP313f-6sTQACCBsAAk32SUmpkcWvjaB0fzYE",
        "CAACAgIAAxkBAAEO_1hof94EJbSVOZsN1XXGhcXBicfzUgADGQACwqBISYeddabOVPbKNgQ",
        "CAACAgIAAxkBAAEO_1pof94IrQYuuMFhdvhEYpPiA5UUZQACV0MAAu6xgEj10Cislr4a9jYE",
        "CAACAgIAAxkBAAEO_1xof94ME0RzPYvxojaHGpjlCojJAgACrkoAAms7gUg6b6vC6WkhRTYE",
        "CAACAgIAAxkBAAEO_15of94ObLNfMyTmmPsRcu8Rgk_pIgACiU4AAqadgEh0Waka_RgyeDYE",
        "CAACAgIAAxkBAAEO_2Jof94kG4ZtGTtEkClMP40abhrJPgACTEUAAuYnQEq_Yh4IjSlo1DYE",
        "CAACAgIAAxkBAAEO_2Nof94tj_cWlrYaav3dHqsXWV1f4AACMyAAAlwRMUhvYB5pix9NHTYE",
        "CAACAgIAAxkBAAEO_2hof95vENHe2Js_8ktYoRcSRr8G5AACZRwAAiWeOUiD47z_pKjrwTYE",
        "CAACAgIAAxkBAAEO_2pof952VjNsUycJy-dhmc8PaHEGVAACFRsAAgbuOUiF5RpQJD6i8jYE",
        "CAACAgIAAxkBAAEO_2xof9598XemDxqKcd-gwvCDDLjYUwACsSAAAjI7OEhdwBrrvtmCXTYE",
        "CAACAgIAAxkBAAEO_25of95_qnFkCivBKju9OE9Z7fMsrAAC7yEAAnKUMUimRHGEandaTjYE",
        "CAACAgIAAxkBAAEO_3Bof96F2IAfN9Oc70tmlA-7IE5GCgACWikAAqrsAUtth5ZXDFoJ1TYE",
        "CAACAgIAAxkBAAEO_3Jof96NZ7JyRkuWqwZyssUu8AbI6AACaCcAAs73-UqmGjJ7wIpCzDYE",
        "CAACAgIAAxkBAAEO_3Rof96RuL1xqTKlFMyqbC_VcMBlrAACISkAAhSP-Ep9TMV8Bwow6DYE",
        "CAACAgIAAxkBAAEO_3Zof96WDg9mg0vT8sY9xE1xr58iFwAC5RoAAjiFcUjx_6bbTA50xTYE",
        "CAACAgIAAxkBAAEO_3hof96YgVV1Hwh_J9ukW7AUNlkoGgACFBwAAlMncUg5i7GDmsymJzYE",
        "CAACAgIAAxkBAAEO_3pof96ithowswWvUAPTQ9OlvBMQYgACnR0AAiQ0aUhxYvZN48pCzzYE",
        "CAACAgIAAxkBAAEO_3xof96wAR6NZgVRDvPs7b0MQ3engwACUUsAAga0SUtr9CgoEynEGDYE",
        "CAACAgIAAxkBAAEO_35of96ygHMrmCIdSdWF8FDEPbGrywACs1QAAkQLQUtKZMiQnzCWNzYE",
        "CAACAgIAAxkBAAEO_4Bof963p0x9o30KppmOJAw4BsRsfgAC5h0AAk7cmUh9K4YwNXcCezYE",
        "CAACAgIAAxkBAAEO_4Jof97aqmAtG9WVXqTs0kNt8wPhQQACF00AAsSrcEpKMom9Lpaf3jYE",
        "CAACAgIAAxkBAAEO_4Rof97f7imtCQXh89jeB8-tK1wf7gACwgADY9arGZgmR8yTvbGiNgQ",
        "CAACAgIAAxkBAAEO_4Zof97hXEBBNDmnKE3HKpzqRkTH9gACHAEAAmPWqxnPChbsZIBshTYE",
        "CAACAgIAAxkBAAEO_4hof97luvpxd9NmLYKw05iP8U1qZAACeAEAAmPWqxmHKcvdn1RP6zYE",
        "CAACAgIAAxkBAAEO_4pof972jpZRBiXkS_p23agPF_1nxAACSCIAAh1H0EkNqdv8_-KwWjYE",
        "CAACAgIAAxkBAAEO_4xof98GDqt3EOwXJh_n5nToVohE6gAC4CUAAvZe0EmW1XK9W16ZKjYE",
        "CAACAgIAAxkBAAEO_45of98aijN5Z1r7dV-w3imNBC7pSAACODUAAuot0Ellr49ci76zoTYE",
        "CAACAgIAAxkBAAEO_5Bof987BPRXrtP3MW6B6fJ0rrWH9gACyxQAAju10UpFkJA1zoT-6zYE",
        "CAACAgIAAxkBAAEO_5Jof99ZGSpQzhzAq5IVX_t_ztILuAACZAADokkeMpCABnkgs4P2NgQ",
        "CAACAgIAAxkBAAEO_5Rof99h4gof-UgylKaGPEcHwj0D3gACs2AAAoz-SEhl9zB6A5-kIzYE",
        "CAACAgIAAxkBAAEO_5Zof9-C-2OINJ1XnOpPRsYrsel2xAACIDAAAsdNGEkgMbt22_ebzjYE",
        "CAACAgQAAxkBAAEO_5hof9-YMa0FneXT8myjPaysc4v-vQACORMAApRgTgMZQ4cY8rkwijYE",
        "CAACAgQAAxkBAAEO_5pof9-aRayVHvaSzX1j5aR94Zz4xAACDRUAApRgTgPqu1ko6Z3B4TYE",
        "CAACAgQAAxkBAAEO_5xof9-cOid_QP8WzR7lkrJ_scYXpwACVhcAApRgTgOczY1d4XxVpjYE",
        "CAACAgQAAxkBAAEO_55of9-hCUfMRiW2a1XrA_Y0YYxq1wAC4BQAApRgTgNOQrvby8BwuDYE",
        "CAACAgQAAxkBAAEO_6Bof9-lXIjIllZNugNWDaPMGbztRgACkwoAAvizsVDJ0aRpkn54XjYE",
        "CAACAgQAAxkBAAEO_6Jof9-mIiqYr_x4xqrfOC6ngDKLqQAC8AkAAiqHsFDtvzde75QzIzYE",
        "CAACAgQAAxkBAAEO_6Rof9-omSwa40eSClP_nd_U3IDl5wACeAgAAqKqqVC9HUqfVj09QzYE",
        "CAACAgQAAxkBAAEO_6Zof9-r8sE8I687D0_BroGJYk0VAgACBgoAAh7BqVCNxvRJo_7VJjYE",
        "CAACAgQAAxkBAAEO_6hof9-tDN4VQlW93pvV1mAZ4RFKegAC_AkAAs26qFBMsagAAdrCybw2BA",
        "CAACAgQAAxkBAAEO_6pof9-vwdwxxIuGbDuEFBwugiHcLAAC8gkAAl3gsVCYA_kmBeF0mjYE",
        "CAACAgEAAxkBAAEO_6xof9_iSuM2fRET_aqRE-JdJFWVjwAC6AMAAg11mEVm37oCrgFv7DYE",
        "CAACAgUAAxkBAAEQN_NpY5JOg2AmOsLhm49GeznfgmMpQAACJQIAAga-KVVI0nw6O0MPZTgE",
        "CAACAgQAAxkBAAEQN_VpY5JbqSirxFbfzeHUBgsjZHm-_QACNgEAAtkEzgIWqerJBL5f_TgE",
        "CAACAgIAAxkBAAEQN_dpY5J0PqTpaMSDcgcJ_Inm-_9Q5gAC4mgAAvHDsUpYZk2psq5F1DgE",
        "CAACAgUAAxkBAAEQN_lpY5MFNbwE3ct14fqOoz-ZhTp5ugAC7AIAAujFcVQeL84rjlwVfjgE",
        "CAACAgUAAxkBAAEQN_tpY5MMbY4yajoaRCShdJPz9AshhgACtAIAAm3UcVQSg_9hVHADjTgE",
        "CAACAgUAAxkBAAEQN_xpY5MNS91dgBImzpnRh45RkrGvaAACoAMAAoMVaVSljAKCVEKnzjgE",
        "CAACAgUAAxkBAAEQN_9pY5MY9QABh9OvGwEzy029hsS7jWUAAmEDAALq6GhU6sewGHaBAxY4BA",
        "CAACAgEAAxkBAAEQOAFpY5MplHDe_r2iKuH1nge-hJXrtAACIAIAAlaFuUTtPRs7XqrHwzgE",
        "CAACAgEAAxkBAAEQOAJpY5MqaNhLgPr4oOXmir2lm-vMsQAC2QEAArpRuETUekCzK70w-TgE",
        "CAACAgEAAxkBAAEQOAVpY5Ms1oVbjSFlb06izWQIRC5SZwACmQEAAtIRuUQ_GwigPbm3jjgE",
        "CAACAgEAAxkBAAEQODNpY7kufgZ-5F5_bkb_2k0CyOQtkwACxwEAAqlqWUaHS8Bld_YQQTgE",
        "CAACAgIAAxkBAAEQODVpY7k693jGToJz7cUDZK6kRjhJ7QACCwADXVdJNMEDaPgI4uLiOAQ",
        "CAACAgIAAxkBAAEQODdpY7k84KRzxuV-1-4Uz39Bk2jC2AACBQADXVdJNHdmMICXhDwyOAQ",
        "CAACAgIAAxkBAAEQODlpY7lGR9UY2VXbpXNUg5WcXGo5NAACGQADXVdJNAd_hTPrLQIzOAQ",
        "CAACAgIAAxkBAAEQODtpY7mUAWvQY8Lt2Dr0IatqtdDHhQACR1MAAoLZuErirwYCAmge3DgE",
        "CAACAgIAAxkBAAEQOD1pY7mXoFSxpYNv1HyQ6HNBwJygFwACp1IAAmz7uUpvkmmO3uD3zTgE",
        "CAACAgIAAxkBAAEQOD9pY7mcmeAHRdguBnj3zlBvjzw0vQACvUsAAkLssEp8t39ewnstEzgE",
        "CAACAgIAAxkBAAEQOEFpY7mgxAkaFLH_7QrWZS9MZhNIUwACKlsAAnKMsEpcIBOxg7lVVDgE",
        "CAACAgIAAxkBAAEQOEJpY7mg_6VhcEGDbzMYaZgkAAFQ26QAAuVKAAJlcrhKpalFWTzxXOc4BA",
        "CAACAgIAAxkBAAEQOEVpY7nITfPkngNUxQ0ISCqaivwcfAACtxMAAkym6EskZwtfmcUc6TgE",
        "CAACAgIAAxkBAAEQOEdpY7nNBwaKCCcH57AWZkN00XuhqgACeRMAAlOl6UvW_BFl3WOUTDgE",
        "CAACAgIAAxkBAAEQOElpY7nT7elNM-SJLF1qI41rB__UqAACERQAAq-n6UvPN4pbpaboIzgE",
        "CAACAgIAAxkBAAEQOEtpY7nbYi-bbNVubPFEOTWRi8onLAACChgAAr8o6UvBazIHf5U1wDgE",
        "CAACAgIAAxkBAAEQOE1pY7n0P5Pidqk5Rbwf3S07owSTRwACVgADys3hF-0saksl1j7OOAQ",
        "CAACAgIAAxkBAAEQOE9pY7n9mVaLEu7Szsex1kRvi0aljwACXwADys3hFzsOAj5yOxeNOAQ",
        "CAACAgQAAxkBAAEQOFFpY7oF4AZ4WmF87Is4m2AWkDYN_AACogADm0fWAAFWW3AgmPn3XTgE",
        "CAACAgIAAxkBAAEQOFNpY7oe_USboqYBOnN2q8sjq0Kh4gACYQEAAvYyhgoFrL2vgcVgrDgE",
        "CAACAgIAAxkBAAEQOFVpY7oplnh5LILYAZ1nu9F0GiQpbgACcwEAAvYyhgrQLmxpNLu6KjgE",
        "CAACAgEAAxkBAAEQOFdpY7o24Wi2ziRBj68i5D5Hg76qdgACMgADnCTsGm-_um7-pspSOAQ",
        "CAACAgQAAxkBAAEQOFlpY7o-m4Du_Imp-r9l-NPDf4xfcQACDgADWoONKOJL7Cy8oA1UOAQ",
        "CAACAgQAAxkBAAEQOFtpY7pVKfGE6ZOMr7iFCsNPxzk53AACEwADWoONKJ6gdUX6pxKOOAQ",
        "CAACAgEAAxkBAAEQOF1pY7p9YTbq0FPcHWCf8pelIGTJxwACLAADnCTsGs_lnv19e9-nOAQ",
        "CAACAgIAAxkBAAEQOGFpY7q9hujK1NOE4csJt-n_cHR-eQAChwEAAvYyhgr1YUfvu0GC1zgE",
        "CAACAgIAAxkBAAEQOGNpY7rFzhAkwPRjM6x3ZGx5JoIGpAACnQEAAvYyhgoUtCDWF_dMHzgE",
        "CAACAgIAAxkBAAEQOGVpY7rPD9W6EpEu3ZxIu1G1_0KLjwACVgAD39G0DblE4EsOawRqOAQ",
        "CAACAgIAAxkBAAEQOGdpY7rcSzTflqE1MqU0tY704DLG_AACXAAD39G0DRbIDmbzlmnVOAQ",
        "CAACAgQAAxkBAAEQOGlpY7rtQiGtoD0oOZkx30DKcg8NTAACgwEAAptH1gAB0Dgebrwg19s4BA",
        "CAACAgQAAxkBAAEQOG1pY7sJjQz3Bf0iLKhemNF6qT8I8AACuAADm0fWAAGbw94yLaRk2jgE",
        "CAACAgIAAxkBAAEQOG9pY7sdSANijw2pOSm2JP_UFy6kDgACNQAD1RIaCxZObRnOc3WiOAQ",
        "CAACAgIAAxkBAAEQOHFpY7svhunPmorq_1jEDh8i-chuywACBQAD1RIaC1UoKrb0hM7oOAQ",
        "CAACAgIAAxkBAAEQOHNpY7tFNCchuN7rS_DP6V55pxNG4AACbAADys3hF6AkKHk5MzNQOAQ",
        "CAACAgIAAxkBAAEQOHVpY7tKUKMO74O129QDKxhwBxfmPgAChwADys3hF1GxmAH7cZ3LOAQ",
        "CAACAgIAAxkBAAEQOHdpY7tQ8y_yTW4temiA6YylDFN4ZwACjgADys3hFz9U4DyQbfAmOAQ",
        "CAACAgIAAxkBAAEQOHlpY7tXnh74a3BVl9gJcyjV8x3zQAACkQADys3hF6lJbRnMXsgWOAQ"
    };

    return stickers;
}

const std::unordered_set<std::string>& Core::getAnimeLetters()
{
    static const std::unordered_set<std::string> letters = {
        "А", "Б", "В", "Г", "Д",
        "Е", "Ё", "Ж", "З", "И",
        "Й", "К", "Л", "М", "Н",
        "О", "П", "Р", "С", "Т",
        "У", "Ф", "Х", "Ц", "Ч",
        "Ш", "Щ", "Э", "Ю", "Я"
    };

    return letters;
}

const std::string Core::animeMarker()
{
    static const std::string marker{ "⋆" };
    return marker;
}

const std::string& Core::separationMarker()
{
    static const std::string marker{ '#' };
    return marker;
}

std::string Core::createStickerLink(const std::string& stickerName)
{
    return "https://t.me/addstickers/" + stickerName;
}