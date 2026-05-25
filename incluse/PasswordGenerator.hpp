#include <string>

using namespace std;

// Абстрактный класс
class PasswordGenerator{
public:
    virtual string generate(const string& masterPass, 
                            const string& site, 
                            size_t len = 16, 
                            string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:,.<>?/") 
                            const = 0;

};    