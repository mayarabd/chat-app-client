#include "Header.h"
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#define RCVBUFSIZE 2000 /* Size of receive buffer */
#define MSGBUFFERSIZE 2000

void ConnectToFriend() {
    
    int mySocket; /* Socket descriptor */
    struct sockaddr_in friendAddr; /* Server address */
    unsigned short friendPort = 0; /* Server port */
    char *friendIP; /* Server IP address (dotted quad) */
    char friend[20];
    char fPort[10];
    
    printf("Please enter your friend's IP address: ");
    scanf("%s", friend);
    getchar();
    friendIP = friend;
    
    printf("Please enter our friend's port number: ");
    scanf("%s", fPort);
    getchar();
    friendPort = atoi(fPort);
    
    printf("Connecting...\n");
    
    /* Create a reliable, stream socket using TCP */
    if ((mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError(" socket () failed") ;
    }
    
    /* Construct the friend address structure */
    memset(&friendAddr, 0, sizeof(friendAddr)); /* Zero out structure */
    friendAddr.sin_family = AF_INET; /* Internet address family */
    friendAddr.sin_addr.s_addr = inet_addr(friendIP); /* Server IP address */
    friendAddr.sin_port = htons(friendPort); /* Server port */
    
    /* Establish the connection with friend */
    if (connect(mySocket, (struct sockaddr *) &friendAddr, sizeof(friendAddr)) < 0) {
        DieWithError(" connect () failed");
    }
    
    printf("Connected!\n");
    ChatWithFriendClient(mySocket);
}

