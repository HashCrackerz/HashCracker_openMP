#ifndef OMP_ESTENSIONE_H
#define OMP_ESTENSIONE_H

#include <openssl/sha.h> 
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Funzione principale che orchestra dizionario e brute force
    void testEstensioneOpenMP(
        unsigned char* target_hash, 
        int min_test_len, 
        int max_test_len, 
        char* charSet, 
        char* salt_str, 
        bool use_dict, 
        char* dict_path
    );

    // Funzione specifica per l'attacco a dizionario
    int attackDictionaryOMP(
        char* flat_dict, 
        int num_words, 
        int salt_len, 
        char* charSet, 
        unsigned char* target_hash, 
        char* result_buffer
    );

    // Funzione specifica per il brute force (adattata per cercare la stringa intera)
    void bruteForceSaltOMP(
        int len, 
        unsigned char* target_hash, 
        char* charSet, 
        char* result
    );

#ifdef __cplusplus
}
#endif

#endif