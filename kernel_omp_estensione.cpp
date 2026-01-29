#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <omp.h>
#include "ESTENSIONE/omp_estensione.h"
#include "UTILS/utils.h"   
#include "UTILS/costanti.h"

int main(int argc, char** argv)
{
    /* Invocazione: 
       ./estensione_omp <password> <min_len> <max_len> <file_charset> <salt> <dizionario Y/N> [file_dizionario]
    */

    if (argc != 7 && argc != 8) {
        printf("Usage: %s <password> <min_len> <max_len> <file_charset> <salt> <dizionario Y/N> [file_dizionario]\n", argv[0]);
        return 1;
    }

    char* secret_password = argv[1];
    int min_test_len, max_test_len;
    
    if (!safe_atoi(argv[2], &min_test_len) || !safe_atoi(argv[3], &max_test_len)) {
        perror("Errore conversione numeri (min/max len)"); 
        exit(1);
    }

    char* charSet = leggiCharSet(argv[4]);
    if(!charSet) { 
        fprintf(stderr, "Errore lettura CharSet dal file: %s\n", argv[4]); 
        exit(1); 
    }

    char* salt = argv[5];
    bool use_dict = (argv[6][0] == 'Y' || argv[6][0] == 'y' || argv[6][0] == 'S' || argv[6][0] == 's');
    
    char* dict_path = (argc == 8) ? argv[7] : (char*)"ASSETS/rockyou.txt";

    char* salted_string = salt_password(secret_password, strlen(secret_password), salt, strlen(salt));
    
    if (salted_string == NULL) {
        fprintf(stderr, "Errore critico allocazione salt password\n");
        exit(1);
    }

    unsigned char target_hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)salted_string, strlen(salted_string), target_hash);

    printf("========================================\n");
    printf("%s Avvio estensione OpenMP ...\n", argv[0]);
    printf("Target Password: %s\n", secret_password);
    printf("Salt applicato : %s\n", salt);
    printf("Stringa target : %s (Hash calcolato)\n", salted_string);
    printf("Dizionario     : %s (%s)\n", use_dict ? "SI" : "NO", dict_path);
    printf("========================================\n\n");

    free(salted_string);

    testEstensioneOpenMP(target_hash, min_test_len, max_test_len, charSet, salt, use_dict, dict_path);

    free(charSet);
    return 0;
}