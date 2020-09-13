### Cipher Notes 

The hybrid cipher was written using python programming language. Before the cipher starts, several libraries are imported to be used locally. The first one is tinyec and it gives the local environment the necessary bindings to execute ECC code in the python language. The second one is xxtea and it provides the local environment the necessary python bindings and pre-written XXTEA methods and classes. Another key library, native to python is hashlib which will be used severally to execute hash functions. 

 The cipher commences by generating a shared keys from a shared point on the elliptic curve. The keys will be used for both encryption and decryption. To generate a curve, the paper uses the predefined curve, brainpoolP256r1, from the tinyec library.  After generating the curve, the keys can be derived from hashing the chosen point on the curve. The xxtea library that was imported to the local environment has inbuilt functions for encryption and decryption. Since XXTEA requires a 16 bit key, we use SPONGENT to hash the chosen point on the curve and truncate it to 16 bits to get our public and private keys. The keys are later used to encrypt and decrypt the message from the user using the XXTEA + ECC hybrid cipher. 

The hybrid cipher has two major processes. In the encryption process, the plain text is first encrypted using a symmetric key from XXTEA to give an encrypted message. At the same time, using ECC, the symmetric key is encrypted using the sender’s public key to give an encrypted symmetric key. Encryption ends when the encrypted message is merged with the encrypted symmetric key. During decryption, we start with a merge of the encrypted message and encrypted symmetric key. The encrypted symmetric key is decrypted with the receiver’s private key using ECC. Once the symmetric key is back to its original form, it can then be used to decrypt the message into plain text using XXTEA. 

### Cipher Tests 

Several tests were run on the algorithm to assess its efficiency. The first among them was the execution time. To perform this test, the inbuilt python library timeit was used. The cipher execution time was an average of 0.1435 seconds per cycle of encryption/decryption. 
###### TODO:
* Side-Channel Attack