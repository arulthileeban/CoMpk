
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<pthread.h>
#include <sys/types.h>
#include <linux/userfaultfd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>

#include<mpt/mpt.h>
#include<mpt/pkey.h>
#include "mpk_apis.h"


#define BUFF_SIZE 4096
#define errExit(msg) do {perror(msg); exit (EXIT_FAILURE);\
    } while(0)

/* Grobal variables */

char *addr;
unsigned long w = 0;



static int e;

static int *
client(int *e)
{

    _init(0.5);

    int vpkeys = rwmmap((void**)&addr);
    BRIDGE_DOMAINRW(vpkeys);

    printf("Wriring data in the trusted thread\n");
    int i;
    for(i = 0; i<4096; i++)
    {
        addr[i] = '&';
        printf("%c", addr[i]);
    }

    printf("\n");
    EXIT_DOMAIN(vpkeys);
    printf("Thread 1----------------\n");

    w = 1;
 return 0;
}





static int *
server(int* e){

/***/
//printf("Thread 2----------------\n");

printf("\n");

    return 0; /*not necessary for now*/
}






int main()
{
    pthread_t client_thr;
    pthread_t server_thr;

    e = pthread_create(&server_thr, NULL, server, &e);
    if ( e != 0) {
        errExit("Failed to create server thread");
    }

    e = pthread_create(&client_thr, NULL, client, &e);
    if ( e != 0) {
        errExit("client thread creation failure");
    }
    
   while(w == 0);
    printf("***Access from the untrusted thread\n");
    
    addr[0]  = '#';
    pthread_join(server_thr, NULL);
    pthread_join(client_thr, NULL);
return 0;
}

