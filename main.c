//
//  main.c
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.

#include "Header.h"
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#define RCVBUFSIZE 2000 /* Size of receive buffer */
#define MSGBUFFERSIZE 2000


//void DieWithError(char *errorMessage);
char message[MSGBUFFERSIZE]; /*msg sent*/

char msgBuffer[RCVBUFSIZE]; /* Buffer for message received  */
int main(int argc, char *argv[]) {
    
    int sock; /* Socket descriptor */
    struct sockaddr_in servAddr; /* Server address */
    unsigned short servPort = 0; /* Server port */
    char *servIP; /* Server IP address (dotted quad) */
    char serv[20];
    char value;
    unsigned int msgLen; /* Length of message to send to server*/
    int bytesRcvd;
    
    struct Login {
        char username[10];
        char password[10];
    };
    
    struct Login users[2];
    
    for (;;) {
        int option = -1;
        printf("------------------------\n");
        printf("0. Connect to the server\n");
        printf("1. Get the user list\n");
        printf("2. Send a message\n");
        printf("3. Get my messages\n");
        printf("4. Initiate a chat with my friend\n");
        printf("5. Chat with my friend\n");
        printf("6. Exit\n");
        
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        
        switch (option) {
            case 0:
                
                printf("Please enter the IP address: ");
                scanf("%s", serv);
                servIP = serv;
                
                printf("Please enter the port number: ");
                scanf("%hu", &servPort);
                
                printf("Connecting...\n");
                
                
                /* Create a reliable, stream socket using TCP */
                if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                    DieWithError(" socket () failed") ;
                }
                
                /* Construct the server address structure */
                memset(&servAddr, 0, sizeof(servAddr)); /* Zero out structure */
                servAddr.sin_family = AF_INET; /* Internet address family */
                servAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
                servAddr.sin_port = htons(servPort); /* Server port */
                
                
                /* Establish the connection to the server */
                if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
                    DieWithError(" connect () failed");
                }
                
                printf("Connected!\n");
                printf("Wecome! Please log in.\n");
                
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '0';
                msgLen = sizeof(value);
                
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                int i = 0;
                printf("Username: ");
                if (strlen(users[i].username) > 0 ) {
                    i++;
                }
                scanf("%s", users[i].username);
                getchar();
                
                //get size of msg to be sent to server
                msgLen = strlen(users[i].username); /* Determine username length */
                
                /* Send the USERNAME to the server */
                if (send(sock, &users[i].username, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                i = 0;
                printf("Password: ");
                if (strlen(users[i].password) > 0 ) {
                    i++;
                }
                scanf("%s", users[i].password);
                getchar();
                msgLen = strlen(users[i].password);
                
                /* Send the PASSWORD to the server */
                if (send(sock, &users[i].password, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                /* Receive LOGIN RESPONSE from the server */
                if ((bytesRcvd = recv(sock, msgBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                    DieWithError("recv() failed or connection closed prematurely");
                }
                msgBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
                printf("\n%s\n\n", msgBuffer); /* Print the response from server */
                bzero(msgBuffer, RCVBUFSIZE);
                
                break;
                
            case 1:
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '1';
                msgLen = sizeof(value);
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                /* Receive USER LIST RESPONSE from the server */
                if ((bytesRcvd = recv(sock, msgBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                    DieWithError("recv() failed or connection closed prematurely");
                }
                msgBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
                printf("\n%s\n", msgBuffer); /* Print the response from server */
                bzero(msgBuffer, MSGBUFFERSIZE);
                break;
                
            case 2:
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '2';
                msgLen = sizeof(value);
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                //get username
                printf("\nEnter username: ");
                scanf("%[^\n ]", message);
                getchar();
                
                /* SEND USERNAME */
                sendMessage(sock);
                
                //get message
                printf("\nEnter message: ");
                scanf("%[^\n]s", message);
                getchar();
                
                /* SEND MESSAGE */
                sendMessage(sock);
                printf("\n\n Message was sent successfully ");
                break;
                
            case 3:
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '3';
                msgLen = sizeof(value);
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                /* Receive user messages */
                receiveMessage(sock);
                int len = strlen(message);
                if (len > 0) {
                    printf("\nMessages:\n%s\n", message);
                } else {
                    printf("\nYou have no messages\n");
                }
                
                bzero(message, MSGBUFFERSIZE);
                break;
            case 4:
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '4';
                msgLen = sizeof(value);
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                /* Disconnects current user from server! */
                printf("%s\n", " \n******* Disconnected With Server *******");
                printf("\nPlease enter the port number you want to listen on: \n");
                char aPort[10];
                scanf("%s", aPort);
                int c;
                c = getchar();
                unsigned short port = atoi(aPort);
                //close(sock);
                
                /* Listen and connect to friend's port */
                ListenToFriend(port);
                break;
            case 5:
                /* Send the SWITCH OPTION to the server */
                //get size of msg to be sent to server
                value = '5';
                msgLen = sizeof(value);
                if (send(sock, &value, msgLen, 0) != msgLen) {
                    DieWithError("send() sent a different number of bytes than expected");
                }
                
                /* Disconnects current user from server! */
                close(sock);
                printf("%s\n", " \n******* Disconnected With Server *******");
                
                /* Start connection with friend */
                ConnectToFriend();
                
                break;
            case 6:
                /* Terminate program */
                exit(0);
            default:
                printf("Please enter a number from 0 - 6\n");
                break;
        }
    }
}

/* Send Message to Server */
void sendMessage (int sock) {
    //get message legth
    unsigned int msgLen = strlen(message);
    
    //convert length to string
    char strMsgLen [5];
    sprintf(strMsgLen,"%d",msgLen);
    
    //get length of message length string
    unsigned int sizeMsgLen = strlen(strMsgLen);
    //printf("message SIZE has length: %d ", msgLen);
    
    /* Send the MESSAGE LENGTH to server */
    if (send(sock, strMsgLen, sizeMsgLen, 0) != sizeMsgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    //receive OK from server: Message received
    int bytesRcvd;
    if ((bytesRcvd = recv(sock, strMsgLen, sizeMsgLen, 0)) <= 0) {
        DieWithError("recv() failed or connection closed prematurely");
    }
    strMsgLen[bytesRcvd] = '\0'; /* Terminate the string! */
    bzero(strMsgLen, 5);
    
    /* Send the MESSAGE to server */
    if (send(sock, message, msgLen, 0) != msgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    bzero(message, MSGBUFFERSIZE);
}


/* Receive Message from Server */
void receiveMessage (int sock) {
    int bytesRcvd = 0;
    int totalBytesRcvd;
    
    /* Receive MESSAGE LENGTH from server */
    char fullMsgSize[5];
    if ((bytesRcvd = recv(sock, fullMsgSize, 4, 0)) < 0) {
        DieWithError("recv() failed") ;
    }
    int msgLen = strlen(fullMsgSize);
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
    /* Send confirmation message to server */
    if (send(sock, fullMsgSize, msgLen, 0) != msgLen) {
        DieWithError("send() failed");
    }
    
    /* Receive MESSAGE from server */
    totalBytesRcvd = 0;
    int fullMsg = atoi(fullMsgSize);
    while (totalBytesRcvd < fullMsg) {
        if ((bytesRcvd = recv(sock, message,  MSGBUFFERSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        if (totalBytesRcvd == fullMsg) {
            message[fullMsg] = '\0'; /* Terminate the string! */
        }
    }
}


