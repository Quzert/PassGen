#include "SodiumGenerator.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

// Макрос для пути к алфавитам, устанавливается при компиляции
#ifndef PASSGEN_ALPHABETS_DIR
#define PASSGEN_ALPHABETS_DIR "./alphabet"
#endif
namespace fs = std::filesystem;

static void printUsage(ostream& out, const char* argv0) {
    out << "Использование: " << argv0 << " <мастер_пароль> <название_сайта> [опции]" << endl;
    out << "Опции:" << endl;
    out << "  -l,     --length <длина>          Длина пароля (по умолчанию: " << PasswordGenerator::DEFAULT_LEN << ")" << endl;
    out << "  -s,     --require-special         Требовать спецсимвол" << endl;
    out << "  -A,     --preset <name>           Использовать preset из alphabet/<name>.txt" << endl;
    out << "  -f,     --file <path>             Загрузить алфавит из файла" << endl;
    out << "  -a,     --alphabet <string>       Использовать буквенный набор напрямую" << endl;
    out << "  --list, --list-alphabets          Показать доступные presets в папке alphabet/" << endl;
}

static string readAlphabetFile(const fs::path &file) {

    // Чтение файла
    if (!fs::exists(file)) throw runtime_error("Файл алфавита не найден: " + file.string());
    if (!fs::is_regular_file(file)) throw runtime_error("Неверный файл алфавита: " + file.string());
    ifstream in(file);
    if (!in) throw runtime_error("Не удалось открыть файл алфавита: " + file.string());
    
    string str;
    getline(in, str);
    // Удаление лишних пробелов слева и справа
    while (!str.empty() && isspace(static_cast<unsigned char>(str.back()))) str.pop_back();
    while (!str.empty() && isspace(static_cast<unsigned char>(str.front()))) str.erase(str.begin());
    
    return str;
}

static vector<string> listAlphabetPresets(const fs::path &dir) {
    vector<string> names;

    if (!fs::exists(dir) || !fs::is_directory(dir)) return names;

    for (auto &entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        auto file = entry.path();
        if (file.extension() == ".txt") names.push_back(file.stem().string());
    }
    sort(names.begin(), names.end());
    return names;
}

static string resolveAlphabetOption(const string &opt, const fs::path &presetsDir) {
    // Проверка является ли файлом
    fs::path file(opt);
    if (fs::exists(file) && fs::is_regular_file(file)) return readAlphabetFile(file);
    // Проверка является ли пресетом
    fs::path presetFile = presetsDir / (opt + ".txt");
    if (fs::exists(presetFile) && fs::is_regular_file(presetFile)) return readAlphabetFile(presetFile);
    // Удаление лишних пробелов слева и справа
    string str = opt;
    while (!str.empty() && isspace(static_cast<unsigned char>(str.back()))) str.pop_back();
    while (!str.empty() && isspace(static_cast<unsigned char>(str.front()))) str.erase(str.begin());

    return str;
}

int main(int argc, char* argv[]) {
    // Определение директории с пресетами
    fs::path presetsDir(PASSGEN_ALPHABETS_DIR);

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--list-alphabets" || string(argv[i]) == "--list") {
            auto list = listAlphabetPresets(presetsDir);
            cout << "Доступные пресеты:" << endl;
            for (auto &n : list) cout << "  " << n << endl;
            return 0;
        } else if (string(argv[i]) == "--help"){
            printUsage(cout, argv[0]);
            return 0;
        }
    }

    if (argc < 3) {
        printUsage(cerr, argv[0]);
        return 1;
    }

    string master = argv[1];
    string site = argv[2];

    size_t length = PasswordGenerator::DEFAULT_LEN;
    bool requireSpecial = false;
    string presetName;
    string filePathOption;
    string literalAlphabet;
    bool listAlphabets = false;

    for (int i = 3; i < argc; ++i) {
        string a = argv[i];
        if (a == "--require-special" || a == "-s") {
            requireSpecial = true;
        } else if (a == "-l" || a == "--length") {
            if (i + 1 < argc) length = stoul(argv[++i]);
        } else if (a == "-A" || a == "--preset") {
            if (i + 1 < argc) presetName = argv[++i];
        } else if (a == "-f" || a == "--file") {
            if (i + 1 < argc) filePathOption = argv[++i];
        } else if (a == "-a" || a == "--alphabet") {
            if (i + 1 < argc) literalAlphabet = argv[++i];
        } else if (a == "--list-alphabets") {
            listAlphabets = true;
        }
    }

    if (listAlphabets) {
        auto list = listAlphabetPresets(presetsDir);
        cout << "Доступные пресеты:" << endl;
        for (auto &n : list) cout << "  " << n << endl;
        return 0;
    }

    string alphabet;
    try {
        if (!filePathOption.empty()) {
            alphabet = resolveAlphabetOption(filePathOption, presetsDir);
        } else if (!presetName.empty()) {
            alphabet = resolveAlphabetOption(presetName, presetsDir);
        } else if (!literalAlphabet.empty()) {
            alphabet = resolveAlphabetOption(literalAlphabet, presetsDir);
        } else {
            alphabet = PasswordGenerator::DEFAULT_ALPHABET;
        }

        unique_ptr<PasswordGenerator> generator = make_unique<SodiumGenerator>();
        string password = generator->generate(master, site, length, alphabet, requireSpecial);

        cout << "Сгенерированный пароль для " << site << ":" << endl;
        cout << password << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
