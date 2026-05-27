#include "PasswordGenerator.hpp"
#include <sodium.h>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

class SodiumGenerator : public PasswordGenerator{
public:
    string generate(const string& masterPass, 
                    const string& site, 
                    size_t len,
                    const string& alphabet,
                    bool requireSpecial) const override{
        if (len == 0) {
            throw runtime_error("Длина пароля должна быть больше нуля");
        }
        if (alphabet.size() < 2) {
            throw runtime_error("Алфавит должен содержать как минимум 2 символа");
        }

        static const bool sodiumReady = []() {
            return sodium_init() >= 0;
        }();
        if (!sodiumReady) {
            throw runtime_error("Не удалось инициализировать libsodium");
        }

        if (requireSpecial) {
            auto isSpecialChar = [](char c){ return !isalnum(static_cast<unsigned char>(c)); };
            bool hasSpecial = any_of(alphabet.begin(), alphabet.end(), isSpecialChar);
            if (!hasSpecial) {
                throw runtime_error("Требуется спецсимвол, но алфавит не содержит спецсимволов");
            }
        }

        // Используем site как соль для Argon2
        unsigned char salt[crypto_pwhash_SALTBYTES];
        if (crypto_generichash(salt, sizeof(salt), 
                        reinterpret_cast<const unsigned char*>(site.data()), 
                        site.size(), nullptr, 0) != 0) {
            throw runtime_error("Не удалось вычислить соль");
        }
        
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
        return toAlphabet(hash, alphabet, len, requireSpecial);
    }    
private:
    uint64_t bytesToNumber(const vector<unsigned char>& data) const {
    uint64_t value = 0;
    for (unsigned char byte : data) {
        value = (value << 8) | byte;
    }
    return value;
}

    string toAlphabet(const vector<unsigned char>& data, const string& alphabet, size_t len, bool requireSpecial) const {
        if (alphabet.empty()) {
            throw runtime_error("Алфавит не может быть пустым");
        }
        if (len == 0) {
            throw runtime_error("Длина пароля должна быть больше нуля");
        }

        string result;
        result.reserve(len);

        for (size_t i = 0; i < len; ++i) {
            unsigned char b = data[i % data.size()];
            result += alphabet[b % alphabet.size()];
        }

        // Проверить наличие спецсимвола
        if (!requireSpecial) return result;

        auto isSpecialChar = [](char c){ return !isalnum(static_cast<unsigned char>(c)); };
        for (char c : result) if (isSpecialChar(c)) return result;

        // Список индексов спецсимволов в alphabet
        vector<size_t> specialIndexes;
        for (size_t i = 0; i < alphabet.size(); ++i) {
            if (isSpecialChar(alphabet[i])) specialIndexes.push_back(i);
        }
        if (specialIndexes.empty()) {
            throw runtime_error("Требуется спецсимвол, но алфавит не содержит спецсимволов");
        }

        size_t pos = static_cast<size_t>(data[0]) % len;
        size_t sidx = static_cast<size_t>(data[0]) % specialIndexes.size();
        result[pos] = alphabet[specialIndexes[sidx]];

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
    
    


