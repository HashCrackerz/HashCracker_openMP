#!/bin/bash

#SBATCH --account=
#SBATCH --partition=
#SBATCH --time=00:10:00
#SBATCH --nodes=1               
#SBATCH --ntasks-per-node=1     
#SBATCH --cpus-per-task=48      
#SBATCH --mem=10GB
#SBATCH --job-name=omp_ext_salt
#SBATCH --output=risultati_estensione.out
#SBATCH --error=error_log_estensione.err
#SBATCH --mail-user=

# 1. Caricamento moduli necessari
module load gcc
module load openssl

# 2. Definizione Variabili e Percorsi
# Assicurati che i file .cpp siano nella cartella corrente e utils.c in UTILS/
SOURCE_FILES="kernel_omp_estensione.cpp ESTENSIONE/omp_estensione.cpp UTILS/utils.cpp"
EXEC_NAME="estensione_omp"

PASSWORD="erty"
MIN_LEN="1"
MAX_LEN="6"
CHARSET="ASSETS/CharSet.txt"
SALT="qw"
USE_DICT="si"
DICT_FILE="ASSETS/rockyou_trimmed.txt"

# 3. Compilazione
echo "--- Compilazione in corso ---"
g++ -I. -fopenmp -O3 $SOURCE_FILES -o $EXEC_NAME -lssl -lcrypto

# Controllo compilazione
if [ ! -f ./$EXEC_NAME ]; then
    echo "Errore Critico: Compilazione fallita! Controlla error_log_estensione.err"
    exit 1
fi

echo "Compilazione riuscita. Inizio Test..."
echo "Target Pass: $PASSWORD | Salt: $SALT | Dizionario: $USE_DICT"
echo "-----------------------------------------------------------"

# 4. Esecuzione (Scalability Test)
# Eseguiamo il codice con numero crescente di thread per vedere lo speedup
for THREADS in 48; do
    export OMP_NUM_THREADS=$THREADS
    echo "Running with OMP_NUM_THREADS=$THREADS ..."
    
    # Comando di lancio
    ./$EXEC_NAME "$PASSWORD" $MIN_LEN $MAX_LEN "$CHARSET" "$SALT" "$USE_DICT" "$DICT_FILE"
    
    echo "-----------------------------------------------------------"
done

echo "Tutti i test completati."