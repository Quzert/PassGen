#include <string>

using namespace std;

class PasswordGenerator{
public:
    virtual string generate(const string& masterPass, 
                            const string& site, 
                            size_t len = 16) const = 0;

};    