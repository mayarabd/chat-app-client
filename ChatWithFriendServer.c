//
//  HandleTCPFriend.c
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
#include "Header.h"
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */
#include <stdbool.h> /* boolean*/
#include <string.h>
#include <ctype.h> /*tolower*/
#include <stdlib.h> /* for atoi() */
#define RCVBUFSIZE 2000 /* Size of receive buffer */
char friend_Server_msg_inbox[RCVBUFSIZE];
char friend_Server_msg_outbox[RCVBUFSIZE];

void getInput();
void sendMSG2 (int sock);
void receiveMSG2 (int sock);

int receiveMsgLength(int sock);
void sendConfirmation(int sock, char* fullMsgSize, int msgLen);

void receiveCofirmation(int sock, char* fullMsgSize, int msgLen );
int sendMsgLength(int sock);

void ChatWithFriendServer(int friendSock) {
    
    while (true) {
        
        printf("\nType 'bye' to stop conversation.\n");
        
        /* Receive message from friend */
        receiveMSG2(friendSock);
        printf("\nBob: %s \n", friend_Server_msg_inbox);
        bzero(friend_Server_msg_inbox, RCVBUFSIZE);
        
        /* Check exit condition */
        if (checkExit(friend_Server_msg_inbox)) {
            close(friendSock); /* Close client socket */
            return;
        }
        
        getInput();
        
        /* Send message to friend */
        sendMSG2(friendSock);
        
        /* Check exit condition */
        if (checkExit(friend_Server_msg_outbox)) {
            close(friendSock); /* Close client socket */
            return;
        }
        
        
    }
}

/* Reads user input */
void getInput() {
    
    printf("\nAlice: \n");
    
    scanf("%[^\n]s", friend_Server_msg_outbox);
    
    getchar();
    
}

/* Checks exit condition */
bool checkExit(char* buffer) {
    int j = 0;
    while (buffer[j]) {
        buffer[j] = tolower(buffer[j]);
        j++;
    }
    
    if (strcmp(buffer, "bye") == 0) {
        return true;
    }
    
    return false;
}

/* Receive confimation that message was received */
void receiveCofirmation(int sock, char* fullMsgSize, int msgLen ) {
    //reveive OK from server: Message received
    int bytesRcvd;
    if ((bytesRcvd = recv(sock, fullMsgSize, msgLen, 0)) <= 0) {
        DieWithError("recv() failed or connection closed prematurely");
    }
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
    //bzero(fullMsgSize, 5)
    
}

/* Send the length of the message to be send next */
int sendMsgLength(int sock) {
    //get message legth
    int msgLen = strlen(friend_Server_msg_outbox);
    
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
    
    receiveCofirmation(sock, strMsgLen, sizeMsgLen);
    
    return sizeMsgLen;
    
}

/* Send message */
void sendMSG2 (int sock) {
    //message size
    int msgLen = strlen(friend_Server_msg_outbox);
    
    //seng msg length
    sendMsgLength(sock);
    
    
    /* Send the MESSAGE to server */
    if (send(sock, friend_Server_msg_outbox, msgLen, 0) != msgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    // bzero(friend_Server_msg_outbox, RCVBUFSIZE);
}

/* Send confirmation that message was received */
void sendConfirmation(int sock, char* fullMsgSize, int msgLen) {
    /* Send confirmation message to server */
    if (send(sock, fullMsgSize, msgLen, 0) != msgLen) {
        DieWithError("send() failed");
    }
}

/* Receive the length of the message to be received next */
int receiveMsgLength(int sock) {
    int bytesRcvd = 0;
    char fullMsgSize[25];
    bzero(fullMsgSize, 25);
    int size = 25;
    
    if ((bytesRcvd = recv(sock, fullMsgSize, 24, 0)) < 0) {
        DieWithError("recv() failed") ;
    }
    
    int msgLen = strlen(fullMsgSize);
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
    sendConfirmation(sock, fullMsgSize, msgLen);
    
    int fullMsg = atoi(fullMsgSize);
    
    return fullMsg;
    
}

/* Receive Message */
void receiveMSG2 (int sock) {
    //bzero(msgBuffer, RCVBUFSIZE);
    int bytesRcvd = 0;
    int totalBytesRcvd;
    
    /* Receive MESSAGE LENGTH from server */
    int fullMsg = receiveMsgLength(sock);
    
    /* Receive MESSAGE from server */
    totalBytesRcvd = 0;
    
    while (totalBytesRcvd < fullMsg) {
        if ((bytesRcvd = recv(sock, friend_Server_msg_inbox,  RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        if (totalBytesRcvd == fullMsg) {
            friend_Server_msg_inbox[fullMsg] = '\0';
        }
    }
}
