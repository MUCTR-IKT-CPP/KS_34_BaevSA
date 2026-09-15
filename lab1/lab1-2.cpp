#include <iostream>
#include <cstdlib>
#include <ctime>

const int ALPHABET_SIZE = 26;
const int MAX_LENGTH = 1000;

/*
 * Генерация случайной строки из строчных латинских букв
 *
 * @param p_text указатель на массив символов, куда записывается строка.
 * @param length длина генерируемой строки.
 * @return ничего не возвращает.
 */
void generateString(char* p_text, int const length) {
    for (int i = 0; i < length; i++) {
        p_text[i] = 'a' + rand() % ALPHABET_SIZE;
    }
    p_text[length] = '\0';
}

/*
 * Подсчёт количества вхождений каждой буквы в строке
 *
 * @param p_text указатель на исследуемую строку.
 * @param p_counts указатель на массив счётчиков размером ALPHABET_SIZE.
 * @return ничего не возвращает.
 */
void countLetters(const char* p_text, int* p_counts) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        p_counts[i] = 0;
    }
    for (int i = 0; p_text[i] != '\0'; i++) {
        p_counts[p_text[i] - 'a']++;
    }
}

/*
 * Поиск буквы, которая встречается чаще всего
 *
 * @param p_counts указатель на массив счётчиков букв.
 * @return возвращает самую частую букву (при равенстве — первую по алфавиту).
 */
char findMostFrequent(const int* p_counts) {
    int best = 0;
    for (int i = 1; i < ALPHABET_SIZE; i++) {
        if (p_counts[i] > p_counts[best]) {
            best = i;
        }
    }
    return 'a' + best;
}

/*
 * Вывод частот букв в виде гистограммы из символов '*'
 *
 * @param p_counts указатель на массив счётчиков букв.
 * @return ничего не возвращает.
 */
void printHistogram(const int* p_counts) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (p_counts[i] > 0) {
            std::cout << static_cast<char>('a' + i) << ": ";
            for (int j = 0; j < p_counts[i]; j++) {
                std::cout << '*';
            }
            std::cout << std::endl;
        }
    }
}

/*
 * Полный анализ строки: генерация, подсчёт, гистограмма и самая частая буква
 *
 * @param length длина генерируемой строки.
 * @return ничего не возвращает.
 */
void analyzeString(int const length) {
    char text[MAX_LENGTH + 1] = {};
    int counts[ALPHABET_SIZE] = {};

    generateString(text, length);
    countLetters(text, counts);

    std::cout << "N = " << length << std::endl;
    std::cout << "Строка: " << text << std::endl;
    printHistogram(counts);
    std::cout << "Чаще всего встречается: " << findMostFrequent(counts) << std::endl;
    std::cout << std::endl;
}

/*
 * Точка входа: запуск анализа для тестовых длин строк
 *
 * @return возвращает код завершения программы.
 */
int main() {
    const int TEST_COUNT = 3;
    const int TEST_LENGTHS[TEST_COUNT] = {10, 100, 1000};

    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < TEST_COUNT; i++) {
        analyzeString(TEST_LENGTHS[i]);
    }
    return 0;
}
