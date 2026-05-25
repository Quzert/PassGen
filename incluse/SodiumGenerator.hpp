#include "PasswordGenerator.hpp"
#include <sodium.h>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class SodiumGenerator : public PasswordGenerator{
public:
    string generate(const string& masterPass, 
                    const string& site, 
                    size_t len = 16) const override{
        unsigned char salt[crypto_pwhash_SALTBYTES];

        crypto_generichash(salt, sizeof(salt), 
                        reinterpret_cast<const unsigned char*>(site.data()), 
                        site.size(), nullptr, 0);
        
        vector<unsigned char> hash(len);

        if(crypto_pwhash(hash.data(), hash.size(), 
                        masterPass.c_str(), masterPass.size(), salt,
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE,
                        crypto_pwhash_ALG_ARGON2ID13) != 0){
                throw runtime_error("Ошибка");
            } 
        return toBase64(hash);
    }    
private:
    string toBase64(const vector<unsigned char>& data) const {
        size_t b64_len = sodium_base64_ENCODED_LEN(data.size(), 
                                        sodium_base64_VARIANT_ORIGINAL_NO_PADDING);

        vector<char> b64(b64_len);
        
        sodium_bin2base64(b64.data(), b64.size(), 
                          data.data(), data.size(), 
                          sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
        
        return string(b64.data());
    }
};
    
    


