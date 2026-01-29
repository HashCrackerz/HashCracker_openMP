<p align="left">
  <img width="150" alt="logo" src="https://github.com/user-attachments/assets/73297594-3581-4afd-ad0b-39d4bc0e66bf" />
</p>

# HashCracker (OpenMP)
## _Parallel SHA-256 Brute Force & Dictionary (salted) Password Cracker_

[🇬🇧 English](README.md) | [🇮🇹 Italiano](README-IT.md)

![alt text](https://img.shields.io/badge/Language-C++/OpenMP-green)

![alt text](https://img.shields.io/badge/Algorithm-SHA256-purple)

Estensione del progetto per il corso di Sistemi di Elaborazione Accelerata della facoltà di Ingegneria Informatica Magistrale di UniBo.
Applicazione **parallela** per il cracking di password tramite attacco Brute Force su hash SHA-256 (anche salted) e attacco a dizionario sfruttando il parallelismo messo a disposizione da sistemi HPC e la libreria OpenMP. 

## 📝 Descrizione
Questo repository contiene l'implementazione OpenMP di un password cracker per invertire hash SHA-256. Supporta attacchi Brute Force e attacchi a dizionario, incluso supporto per salt. L'obiettivo è mostrare come riprogettare la strategia di parallelismo passando da milioni di thread leggeri (CUDA) a un numero limitato di thread CPU più potenti (OpenMP), preservando correttezza e ottenendo buoni speedup su multi-core.

## ⚙️ Funzionalità
- **Brute Force**: suddivisione del search-space per primo carattere (strategie di scheduling dinamico).
- **Attacco a Dizionario**: caricamento efficiente in memoria e processing parallelo delle parole.
- **Supporto Salted Hashes**: gestione salt come prefisso/suffisso nelle verifiche.
- **Early Exit: flag volatile condivisa per terminazione anticipata quando la password è trovata**.
- **Script di test / SLURM**: template per eseguire test di scalabilità su cluster (G100).

## 📂 Struttura del Progetto
- `kernel_omp.cpp` — kernel principale OpenMP (brute force).
- `kernel_omp_estensione.cpp` — versione con salt & dizionario.
- `ESTENSIONE/` — codice specifico per attacchi ibridi (dizionario + salt).
- `UTILS/` — funzioni di utilità (I/O, parsing argomenti, charset loader).
- `ASSETS/` — charset, wordlists (es. CharSet.txt, rockyou_trimmed.txt).

## 🛠️ Requisiti
- **Hardware**: CPU multi-core 
- **Software**:
  - g++ con supporto OpenMP (GCC / Clang).
  - OpenSSL (per <openssl/sha.h>).
 
## 🚀 Compilazione

Assicurarsi di avere le librerie OpenSSL linkate correttamente.
```shell
g++ -fopenmp -O3 kernel_omp.cpp sequenziale.cpp UTILS/utils.cpp -o bruteforce_omp -lssl -lcrypto
```
_(cambiare i nomi dei file e delle dipendenze in base alla versione da compilare)_

## 💻 Utilizzo
Se il sistema che si utilizza non è gestito da un job scheduler è possibile eseguire direttamente il file compilato: 
```shell
./bruteforce_estensione_omp <hash_target> <min_len> <max_len> <file_charset> [<use_dictionary(si/no)> <file_dizionario>] 
```
_(esempio con la versione estesa con salt e dizionario)_

Altrimenti è necessario schedulare l'esecuzione tramite uno scheduler (come ad esempio SLURM): 

```shell
sbatch ./launcher.sh
```

## 👥 Autori
- [Andrea Vitale](https://github.com/WHYUBM)
- [Matteo Fontolan](https://github.com/itsjustwhitee)
















