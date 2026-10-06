#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

const int DEFAULT_CAPACITY = 4;
const int GROW_FACTOR = 2;
const double MIN_TEMPERATURE = -40.0;
const double MAX_TEMPERATURE = 60.0;
const double MIN_PRESSURE = 720.0;
const double MAX_PRESSURE = 790.0;
const int MAX_MOVEMENTS = 10;
const int RANDOM_PRECISION = 10000;

/**
 * Тип сенсора.
 */
enum class SensorType { TEMPERATURE, PRESSURE, MOTION };

/**
 * Данные датчика температуры.
 */
struct TemperatureSensor {
    double celsius = 0.0;
};

/**
 * Данные датчика давления.
 */
struct PressureSensor {
    double mm_hg = 0.0;
};

/**
 * Данные датчика движения.
 */
struct MotionSensor {
    bool detected = false;
    int movements = 0;
};

/**
 * Возвращает случайное вещественное число в диапазоне [min, max].
 */
double randomDouble(double min, double max){
    double r = static_cast<double>(std::rand() % (RANDOM_PRECISION + 1)) / RANDOM_PRECISION;
    return min + r * (max - min);
}

/**
 * Возвращает название типа сенсора.
 */
std::string typeName(SensorType type){
    std::string name = "";
    switch (type) {
        case SensorType::TEMPERATURE:
            name = "TEMPERATURE";
            break;
        case SensorType::PRESSURE:
            name = "PRESSURE";
            break;
        case SensorType::MOTION:
            name = "MOTION";
            break;
    }
    return name;
}

/**
 * Сенсор, хранящий данные конкретного типа через void*.
 * Эмуляция полиморфизма: поведение выбирается по полю _type.
 */
class Sensor {
private:
    void* _p_sensor_data;
    SensorType _type;
    int _timestamp;

    static int _clock;

    /**
     * Выделяет память под данные нужного типа (значения по умолчанию).
     */
    static void* _allocate(SensorType type){
        void* p_data = nullptr;
        switch (type) {
            case SensorType::TEMPERATURE:
                p_data = new TemperatureSensor();
                break;
            case SensorType::PRESSURE:
                p_data = new PressureSensor();
                break;
            case SensorType::MOTION:
                p_data = new MotionSensor();
                break;
        }
        return p_data;
    }

    /**
     * Создаёт глубокую копию данных.
     */
    static void* _clone(const void* p_data, SensorType type){
        void* p_copy = nullptr;
        if (p_data == nullptr) {
            return p_copy;
        }
        switch (type) {
            case SensorType::TEMPERATURE:
                p_copy = new TemperatureSensor(*static_cast<const TemperatureSensor*>(p_data));
                break;
            case SensorType::PRESSURE:
                p_copy = new PressureSensor(*static_cast<const PressureSensor*>(p_data));
                break;
            case SensorType::MOTION:
                p_copy = new MotionSensor(*static_cast<const MotionSensor*>(p_data));
                break;
        }
        return p_copy;
    }

    /**
     * Освобождает память с приведением к правильному типу.
     */
    void _release(){
        if (_p_sensor_data == nullptr) {
            return;
        }
        switch (_type) {
            case SensorType::TEMPERATURE:
                delete static_cast<TemperatureSensor*>(_p_sensor_data);
                break;
            case SensorType::PRESSURE:
                delete static_cast<PressureSensor*>(_p_sensor_data);
                break;
            case SensorType::MOTION:
                delete static_cast<MotionSensor*>(_p_sensor_data);
                break;
        }
        _p_sensor_data = nullptr;
    }

public:
    /**
     * Конструктор по умолчанию: датчик температуры, вне сети (t = 0).
     */
    Sensor() : _p_sensor_data(nullptr), _type(SensorType::TEMPERATURE), _timestamp(0){
        _p_sensor_data = _allocate(_type);
    }

    /**
     * Конструктор с параметром типа.
     */
    explicit Sensor(SensorType type) : _p_sensor_data(nullptr), _type(type), _timestamp(++_clock){
        _p_sensor_data = _allocate(_type);
    }

    /**
     * Конструктор копирования (глубокая копия).
     */
    Sensor(const Sensor& other)
        : _p_sensor_data(_clone(other._p_sensor_data, other._type)),
          _type(other._type),
          _timestamp(other._timestamp){
    }

    /**
     * Оператор копирующего присваивания.
     */
    Sensor& operator=(const Sensor& other){
        if (this != &other) {
            void* p_copy = _clone(other._p_sensor_data, other._type);
            _release();
            _p_sensor_data = p_copy;
            _type = other._type;
            _timestamp = other._timestamp;
        }
        return *this;
    }

    /**
     * Конструктор перемещения.
     */
    Sensor(Sensor&& other) noexcept
        : _p_sensor_data(other._p_sensor_data),
          _type(other._type),
          _timestamp(other._timestamp){
        other._p_sensor_data = nullptr;
    }

    /**
     * Оператор перемещающего присваивания.
     */
    Sensor& operator=(Sensor&& other) noexcept {
        if (this != &other) {
            _release();
            _p_sensor_data = other._p_sensor_data;
            _type = other._type;
            _timestamp = other._timestamp;
            other._p_sensor_data = nullptr;
        }
        return *this;
    }

    /**
     * Деструктор.
     */
    ~Sensor(){
        _release();
    }

    SensorType getType() const {
        return _type;
    }

    int getTimestamp() const {
        return _timestamp;
    }

    /**
     * Опрос: генерация случайных данных.
     */
    void poll(){
        if (_p_sensor_data == nullptr) {
            _p_sensor_data = _allocate(_type);
        }
        switch (_type) {
            case SensorType::TEMPERATURE: {
                TemperatureSensor* p_t = static_cast<TemperatureSensor*>(_p_sensor_data);
                p_t->celsius = randomDouble(MIN_TEMPERATURE, MAX_TEMPERATURE);
                break;
            }
            case SensorType::PRESSURE: {
                PressureSensor* p_p = static_cast<PressureSensor*>(_p_sensor_data);
                p_p->mm_hg = randomDouble(MIN_PRESSURE, MAX_PRESSURE);
                break;
            }
            case SensorType::MOTION: {
                MotionSensor* p_m = static_cast<MotionSensor*>(_p_sensor_data);
                p_m->movements = std::rand() % (MAX_MOVEMENTS + 1);
                p_m->detected = p_m->movements > 0;
                break;
            }
        }
    }

    /**
     * Калибровка: сброс значений.
     */
    void calibrate(){
        _release();
        _p_sensor_data = _allocate(_type);
    }

    /**
     * Текстовое представление текущего значения.
     */
    std::string valueString() const {
        std::string text = "нет данных";
        if (_p_sensor_data == nullptr) {
            return text;
        }
        switch (_type) {
            case SensorType::TEMPERATURE: {
                const TemperatureSensor* p_t = static_cast<const TemperatureSensor*>(_p_sensor_data);
                text = std::to_string(p_t->celsius) + " C";
                break;
            }
            case SensorType::PRESSURE: {
                const PressureSensor* p_p = static_cast<const PressureSensor*>(_p_sensor_data);
                text = std::to_string(p_p->mm_hg) + " мм рт. ст.";
                break;
            }
            case SensorType::MOTION: {
                const MotionSensor* p_m = static_cast<const MotionSensor*>(_p_sensor_data);
                text = std::string(p_m->detected ? "движение" : "покой") +
                    ", событий: " + std::to_string(p_m->movements);
                break;
            }
        }
        return text;
    }

    void print() const {
        std::cout << "[t=" << std::setw(3) << _timestamp << "] "
                  << std::setw(11) << typeName(_type) << " : " << valueString() << std::endl;
    }
};

int Sensor::_clock = 0;

/**
 * Класс-менеджер: сеть сенсоров (динамический массив).
 */
class SensorNetwork {
private:
    Sensor* _p_sensors;
    int _size;
    int _capacity;
    std::string _name;

    void _grow(){
        int new_capacity = _capacity == 0 ? DEFAULT_CAPACITY : _capacity * GROW_FACTOR;
        Sensor* p_new = new Sensor[new_capacity];
        for (int i = 0; i < _size; i++) {
            p_new[i] = std::move(_p_sensors[i]);
        }
        delete[] _p_sensors;
        _p_sensors = p_new;
        _capacity = new_capacity;
    }

public:
    /**
     * Конструктор по умолчанию.
     */
    SensorNetwork() : _p_sensors(nullptr), _size(0), _capacity(0), _name("Безымянная сеть"){
    }

    /**
     * Конструктор с параметрами: имя и начальная ёмкость.
     */
    SensorNetwork(const std::string& name, int capacity)
        : _p_sensors(nullptr), _size(0), _capacity(capacity > 0 ? capacity : 0), _name(name){
        if (_capacity > 0) {
            _p_sensors = new Sensor[_capacity];
        }
    }

    /**
     * Конструктор копирования.
     */
    SensorNetwork(const SensorNetwork& other)
        : _p_sensors(nullptr), _size(other._size), _capacity(other._capacity), _name(other._name){
        if (_capacity > 0) {
            _p_sensors = new Sensor[_capacity];
            for (int i = 0; i < _size; i++) {
                _p_sensors[i] = other._p_sensors[i];
            }
        }
    }

    /**
     * Оператор копирующего присваивания (copy-and-swap).
     */
    SensorNetwork& operator=(const SensorNetwork& other){
        if (this != &other) {
            SensorNetwork copy(other);
            std::swap(_p_sensors, copy._p_sensors);
            std::swap(_size, copy._size);
            std::swap(_capacity, copy._capacity);
            std::swap(_name, copy._name);
        }
        return *this;
    }

    /**
     * Конструктор перемещения.
     */
    SensorNetwork(SensorNetwork&& other) noexcept
        : _p_sensors(other._p_sensors),
          _size(other._size),
          _capacity(other._capacity),
          _name(std::move(other._name)){
        other._p_sensors = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    /**
     * Оператор перемещающего присваивания.
     */
    SensorNetwork& operator=(SensorNetwork&& other) noexcept {
        if (this != &other) {
            delete[] _p_sensors;
            _p_sensors = other._p_sensors;
            _size = other._size;
            _capacity = other._capacity;
            _name = std::move(other._name);
            other._p_sensors = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    /**
     * Деструктор.
     */
    ~SensorNetwork(){
        delete[] _p_sensors;
    }

    int getSize() const {
        return _size;
    }

    void addSensor(SensorType type){
        if (_size == _capacity) {
            _grow();
        }
        _p_sensors[_size] = Sensor(type);
        _size++;
    }

    /**
     * Опрос всех сенсоров.
     */
    void pollAll(){
        for (int i = 0; i < _size; i++) {
            _p_sensors[i].poll();
        }
    }

    /**
     * Фильтрация: новая сеть, содержащая копии сенсоров заданного типа.
     */
    SensorNetwork filterByType(SensorType type) const {
        SensorNetwork result(_name + " / " + typeName(type), _size);
        for (int i = 0; i < _size; i++) {
            if (_p_sensors[i].getType() == type) {
                result._p_sensors[result._size] = _p_sensors[i];
                result._size++;
            }
        }
        return result;
    }

    /**
     * Калибровка всех сенсоров (сброс значений).
     */
    void calibrateAll(){
        for (int i = 0; i < _size; i++) {
            _p_sensors[i].calibrate();
        }
    }

    /**
     * Журнал последних значений (консольный вывод).
     */
    void writeLog() const {
        std::cout << "=== Журнал сети \"" << _name << "\" (сенсоров: " << _size << ") ===" << std::endl;
        for (int i = 0; i < _size; i++) {
            _p_sensors[i].print();
        }
    }

    /**
     * Поиск сенсора по времени создания. Возвращает nullptr, если не найден.
     */
    const Sensor* findByTimestamp(int timestamp) const {
        const Sensor* p_found = nullptr;
        for (int i = 0; i < _size && p_found == nullptr; i++) {
            if (_p_sensors[i].getTimestamp() == timestamp) {
                p_found = &_p_sensors[i];
            }
        }
        return p_found;
    }
};

int main(){
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    SensorNetwork network("Цех 1", 2);
    network.addSensor(SensorType::TEMPERATURE);
    network.addSensor(SensorType::PRESSURE);
    network.addSensor(SensorType::MOTION);
    network.addSensor(SensorType::TEMPERATURE);
    network.addSensor(SensorType::MOTION);

    std::cout << "--- Опрос сенсоров ---" << std::endl;
    network.pollAll();
    network.writeLog();

    std::cout << std::endl << "--- Фильтрация по типу TEMPERATURE ---" << std::endl;
    SensorNetwork temperature_network = network.filterByType(SensorType::TEMPERATURE);
    temperature_network.writeLog();

    std::cout << std::endl << "--- Копирование и калибровка копии ---" << std::endl;
    SensorNetwork copy_network(network);
    copy_network.calibrateAll();
    copy_network.writeLog();
    std::cout << "Оригинал не изменился:" << std::endl;
    network.writeLog();

    std::cout << std::endl << "--- Копирующее и перемещающее присваивание ---" << std::endl;
    SensorNetwork assigned_network;
    assigned_network = network;
    SensorNetwork moved_network;
    moved_network = std::move(assigned_network);
    SensorNetwork constructed_network(std::move(moved_network));
    constructed_network.writeLog();
    std::cout << "Размер сети после перемещения из неё: " << moved_network.getSize() << std::endl;

    std::cout << std::endl << "--- Поиск по времени создания ---" << std::endl;
    int search_time = 0;
    std::cout << "Введите время создания (timestamp): ";
    if (!(std::cin >> search_time)) {
        search_time = 0;
    }
    const Sensor* p_sensor = network.findByTimestamp(search_time);
    if (p_sensor != nullptr) {
        p_sensor->print();
    } else {
        std::cout << "Сенсор с t=" << search_time << " не найден" << std::endl;
    }
    return 0;
}
