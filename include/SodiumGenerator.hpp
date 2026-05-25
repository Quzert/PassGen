#include "PasswordGenerator.hpp"
#include <sodium.h>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

class SodiumGenerator : public PasswordGenerator{
public:
    string generate(const string& masterPass, 
                    const string& site, 
                    size_t len = 16,
                    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:,.<>?/"                
                ) const override{
        
        // Используем site как соль для Argon2
        unsigned char salt[crypto_pwhash_SALTBYTES];
        
        crypto_generichash(salt, sizeof(salt), 
                        reinterpret_cast<const unsigned char*>(site.data()), 
                        site.size(), nullptr, 0);
        
        size_t hashSize = static_cast<size_t>(ceil((static_cast<double>(len) * log2(alphabet.size())) / 8.0));
        hashSize = max<size_t>(crypto_pwhash_BYTES_MIN, hashSize);

        vector<unsigned char> hash(hashSize);
        
        if(crypto_pwhash(hash.data(), hash.size(), 
                        masterPass.c_str(), masterPass.size(), salt,
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE,
                        crypto_pwhash_ALG_ARGON2ID13) != 0){
                throw runtime_error("Ошибка");
            } 
        return toAlphabet(hash, alphabet);
    }    
private:
    uint64_t bytesToNumber(const vector<unsigned char>& data) const {
    uint64_t value = 0;
    for (unsigned char byte : data) {
        value = (value << 8) | byte;
    }
    return value;
}

    string toAlphabet(const vector<unsigned char>& data, const string& alphabet) const {
        string result = ""; 
        uint64_t hash = bytesToNumber(data);
        while (hash != 0){
            result += alphabet[hash % alphabet.size()];
            hash /= alphabet.size();
        }
        if (result.empty() && !alphabet.empty()) {
            result += alphabet[0];
        }
        reverse(result.begin(), result.end());
        return result;
    }

    // Преобразует байты в строку подходящую для пароля
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
    
    


