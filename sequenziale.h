#ifndef SEQUENZIALE_H
#define SEQUENZIALE_H

#include <openssl/sha.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include <openssl/sha.h>
#include <omp.h> // Header fondamentale per OpenMP

#ifdef __cplusplus
extern "C" {
#endif

    void bruteForceSequenziale(int len, unsigned char target_hash[SHA256_DIGEST_LENGTH], char charSet[], char* result);
    void testSequenziale(unsigned char* target_hash, int min_test_len, int max_test_len, char charSet[]);
    double cpuSecond();

#ifdef __cplusplus
}
#endif

#endif