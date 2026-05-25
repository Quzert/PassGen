#include <string>

using namespace std;

// Абстрактный класс
class PasswordGenerator{
public:
    static constexpr size_t DEFAULT_LEN = 16;
    inline static const std::string DEFAULT_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:,.<>?/";

    virtual string generate(const string& masterPass, 
                            const string& site, 
                            size_t len = DEFAULT_LEN, 
                            const string& alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:,.<>?/",
                            bool requireSpecial = false) 
                            const = 0;

};    