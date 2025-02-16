#include <stdio.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "crypt.h"

char *read_db(EVP_CIPHER_CTX *de, const char *DATA_LOCATION)
{
    std::ifstream FILE(DATA_LOCATION, std::ios_base::binary);

    char *buffer;

    // Length of file
    FILE.seekg(0, std::ios::end);
    int length = FILE.tellg();
    FILE.seekg(0, std::ios::beg);

    // Allocate memory for plaintext
    buffer = (char *)malloc(length);

    FILE.read(buffer, length);

    char *pt = (char *)aes_decrypt(de, (unsigned char *)buffer, &length);

    FILE.close();

    return pt;
}

int main(int argc, char *argv[])
{
    const char *DATA_LOCATION = "data/db.txt";

    // Make pointers for both contexts
    EVP_CIPHER_CTX *en = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX *de = EVP_CIPHER_CTX_new();

    // Define salt, key_data, len and i
    unsigned int salt[] = {1234, 5678};
    int key_data_len, i;

    // Input for encryption
    char *input;

    // Password and its length

    printf("%s\n", "Please enter your private key.");

    std::string key_data;
    getline(std::cin, key_data);

    key_data_len = strlen((const char *)key_data.c_str());

    // Trying to initialize ciphers
    if (aes_init((unsigned char *)key_data.c_str(), key_data_len, (unsigned char *)&salt, en, de))
    {
        printf("%s\n", "Couldn't initialize AES...");
        return -1;
    }

    while (true)
    {
        printf("%s\n", "Select which action you would like to choose: ");
        printf("%s\n%s\n", "1. Read passwords", "2. Save password");

        std::string input;

        getline(std::cin, input);

        if (input != "1" && input != "2")
        {
            printf("%s\n", "Your input is wrong.");
            exit(1);
        }

        // Read all passwords from database
        if (input == "1")
        {
            printf("%s\n", "Reading all passwords from database...");

            const char *pt = read_db(de, DATA_LOCATION);

            printf("%s\n", pt);
        }

        // Save a password to the database
        if (input == "2")
        {
            std::string pass;

            printf("%s\n", "Enter the password you want to save.");
            getline(std::cin, pass);

            std::string user;

            printf("%s\n", "Enter the username for which you want to store the password.");
            getline(std::cin, user);

            std::string plaintext = user + " " + pass + "\n";

            if (std::filesystem::exists(DATA_LOCATION))
            {
                // Read current database entries
                const char *pt = read_db(de, DATA_LOCATION);
                int length = strlen(pt);

                // Create new buffer for old entries and new entry
                char *final_buffer = (char *)malloc(length + strlen(plaintext.c_str()));

                // Concat pt (old) and plaintext (new) to buffer
                memcpy(final_buffer, pt, length);
                memcpy(final_buffer + length, plaintext.c_str(), strlen(plaintext.c_str()));

                // Encrypt final_buffer
                int len = strlen(final_buffer);
                unsigned char *ciphertext = aes_encrypt(en, (unsigned char *)final_buffer, &len);

                // Open read stream from db file
                std::ofstream FILE_WRITE(DATA_LOCATION, std::ios_base::binary);
                
                // Write encrypted data to file and close
                FILE_WRITE.write((char *)ciphertext, len);
                FILE_WRITE.close();
            }
        }
    }

    // Free memory
    EVP_CIPHER_CTX_free(en);
    EVP_CIPHER_CTX_free(de);

    return 0;
}