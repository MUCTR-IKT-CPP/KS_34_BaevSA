#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <utility>

const int WORK_DAY_HOURS = 8;
const int MONTH_WORK_DAYS = 22;
const double DAY_BUDGET = 1000.0;
const double PRICE_PER_KWH = 5.0;
const double WATTS_IN_KILOWATT = 1000.0;
const double TURN_ON_CHANCE = 0.6;
const double BREAKDOWN_CHANCE = 0.03;
const double TURN_OFF_CHANCE = 0.1;
const double MIN_LOAD_FACTOR = 0.8;
const double MAX_LOAD_FACTOR = 1.2;
const double REPAIR_COST_FACTOR = 0.1;
const int DEFAULT_CAPACITY = 4;
const int GROW_FACTOR = 2;
const int TYPE_COUNT = 3;
const double COMPUTER_BASE_POWER = 150.0;
const double COMPUTER_POWER_PER_GB = 5.0;
const double PRINTER_BASE_POWER = 50.0;
const double PRINTER_POWER_PER_PPM = 10.0;
const double SERVER_BASE_POWER = 200.0;
const double SERVER_POWER_PER_CORE = 25.0;
const double PERCENT = 100.0;

/**
 * Тип устройства.
 */
enum class DeviceType { COMPUTER, PRINTER, SERVER };

/**
 * Возвращает название типа устройства.
 */
std::string deviceTypeName(DeviceType type){
    std::string name = "";
    switch (type) {
        case DeviceType::COMPUTER:
            name = "Компьютер";
            break;
        case DeviceType::PRINTER:
            name = "Принтер";
            break;
        case DeviceType::SERVER:
            name = "Сервер";
            break;
    }
    return name;
}

/**
 * Возвращает общий генератор случайных чисел.
 */
std::mt19937& randomGenerator(){
    static std::mt19937 generator(std::random_device{}());
    return generator;
}

/**
 * Возвращает true с вероятностью probability (от 0 до 1).
 */
bool randomChance(double probability){
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    bool result = distribution(randomGenerator()) < probability;
    return result;
}

/**
 * Возвращает случайное вещественное число в диапазоне [min, max].
 */
double randomDouble(double min, double max){
    std::uniform_real_distribution<double> distribution(min, max);
    double result = distribution(randomGenerator());
    return result;
}

/**
 * Базовый класс «Устройство».
 */
class Device {
private:
    std::string _name;
    double _power;
    bool _is_on;
    double _cost;
    int _breakdown_count;
    bool _is_broken;
    int _day_hours;
    int _day_breakdowns;
    int _total_hours;

public:
    Device(const std::string& name, double power, double cost)
        : _name(name),
          _power(power),
          _is_on(false),
          _cost(cost),
          _breakdown_count(0),
          _is_broken(false),
          _day_hours(0),
          _day_breakdowns(0),
          _total_hours(0){
    }

    virtual ~Device(){
    }

    /**
     * Создаёт копию устройства нужного наследника.
     */
    virtual Device* clone() const = 0;

    virtual DeviceType getType() const = 0;

    /**
     * Описание специфического параметра наследника.
     */
    virtual std::string specificInfo() const = 0;

    /**
     * Один час работы. Возвращает потреблённую энергию в Вт*ч.
     */
    virtual double work(){
        double load_factor = randomDouble(MIN_LOAD_FACTOR, MAX_LOAD_FACTOR);
        double energy = _power * load_factor;
        _day_hours++;
        _total_hours++;
        return energy;
    }

    /**
     * Диагностика: текстовый отчёт о состоянии.
     */
    virtual std::string diagnose() const {
        std::string state = _is_broken ? "СЛОМАНО" : (_is_on ? "включено" : "выключено");
        std::string report = deviceTypeName(getType()) + " \"" + _name + "\": " + state +
            ", поломок всего: " + std::to_string(_breakdown_count);
        return report;
    }

    const std::string& getName() const {
        return _name;
    }

    double getPower() const {
        return _power;
    }

    bool isOn() const {
        return _is_on;
    }

    bool isBroken() const {
        return _is_broken;
    }

    double getCost() const {
        return _cost;
    }

    int getBreakdownCount() const {
        return _breakdown_count;
    }

    int getDayHours() const {
        return _day_hours;
    }

    int getDayBreakdowns() const {
        return _day_breakdowns;
    }

    int getTotalHours() const {
        return _total_hours;
    }

    void turnOn(){
        if (!_is_broken) {
            _is_on = true;
        }
    }

    void turnOff(){
        _is_on = false;
    }

    /**
     * Поломка: устройство выключается до конца дня.
     */
    void breakDown(){
        _is_broken = true;
        _is_on = false;
        _breakdown_count++;
        _day_breakdowns++;
    }

    /**
     * Стоимость ремонта поломок за текущий день.
     */
    double dayRepairCost() const {
        double repair_cost = _cost * REPAIR_COST_FACTOR * _day_breakdowns;
        return repair_cost;
    }

    /**
     * Ремонт в конце дня.
     */
    void repair(){
        _is_broken = false;
    }

    /**
     * Сброс дневной статистики перед новым днём.
     */
    void startDay(){
        _is_on = false;
        _day_hours = 0;
        _day_breakdowns = 0;
    }

    /**
     * Сброс всей статистики перед новой симуляцией месяца.
     */
    void resetStatistics(){
        startDay();
        _is_broken = false;
        _breakdown_count = 0;
        _total_hours = 0;
    }
};

/**
 * Компьютер: специфический параметр — объём ОЗУ.
 */
class Computer : public Device {
private:
    int _ram_gb;

public:
    Computer(const std::string& name, int ram_gb, double cost)
        : Device(name, COMPUTER_BASE_POWER + ram_gb * COMPUTER_POWER_PER_GB, cost), _ram_gb(ram_gb){
    }

    Device* clone() const override {
        return new Computer(*this);
    }

    DeviceType getType() const override {
        return DeviceType::COMPUTER;
    }

    std::string specificInfo() const override {
        return "ОЗУ: " + std::to_string(_ram_gb) + " ГБ";
    }

    double work() override {
        double energy = Device::work();
        return energy;
    }

    std::string diagnose() const override {
        std::string report = Device::diagnose() + ", проверка памяти " + std::to_string(_ram_gb) + " ГБ: OK";
        return report;
    }
};

/**
 * Принтер: специфический параметр — скорость печати.
 */
class Printer : public Device {
private:
    int _pages_per_minute;
    int _printed_pages;

public:
    Printer(const std::string& name, int pages_per_minute, double cost)
        : Device(name, PRINTER_BASE_POWER + pages_per_minute * PRINTER_POWER_PER_PPM, cost),
          _pages_per_minute(pages_per_minute),
          _printed_pages(0){
    }

    Device* clone() const override {
        return new Printer(*this);
    }

    DeviceType getType() const override {
        return DeviceType::PRINTER;
    }

    std::string specificInfo() const override {
        return "скорость: " + std::to_string(_pages_per_minute) + " стр/мин";
    }

    /**
     * Принтер печатает часть часа, поэтому считает напечатанные страницы.
     */
    double work() override {
        const int MINUTES_IN_HOUR = 60;
        const int PRINT_SHARE = 4;
        double energy = Device::work();
        _printed_pages += _pages_per_minute * MINUTES_IN_HOUR / PRINT_SHARE;
        return energy;
    }

    std::string diagnose() const override {
        std::string report = Device::diagnose() + ", напечатано страниц: " + std::to_string(_printed_pages);
        return report;
    }
};

/**
 * Сервер: специфический параметр — количество ядер.
 */
class Server : public Device {
private:
    int _core_count;

public:
    Server(const std::string& name, int core_count, double cost)
        : Device(name, SERVER_BASE_POWER + core_count * SERVER_POWER_PER_CORE, cost), _core_count(core_count){
    }

    Device* clone() const override {
        return new Server(*this);
    }

    DeviceType getType() const override {
        return DeviceType::SERVER;
    }

    std::string specificInfo() const override {
        return "ядер: " + std::to_string(_core_count);
    }

    double work() override {
        double energy = Device::work();
        return energy;
    }

    std::string diagnose() const override {
        std::string report = Device::diagnose() + ", нагрузка на " + std::to_string(_core_count) + " ядер в норме";
        return report;
    }
};

/**
 * Итоги одного рабочего дня.
 */
struct DayReport {
    double energy_kwh = 0.0;
    double energy_cost = 0.0;
    double repair_cost = 0.0;
};

/**
 * ИТ-инфраструктура: управляет динамическим массивом устройств.
 */
class ITInfrastructure {
private:
    Device** _p_devices;
    int _size;
    int _capacity;

    void _grow(){
        int new_capacity = _capacity == 0 ? DEFAULT_CAPACITY : _capacity * GROW_FACTOR;
        Device** p_new = new Device*[new_capacity];
        for (int i = 0; i < _size; i++) {
            p_new[i] = _p_devices[i];
        }
        delete[] _p_devices;
        _p_devices = p_new;
        _capacity = new_capacity;
    }

    void _clear(){
        for (int i = 0; i < _size; i++) {
            delete _p_devices[i];
        }
        delete[] _p_devices;
        _p_devices = nullptr;
        _size = 0;
        _capacity = 0;
    }

    /**
     * Один час работы всех устройств. Возвращает энергию в Вт*ч.
     */
    double _simulateHour(){
        double hour_energy = 0.0;
        for (int i = 0; i < _size; i++) {
            Device* p_device = _p_devices[i];
            if (p_device->isBroken()) {
                continue;
            }
            if (!p_device->isOn() && randomChance(TURN_ON_CHANCE)) {
                p_device->turnOn();
            }
            if (p_device->isOn()) {
                hour_energy += p_device->work();
                if (randomChance(BREAKDOWN_CHANCE)) {
                    p_device->breakDown();
                } else if (randomChance(TURN_OFF_CHANCE)) {
                    p_device->turnOff();
                }
            }
        }
        return hour_energy;
    }

    /**
     * Полный рабочий день без вывода. Ремонт выполняется в конце дня.
     */
    DayReport _simulateDay(){
        DayReport report;
        double day_energy = 0.0;
        for (int i = 0; i < _size; i++) {
            _p_devices[i]->startDay();
        }
        for (int hour = 0; hour < WORK_DAY_HOURS; hour++) {
            day_energy += _simulateHour();
        }
        report.energy_kwh = day_energy / WATTS_IN_KILOWATT;
        report.energy_cost = report.energy_kwh * PRICE_PER_KWH;
        for (int i = 0; i < _size; i++) {
            report.repair_cost += _p_devices[i]->dayRepairCost();
        }
        return report;
    }

    void _repairAll(){
        for (int i = 0; i < _size; i++) {
            _p_devices[i]->repair();
        }
    }

public:
    ITInfrastructure() : _p_devices(nullptr), _size(0), _capacity(0){
    }

    explicit ITInfrastructure(int capacity) : _p_devices(nullptr), _size(0), _capacity(capacity > 0 ? capacity : 0){
        if (_capacity > 0) {
            _p_devices = new Device*[_capacity];
        }
    }

    ITInfrastructure(const ITInfrastructure& other) : _p_devices(nullptr), _size(other._size), _capacity(other._capacity){
        if (_capacity > 0) {
            _p_devices = new Device*[_capacity];
            for (int i = 0; i < _size; i++) {
                _p_devices[i] = other._p_devices[i]->clone();
            }
        }
    }

    ITInfrastructure& operator=(const ITInfrastructure& other){
        if (this != &other) {
            ITInfrastructure copy(other);
            std::swap(_p_devices, copy._p_devices);
            std::swap(_size, copy._size);
            std::swap(_capacity, copy._capacity);
        }
        return *this;
    }

    ITInfrastructure(ITInfrastructure&& other) noexcept
        : _p_devices(other._p_devices), _size(other._size), _capacity(other._capacity){
        other._p_devices = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    ITInfrastructure& operator=(ITInfrastructure&& other) noexcept {
        if (this != &other) {
            _clear();
            _p_devices = other._p_devices;
            _size = other._size;
            _capacity = other._capacity;
            other._p_devices = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    ~ITInfrastructure(){
        _clear();
    }

    int getSize() const {
        return _size;
    }

    /**
     * Добавляет устройство. Инфраструктура становится его владельцем.
     */
    void addDevice(Device* p_device){
        if (p_device == nullptr) {
            return;
        }
        if (_size == _capacity) {
            _grow();
        }
        _p_devices[_size] = p_device;
        _size++;
    }

    /**
     * Удаляет устройство по имени. Возвращает true, если устройство найдено.
     */
    bool removeDevice(const std::string& name){
        int found_index = -1;
        for (int i = 0; i < _size && found_index == -1; i++) {
            if (_p_devices[i]->getName() == name) {
                found_index = i;
            }
        }
        if (found_index == -1) {
            return false;
        }
        delete _p_devices[found_index];
        for (int i = found_index; i < _size - 1; i++) {
            _p_devices[i] = _p_devices[i + 1];
        }
        _size--;
        return true;
    }

    void printDevices() const {
        std::cout << "Устройства (" << _size << "):" << std::endl;
        for (int i = 0; i < _size; i++) {
            const Device* p_device = _p_devices[i];
            std::cout << "  " << deviceTypeName(p_device->getType()) << " \"" << p_device->getName() << "\": "
                      << std::fixed << std::setprecision(1) << p_device->getPower() << " Вт, "
                      << std::setprecision(0) << p_device->getCost() << " руб, "
                      << p_device->specificInfo() << std::endl;
        }
    }

    /**
     * Статистика по типам устройств.
     */
    void printTypeStatistics() const {
        int counts[TYPE_COUNT] = {0, 0, 0};
        double powers[TYPE_COUNT] = {0.0, 0.0, 0.0};
        for (int i = 0; i < _size; i++) {
            int type_index = static_cast<int>(_p_devices[i]->getType());
            counts[type_index]++;
            powers[type_index] += _p_devices[i]->getPower();
        }
        std::cout << "Статистика по типам:" << std::endl;
        for (int t = 0; t < TYPE_COUNT; t++) {
            std::cout << "  " << deviceTypeName(static_cast<DeviceType>(t)) << ": " << counts[t] << " шт., суммарная мощность "
                      << std::fixed << std::setprecision(1) << powers[t] << " Вт" << std::endl;
        }
    }

    void printDiagnostics() const {
        std::cout << "Диагностика:" << std::endl;
        for (int i = 0; i < _size; i++) {
            std::cout << "  " << _p_devices[i]->diagnose() << std::endl;
        }
    }

    /**
     * Имитация одного рабочего дня с подробным отчётом.
     */
    void simulateWorkDay(){
        for (int i = 0; i < _size; i++) {
            _p_devices[i]->resetStatistics();
        }
        DayReport report = _simulateDay();
        double remaining_budget = DAY_BUDGET - report.energy_cost;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Потребление энергии: " << report.energy_kwh << " кВт*ч" << std::endl;
        std::cout << "Стоимость энергии: " << report.energy_cost << " руб (остаток бюджета "
                  << remaining_budget << " из " << DAY_BUDGET << " руб)" << std::endl;

        std::cout << "Отработано часов:" << std::endl;
        for (int i = 0; i < _size; i++) {
            std::cout << "  " << _p_devices[i]->getName() << ": " << _p_devices[i]->getDayHours() << " из " << WORK_DAY_HOURS << std::endl;
        }

        std::cout << "Сломанные устройства:" << std::endl;
        bool has_broken = false;
        for (int i = 0; i < _size; i++) {
            if (_p_devices[i]->getDayBreakdowns() > 0) {
                has_broken = true;
                std::cout << "  " << _p_devices[i]->getName() << " (ремонт "
                          << _p_devices[i]->dayRepairCost() << " руб)" << std::endl;
            }
        }
        if (!has_broken) {
            std::cout << "  нет" << std::endl;
        }
        std::cout << "Затраты на ремонт: " << report.repair_cost << " руб" << std::endl;

        printDiagnostics();
        _repairAll();
    }

    /**
     * Имитация месяца (22 рабочих дня) со статистикой и рекомендациями.
     */
    void simulateMonth(){
        double month_energy_cost = 0.0;
        double month_repair_cost = 0.0;
        double month_energy_kwh = 0.0;
        int type_breakdowns[TYPE_COUNT] = {0, 0, 0};

        for (int i = 0; i < _size; i++) {
            _p_devices[i]->resetStatistics();
        }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "День | Энергия, кВт*ч | Энергия, руб | Ремонт, руб" << std::endl;
        for (int day = 1; day <= MONTH_WORK_DAYS; day++) {
            DayReport report = _simulateDay();
            for (int i = 0; i < _size; i++) {
                int type_index = static_cast<int>(_p_devices[i]->getType());
                type_breakdowns[type_index] += _p_devices[i]->getDayBreakdowns();
            }
            _repairAll();
            month_energy_kwh += report.energy_kwh;
            month_energy_cost += report.energy_cost;
            month_repair_cost += report.repair_cost;
            std::cout << std::setw(4) << day << " | " << std::setw(14) << report.energy_kwh << " | "
                      << std::setw(12) << report.energy_cost << " | " << std::setw(11) << report.repair_cost
                      << std::endl;
        }

        std::cout << "Итого за месяц: " << month_energy_kwh << " кВт*ч, электроэнергия "
                  << month_energy_cost << " руб, ремонт " << month_repair_cost << " руб" << std::endl;
        std::cout << "Средние затраты на энергию в день: " << month_energy_cost / MONTH_WORK_DAYS << " руб"
                  << std::endl;

        std::cout << "Сбои по типам устройств:" << std::endl;
        for (int t = 0; t < TYPE_COUNT; t++) {
            std::cout << "  " << deviceTypeName(static_cast<DeviceType>(t)) << ": " << type_breakdowns[t] << std::endl;
        }

        const int MONTH_HOURS = MONTH_WORK_DAYS * WORK_DAY_HOURS;
        std::cout << "Процент рабочего времени:" << std::endl;
        for (int i = 0; i < _size; i++) {
            double work_percent = PERCENT * _p_devices[i]->getTotalHours() / MONTH_HOURS;
            std::cout << "  " << _p_devices[i]->getName() << ": " << std::setprecision(1) << work_percent << "% (" << _p_devices[i]->getTotalHours()
                      << " из " << MONTH_HOURS << " ч), поломок: " << _p_devices[i]->getBreakdownCount()
                      << std::endl;
        }

        printRecommendations();
    }

    /**
     * Рекомендации: заменить устройства с наибольшим числом поломок.
     */
    void printRecommendations() const {
        int max_breakdowns = 0;
        for (int i = 0; i < _size; i++) {
            if (_p_devices[i]->getBreakdownCount() > max_breakdowns) {
                max_breakdowns = _p_devices[i]->getBreakdownCount();
            }
        }
        std::cout << "Рекомендации по оптимизации:" << std::endl;
        if (max_breakdowns == 0) {
            std::cout << "  поломок не было, замена не требуется" << std::endl;
            return;
        }
        for (int i = 0; i < _size; i++) {
            const Device* p_device = _p_devices[i];
            if (p_device->getBreakdownCount() == max_breakdowns) {
                double repair_total = p_device->getCost() * REPAIR_COST_FACTOR * max_breakdowns;
                std::cout << "  заменить " << deviceTypeName(p_device->getType()) << " \"" << p_device->getName()
                          << "\": " << max_breakdowns << " поломок, ремонт обошёлся в " << std::setprecision(2)
                          << repair_total << " руб" << std::endl;
            }
        }
    }
};

int main(){
    ITInfrastructure office(2);
    office.addDevice(new Computer("PC-Бухгалтер", 8, 60000.0));
    office.addDevice(new Computer("PC-Дизайнер", 32, 150000.0));
    office.addDevice(new Computer("PC-Стажёр", 4, 35000.0));
    office.addDevice(new Printer("HP-Офис", 20, 25000.0));
    office.addDevice(new Printer("Canon-Цвет", 35, 45000.0));
    office.addDevice(new Server("Srv-Почта", 8, 250000.0));
    office.addDevice(new Server("Srv-База", 32, 600000.0));

    std::cout << "===== Инфраструктура =====" << std::endl;
    office.printDevices();
    office.printTypeStatistics();

    std::cout << std::endl << "===== Удаление устройства \"PC-Стажёр\" =====" << std::endl;
    bool is_removed = office.removeDevice("PC-Стажёр");
    std::cout << (is_removed ? "Удалено" : "Не найдено") << std::endl;
    office.printTypeStatistics();

    std::cout << std::endl << "===== Рабочий день =====" << std::endl;
    office.simulateWorkDay();

    std::cout << std::endl << "===== Месяц (" << MONTH_WORK_DAYS << " рабочих дня) =====" << std::endl;
    office.simulateMonth();
    return 0;
}
