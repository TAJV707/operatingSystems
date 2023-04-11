// *test machine: apollo/vole
// * group number: G[43]
// * name: Gabe Campos , Tyler Johnson-Vasquez, Zachary Guldberg
// * x500: campo193 , joh20191, Guldb016*/


#include "server.h"

#define SA struct sockaddr

Total_t* NewBal;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid;
int BalSize = 0;

char* logFileName = "output/balances.csv";

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./server server_addr server_port num_workers\n");
}


void iterate(void *arg){

    
    int sockfd = *((int *) arg);
    free(arg);

while(1){


    char buffer[MAX_STR];
    memset(buffer, 0, MAX_STR);
    if(read(sockfd, buffer, MAX_STR) < 0){
        printf("Message could not be accepted.\n");
    }

    int mesType = atoi(buffer);
    time_t birthday;
    char name[MAX_STR];
    memset(name, 0, MAX_STR);
    char userName[MAX_STR];
    memset(userName, 0, MAX_STR);

    struct account accounts[MAX_ACC];
    float newBalance = 0;
    int accountNumber;
    float amount;
    float Returned = CASH_AMOUNT;
    float Requested;

    int accountinfo = ACCOUNT_INFO;
    int difbal = BALANCE;

    switch(mesType){

        // register
        case 0:

            if(read(sockfd, userName, (sizeof(char)*MAX_STR) != (sizeof(char)*MAX_STR))){
                printf("Could not read username.(REGISTER)\n");
                exit(1);
            }
            if(read(sockfd, name, (sizeof(char)*MAX_STR) != (sizeof(char)*MAX_STR))){
                printf("Could not read name.(REGISTER)\n");
                exit(1);
            }
            if(read(sockfd, &birthday, (sizeof(time_t)) != (sizeof(time_t)))){
                printf("Could not read birthday.(REGISTER)\n");
                exit(1);
            }

            if(write(sockfd, &newBalance, sizeof(float)) < 0){
                printf("Could not write balance.(REGISTER)\n");
                exit(1);
            }

            pthread_mutex_lock(&lock);

            strcpy(NewBal[BalSize].userName, userName);
            strcpy(NewBal[BalSize].name, name);
            NewBal[BalSize].birthday = birthday;
            NewBal[BalSize].accountNumber= BalSize;
            NewBal[BalSize].balance = 0.0;

            

            if(write(sockfd, &difbal, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write BALANCE.(REGISTER)\n");
                exit(1);
            }

            if(write(sockfd, &BalSize, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write account number.(REGISTER)\n");
                exit(1);
            }

            if(write(sockfd, &(NewBal[BalSize].balance), (sizeof(float)) != (sizeof(float)))){
                printf("Could not write balance.(REGISTER)\n");
                exit(1);
            }

            BalSize++;
            pthread_mutex_unlock(&lock);

            break;

        // get account info
        case 1:

            break;

        // transact
        case 2:
            if(read(sockfd, &accountNumber, (sizeof(int)) != (sizeof(int)))){
                printf("Could not read account number. (TRANSACT)\n");
                exit(1);
            }
            if(read(sockfd, &amount, (sizeof(float)) != (sizeof(float)))){
                printf("Could not read amount. (TRANSACT)\n");
                exit(1);
            }

            pthread_mutex_lock(&lock);

            if(NewBal[accountNumber].balance < (-amount)){
                printf("Need more money for transaction.\n");
                pthread_mutex_unlock(&lock);
                return;
            }

            NewBal[accountNumber].balance += amount;

            if(write(sockfd, &mesType, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write message type. (TRANSACT)\n");
                exit(1);
            }
            if(write(sockfd, &accountNumber, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write accountnumber.\n");
                exit(1);
            }
            if(write(sockfd, &(NewBal[accountNumber].balance), (sizeof(float)) != (sizeof(float)))){
                printf("Could not write balance.\n");
                exit(1);
            }
            pthread_mutex_unlock(&lock);
            
            break;

        // get balance
        case 3:
            break;
        // account info
        case 4:
            if(read(sockfd, &accountNumber, (sizeof(int)) != (sizeof(int)))){
                printf("Could not read account number.(ACCOUNT_INFO)\n");
                exit(1);
            }

            pthread_mutex_lock(&lock);

            if(write(sockfd, &accountinfo, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write ACCOUNT INFO.(ACCOUNT_INFO)\n");
                exit(1);
            }
            if(write(sockfd, NewBal[accountNumber].userName, (sizeof(char)*MAX_STR) != (sizeof(char)*MAX_STR))){
                printf("Could not write userName.(ACCOUNT_INFO)\n.");
                exit(1);
            }
            if(write(sockfd, NewBal[accountNumber].name, (sizeof(char)*MAX_STR) != (sizeof(char)*MAX_STR))){
                printf("Could not write name. (ACCOUNT_INFO)\n.");
                exit(1);
            }
            if(write(sockfd, NewBal[accountNumber].birthday, (sizeof(time_t)) != (sizeof(time_t)))){
                printf("Could not write username.(ACCOUNT_INFO)\n.");
                exit(1);
            }
            pthread_mutex_unlock(&lock);

            break;
        
        // balance
        case 5:
            if(read(sockfd, &accountNumber, (sizeof(int)) != (sizeof(int)))){
                printf("Could not read account number. (BALANCE)\n");
                exit(1);
            }

            pthread_mutex_lock(&lock);

            if(write(sockfd, &mesType, (sizeof(int)) != (sizeof(int)))){
                printf("Could not read message type. (BALANCE)\n");
                exit(1);
            }
            if(write(sockfd, &accountNumber, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write account number. (BALANCE)\n");
                exit(1);
            }
            if(write(sockfd, &(NewBal[accountNumber].balance), (sizeof(float)) != (sizeof(float)))){
                printf("Could not write balance. (BALANCE).\n");
                exit(1);
            }
            
            pthread_mutex_unlock(&lock);
            
            break;
        // request cash
        case 6:
            break;
        // cash
        case 7:
            if(read(sockfd, &Requested, (sizeof(float)) != (sizeof(float)))){
                printf("Could not read request. (CASH)\n");
                exit(1);
            }
            if(write(sockfd, &mesType, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write message type. (CASH)\n");
                exit(1);
            }
            if(write(sockfd, &Returned, (sizeof(float)) != (sizeof(float)))){
                printf("Could not write returned cash. (CASH)\n");
                exit(1);
            }
            break;
        //error
        case 8:
            if(write(sockfd, &mesType, (sizeof(int)) != (sizeof(int)))){
                printf("Could not write message type. (ERROR)\n");
                exit(1);
            }
            break;
        //terminate
        case 9:
            return NULL;
    }
}
}



void *log_file() {

    while(1){
        sleep(5);
        FILE* logFile;
        logFile = fopen(logFileName, "w");
        if(logFile < 0){
            printf("failed to open logFile\n");
            exit(0);
        }
        for(int i = 0; i < BalSize; i++){
            pthread_mutex_lock(&lock);
            fprintf(logFile, "%d,%.2f,%s,%s,%ld\n", NewBal[i].accountNumber, NewBal[i].balance, NewBal[i].name, NewBal[i].userName, NewBal[i].birthday);
            pthread_mutex_unlock(&lock);
        }
        fclose(logFile);
    }


}

int main(int argc, char *argv[]){
    // argument handling
    if(argc != 4)
    {
        printSyntax();
        return 0;
    }

    bookeepingCode();

    char* serverAddr = argv[1];
    int serverPort = atoi(argv[2]);
    int numWorkers = atoi(argv[3]);

    int sockfd, len, connfd;
    struct sockaddr_in servaddr, cli;
    
    pthread_create(&tid, NULL, log_file, NULL);

    //create sockets
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation has failed.\n");
        exit(0);
    } else{
        printf("Socket has been created.\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    //assign IP and ports
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serverPort);

    //socket bind to server
    if ((bind(sockfd, (SA *) &servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind has failed.....\n");
        exit(0);
    } else
        printf("Socket is binded.....\n");


    //server is listening
    if ((listen(sockfd, numWorkers)) != 0) {
        printf("Listen failed......\n");
        exit(0);
    } else{
        printf("Server listening.....\n");
    }

    len = sizeof(cli);
    
    while(1) {

        connfd = accept(sockfd, (SA *) &cli, &len); 

        if (connfd < 0) {
            printf("Server accept failed.\n");
            exit(0);
        } else{
             printf("Server accept the client.\n");
        }

        int *arg = malloc(sizeof(*arg));

        *arg = connfd;
      
        pthread_t workerThread;
        if(pthread_create(&workerThread, NULL, iterate, (void*) arg) != 0){ 
            fprintf(stderr, "Could not create worker thread.\n");
        }
    }

    close(sockfd);
    close(connfd);
    return 0; 
}

