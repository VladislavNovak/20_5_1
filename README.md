## <font color="tomato">Реализация записи в ведомость учёта</font>

Требуется создать простую программу записи в эту ведомость 
(по аналогии с [19_5_3](https://github.com/VladislavNovak/19_5_3)).

Сначала идёт имя и фамилия получателя денежных средств, далее располагается дата выдачи в формате ДД.ММ.ГГГГ. 
Завершает запись сумма выплаты в рублях. Данные разделяются между собой пробелами. 
В конце каждой записи должен быть расположен перевод строки.

При старте программы пользователь последовательно вводит данные для новой записи, 
которые записываются затем в файл в текстовом режиме. 

Программа должна добавлять новые записи в конец файла, не удаляя его текущее содержимое.

### <font color="tomato">Примечание к решению:</font>

Использованы функции из [16_6_4](https://github.com/VladislavNovak/16_6_4):

`trim`, `isNumeric`, `getUserString`, `getNumeric`, `getRecords`, `setComplexData`

Важно: решения для ввода кириллицы пока не нашел

### <font color="tomato">Перенаправление потока:</font>

Когда нужно перенаправить поток, можно использовать или тот вариант, 
который представлен в исполняемом коде, в функции `printComplexData` и 
производных `writeComplexDataToFile`, `displayComplexDataToScreen`, либо использовать следующий подход:

```c++
void printComplexData(vector<vector<string>> const &complexData) {
    for (auto &line : complexData) {
        for (int i = 0; i < line.size(); ++i) {
            std::cout << line[i] << (i != line.size() - 1 ? " " : "");
        }
        std::cout << std::endl;
    }
}

void writeComplexDataToFile(const char* path, vector<vector<string>> const &complexData) {
    std::ofstream file(path);

    // Сохраняем поток в буфер
    std::streambuf* buf;
    buf = std::cout.rdbuf();
    // Теперь std::cout печатает в файл
    std::cout.rdbuf(file.rdbuf());
    printComplexData(complexData);

    // Восстанавливаем стандартный поток
    std::cout.rdbuf(buf);

    file.close();
}
```

### <font color="tomato">Полезные ссылки</font>

В двух следующих ссылках - обсуждение вопроса перенаправления потока:

[Перенаправление стандартного ввода/вывода в файл](https://programmersforum.ru/showthread.php?t=80053)

[Как перенаправить вывод](https://rsdn.org/forum/cpp/2361792.hot)
