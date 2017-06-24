//
//  listenToFriend.c
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 3/4/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//

#include "Header.h"
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */


#define MAXPENDING 1 /* Maximum outstanding connection requests */

void ListenToFriend(unsigned short port) {
    int mySock; /* Socket descriptor for server */
    int friendSock; /* Socket descriptor for client */
    struct sockaddr_in myAddr; /* Local address */
    struct sockaddr_in friendAddr; /* Client address */
    unsigned short myPort; /* Server port */
    unsigned int friendAddressLen; /* Length of client address data structure */
    
    myPort = port;
    
    /* Create socket for incoming connections */
    if ((mySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError( "socket () failed") ;
    
    /* Construct local address structure */
    memset(&myAddr, 0, sizeof(myAddr)); /* Zero out structure */
    myAddr.sin_family = AF_INET; /* Internet address family */
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    myAddr.sin_port = htons(myPort); /* Local port */
    
    /* Bind to the local address */
    if (bind(mySock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0)
        DieWithError ( "bind () failed");
    /* Mark the socket so it will listen for incoming connections */
    if (listen(mySock, MAXPENDING) < 0)
        DieWithError("listen() failed") ;
    printf("\nI'm listening on port number %d\n", port);
    
    /* Set the size of the in-out parameter */
    friendAddressLen = sizeof(friendAddr);
    /* Wait for a client to connect */
    
    if ((friendSock = accept(mySock, (struct sockaddr *) &friendAddr, &friendAddressLen)) < 0)
        DieWithError("accept() failed");
    /* clntSock is connected to a friend! */
    printf("\nBob is connected\n");
    
    
    ChatWithFriendServer(friendSock);
    
    
    //testing here
    close(mySock);
    
}

