#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

typedef struct account
{
  int accountNumber;
  float balance;
  char userName[MAX_STR];
  char name[MAX_STR];
  time_t birthday;
  pthread_mutex_t accountLock;
} 
Total_t;



void printSyntax();

#endif
