#include <iostream>
#include <vector>
#include <tuple>
#include <windows.h>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <string>

int safeVarRead(const std::string &preMessage, bool mayBeNegative) {
	std::cout << preMessage << "\n";
	std::string input;
	std::cin >> input;
	// Если число слишком большое или это вообще не число - просим строку заново
	return input.length() < 9 && std::regex_match(input, std::regex(
			std::string(mayBeNegative ? "-" : " ") + "?[0-9]+([\\.][0-9]+)?")
	) ? std::stoi(input) : safeVarRead("Wrong number... Try again:", mayBeNegative);
}

int main() {
	// Обозначаем размер матрицы
	int m = safeVarRead("Enter rows count...", false),
			n = safeVarRead("Enter columns count...", false);

	// Создание матрицы m X (ключ - значение)
	std::vector<std::pair<int, std::vector<double>>> matrix;

	// Максимальное число для подсчета размера пробела
	double max = DBL_MIN;

	// Ввод каждого элемента матрицы
	for (auto i = 0; i < m; i++) {
		for (auto j = 0; j < n; j++) {
			// Если номер строки >4 или номер столбца >4, тогда ставим случайное число
			double number = i >= 3 || j >= 3 ?
							((int) (((double) std::rand() / RAND_MAX - 0.5) * 20)) :
							safeVarRead("Enter " + std::to_string(i * n + j + 1) + "`st number:", true);
			// Если число по модулю больше максимума - переписываем максимум
			max = max < abs(number) ? abs(number) : max;
			// Засовываем число в матрицу, если нужна новая строчка - то создаем ее
			if (matrix.size() < i + 1) { matrix.push_back({i, {number}}); }
			else { matrix[i].second.push_back(number); }
		}
	}

	// Создаем копию матрицы
	auto copy = std::vector<std::pair<int, std::vector<double>>>(matrix);

	// Сортируем копию по первому значению
	sort(copy.begin(), copy.end(), [](auto &first, auto &second) { return first.second[0] > second.second[0]; });
	// Оставляем только дубликаты / Дубликат если ключи равны, а значения не равны по условию
	copy.erase(unique(copy.begin(), copy.end(), [](auto &first, auto &second) {
		return first.second[0] == second.second[0] && first.second != second.second;
	}), copy.end());

	auto stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Перебираем строчки
	std::cout << "Your matrix:\n";
	for (const auto &item: matrix) {
		// Если в копии, в которой остались только дубликаты, есть проходимый элемент, значит красим и помечаем копией
		auto isCopy = std::find(copy.begin(), copy.end(), item) == copy.end();

		// Если эта строчка дубликат - ставим красный фон и белый цвет текста
		SetConsoleTextAttribute(stdHandle, isCopy ? 15 | BACKGROUND_RED : 15);

		// Выводим число с отступом в зависимости от самого длинного числа
		for (const auto &node: item.second)
			std::cout << std::setw(std::to_string(max).length() + 1) << node;
		// Дополнительно помечаем строку словом DUPLICATE
		std::cout << (isCopy ? " < DUPLICATE" : "") << "\n";
	}
	return 0;
}