#include <stdio.h>
#include <string.h>
#include "xxtea.c"
#include "uECC.c"
#include "spongent.c"

int main() {

    // password allocated 128 bits
    BitSequence password[128];
    printf("Enter Password:");
    gets(password);
    printf("Original Password :%s\n", password);

    // hash the password
    int i;
    char temporary_hash[16];
    char password_hash[32];
    BitSequence hash_value[hashsize/8]={0};
    DataLength databitlen = 128;
    SpongentHash(password, databitlen, hash_value);

    printf("Password Spongent Hash :");
    for(i=0; i<hashsize/8; i++){
        sprintf(temporary_hash,"%02X", hash_value[i]);
        strcat(password_hash,temporary_hash) ;
    }
    printf("%s",password_hash);
    printf("\n");

    // ECC Key Generation and Key Exchange ECDH - User's wont share keys but will generate the same key from their ends
    const struct uECC_Curve_t *curve = uECC_secp256r1(); //initialize curve
    // User 1's Keys
    uint8_t priv_key1[32]; // temporary key must be same size as curve
    uint8_t pub_key1[64]; // temporary key must be double the curve in size
    uint8_t shared_secret1[32]; // temporary key must be double the curve in size
    uECC_make_key(pub_key1, priv_key1, curve); // generate public/private key pair

    //User 2's Keys
    uint8_t priv_key2[32];
    uint8_t pub_key2[64];
    uint8_t shared_secret2[32];
    uECC_make_key(pub_key2, priv_key2, curve);




    // Encryption Phase
    // User 1's secret key
    int secret1 = uECC_shared_secret(pub_key2, priv_key1, shared_secret1, curve); // generate a secret key to be used by xxtea for encryption

    // xxtea encrypt hash
    size_t len;
    size_t pw_length = strlen(password_hash);
    unsigned char *encrypted_hash = xxtea_encrypt(password_hash, pw_length, shared_secret1, &len);
    printf("Encrypted Password Hash: ");
    printf("%s",*(&encrypted_hash));
    printf("\n");


    // Decryption Phase
    // User 2's secret key
    int secret2 = uECC_shared_secret(pub_key1, priv_key2, shared_secret2, curve); // generate a secret key to be used by xxtea for decryption

    //xxtea decrypt hash
    char *decrypted_hash = xxtea_decrypt(encrypted_hash, len, shared_secret2, &len);
    printf("Decrypted Password Hash: ");
    printf( "%s",decrypted_hash);
    printf("\n");

    // check if the two password hashes match
    if (strcmp(password_hash, decrypted_hash) == 0){
        printf("%s", "The Two Hashes Match!");
    } else{
        printf("%s", "Hashes Compromised!");
    }
    return 0;
}



