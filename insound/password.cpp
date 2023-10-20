#include "password.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/CipherKey.h"
#include <insound/env.h>
#include <insound/log.h>

#include <memory>

using Poco::Crypto::Cipher;
using Poco::Crypto::CipherFactory;
using Poco::Crypto::CipherKey;

namespace Insound {

    static Cipher &getCipher()
    {
        static std::shared_ptr<Cipher> sCipher;
        if (!sCipher)
        {
            auto &factory = CipherFactory::defaultFactory();
            auto password = requireEnv("PASSWORD_SECRET");
            auto salt = requireEnv("PASSWORD_SALT");

            auto key = CipherKey("aes-256-cbc",
                Cipher::ByteVec{password.begin(), password.end()},
                Cipher::ByteVec{salt.begin(), salt.end()});
            sCipher = std::shared_ptr<Cipher>(factory.createCipher(key));
        }

        return *sCipher;
    }

    std::string hash(const std::string &text)
    {
        return getCipher().encryptString(text);
    }

    bool compare(const std::string &text, const std::string &encrypted)
    {
        return getCipher().decryptString(encrypted) == text;
    }
}
