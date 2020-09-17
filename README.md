### Cipher Notes 

We assume we have already generated an ECC private and public key pair for use in decrypting/encrypting and that the user has set a password and saved it in the database.  In the encryption stage, we get the user's password  and hash it using SPONGENT and then encrypt that hash with the XXTEA symmetric key. We then encrypt that symmetric key with the ECC public key. The final ciphertext will then be the encrypted and hashed password + the encrypted symmetric key.To decrypt that cyphertext, we first decrypt the symmetric key using the ECC private key. Now we can use the decrypted symmetric key to further decrypt the encrypted and hashed password to get a hashed password. If that hash exists in our database, we know the password is correct.

##### Testing
* check generzted keylength in bits
* size of algorithm in bits
* speed of encryption and decryption

