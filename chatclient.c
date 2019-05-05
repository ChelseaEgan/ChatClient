/*****************************************************************
 * Name: Chelsea Egan
 * Course: CS 372-400
 * Program: chatclient.c
 * Description: This program acts as a client, which can connect
 * to a server and chat. It requests a connection to a specified
 * socket (identified by hostname and port). Once connected, the
 * client and server can take turns sending messages until someone
 * sends "\quit"
 * Last Modified: May 4, 2019
*****************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "chatUtilities.h"

int main(int argc, char *argv[]) {
    int newSocket;
    char username[13];

    // User should have entered three arguments on the command line:
    // file name, host name, and port number
    if (argc != 3) {
        printf("Please try again with the following format:\n");
        printf("./chatclient.exe <hostname> <port>");
        return 1;
    }

    // Set the hostname and port of the server to which the user
    // wishes to connect
    char *hostname = argv[1];
    char *port = argv[2];

    // Create a socket and connect to server
    connectToServer(hostname, port, &newSocket);

    // Get name used to display user's messages
    setUpUsername(username);

    printf("\n\n***** STARTING THE CHAT *****\n\n");

    // Continue to send and receive messages until the connection
    // is ended or an error is encountered
    while (true) {
        if((sendMessage(username, &newSocket)) == 1) {
            break;
        }

        if((receiveMessage(&newSocket)) == 1) {
            break;
        }
    }

    // Close the connection with the server
    close(newSocket);

    return 0;
}