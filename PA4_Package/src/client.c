// *test machine: apollo/vole
// * group number: G[43]
// * name: Gabe Campos , Tyler Johnson-Vasquez, Zachary Guldberg
// * x500: campo193 , joh20191, Guldb016*/


#include "client.h"

#define MAX 516
#define PORT 44061
#define SA struct sockaddr

void printSyntax(){
    printf("incorrect usage syntax!\n");
    printf("usage: $ ./client input_filename server_addr server_port\n");
}

int main(int argc, char *argv[]){
    
    // argument handling
    if(argc != 4)
    {
        printSyntax();
        return 0;
    }

    struct timespec* start;
    struct timespec* end;
    clock_gettime(CLOCK_REALTIME, start);

    char* fileName = argv[1];
    char* serverAddr = argv[2];
    int serverPort = atoi(argv[3]);

    int sockfd, len;
    struct sockaddr_in servaddr, cli;
    int Connected = 1;
    bzero(&servaddr, sizeof(servaddr));


    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation has failed.\n");
        exit(0);
    } else
        printf("Socket has been created.\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serverPort);

    // open file for reading
    FILE* fp;
    char inputFileName[25];
    sprintf(inputFileName, "input/%s", fileName);
    fp = fopen(inputFileName, "r");
    if(fp == NULL){
        printf("Failed to open file %s\n", fileName);
        exit(0);
    }

    // extras
    char line[MAX_STR];
    int toConnect = 1;

    int messageType;
    int accountnumber;
    char* name;
    char* userName;
    time_t birthday;
    float balance;

    int response;
    int AccountNumber;
    float cashamount = CASH_AMOUNT;
    float cashReturned;
    float retBalance;
    int transact = TRANSACT;
    float val;

    float amount;
    int numTransactions;
    float STARTCASH = START_CASH;


    while(fgets(line, 500, fp) != NULL){
        
        // connect socket
        if(Connected){
            // Create socket and validate
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(sockfd == -1){
                printf("Socket binding failed.\n");
                exit(0);
            } else {
                printf("Socket binded.\n");
            }

            // Connect client socket to server port
            if(connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) != 0){
                printf("Failed to connect to server.\n");
                exit(0);
            } else {
                printf("Connected to server.\n");
            }
            Connected = 0;
        }
        sscanf(line, "%d,%d,%s,%s,%ld,%f,%d\n", &transact, &accountnumber, name, userName, &birthday, &amount, &numTransactions);


        
        switch(messageType){
            //register
            case 0:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type.(REGISTER)\n");
                    exit(1);
                }
                if(write(sockfd, userName, (sizeof(char)* MAX_STR) != (sizeof(char)* MAX_STR))){
                    printf("Could not write userName.(REGISTER)\n");
                    exit(1);
                }
                if(write(sockfd, name, (sizeof(char)* MAX_STR) != (sizeof(char)* MAX_STR))){
                    printf("Could not write name.(REGISTER)\n");
                    exit(1);
                }
                if(write(sockfd, &birthday, (sizeof(time_t)) != (sizeof(time_t)))){
                    printf("Could not write birthday.(REGISTER)\n");
                    exit(1);
                }
                if(read(sockfd, &response, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read response.(REGISTER)\n");
                    exit(1);
                }
                if(read(sockfd, &accountnumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read accountnumber.(REGISTER)\n");
                    exit(1);
                }
                if(read(sockfd, &balance, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not read balance.(REGISTER)\n");
                    exit(1);
                }
                break;
            // get account info
            case 1:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type. (GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                if(write(sockfd, &accountnumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write account number.(GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                if(read(sockfd, &response, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read response. (GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                if(read(sockfd, userName, (sizeof(char)* MAX_STR) != (sizeof(char)* MAX_STR))){
                    printf("Could not read userName. (GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                if(read(sockfd, name, (sizeof(char)* MAX_STR) != (sizeof(char)* MAX_STR))){
                    printf("Could not read name. (GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                if(read(sockfd, &birthday, (sizeof(time_t)) != (sizeof(time_t)))){
                    printf("Could not read birthday. (GET_ACCOUNT_INFO)\n");
                    exit(1);
                }
                break;
            // transact
            case 2:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type. (TRANSACT)\n");
                    exit(1);
                }
                if(write(sockfd, &accountnumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write account number. (TRANSACT)\n");
                    exit(1);
                }
                if(write(sockfd, &val, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not write value. (TRANSACT)\n");
                    exit(1);
                }
                if(read(sockfd, &response, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write response type. (TRANSACT)\n");
                    exit(1);
                }
                if(read(sockfd, &AccountNumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write updated account number. (TRANSACT)\n");
                    exit(1);
                }
                if(read(sockfd, &retBalance, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not write returned balance. (TRANSACT)\n");
                    exit(1);
                }
                break;
            //get balance
            case 3:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type. (GET_BALANCE)\n");
                    exit(1);
                }
                if(write(sockfd, &accountnumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write account number. (GET_BALANCE)\n");
                    exit(1);
                }
                if(read(sockfd, &response, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read response type. (GET_BALANCE)\n");
                    exit(1);
                }
                if(read(sockfd, &AccountNumber, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read account number. (GET_BALANCE)\n");
                    exit(1);
                }
                if(read(sockfd, &balance, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not read balance. (GET_BALANCE)\n");
                    exit(1);
                }
                break;
            //request cash
            case 6:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type. (REQUEST_CASH)\n");
                    exit(1);
                }
                if(write(sockfd, &cashamount, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not write cash amount. (REQUEST_CASH)\n");
                    exit(1);
                }
                if(read(sockfd, &response, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not read response. (REQUEST_CASH)\n");
                    exit(1);
                }
                if(write(sockfd, &cashReturned, (sizeof(float)) != (sizeof(float)))){
                    printf("Could not write the amount returned. (REQUEST_CASH)\n");
                    exit(1);
                }
                break;  
            //error       
            case 8:
                printf("ERROR\n");
                break;
            //terminate
            case 9:
                if(write(sockfd, &messageType, (sizeof(int)) != (sizeof(int)))){
                    printf("Could not write message type. (TERMINATE)\n");
                    exit(1);
                }
                break;        

        }


    }

    if(Connected){
        close(sockfd);
        Connected = 1;
    }
    
    clock_gettime(CLOCK_REALTIME, end);
    float timeDiff = (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
    printf("Elapsed Time: %.2f\n", timeDiff);

    fclose(fp);

    return 0; 
}

