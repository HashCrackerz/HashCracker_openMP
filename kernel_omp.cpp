#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "sequenziale.h" 
#include "UTILS/utils.h"

int main(int argc, char** argv)
{
    /* Invocazione: ./kernel_omp <password_in_chiaro> <min_len> <max_len> <file_charset> */

    char* charSet, * secret_password;
    int min_test_len, max_test_len;

    /* --- CONTROLLO ARGOMENTI DI INVOCAZIONE --- */
    if (argc != 5) {
        printf("Usage: %s <password_in_chiaro> <min_len> <max_len> <file_charset> \n", argv[0]);
        return 1;
    }
    secret_password = argv[1];

    if (!safe_atoi(argv[2], &min_test_len))
    {
        perror("Errore nella conversione di min_test_len");
        exit(1);
    }
    if (!safe_atoi(argv[3], &max_test_len))
    {
        perror("Errore nella conversione di max_test_len");
        exit(1);
    }

    // Caricamento del charset dal file
    charSet = leggiCharSet(argv[4]);
    if (charSet == NULL) {
        fprintf(stderr, "Errore nella lettura del CharSet.\n");
        exit(1);
    }
    int charSetLen = strlen(charSet);

    // Calcolo dell'hash target
    unsigned char target_hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)secret_password, strlen(secret_password), target_hash);

    printf("========================================\n");
    printf("%s Starting OpenMP Brute Force...\n", argv[0]);
    
    printf("Target Password: %s\n", secret_password);
    printf("Range lunghezza: %d - %d\n", min_test_len, max_test_len);
    printf("Charset len: %d\n", charSetLen);
    printf("========================================\n\n");

    /*-----------------------------------------------------------------------------------------------------------------------------------------*/
    /* TEST VERSIONE PARALLELA (OpenMP) */
    /*-----------------------------------------------------------------------------------------------------------------------------------------*/
    testSequenziale(target_hash, min_test_len, max_test_len, charSet);

    free(charSet);

    return 0;
}