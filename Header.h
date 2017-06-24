//
//  Header.h
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 3/1/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//
#include <stdbool.h>
#ifndef Header_h
#define Header_h

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               MESSAGE FUNCTIONS                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void receiveMSG (int sock); /* Receive for Message exchange between friends */

void sendMSG (int sock); /* Send for Message exchange between friends */

void sendMessage(int socket); /* Send for Message exchange between Client and Server */

void receiveMessage (int sock); /* Receive for Message exchange between Client and Server */

void ListenToFriend(unsigned short port); /* Server-Friend listen to port number */

void ConnectToFriend(); /* Client-Friend Connects to Server-Friend */

void ChatWithFriendServer(int friendSock); /* Messaging interface for Server-Friend */

void ChatWithFriendClient(int mySocket); /* Messaging interface for Client-Friend */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *         ERROR / CONDITION CHECKING FUNCTIONS        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DieWithError(char *errorMessage); /* Handles TCP errors */

bool checkExit(char* buffer); /* Checks chat exit condition */

#endif /* Header_h */
