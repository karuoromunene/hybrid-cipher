#include <stdio.h>
#include <string.h>
#include "xxtea.c"
#include "uECC.c"
#include "spongent.c"


char ecc_encrypt(uchar p_hash, uint8_t public_key[64]) {
    // initialize ECC curve
    const struct uECC_Curve_t *curve = uECC_secp256r1();
    uint8_t temp_priv_key[32]; // temporary key must be same size as curve
    uint8_t temp_pub_key[64]; // temporary key must be double the curve in size
    uint8_t secret[64]; // to be used as symmetrical key by xxtea
    uECC_shared_secret(temp_pub_key, temp_priv_key, secret, curve);
    size_t len;
    unsigned char *encrypted_p_hash = xxtea_encrypt(p_hash, strlen(p_hash), secret, &len);
    printf("success");

    return (encrypted_p_hash, temp_pub_key);
}

char ecc_decrypt(temp_pub_key, encrypted_p_hash, priv_key) {
    int shared_secret = temp_pub_key * priv_key;
    size_t len;
    char *decrypted_p_hash = xxtea_decrypt(encrypted_p_hash, len, shared_secret, &len);
    return decrypted_p_hash;
}

int main() {
    //enter your password p_word allocated 128 bits maximum
    uchar p_word[128];
    printf("Enter password: ");
    scanf("%c", &p_word);

    //feed input to spongent
    uchar p_hash[n / 8];
    spongent(p_word, p_hash);

    //check returned hash
    printf("Password Hash: ");
    for (unsigned i = 0; i < n / 8; ++i)
        printf("%02X", p_hash[i]);
    printf("\n");

    // initialize ECC curve
    const struct uECC_Curve_t *curve = uECC_secp256r1();
    uint8_t private_key[32]; // must be same size as curve
    uint8_t public_key[64]; // must be double the curve in size

    uECC_make_key(public_key, private_key, curve); // an inbuilt tiny-ecc method
    //display keys
    printf("User's public key: ");
    printf("%02X", public_key);
    printf("\n");
    printf("User's private key: ");
    printf("%02X", private_key);
    printf("\n");

    //encrypt the hash
    char result = ecc_encrypt(p_hash, public_key);

    //decrypt the encrypted hash
    //ecc_decrypt(temp_pub_key, encrypted_p_hash, priv_key);

    //free(encrypted_p_hash);
    //free(decrypted_p_hash);
    return 0;
}



