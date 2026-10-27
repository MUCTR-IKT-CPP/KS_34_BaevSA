#include <iostream>
#include <string>
#include <vector>

const int ALPHABET_SIZE = 26;
const int SECRET_KEY = 7;
const int DOCUMENT_COUNT = 5;

/**
 * Интерфейс шифрования: только чисто виртуальные методы, без полей.
 */
class IEncryptable {
public:
    virtual ~IEncryptable(){
    }

    virtual void encrypt(int key) = 0;
    virtual void decrypt(int key) = 0;
    virtual bool isEncrypted() const = 0;
};

/**
 * Сдвигает латинские буквы строки на shift позиций (шифр Цезаря).
 */
std::string shiftLetters(const std::string& text, int shift){
    std::string result = text;
    int normalized_shift = (shift % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
    for (size_t i = 0; i < result.size(); i++) {
        char c = result[i];
        if (c >= 'a' && c <= 'z') {
            result[i] = static_cast<char>('a' + (c - 'a' + normalized_shift) % ALPHABET_SIZE);
        } else if (c >= 'A' && c <= 'Z') {
            result[i] = static_cast<char>('A' + (c - 'A' + normalized_shift) % ALPHABET_SIZE);
        }
    }
    return result;
}

/**
 * Абстрактный класс документа.
 */
class Document {
private:
    std::string _title;
    double _size_kb;
    std::string _author;
    bool _is_open;
    bool _is_confidential;
    std::string _content;

protected:
    const std::string& getContent() const {
        return _content;
    }

    void setContent(const std::string& content){
        _content = content;
    }

    /**
     * Общая часть метаданных для всех наследников.
     */
    void printBaseMetadata() const {
        std::cout << "  Название: " << _title << std::endl;
        std::cout << "  Размер: " << _size_kb << " КБ" << std::endl;
        std::cout << "  Автор: " << _author << std::endl;
        std::cout << "  Конфиденциальный: " << (_is_confidential ? "да" : "нет") << std::endl;
    }

    void markOpened(){
        _is_open = true;
    }

public:
    Document(
        const std::string& title,
        double size_kb,
        const std::string& author,
        bool is_confidential,
        const std::string& content
    ) : _title(title),
        _size_kb(size_kb),
        _author(author),
        _is_open(false),
        _is_confidential(is_confidential),
        _content(content){
    }

    virtual ~Document(){
    }

    virtual void open() = 0;
    virtual void save() = 0;
    virtual void getMetadata() const = 0;

    const std::string& getTitle() const {
        return _title;
    }

    bool isOpen() const {
        return _is_open;
    }

    bool isConfidential() const {
        return _is_confidential;
    }
};

/**
 * Текстовый документ. Поддерживает шифрование.
 */
class TextDocument : public Document, public IEncryptable {
private:
    int _word_count;
    bool _is_encrypted;

public:
    TextDocument(
        const std::string& title,
        double size_kb,
        const std::string& author,
        bool is_confidential,
        const std::string& text,
        int word_count
    ) : Document(title, size_kb, author, is_confidential, text), _word_count(word_count), _is_encrypted(false){
    }

    void open() override {
        markOpened();
        std::cout << "[Текст] Открыт \"" << getTitle() << "\": " << getContent() << std::endl;
    }

    void save() override {
        std::cout << "[Текст] Сохранён \"" << getTitle() << "\" (" << (_is_encrypted ? "зашифрован" : "открытый текст")
                  << "): " << getContent() << std::endl;
    }

    void getMetadata() const override {
        std::cout << "Текстовый документ" << std::endl;
        printBaseMetadata();
        std::cout << "  Количество слов: " << _word_count << std::endl;
    }

    void encrypt(int key) override {
        if (_is_encrypted) {
            return;
        }
        setContent(shiftLetters(getContent(), key));
        _is_encrypted = true;
    }

    void decrypt(int key) override {
        if (!_is_encrypted) {
            return;
        }
        setContent(shiftLetters(getContent(), -key));
        _is_encrypted = false;
    }

    bool isEncrypted() const override {
        return _is_encrypted;
    }
};

/**
 * Электронная таблица. Поддерживает шифрование.
 */
class Spreadsheet : public Document, public IEncryptable {
private:
    int _sheet_count;
    bool _is_encrypted;

public:
    Spreadsheet(
        const std::string& title,
        double size_kb,
        const std::string& author,
        bool is_confidential,
        const std::string& cells,
        int sheet_count
    ) : Document(title, size_kb, author, is_confidential, cells), _sheet_count(sheet_count), _is_encrypted(false){
    }

    void open() override {
        markOpened();
        std::cout << "[Таблица] Открыта \"" << getTitle() << "\", листов: " << _sheet_count
                  << ", ячейки: " << getContent() << std::endl;
    }

    void save() override {
        std::cout << "[Таблица] Сохранена \"" << getTitle() << "\" (" << (_is_encrypted ? "зашифрована" : "открыта")
                  << "): " << getContent() << std::endl;
    }

    void getMetadata() const override {
        std::cout << "Электронная таблица" << std::endl;
        printBaseMetadata();
        std::cout << "  Количество листов: " << _sheet_count << std::endl;
    }

    void encrypt(int key) override {
        if (_is_encrypted) {
            return;
        }
        setContent(shiftLetters(getContent(), key));
        _is_encrypted = true;
    }

    void decrypt(int key) override {
        if (!_is_encrypted) {
            return;
        }
        setContent(shiftLetters(getContent(), -key));
        _is_encrypted = false;
    }

    bool isEncrypted() const override {
        return _is_encrypted;
    }
};

/**
 * Изображение. Шифрование НЕ поддерживает.
 */
class ImageDocument : public Document {
private:
    std::string _resolution;

public:
    ImageDocument(
        const std::string& title,
        double size_kb,
        const std::string& author,
        bool is_confidential,
        const std::string& resolution
    ) : Document(title, size_kb, author, is_confidential, "<пиксели>"), _resolution(resolution){
    }

    void open() override {
        markOpened();
        std::cout << "[Изображение] Открыто \"" << getTitle() << "\", разрешение " << _resolution << std::endl;
    }

    void save() override {
        std::cout << "[Изображение] Сохранено \"" << getTitle() << "\" (" << _resolution << ")" << std::endl;
    }

    void getMetadata() const override {
        std::cout << "Изображение" << std::endl;
        printBaseMetadata();
        std::cout << "  Разрешение: " << _resolution << std::endl;
    }
};

/**
 * Пакетная обработка документов. Агрегация: документы не принадлежат
 * обработчику и не удаляются им.
 */
class DocumentProcessor {
private:
    std::vector<Document*> _documents;

public:
    void addDocument(Document* p_doc){
        if (p_doc != nullptr) {
            _documents.push_back(p_doc);
        }
    }

    /**
     * Сохраняет все документы очереди.
     */
    void processAll(){
        for (size_t i = 0; i < _documents.size(); i++) {
            _documents[i]->save();
        }
    }

    /**
     * Шифрует конфиденциальные документы, которые это поддерживают.
     * Возвращает количество зашифрованных документов.
     */
    int encryptConfidential(int key){
        int encrypted_count = 0;
        for (size_t i = 0; i < _documents.size(); i++) {
            Document* p_doc = _documents[i];
            if (!p_doc->isConfidential()) {
                continue;
            }
            IEncryptable* p_encryptable = dynamic_cast<IEncryptable*>(p_doc);
            if (p_encryptable != nullptr) {
                p_encryptable->encrypt(key);
                encrypted_count++;
                std::cout << "  Зашифрован: \"" << p_doc->getTitle() << "\"" << std::endl;
            } else {
                std::cout << "  ВНИМАНИЕ: \"" << p_doc->getTitle()
                          << "\" конфиденциальный, но шифрование не поддерживается" << std::endl;
            }
        }
        return encrypted_count;
    }

    /**
     * Расшифровывает все зашифрованные документы.
     */
    void decryptAll(int key){
        for (size_t i = 0; i < _documents.size(); i++) {
            IEncryptable* p_encryptable = dynamic_cast<IEncryptable*>(_documents[i]);
            if (p_encryptable != nullptr && p_encryptable->isEncrypted()) {
                p_encryptable->decrypt(key);
                std::cout << "  Расшифрован: \"" << _documents[i]->getTitle() << "\"" << std::endl;
            }
        }
    }

    /**
     * Симуляция: шифрование конфиденциальных файлов и пакетное сохранение.
     */
    void simulate(int key){
        std::cout << "Шаг 1. Шифрование конфиденциальных документов:" << std::endl;
        int encrypted_count = encryptConfidential(key);
        std::cout << "Зашифровано документов: " << encrypted_count << " из " << _documents.size() << std::endl;
        std::cout << std::endl << "Шаг 2. Пакетное сохранение:" << std::endl;
        processAll();
    }
};

int main(){
    TextDocument contract("Contract", 48.5, "Ivanov", true, "Salary of director is secret", 5);
    TextDocument manual("Manual", 120.0, "Petrova", false, "Press the green button to start", 6);
    Spreadsheet budget("Budget", 256.0, "Sidorov", true, "Revenue=900; Costs=400; Profit=500", 3);
    ImageDocument logo("Logo", 1024.0, "Smirnova", false, "1920x1080");
    ImageDocument passport_scan("PassportScan", 2048.0, "Ivanov", true, "3000x2000");

    std::cout << "===== Базовый вариант: полиморфизм =====" << std::endl;
    Document* p_documents[DOCUMENT_COUNT] = {&contract, &manual, &budget, &logo, &passport_scan};
    for (int i = 0; i < DOCUMENT_COUNT; i++) {
        p_documents[i]->open();
        p_documents[i]->getMetadata();
        std::cout << std::endl;
    }

    std::cout << "===== Усложнённый вариант: пакетная обработка с шифрованием =====" << std::endl;
    DocumentProcessor processor;
    for (int i = 0; i < DOCUMENT_COUNT; i++) {
        processor.addDocument(p_documents[i]);
    }
    processor.simulate(SECRET_KEY);

    std::cout << std::endl << "Шаг 3. Расшифровка для авторизованного пользователя:" << std::endl;
    processor.decryptAll(SECRET_KEY);
    std::cout << std::endl << "Шаг 4. Повторное сохранение:" << std::endl;
    processor.processAll();
    return 0;
}
