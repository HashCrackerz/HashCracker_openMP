#include "sequenziale.h"

// Variabile globale per fermare la ricerca tra i thread
volatile int stop_search = 0;

double cpuSecond() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

// Funzione helper per verificare l'hash (Thread Safe perché usa memoria locale allo stack)
int check_hash(const char* password, int pass_len, unsigned char* target_hash) {
    unsigned char current_hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password, pass_len, current_hash);

    if (memcmp(current_hash, target_hash, SHA256_DIGEST_LENGTH) == 0) {
        return 1; 
    }
    return 0;
}

void bruteForceSequenziale(int len, unsigned char target_hash[SHA256_DIGEST_LENGTH], char charSet[], char* result)
{
    int charSetLen = strlen(charSet);
    if (len <= 0) return;

    // Resetta flag di stop per ogni nuova lunghezza testata
    stop_search = 0;

    // Ogni thread si occupa di tutte le combinazioni che iniziano con charSet[i]
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < charSetLen; i++) {
        
        // Controllo se un altro thread ha già trovato la password
        if (stop_search) continue;

        
        // Allocazione buffer thread-local
        char* buf = (char*)malloc(sizeof(char) * (len + 1));
        int* indices = (int*)malloc(sizeof(int) * len);
        

        buf[len] = '\0';

        // Inizializzazione:
        // Il primo carattere è fissato dal ciclo for parallelo (i)
        indices[0] = i; 
        // Gli altri partono da 0
        for (int k = 1; k < len; k++) {
            indices[k] = 0;
        }

        while (!stop_search) {
            // Costruisco la stringa
            for (int k = 0; k < len; k++) {
                buf[k] = charSet[indices[k]];
            }

            // Verifico hash
            if (check_hash(buf, len, target_hash)) {
                #pragma omp critical //la scrittura del risultato (condiviso) è una sezione critica
                {
                    if (!stop_search) {
                        strcpy(result, buf);
                        stop_search = 1; //Notifico agli altri che la password è stata trovata 
                    }
                }
                break;
            }

            int k = len - 1;
            while (k > 0) { // Ci fermiamo se k == 0
                indices[k]++;
                if (indices[k] < charSetLen) {
                    break;
                } else {
                    indices[k] = 0;
                    k--;
                }
            }

            if (k == 0) {
                break;
            }
        }

        free(indices);
        free(buf);
    }
}

void testSequenziale(unsigned char* target_hash, int min_test_len, int max_test_len, char charSet[]) {
    printf("--- Inizio Test Brute Force OpenMP ---\n");
    // Stampa il numero di thread disponibili
    printf("Numero max threads OpenMP: %d\n", omp_get_max_threads());

    char found_password[100] = { 0 };

    double iStart, iElaps;
    iStart = cpuSecond();

    for (int len = min_test_len; len <= max_test_len; len++) {
        printf("Tentativo lunghezza %d... ", len);
        fflush(stdout);

        bruteForceSequenziale(len, target_hash, charSet, found_password);

        if (strlen(found_password) > 0) {
            printf("TROVATA!\n");
            printf("Password decifrata: %s\n", found_password);
            break;
        }
        else {
            printf("Nessuna corrispondenza.\n");
        }
    }

    iElaps = cpuSecond() - iStart;
    printf("Tempo Totale: %.4f secondi\n", iElaps);

    if (strlen(found_password) == 0) {
        printf("\nPassword non trovata nel range di lunghezza %d-%d.\n", min_test_len, max_test_len);
    }
}