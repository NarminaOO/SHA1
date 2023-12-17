#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bitset>


static std::string Sha1(const std::string& input)
{
    //Initial hash values 
    uint32_t H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;

    //Calculate the length of the input in bits
    uint64_t length = input.length() * 8;
    std::vector<uint8_t> message;

    //Convert input to bytes
    for (char c : input)
    {
        message.push_back(static_cast<uint8_t>(c));
    }

    // Add '1' bit and padding with zeros 
    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448)
    {
        message.push_back(0x00);
    }
    
    // Append original length in bits
    message.push_back(length >> 56);
    message.push_back(length >> 48);
    message.push_back(length >> 40);
    message.push_back(length >> 32);
    message.push_back(length >> 24);
    message.push_back(length >> 16);
    message.push_back(length >> 8);
    message.push_back(length);

    // Process the message in 512-bit blocks
    for (size_t blocks = 0; blocks < message.size(); blocks += 64)
    {
        std::vector<uint32_t> W(80);

        // Break block into 16 32-bit words
        for (size_t i = 0; i < 16; ++i)
        {
            W[i] = (message[blocks + 4 * i] << 24) |
                (message[blocks + 4 * i + 1] << 16) |
                (message[blocks + 4 * i + 2] << 8) |
                (message[blocks + 4 * i + 3]);
        }

        // Extend 16 words into 80 words
        for (size_t i = 16; i < 80; ++i)
        {
            W[i] = (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
            W[i] = (W[i] << 1) | (W[i] >> 31);
        }

        uint32_t A = H0;
        uint32_t B = H1;
        uint32_t C = H2;
        uint32_t D = H3;
        uint32_t E = H4;

        // Main loop for each 80-word block
        for (size_t i = 0; i < 80; ++i)
        {
            uint32_t Ft, Kt;

            // Define functions and constants based on iteration
            if (i < 20)
            {
                Ft = (B & C) | ((~B) & D);
                Kt = 0x5A827999;
            }
            else if (i < 40)
            {
                Ft = B ^ C ^ D;
                Kt = 0x6ED9EBA1;
            }
            else if (i < 60)
            {
                Ft = (B & C) | (B & D) | (C & D);
                Kt = 0x8F1BBCDC;
            }
            else
            {
                Ft = B ^ C ^ D;
                Kt = 0xCA62C1D6;
            }

            uint32_t temp = ((A << 5) | (A >> 27)) + Ft + E + Kt + W[i];
            E = D;
            D = C;
            C = (B << 30) | (B >> 2);
            B = A;
            A = temp;
        }

        // Add block's hash to result
        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += E;
    }

    // Concatenate 
    std::stringstream ss;
    ss << std::setfill('0') << std::hex << std::setw(8) << H0
        << std::setw(8) << H1 << std::setw(8) << H2
        << std::setw(8) << H3 << std::setw(8) << H4;
    return ss.str();
}


int main()
{
    std::string input;

    input = "hello world";
    std::cout << "for " << input << " SHA1->" << Sha1(input) << std::endl;

    
    std::cout << "Enter string to encode: ";
    std::getline(std::cin, input);
    std::cout << "SHA-1 : " << Sha1(input) << std::endl;

    return 0;
}
