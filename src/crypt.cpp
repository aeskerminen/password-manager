#include "crypt.h"

// Key data used for generating key and iv, salt is for added security.
int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
    // Number of bits in key
    int i;
    // Number of hashing rounds
    int nrounds = 5;

    // Buffers for key and iv
    unsigned char key[32], iv[32];

    // This function generates the key and iv for AES 256 CBC mode. Uses SHA1 for hashing.
    i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);

    // Error checking
    if (i != 32)
    {
        printf("Key size is %d bits, should be 256 bits.\n", i);
        return -1;
    }

    // Clear all info from cipher context and free any memory (basically reset the whole struct)
    EVP_CIPHER_CTX_init(e_ctx);
    // Sets the cipher up for encryption with the correct type (aes 256 cbc), key and iv
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Clear all info from cipher context and free any memory (basically reset the whole struct)
    EVP_CIPHER_CTX_init(d_ctx);
    // Sets the cipher up for decryption with the correct type (aes 256 cbc), key and iv
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    return 0;
}

unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
    // Length of AES 256 CBC ciphertext (lenght of plaintext in bytes + AES block size)
    int c_len = *len + AES_BLOCK_SIZE;

    int f_len = 0;

    // Allocate c_len amount of bytes for ciphertext buffer
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);

    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

    // Encrypt len bytes from plaintext, write to ciphertext buffer. Actual amount of bytes written is stored in c_len.
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

    // Encrypts the final data (data remaining in a partial block). It is written to the end of the ciphertext block.
    // Number of actual bytes written is stored in f_len.
    EVP_EncryptFinal(e, ciphertext + c_len, &f_len);

    // Update the length of the ciphertext to the actual length of the bytes written (can be <= to the original c_len).
    *len = c_len + f_len;

    // Return ciphertext buffer.
    return ciphertext;
}
