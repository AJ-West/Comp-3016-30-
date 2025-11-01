#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Beat dungeon/gameManager.h"

// Helper class to test base64 functions without SDL dependencies
class Base64TestHelper {
public:
    static std::string base64_encode(const std::string& input) {
        static const char table[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string output;
        int val = 0, valb = -6;
        for (unsigned char c : input) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                output.push_back(table[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) output.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
        while (output.size() % 4) output.push_back('=');
        return output;
    }

    static std::string base64_decode(const std::string& input) {
        static const std::string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::vector<int> decoding_table(256, -1);
        for (int i = 0; i < 64; ++i) decoding_table[table[i]] = i;

        std::string output;
        int val = 0, valb = -8;
        for (unsigned char c : input) {
            if (decoding_table[c] == -1) break;
            val = (val << 6) + decoding_table[c];
            valb += 6;
            if (valb >= 0) {
                output.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return output;
    }
};

TEST(Base64Test, EncodeSimpleString) {
    std::string input = "Hello World";
    std::string encoded = Base64TestHelper::base64_encode(input);
    std::string decoded = Base64TestHelper::base64_decode(encoded);
    EXPECT_EQ(input, decoded);
}

TEST(Base64Test, EncodeEmptyString) {
    std::string input = "";
    std::string encoded = Base64TestHelper::base64_encode(input);
    std::string decoded = Base64TestHelper::base64_decode(encoded);
    EXPECT_EQ(input, decoded);
}

TEST(Base64Test, EncodeNumber) {
    std::string input = "1";
    std::string encoded = Base64TestHelper::base64_encode(input);
    std::string decoded = Base64TestHelper::base64_decode(encoded);
    EXPECT_EQ(input, decoded);
}

TEST(Base64Test, EncodeMultiByte) {
    std::string input = "Test123";
    std::string encoded = Base64TestHelper::base64_encode(input);
    std::string decoded = Base64TestHelper::base64_decode(encoded);
    EXPECT_EQ(input, decoded);
}

TEST(Base64Test, EncodeSpecialCharacters) {
    std::string input = "P\n0 3 1 1 1 2 1 1 1 1 4 0";
    std::string encoded = Base64TestHelper::base64_encode(input);
    std::string decoded = Base64TestHelper::base64_decode(encoded);
    EXPECT_EQ(input, decoded);
}

TEST(Base64Test, RoundTripKnownValue) {
    // Test with a known base64 value from the project
    std::string known_encoded = "OQ==";
    std::string decoded = Base64TestHelper::base64_decode(known_encoded);
    std::string re_encoded = Base64TestHelper::base64_encode(decoded);
    EXPECT_EQ(known_encoded, re_encoded);
}

