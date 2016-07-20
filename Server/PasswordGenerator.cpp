// Compile this file with the following command
// g++ PasswordGenerator.cpp SQLConnector.cpp -lcrypto -lsqlite3 -o passwordgen.o

// NOTE: OpenSSL 1.0.2h needs to be installed on the system! It is the LTS solution and will be supported until Dec 2019

#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
#include <openssl/bn.h>
#include "SQLConnector.h"

//
//// Terminology:
//
// message = plaintext
// message digest function = hash function
// message digest (MD) = digest = fingerprint = output of a hash function

//
//// Task:
//
// Create a program that will take an input password,
// generate a random salt, stretch the password for n iterations,
// save the salted password and salt in the sqlite db, and 
// time the process to see how long it took, sending an error if too quick (< 200ms)


// TODO: Impose restrinctions on account length
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 1000
#define SALT_BIT_SIZE 512
// Iterations should be calibrated so the whole process takes 200-1000 ms
// If it is any quicker, it defeats the point of salting and stretching!
// 2^20 iterations will effectively add 20 bits to the password
const long long ITERATIONS = 1 << 20;
// NOTE: The generated key depends on the number of iterations.
//
//// TESTING:
//
// Run this: 
//      ./passwordgen.o [account_name] [password] new
// This will generate a new account and create a key using a randomly-created salt and the password.
//      ./passwordgen.o [account_name] [password]




// TODO: Time code and print that to output
// TODO: Have option to quiet output
// Pass in an empty, initialized BIGNUM for salt, a string for password,
// and an empty array pointer of size EVP_MAX_MD_SIZE for the output
// TODO: What's the better way - passing and modifying a pointer to an array on the stack,
// or returning a pointer to an malloc'ed array on the heap that need to be freed?
// Returns the number of iterations it took
long long generate_key(const char *password, BIGNUM *salt, unsigned char *md_value){
    EVP_MD_CTX *md_context;
    const EVP_MD *md_function;
    unsigned int md_len, i;

    printf("\n\nSalting and stretching the password...\n");

    printf("Hash Function: SHA-512\n");
    md_function = EVP_sha512();

    printf("Password: \"%s\"\n", password);

    int salt_bytes = BN_num_bytes(salt);
    int salt_bits = BN_num_bits(salt);
    unsigned char salt_string_bin[salt_bytes];
    int salt_length = BN_bn2bin(salt, salt_string_bin);
    
    printf("Salt (hex):\n");
    for(int i = 0; i < salt_length; i++){
        printf("%02x", salt_string_bin[i]);
    }
    printf("\n");
    printf("Salt length : bytes : bits\n%d : %d : %d\n", salt_length, salt_bytes, salt_bits);
    // Run through the stretching algorithm
    
    // md_temp_value needs to be initialized to all zeros, since it's on the stack
    for(int i = 0; i < EVP_MAX_MD_SIZE; ++i){
        md_value[i] = 0;
    } 

    printf("Iterations: %d\n", ITERATIONS);
    for (long long i = 0; i < ITERATIONS; ++i) {
        //printf("%dth iteration: \n", i);
        md_context = EVP_MD_CTX_create();
        EVP_DigestInit(md_context, md_function);
        // Append the previous hash, pasword, and salt together in this order: 
        //x = hash512(x || password || salt);
        EVP_DigestUpdate(md_context, md_value, EVP_MAX_MD_SIZE);
        EVP_DigestUpdate(md_context, password, strlen(password));
        EVP_DigestUpdate(md_context, salt_string_bin, salt_bytes);
        // Execute the hash and store it in md_value
        EVP_DigestFinal_ex(md_context, md_value, &md_len);
        // clean up md_context
        EVP_MD_CTX_destroy(md_context);
        // NOTE: EVP_DigestFinal() should == EVP_DigestFinal_ex() + EVP_MD_CTX_destroy()
    }
    // End loop

    printf("Final salted and stretched password/key is: \n");
    for(int i = 0; i < md_len; i++){
        printf("%02x", md_value[i]);
    }
    printf("\n");

    return ITERATIONS;
}


main(int argc, char *argv[]) {
    // The account name will be used to look up the salt to work with
    if(!argv[1] || !argv[2]) {
        printf("Usage: ./passwordgen.o account_name password [new]\n");
        exit(1);
    }

    // get the account name and password
    char * account_name = argv[1];
    char * password = argv[2];
    // Check to make sure password is reasonable
    if(strlen(password) > MAX_PASSWORD_LENGTH){
        printf("Password is too large to process!\n");
        exit(0); 
    }
    else if(strlen(password) < MIN_PASSWORD_LENGTH){
        printf("Password is too small to process!\n" );
        exit(0); 
    }
    else {
        // Password is of a good length
    }

    // TODO: Impose restrictions on account length

    // Initialize salt and generated key
    BIGNUM *salt = BN_new();
    unsigned char generated_key[EVP_MAX_MD_SIZE];
    // NOTE: EVP_MAX_MD_SIZE is 64 (512 bits)

    if(argv[3]) {
        //
        //// Create a new account
        //
        
        // TODO: First check to make sure account name does not already exist
        // Exit if it does   

 
        // Since argument was not supplied, generate a random salt
        // Create the random number (openssl should auto-seed from /dev/urandom)
        BN_rand(salt, SALT_BIT_SIZE, -1, 0);
    
        int iterations = generate_key(password, salt, generated_key);
        
        SQLConnector *sql = new SQLConnector();
        printf("Creating new account and saving account_name, salt, key, and iterations\n");
        //sql->execute();
        delete sql;
    }
    else {
        //
        //// Authenticate - perform a key lookup and check
        //
       
        // TODO: Check to make sure account already exists
        // Exit if it doesn't
 
        // TODO: Use the passed account name to look up the salt

        // Have the second param be the salt (in hex), so that the key
        // can be manually regenerated if the salt is known
        // convert salt argument from hex to bn
        // TODO: insert the actual salt
        BN_hex2bn(&salt, argv[2]);     
        int iterations = generate_key(password, salt, generated_key);
        SQLConnector *sql = new SQLConnector();
        printf("Checking to see if generated key matches key in account\n");
        //sql->execute();
        delete sql;
    }
    
    // TODO: Save the salt and the generated key (salted password) in the sqlite db
    // Make sure when storing the salt and key, that any leading zeros or zero bits are preserved!
    // TODO: Save the number of iterations used to generate the key in the db 

    //
    //// Free up memory allocations
    //

    // Clear the allocated BIGNUM pointer
    BN_clear_free(salt);
    // TODO: Overwrite stack sensitive variables with with 0's,
    // since it doesn't get zeroed out once it's off the stack
    //for(int i = 0; i < EVP_MAX_MD_SIZE; ++i){
    //    generated_key[i] = 0;
    //} 
    //for(int i = strlen(); i > 0); --i){
    //    password[i] = 0;
    //} 

    // NOTE: clear_free variants are for sensitive info, opposed to just free.
    // The salts aren't sensitive, but the password and key are.
    // So just use clear_free for everything I can

    exit(0);
}


/*
// Pseudocode for salting and stretching a password
// See pg 304 of Cryptography Engineering (21.2.1 - Salting and Stretching)

x = 0
// The salt is simply a random number that is stored alongside the key. Use at least a 256bit salt.
// Each user needs a different salt, so an attacker would always have to recalculate the key per user,
// even if the attacker guesses the same password (e.g. "password") for each user.
salt = rand256()

for (i = 0; i < ITERATIONS; ++i) {
    // (note: || means append)
    x = hash512(x || password || salt)
}

key = x
// Store the salt and the key in the db. The salt can be public.
*/
