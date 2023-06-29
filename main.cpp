#include <windows.h>
#include <iostream>
#include <algorithm> // find_if
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

// Обрезаем с начала и с конца все НЕ символы
string trim(std::string str, std::string const &whiteSpaces = " \r\n\t\v\f") {
    auto start = str.find_first_not_of(whiteSpaces);
    str.erase(0, start);
    auto end = str.find_last_not_of(whiteSpaces);
    str.erase(end + 1);

    return str;
}

void printWarning(const char* currentPath) {
    printf("%s не обнаружен.\n"
           "Он должен находиться в директории с исполняемым файлом!\n", currentPath);
}

void printComplexData(std::ostream &out, vector<vector<string>> const &complexData) {
    system("cls");
    std::cout << "---------Читаем данные из файла-------------" << std::endl;
    for (auto &line : complexData) {
        for (int i = 0; i < line.size(); ++i) {
            out << line[i] << (i != line.size() - 1 ? " " : "");
        }
        out << std::endl;
    }
}

void displayComplexDataToScreen(vector<vector<string>> const &complexData) {
    // Печатаем в поток std::cout
    printComplexData(std::cout, complexData);
}

void writeComplexDataToFile(const char* path, vector<vector<string>> const &complexData) {
    std::ofstream file(path, std::ios::app);

    // Печатаем в поток file
    printComplexData(file, complexData);

    file.close();
}

// Записывает в data данные из файла по принципу: массив строк.
// Возвращает true, если чтение прошло успешно
bool readFileToVector(const char* pathName, vector<string> &data) {
    bool isFileReadSuccessfully = false;
    std::ifstream in(pathName);

    if (in.is_open() && !in.bad()) {
        string textLine;

        while (std::getline(in, textLine)) {
            data.push_back(textLine);
        }

        in.clear();
        in.seekg(0, std::ios_base::beg);
        isFileReadSuccessfully = true;
    }

    in.close();

    return isFileReadSuccessfully;
}

// Разбивает строку на подстроки и возвращает вектор
vector<string> splitStringToParts(std::string const &str, const char delim = ',', bool isEmptyDenied = true) {
    vector<string> words;
    string word;
    std::stringstream ss(str);

    // Делим строки на токены по запятой
    while (std::getline(ss, word, delim)) {
        string trimStr = trim(word);
        // Не позволяет добавлять пустой элемент
        if (trimStr.length() == 0 && isEmptyDenied) {
            continue;
        }

        words.push_back(trimStr);
    }

    return words;
}

// Записывает в data данные из файла по принципу: массив строк <- массив слов.
bool readFileToComplexData(const char* pathName, vector<vector<string>> &complexData) {
    vector<string> rows;
    bool isFileReadSuccessfully = readFileToVector(pathName, rows);

    if (isFileReadSuccessfully && !rows.empty()) {
        for (const auto &row : rows) {
            complexData.push_back(splitStringToParts(row, ','));
        }
    }

    return isFileReadSuccessfully;
}

bool isNumeric(std::string const &str) {
    auto it = std::find_if(
            str.begin(),
            str.end(),
            [](char const &c) { return !std::isdigit(c); });

    return !str.empty() && it == str.end();
}

bool isDate(std::string const &str, std::string &cause) {
    bool isValid = true;
    vector<vector<int>> ranges = { { 1, 31 }, { 1, 12 }, { 1950, 2030 } };
    vector<string> parts = splitStringToParts(str, '.', false);

    if (parts.size() != 3) {
        cause += "Формат ввода: ДД.ММ.ГГГГ\n";
        isValid = false;
    }

    for (int i = 0; i < parts.size(); ++i) {
        std::string current = parts[i];

        if (!isNumeric(current)) {
            char warning[100];
            sprintf(warning, "%i часть (%s) не является цифрой\n", (i + 1), current.c_str());
            cause += warning;
            isValid = false;
            continue;
        }

        int part = std::stoi(current);
        auto range = ranges[i];

        if (part < range[0] || part > range[1]) {
            char warning[100];
            sprintf(warning, "%i часть (%i) должна быть в диапазоне %i - %i\n", (i + 1), part, range[0], range[1]);
            cause += warning;
            isValid = false;
        }
    }

    return isValid;
}

std::string getUserWord(const std::string &propose) {
    std::string userInput;
    printf("%s: ", propose.c_str());
    std::getline(std::cin, userInput);

    // Извлечём первое слово из введенных
    return splitStringToParts(userInput, ' ')[0];
}

int getNumeric(const std::string &propose = "Введите цифры: ") {
    std::string userInput;

    while (true) {
        std::string temp = getUserWord(propose);

        if (isNumeric(temp)) {
            userInput = temp;
            break;
        }

        std::cout << "Попробуйте снова. Это должно быть целое число" << std::endl;
    }

    return std::stoi(userInput);
}

vector<string> getRecords(vector<string> const &dataFormat, int numberOfRecord = 0) {
    vector<string> records;
    printf("-----Запись номер %i -----\n", numberOfRecord + 1);

    for (int i = 0; i < dataFormat.size(); ++i) {
        string currentRecord;
        bool isValidInput = false;

        while (!isValidInput) {
            std::string temp = getUserWord("Введите " + dataFormat[i]);

            if (!temp.length()) {
                std::cout << "Попробуйте снова. Строка не должна быть пустой" << std::endl;
                continue;
            }

            string cause;
            if (i == 2 && !isDate(temp, cause)) {
                std::cout << cause << std::endl;
                continue;
            }

            if (i == 3 && !isNumeric(temp)) {
                std::cout << "Попробуйте снова. Это должно быть целое число" << std::endl;
                continue;
            }

            currentRecord = temp;
            isValidInput = true;
        }

        records.push_back(currentRecord);
    }

    return records;
}

void setComplexData(vector<string> const &dataFormat, vector<vector<string>> &complexData) {
    int userCount = getNumeric("Введите количество пользователей");

    for (int i = 0; i < userCount; ++i) {
        complexData.push_back(getRecords(dataFormat, i));
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    const char* path = R"(..\test.txt)";
    vector<string> dataFormat = { "фамилию (латиницей)", "имя (латиницей)", "дату выдачи (ДД.ММ.ГГГГ)", "сумму (целое число)" };

    vector<vector<string>> complexData;
    vector<vector<string>> complexDataFromFile;

    // Получаем от пользователя данные. Пока в оперативную память (complexData)
    setComplexData(dataFormat, complexData);
    // Записываем данные в файл
    writeComplexDataToFile(path, complexData);

    // Извлекаем данные из файла
    bool isFileReadSuccessfully = readFileToComplexData(path, complexDataFromFile);
    // Проверяем флаг о состоянии чтения
    if (!isFileReadSuccessfully) {
        printWarning(path);
        return 1;
    }
    // Проверяем флаг о состоянии прочтенных данных. Что-то должно быть
    if (complexDataFromFile.empty()) {
        std::cout << "Проверьте файлы на корректность данных" << std::endl;
        return 1;
    }

    displayComplexDataToScreen(complexDataFromFile);
}