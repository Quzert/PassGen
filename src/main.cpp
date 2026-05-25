#include "SodiumGenerator.hpp"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Использование: " << argv[0] << " <мастер_пароль> <название_сайта> [длина]" << endl;
        return 1;
    }

    string master = argv[1];
    string site = argv[2];
    size_t length = (argc >= 4) ? stoul(argv[3]) : 16;

    try {
        unique_ptr<PasswordGenerator> generator = make_unique<SodiumGenerator>();
        string password = generator->generate(master, site, length);

            cout << "Сгенерированный пароль для " << site << ":" << endl;
        cout << password << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
