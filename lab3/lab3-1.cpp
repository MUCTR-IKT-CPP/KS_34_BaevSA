#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

const int MIN_COUNT = 1;
const int MAX_COUNT = 100;
const int TYPE_COUNT = 4;
const int MIN_YEAR = 2015;
const int MAX_YEAR = 2025;
const int MONTH_COUNT = 12;
const int WORD_COUNT = 10;
const int MAX_INDEX = 1000;
const int CENTS = 100;
const int IGNORE_LIMIT = 10000;
const std::string DATA_FILE_NAME = "media_files.txt";

/**
 * Тип медиафайла.
 */
enum FileType {
    AUDIO = 0,
    VIDEO = 1,
    IMAGE = 2,
    DOCUMENT = 3
};

/**
 * Календарная дата.
 */
struct Date {
    int day = 1;
    int month = 1;
    int year = MIN_YEAR;
};

/**
 * Описание одного медиафайла в системе управления контентом.
 */
struct MediaFile {
    int id = 0;
    std::string filename = "";
    double size = 0.0;
    FileType type = AUDIO;
    Date creation_date = {};
};

/**
 * Переводит тип файла в его текстовое название.
 *
 * @param type тип медиафайла.
 * @return строка с названием типа.
 */
std::string typeToString(FileType type) {
    if (type == AUDIO) {
        return "Audio";
    } else if (type == VIDEO) {
        return "Video";
    } else if (type == IMAGE) {
        return "Image";
    }
    return "Document";
}

/**
 * Возвращает расширение файла, характерное для указанного типа.
 *
 * @param type тип медиафайла.
 * @return строка с расширением, включая точку.
 */
std::string typeToExtension(FileType type) {
    if (type == AUDIO) {
        return ".mp3";
    } else if (type == VIDEO) {
        return ".mp4";
    } else if (type == IMAGE) {
        return ".png";
    }
    return ".pdf";
}

/**
 * Проверяет, является ли год високосным.
 *
 * @param year проверяемый год.
 * @return true, если год високосный.
 */
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

/**
 * Определяет количество дней в месяце указанного года.
 *
 * @param month номер месяца от 1 до 12.
 * @param year год, к которому относится месяц.
 * @return количество дней в месяце.
 */
int daysInMonth(int month, int year) {
    const int DAYS[MONTH_COUNT] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return DAYS[month - 1];
}

/**
 * Приводит строку к нижнему регистру для сравнения без учёта регистра.
 *
 * @param text исходная строка.
 * @return копия строки, приведённая к нижнему регистру.
 */
std::string toLower(const std::string& text) {
    std::string result = text;

    for (size_t i = 0; i < result.size(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    return result;
}

/**
 * Проверяет, содержит ли строка заданную подстроку без учёта регистра.
 *
 * @param text строка, в которой выполняется поиск.
 * @param pattern искомая подстрока.
 * @return true, если подстрока найдена.
 */
bool containsIgnoreCase(const std::string& text, const std::string& pattern) {
    return toLower(text).find(toLower(pattern)) != std::string::npos;
}

/**
 * Генерирует случайный размер файла в мегабайтах в границах, разумных для его типа.
 *
 * @param type тип медиафайла.
 * @return размер файла в мегабайтах.
 */
double generateSize(FileType type) {
    int min_size = 1;
    int max_size = 10;

    if (type == AUDIO) {
        min_size = 3;
        max_size = 20;
    } else if (type == VIDEO) {
        min_size = 50;
        max_size = 2000;
    } else if (type == IMAGE) {
        min_size = 1;
        max_size = 15;
    } else {
        min_size = 1;
        max_size = 5;
    }
    double whole = min_size + rand() % (max_size - min_size + 1);
    double fraction = (rand() % CENTS) / static_cast<double>(CENTS);
    return whole + fraction;
}

/**
 * Генерирует случайную корректную дату создания файла.
 *
 * @return структура Date со случайными днём, месяцем и годом.
 */
Date generateDate() {
    Date date = {};

    date.year = MIN_YEAR + rand() % (MAX_YEAR - MIN_YEAR + 1);
    date.month = 1 + rand() % MONTH_COUNT;
    date.day = 1 + rand() % daysInMonth(date.month, date.year);
    return date;
}

/**
 * Генерирует один медиафайл со случайными, но правдоподобными данными.
 *
 * @param id уникальный идентификатор создаваемого файла.
 * @return заполненная структура MediaFile.
 */
MediaFile generateFile(int id) {
    const std::string WORDS[WORD_COUNT] = {
        "report", "holiday", "concert", "lecture", "backup",
        "summer", "project", "family", "meeting", "travel"
    };
    MediaFile file = {};

    file.id = id;
    file.type = static_cast<FileType>(rand() % TYPE_COUNT);
    file.filename = WORDS[rand() % WORD_COUNT] + "_" + std::to_string(rand() % MAX_INDEX) + typeToExtension(file.type);
    file.size = generateSize(file.type);
    file.creation_date = generateDate();
    return file;
}

/**
 * Создаёт вектор из указанного количества случайных медиафайлов.
 *
 * @param count количество генерируемых файлов.
 * @return вектор структур MediaFile.
 */
std::vector<MediaFile> generateFiles(int count) {
    std::vector<MediaFile> files = {};

    for (int i = 0; i < count; i++) {
        files.push_back(generateFile(i + 1));
    }
    return files;
}

/**
 * Выводит в терминал строку заголовка таблицы файлов.
 * Названия столбцов заданы латиницей, чтобы ширина полей setw совпадала
 * с шириной строк данных: в UTF-8 кириллический символ занимает два байта.
 *
 * @return ничего не возвращает.
 */
void printHeader() {
    std::cout << std::left << std::setw(5) << "ID" << std::setw(22) << "Filename";
    std::cout << std::setw(10) << "Type" << std::right << std::setw(11) << "Size";
    std::cout << "   Date" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
}

/**
 * Выводит в терминал данные одного медиафайла в табличном виде.
 *
 * @param file выводимый медиафайл.
 * @return ничего не возвращает.
 */
void printFile(const MediaFile& file) {
    std::cout << std::left << std::setw(5) << file.id << std::setw(22) << file.filename;
    std::cout << std::setw(10) << typeToString(file.type);
    std::cout << std::right << std::setw(8) << std::fixed << std::setprecision(2) << file.size << " MB";
    std::cout << "   " << std::setfill('0') << std::setw(2) << file.creation_date.day << '.';
    std::cout << std::setw(2) << file.creation_date.month << '.' << file.creation_date.year;
    std::cout << std::setfill(' ') << std::endl;
}

/**
 * Выводит в терминал весь список медиафайлов.
 *
 * @param files вектор медиафайлов, передаётся по константной ссылке.
 * @return ничего не возвращает.
 */
void printFiles(const std::vector<MediaFile>& files) {
    if (files.empty()) {
        std::cout << "Список пуст" << std::endl;
        return;
    }
    printHeader();
    for (size_t i = 0; i < files.size(); i++) {
        printFile(files[i]);
    }
}

/**
 * Находит все файлы, имя которых содержит заданную подстроку без учёта регистра.
 *
 * @param files исходный вектор медиафайлов.
 * @param pattern искомая подстрока имени.
 * @return новый вектор с подходящими файлами.
 */
std::vector<MediaFile> searchByName(const std::vector<MediaFile>& files, const std::string& pattern) {
    std::vector<MediaFile> result = {};

    for (size_t i = 0; i < files.size(); i++) {
        if (containsIgnoreCase(files[i].filename, pattern)) {
            result.push_back(files[i]);
        }
    }
    return result;
}

/**
 * Формирует новый массив файлов указанного типа с размером больше заданного.
 *
 * @param files исходный вектор медиафайлов.
 * @param type требуемый тип файлов.
 * @param min_size минимальный размер в мегабайтах, не включая само значение.
 * @return новый вектор с подходящими файлами.
 */
std::vector<MediaFile> filterByTypeAndSize(
    const std::vector<MediaFile>& files,
    FileType type,
    double min_size
) {
    std::vector<MediaFile> result = {};

    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].type == type && files[i].size > min_size) {
            result.push_back(files[i]);
        }
    }
    return result;
}

/**
 * Выводит статистику: количество файлов, суммарный размер и распределение по типам.
 *
 * @param files вектор медиафайлов, передаётся по константной ссылке.
 * @return ничего не возвращает.
 */
void printStatistics(const std::vector<MediaFile>& files) {
    int counts[TYPE_COUNT] = {};
    double sizes[TYPE_COUNT] = {};
    double total_size = 0.0;

    for (size_t i = 0; i < files.size(); i++) {
        counts[files[i].type]++;
        sizes[files[i].type] = sizes[files[i].type] + files[i].size;
        total_size = total_size + files[i].size;
    }
    std::cout << "Всего файлов: " << files.size() << std::endl;
    std::cout << "Суммарный размер: " << std::fixed << std::setprecision(2) << total_size << " MB" << std::endl;
    std::cout << "Распределение по типам:" << std::endl;
    for (int i = 0; i < TYPE_COUNT; i++) {
        FileType type = static_cast<FileType>(i);

        std::cout << "  " << std::left << std::setw(10) << typeToString(type) << counts[i] << " шт.";
        std::cout << " (" << std::fixed << std::setprecision(2) << sizes[i] << " MB)" << std::endl;
    }
    std::cout << std::right;
}

/**
 * Сравнивает две даты по хронологии.
 *
 * @param first первая сравниваемая дата.
 * @param second вторая сравниваемая дата.
 * @return отрицательное число, если first раньше second, 0 при равенстве, иначе положительное.
 */
int compareDates(const Date& first, const Date& second) {
    if (first.year != second.year) {
        return first.year - second.year;
    }
    if (first.month != second.month) {
        return first.month - second.month;
    }
    return first.day - second.day;
}

/**
 * Определяет, должен ли первый файл стоять раньше второго при сортировке.
 * Сначала сравниваются даты от новых к старым, при равных датах — имена по алфавиту.
 *
 * @param first первый сравниваемый файл.
 * @param second второй сравниваемый файл.
 * @return true, если первый файл должен стоять раньше второго.
 */
bool goesBefore(const MediaFile& first, const MediaFile& second) {
    int date_diff = compareDates(first.creation_date, second.creation_date);

    if (date_diff != 0) {
        return date_diff > 0;
    }
    return toLower(first.filename) < toLower(second.filename);
}

/**
 * Сортирует вектор файлов по дате создания от новых к старым,
 * а при совпадении дат — по имени файла в алфавитном порядке.
 * Используется сортировка пузырьком, вектор изменяется по ссылке.
 *
 * @param files сортируемый вектор медиафайлов, передаётся по ссылке.
 * @return ничего не возвращает.
 */
void sortFiles(std::vector<MediaFile>& files) {
    for (size_t i = 0; i + 1 < files.size(); i++) {
        for (size_t j = 0; j + 1 < files.size() - i; j++) {
            if (goesBefore(files[j + 1], files[j])) {
                MediaFile tmp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = tmp;
            }
        }
    }
}

/**
 * Сохраняет вектор медиафайлов в текстовый файл.
 * Каждая строка содержит поля одной структуры, разделённые пробелами.
 *
 * @param files сохраняемый вектор медиафайлов.
 * @param file_name имя текстового файла.
 * @return true, если запись прошла успешно.
 */
bool saveToFile(const std::vector<MediaFile>& files, const std::string& file_name) {
    std::ofstream output(file_name);

    if (!output.is_open()) {
        return false;
    }
    output << files.size() << std::endl;
    for (size_t i = 0; i < files.size(); i++) {
        output << files[i].id << ' ' << files[i].filename << ' ';
        output << std::fixed << std::setprecision(2) << files[i].size << ' ';
        output << static_cast<int>(files[i].type) << ' ';
        output << files[i].creation_date.day << ' ' << files[i].creation_date.month << ' ';
        output << files[i].creation_date.year << std::endl;
    }
    output.close();
    return true;
}

/**
 * Загружает вектор медиафайлов из текстового файла, заменяя его текущее содержимое.
 *
 * @param files вектор, в который загружаются данные, передаётся по ссылке.
 * @param file_name имя текстового файла.
 * @return true, если чтение прошло успешно.
 */
bool loadFromFile(std::vector<MediaFile>& files, const std::string& file_name) {
    std::ifstream input(file_name);

    if (!input.is_open()) {
        return false;
    }
    int count = 0;

    input >> count;
    if (input.fail() || count < 0) {
        input.close();
        return false;
    }
    std::vector<MediaFile> loaded = {};

    for (int i = 0; i < count; i++) {
        MediaFile file = {};
        int type_index = 0;

        input >> file.id >> file.filename >> file.size >> type_index;
        input >> file.creation_date.day >> file.creation_date.month >> file.creation_date.year;
        if (input.fail() || type_index < 0 || type_index >= TYPE_COUNT) {
            input.close();
            return false;
        }
        file.type = static_cast<FileType>(type_index);
        loaded.push_back(file);
    }
    input.close();
    files = loaded;
    return true;
}

/**
 * Читает целое число из заданного диапазона, повторяя запрос при ошибке ввода.
 *
 * @param prompt приглашение, выводимое пользователю.
 * @param min_value минимально допустимое значение.
 * @param max_value максимально допустимое значение.
 * @return корректное число из диапазона.
 */
int readInt(const std::string& prompt, int min_value, int max_value) {
    int value = 0;

    std::cout << prompt;
    std::cin >> value;
    while (
        std::cin.fail() ||
        value < min_value ||
        value > max_value
    ) {
        std::cin.clear();
        std::cin.ignore(IGNORE_LIMIT, '\n');
        std::cout << "Некорректное значение, повторите ввод: ";
        std::cin >> value;
    }
    return value;
}

/**
 * Читает вещественное число, повторяя запрос при ошибке ввода.
 *
 * @param prompt приглашение, выводимое пользователю.
 * @return введённое число.
 */
double readDouble(const std::string& prompt) {
    double value = 0.0;

    std::cout << prompt;
    std::cin >> value;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(IGNORE_LIMIT, '\n');
        std::cout << "Некорректное значение, повторите ввод: ";
        std::cin >> value;
    }
    return value;
}

/**
 * Запрашивает у пользователя тип файла по номеру из списка.
 *
 * @return выбранный тип медиафайла.
 */
FileType readType() {
    std::cout << "Типы: ";
    for (int i = 0; i < TYPE_COUNT; i++) {
        std::cout << i + 1 << " - " << typeToString(static_cast<FileType>(i)) << "  ";
    }
    std::cout << std::endl;

    int number = readInt("Выберите тип: ", 1, TYPE_COUNT);
    return static_cast<FileType>(number - 1);
}

/**
 * Выводит меню доступных действий над коллекцией файлов.
 *
 * @return ничего не возвращает.
 */
void printMenu() {
    std::cout << std::endl;
    std::cout << "Выберите действие:" << std::endl;
    std::cout << "1 - показать все файлы" << std::endl;
    std::cout << "2 - поиск по имени" << std::endl;
    std::cout << "3 - фильтр по типу и размеру" << std::endl;
    std::cout << "4 - статистика" << std::endl;
    std::cout << "5 - сортировка по дате и имени" << std::endl;
    std::cout << "6 - сохранить в файл" << std::endl;
    std::cout << "7 - загрузить из файла" << std::endl;
    std::cout << "0 - выход" << std::endl;
    std::cout << "> ";
}

/**
 * Выполняет выбранное пользователем действие над коллекцией файлов.
 *
 * @param files коллекция медиафайлов, передаётся по ссылке, так как может изменяться.
 * @param command номер выбранного действия.
 * @return ничего не возвращает.
 */
void applyCommand(std::vector<MediaFile>& files, int command) {
    if (command == 1) {
        printFiles(files);
    } else if (command == 2) {
        std::string pattern = "";

        std::cout << "Введите подстроку имени: ";
        std::cin >> pattern;

        std::vector<MediaFile> found = searchByName(files, pattern);
        std::cout << "Найдено файлов: " << found.size() << std::endl;
        printFiles(found);
    } else if (command == 3) {
        FileType type = readType();
        double min_size = readDouble("Введите минимальный размер в MB: ");

        std::vector<MediaFile> filtered = filterByTypeAndSize(files, type, min_size);
        std::cout << "Подходящих файлов: " << filtered.size() << std::endl;
        printFiles(filtered);
    } else if (command == 4) {
        printStatistics(files);
    } else if (command == 5) {
        sortFiles(files);
        std::cout << "Массив отсортирован:" << std::endl;
        printFiles(files);
    } else if (command == 6) {
        if (saveToFile(files, DATA_FILE_NAME)) {
            std::cout << "Сохранено в файл " << DATA_FILE_NAME << std::endl;
        } else {
            std::cout << "Не удалось открыть файл для записи" << std::endl;
        }
    } else if (command == 7) {
        if (loadFromFile(files, DATA_FILE_NAME)) {
            std::cout << "Загружено из файла " << DATA_FILE_NAME << std::endl;
            printFiles(files);
        } else {
            std::cout << "Не удалось прочитать файл" << std::endl;
        }
    } else {
        std::cout << "Неизвестное действие" << std::endl;
    }
}

/**
 * Точка входа: создаёт коллекцию файлов и обрабатывает команды пользователя.
 *
 * @return код завершения программы.
 */
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    int const count = readInt("Введите N (от 1 до 100): ", MIN_COUNT, MAX_COUNT);
    std::vector<MediaFile> files = generateFiles(count);
    int command = -1;

    std::cout << "Сгенерированная коллекция:" << std::endl;
    printFiles(files);

    while (command != 0) {
        printMenu();
        std::cin >> command;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(IGNORE_LIMIT, '\n');
            std::cout << "Некорректный ввод" << std::endl;
            command = -1;
        } else if (command != 0) {
            applyCommand(files, command);
        }
    }
    std::cout << "Завершение работы" << std::endl;
    return 0;
}
