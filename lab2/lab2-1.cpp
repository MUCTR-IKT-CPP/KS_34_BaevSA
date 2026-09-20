#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_VALUE = 100;
const int MIN_SIZE = 1;
const int MAX_SIZE = 20;

/*
 * Выделение памяти под квадратную матрицу размером size * size
 *
 * @param size размер стороны матрицы.
 * @return возвращает указатель на массив указателей на строки матрицы.
 */
int** allocateMatrix(int size) {
    int** p_matrix = new int*[size];

    for (int i = 0; i < size; i++) {
        p_matrix[i] = new int[size];
    }
    return p_matrix;
}

/*
 * Освобождение памяти, занятой квадратной матрицей
 *
 * @param p_matrix указатель на матрицу.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void freeMatrix(int** p_matrix, int size) {
    for (int i = 0; i < size; i++) {
        delete[] p_matrix[i];
    }
    delete[] p_matrix;
}

/*
 * Заполнение матрицы случайными числами от 0 до 99
 *
 * @param p_matrix указатель на матрицу.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void fillMatrix(int** p_matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            p_matrix[i][j] = rand() % MAX_VALUE;
        }
    }
}

/*
 * Вывод матрицы в терминал построчно
 *
 * @param p_matrix указатель на матрицу.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void printMatrix(int** p_matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << p_matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

/*
 * Отражение матрицы относительно главной диагонали
 *
 * @param p_matrix указатель на матрицу, изменяемую на месте.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void reflectMainDiagonal(int** p_matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int tmp = p_matrix[i][j];
            p_matrix[i][j] = p_matrix[j][i];
            p_matrix[j][i] = tmp;
        }
    }
}

/*
 * Отражение матрицы относительно побочной диагонали
 *
 * @param p_matrix указатель на матрицу, изменяемую на месте.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void reflectSideDiagonal(int** p_matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            int tmp = p_matrix[i][j];
            p_matrix[i][j] = p_matrix[size - 1 - j][size - 1 - i];
            p_matrix[size - 1 - j][size - 1 - i] = tmp;
        }
    }
}

/*
 * Подсчёт суммы элементов одной строки матрицы
 *
 * @param p_row указатель на строку матрицы.
 * @param size количество элементов в строке.
 * @return возвращает сумму элементов строки.
 */
int rowSum(const int* p_row, int size) {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        sum = sum + p_row[i];
    }
    return sum;
}

/*
 * Обмен местами строк с максимальной и минимальной суммой элементов
 *
 * @param p_matrix указатель на матрицу, изменяемую на месте.
 * @param size размер стороны матрицы.
 * @return ничего не возвращает.
 */
void swapMinMaxRows(int** p_matrix, int size) {
    int max_index = 0;
    int min_index = 0;
    int max_sum = rowSum(p_matrix[0], size);
    int min_sum = max_sum;

    for (int i = 1; i < size; i++) {
        int sum = rowSum(p_matrix[i], size);

        if (sum > max_sum) {
            max_sum = sum;
            max_index = i;
        }
        if (sum < min_sum) {
            min_sum = sum;
            min_index = i;
        }
    }
    int* p_tmp_row = p_matrix[max_index];
    p_matrix[max_index] = p_matrix[min_index];
    p_matrix[min_index] = p_tmp_row;

    std::cout << "Строка с максимальной суммой: " << max_index + 1 << " (" << max_sum << ")" << std::endl;
    std::cout << "Строка с минимальной суммой: " << min_index + 1 << " (" << min_sum << ")" << std::endl;
}

/*
 * Построение транспонированной копии матрицы без изменения исходной
 *
 * @param p_matrix указатель на исходную матрицу.
 * @param size размер стороны матрицы.
 * @return возвращает указатель на новую транспонированную матрицу.
 */
int** buildTransposed(int** p_matrix, int size) {
    int** p_result = allocateMatrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            p_result[j][i] = p_matrix[i][j];
        }
    }
    return p_result;
}

/*
 * Вывод меню доступных действий над матрицей
 *
 * @return ничего не возвращает.
 */
void printMenu() {
    std::cout << std::endl;
    std::cout << "Выберите действие:" << std::endl;
    std::cout << "1 - отразить относительно главной диагонали" << std::endl;
    std::cout << "2 - отразить относительно побочной диагонали" << std::endl;
    std::cout << "3 - поменять местами строки с максимальной и минимальной суммой" << std::endl;
    std::cout << "4 - построить транспонированную копию" << std::endl;
    std::cout << "0 - выход" << std::endl;
    std::cout << "> ";
}

/*
 * Выполнение выбранного пользователем действия над матрицей
 *
 * @param p_matrix указатель на матрицу.
 * @param size размер стороны матрицы.
 * @param command номер выбранного действия.
 * @return ничего не возвращает.
 */
void applyCommand(int** p_matrix, int size, int command) {
    if (command == 1) {
        reflectMainDiagonal(p_matrix, size);
        std::cout << "Результат:" << std::endl;
        printMatrix(p_matrix, size);
    } else if (command == 2) {
        reflectSideDiagonal(p_matrix, size);
        std::cout << "Результат:" << std::endl;
        printMatrix(p_matrix, size);
    } else if (command == 3) {
        swapMinMaxRows(p_matrix, size);
        std::cout << "Результат:" << std::endl;
        printMatrix(p_matrix, size);
    } else if (command == 4) {
        int** p_transposed = buildTransposed(p_matrix, size);

        std::cout << "Транспонированная копия:" << std::endl;
        printMatrix(p_transposed, size);
        freeMatrix(p_transposed, size);
        std::cout << "Исходная матрица не изменилась:" << std::endl;
        printMatrix(p_matrix, size);
    } else {
        std::cout << "Неизвестное действие" << std::endl;
    }
}

/*
 * Вывод тестовых наборов из задания: заданная матрица 2x2 и случайная 4x4
 *
 * @return ничего не возвращает.
 */
void runTests() {
    const int FIRST_SIZE = 2;
    const int SECOND_SIZE = 4;
    const int FIRST_VALUES[FIRST_SIZE][FIRST_SIZE] = {{1, 2}, {3, 4}};

    std::cout << "=== Тестовый набор 1: N=2, матрица [[1,2],[3,4]] ===" << std::endl;
    int** p_first = allocateMatrix(FIRST_SIZE);

    for (int i = 0; i < FIRST_SIZE; i++) {
        for (int j = 0; j < FIRST_SIZE; j++) {
            p_first[i][j] = FIRST_VALUES[i][j];
        }
    }
    std::cout << "Матрица:" << std::endl;
    printMatrix(p_first, FIRST_SIZE);

    int** p_transposed = buildTransposed(p_first, FIRST_SIZE);
    std::cout << "Транспонирование:" << std::endl;
    printMatrix(p_transposed, FIRST_SIZE);
    freeMatrix(p_transposed, FIRST_SIZE);
    freeMatrix(p_first, FIRST_SIZE);

    std::cout << std::endl;
    std::cout << "=== Тестовый набор 2: N=4, случайные числа ===" << std::endl;
    int** p_second = allocateMatrix(SECOND_SIZE);

    fillMatrix(p_second, SECOND_SIZE);
    std::cout << "Матрица:" << std::endl;
    printMatrix(p_second, SECOND_SIZE);

    reflectMainDiagonal(p_second, SECOND_SIZE);
    std::cout << "Отражение относительно главной диагонали:" << std::endl;
    printMatrix(p_second, SECOND_SIZE);

    reflectSideDiagonal(p_second, SECOND_SIZE);
    std::cout << "Отражение относительно побочной диагонали:" << std::endl;
    printMatrix(p_second, SECOND_SIZE);

    swapMinMaxRows(p_second, SECOND_SIZE);
    std::cout << "Обмен строк с максимальной и минимальной суммой:" << std::endl;
    printMatrix(p_second, SECOND_SIZE);
    freeMatrix(p_second, SECOND_SIZE);
    std::cout << std::endl;
}

/*
 * Запрос у пользователя размера матрицы с проверкой допустимого диапазона
 *
 * @return возвращает корректный размер стороны матрицы.
 */
int readSize() {
    int size = 0;

    std::cout << "Введите N (от " << MIN_SIZE << " до " << MAX_SIZE << "): ";
    std::cin >> size;
    while (
        std::cin.fail() ||
        size < MIN_SIZE ||
        size > MAX_SIZE
    ) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Некорректное значение, повторите ввод: ";
        std::cin >> size;
    }
    return size;
}

/*
 * Точка входа: создание матрицы и работа с пользователем в цикле меню
 *
 * @return возвращает код завершения программы.
 */
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    runTests();

    int const size = readSize();
    int** p_matrix = allocateMatrix(size);
    int command = -1;

    fillMatrix(p_matrix, size);
    std::cout << "Матрица:" << std::endl;
    printMatrix(p_matrix, size);

    while (command != 0) {
        printMenu();
        std::cin >> command;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Некорректный ввод" << std::endl;
            command = -1;
        } else if (command != 0) {
            applyCommand(p_matrix, size, command);
        }
    }
    freeMatrix(p_matrix, size);
    std::cout << "Завершение работы" << std::endl;
    return 0;
}
