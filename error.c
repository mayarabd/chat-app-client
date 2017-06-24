//
//  error.c
//  Client_Chat_App
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//

#include "Header.h"
#include <stdio.h> /* for perror() */
#include <stdlib.h> /* for exit() */

void DieWithError(char *errorMessage)
{
    perror(errorMessage) ;
    exit(1);
}
