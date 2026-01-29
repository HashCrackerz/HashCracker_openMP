#!/bin/bash

#SBATCH --account=
#SBATCH --partition=
#SBATCH --time=00:10:00
#SBATCH --nodes=1               # OpenMP lavora su memoria condivisa (1 nodo)
#SBATCH --ntasks-per-node=1     # Un solo processo principale
#SBATCH --cpus-per-task=48      # Riserviamo 48 core fisici per i thread OpenMP
#SBATCH --mem=10GB
#SBATCH --job-name=omp_bruteforce
#SBATCH --output=risultati_bruteforce.out
#SBATCH --error=error_log_bruteforce.err
#SBATCH --mail-user=

# Caricamento moduli
module load gcc
module load openssl  # Necessario per <openssl/sha.h>

# --- COMPILAZIONE ---
echo "Compilazione in corso..."
g++ -fopenmp -O3 kernel_omp.cpp sequenziale.cpp UTILS/utils.cpp -o bruteforce_omp -lssl -lcrypto

# Controllo se la compilazione è andata a buon fine
if [ ! -f ./bruteforce_omp ]; then
    echo "Errore: Compilazione fallita!"
    exit 1
fi

# --- DEFINIZIONE ARGOMENTI ---
PASSWORD="qwerty"
MIN_LEN=1
MAX_LEN=6
CHARSET="ASSETS/CharSet.txt"  

# --- CICLO DI TEST SUI THREAD ---
# Testiamo diverse configurazioni di thread per vedere lo speedup
for P in 1 2 4 8 16 24 32 48; do
    export OMP_NUM_THREADS=$P
    
    echo "Running with OMP_NUM_THREADS=$P ..."
    ./bruteforce_omp "$PASSWORD" $MIN_LEN $MAX_LEN "$CHARSET"
    
    echo "------------------------------------------------"
done

echo "Test Completati."