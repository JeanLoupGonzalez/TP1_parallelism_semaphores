/*-------------------------------------------------------------*/
/* AS/CB/UPS/IRIT  : modèle de code                            */
/*-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define N 2  // nombre de processus fils dans ce modèle de code

int NBM = 2;
int NBL = 2;

/* Opération P */
int P(int numEns, int numSem, int nbTickets) {
    // TODO
}

/* Opération V */
int V(int numEns, int numSem, int nbTickets) {
    // TODO
}

/*Programme principal : Lancer les processus */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s x y\n", argv[0]);
        exit(1);
    }
    NBL = atoi(argv[1]);
    NBM = atoi(argv[2]);

    // Cette union n'est pas definie par defaut dans ipc.h
    union typeArgument {
        int valeurSem;
        struct semid_ds *descriptionEns;
        ushort *valeursSems;
    } arg;

    union typeArgument u;
    //met un jeton dans le semaphore
    u.valeurSem=1;


    struct sembuf P = { 0, -1, SEM_UNDO};
    struct sembuf V = { 0, +1, SEM_UNDO};
    /* Creation ensemble de semaphores */
    // TODO
    //transforme nom de fichier (n'importe lequel) en clé
    //retourne une clé UNIQUE
    /*key_t KEY= ftok(1,home/jl/CLionProjects/progParaTP1);
    int semget(KEY,1,IPC_CREAT|0777);*/

    //quand les processus sont parents, on peut utiliser cette cte
    //IPC crée utilisable QUE par la descendance du processus qui a cree l'IPC
    int semid = semget(IPC_PRIVATE,1,IPC_CREAT|0777);

    /* Initialisation des semaphores */
    //1er semaphore demarre a 0
    semctl(semid,0,SETALL,arg);

    //creation d'un nouveau processus
    for (int i = 0; i < N; i++) {
        switch (fork()) {
            case -1: /* Code exécuté par le père en cas de problème*/
                printf("JE SUIS LE PERE mais j'ai un probleme\n");
                exit(1);
            case 0 : /* Code exécuté par le fils */
                for(int k=0;k<NBM;k++){
                    srand(getpid());
                    if(semop(semid, &P, 1) < 0){
                        perror("semop p"); exit(13);
                    }
                    printf("je suis fils. L%d %d\n", k,getpid());
                    usleep(rand()%100);
                    if(semop(semid, &V, 1) < 0)
                    {
                        perror("semop p"); exit(14);
                    }
              }
              exit(0);
            default: /* Code exécuté par le père */
                break;
        }
    }
    //destruction semaphores
    semctl(semid,0,IPC_RMID,arg);

    /* Attente de la fin de N processus fils */
    for (int j = 0; j < N; j++) {
        wait(NULL);
    }
    printf("JE SUIS LE PERE %d\n", getpid());
    return 0;
}

