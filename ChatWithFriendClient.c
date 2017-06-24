//
//  ChatWithFriend.c
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 3/4/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//
#include "Header.h"
#include <stdio.h>
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */
#include <stdbool.h> /* boolean*/
#include <string.h>
#include <ctype.h> /*tolower*/
#include <stdlib.h> /* for atoi() */
#define RCVBUFSIZE 2000 /* Size of receive buffer */

char friend_Client_msg[RCVBUFSIZE];
void receiveMSG (int sock);
void sendMSG (int sock);

int receiveMsgLength2(int sock);
void sendConfirmation2(int sock, char* fullMsgSize, int msgLen);

void receiveCofirmation2(int sock, char* fullMsgSize, int msgLen );
int sendMsgLength2(int sock);

void ChatWithFriendClient(int mySocket) {
    int recvMsgSize; /* Size of received message */
    
    while (true) {
        printf("\n\nType 'bye' to stop conversation.\n");
        
        printf("\nBob: ");
        scanf("%[^\n]s", friend_Client_msg);
        getchar();
        
        /* Send message to friend */
        sendMSG(mySocket);
        bzero(friend_Client_msg, RCVBUFSIZE);
        
        /* Check exit condition */
        if (checkExit(friend_Client_msg)) {
            close(mySocket); /* Close client socket */
            return;
            
        }
        
        /* Receive message from friend */
        receiveMSG(mySocket);
        printf("\nAlice: %s\n", friend_Client_msg);
        
        /* Check exit condition */
        if (checkExit(friend_Client_msg)) {
            close(mySocket); /* Close client socket */
            return;
        }
    }
}

/* Receive confimation that message was received */
void receiveCofirmation2(int sock, char* fullMsgSize, int msgLen ) {
    //reveive OK from server: Message received
    int bytesRcvd;
    if ((bytesRcvd = recv(sock, fullMsgSize, msgLen, 0)) <= 0) {
        DieWithError("recv() failed or connection closed prematurely");
    }
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
}

/* Send the length of the message to be send next */
int sendMsgLength2(int sock) {
    //get message legth
    int msgLen = strlen(friend_Client_msg);
    
    //convert length to string
    char strMsgLen [5];
    sprintf(strMsgLen,"%d",msgLen);
    //getchar();
    
    //get length of message length string
    int sizeMsgLen = strlen(strMsgLen);
    
    /* Send the MESSAGE LENGTH to server */
    if (send(sock, strMsgLen, sizeMsgLen, 0) != sizeMsgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    receiveCofirmation2(sock, strMsgLen, sizeMsgLen);
    
    return sizeMsgLen;
    
}


/* Send message */
void sendMSG (int sock) {
    //message size
    int msgLen = strlen(friend_Client_msg);
    
    //seng msg length
    sendMsgLength2(sock);
    
    
    /* Send the MESSAGE to server */
    if (send(sock, friend_Client_msg, msgLen, 0) != msgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    bzero(friend_Client_msg, RCVBUFSIZE);
}

/* Send confirmation that message was received */
void sendConfirmation2(int sock, char* fullMsgSize, int msgLen) {
    /* Send confirmation message to server */
    if (send(sock, fullMsgSize, msgLen, 0) != msgLen) {
        DieWithError("send() failed");
    }
}

/* Receive the length of the message to be received next */
int receiveMsgLength2(int sock) {
    int bytesRcvd = 0;
    char fullMsgSize[25];
    bzero(fullMsgSize, 25);
    int size = 25;
    
    if ((bytesRcvd = recv(sock, fullMsgSize, 24, 0)) < 0) {
        DieWithError("recv() failed");
    }
    
    int msgLen = strlen(fullMsgSize);
    fullMsgSize[bytesRcvd] = '\0';
    
    sendConfirmation2(sock, fullMsgSize, msgLen);
    
    int fullMsg = atoi(fullMsgSize);
    
    return fullMsg;
    
}


/* Receive Message */
void receiveMSG (int sock) {
    //bzero(msgBuffer, RCVBUFSIZE);
    int bytesRcvd = 0;
    int totalBytesRcvd;
    
    /* Receive MESSAGE LENGTH from server */
    int fullMsg = receiveMsgLength2(sock);
    
    /* Receive MESSAGE from server */
    totalBytesRcvd = 0;
    
    while (totalBytesRcvd < fullMsg) {
        if ((bytesRcvd = recv(sock, friend_Client_msg,  RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        if (totalBytesRcvd == fullMsg) {
            friend_Client_msg[fullMsg] = '\0';
        }
        
    }
}
