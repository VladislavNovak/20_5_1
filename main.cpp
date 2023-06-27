#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

// Обрезаем с начала и с конца все НЕ символы
std::string trim(std::string const &str, std::string const &whitespace=" \r\n\t\v\f") {
    auto start = str.find_first_not_of(whitespace);
    auto end = str.find_last_not_of(whitespace);

    return str.substr(start, end - start + 1);
}

void printWarning(const char* currentPath) {
    printf("%s не обнаружен.\n"
           "Он должен находиться в директории с исполняемым файлом!\n", currentPath);
}

void printComplexData(std::ostream &out, vector<vector<string>> const &complexData) {
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
    std::ofstream file(path);

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

// Записывает в data данные из файла по принципу: массив строк <- массив слов.
bool readFileToVectorOfVectors(const char* pathName, vector<vector<string>> &data) {
    vector<string> rows;
    bool isFileReadSuccessfully = readFileToVector(pathName, rows);

    if (isFileReadSuccessfully && !rows.empty()) {
        for (const auto &row : rows) {
            vector<string> words;
            string word;
            std::stringstream ss(row);

            // Делим строки на токены по запятой
            while (std::getline(ss, word, ',')) {
                string retStr = trim(word);
                words.push_back(retStr);
            }

            data.push_back(words);
        }
    }

    return isFileReadSuccessfully;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    const char* path = R"(..\test.txt)";
    vector<vector<string>> complexData = {
            { "Tom", "Hanks", "35500", "10.11.2020" },
            { "Rebecca", "Williams", "85000", "1.1.2021" },
            { "Sally", "Field", "15600", "15.8.2021" },
            { "Michael", "Humphreys", "29400", "23.5.2020" },
            { "Michael2", "Humphreys2", "294002", "23.5.20202" },
    };

    vector<vector<string>> complexDataFromFile;

    writeComplexDataToFile(path, complexData);

    bool isFileReadSuccessfully = readFileToVectorOfVectors(path, complexDataFromFile);

    if (!isFileReadSuccessfully) {
        printWarning(path);
        return 1;
    }

    if (complexDataFromFile.empty()) {
        std::cout << "Проверьте файлы на корректность данных" << std::endl;
        return 1;
    }

    displayComplexDataToScreen(complexDataFromFile);
}