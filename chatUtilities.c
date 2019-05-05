/*****************************************************************
 * Name: Chelsea Egan
 * Course: CS 372-400
 * Program: chatUtilities.c
 * Description: This file provides the initialization of functions
 * used by chatclient.c
 * Last Modified: May 4, 2019
*****************************************************************/

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chatUtilities.h"

/*****************************************************************
 * name: connectToServer
 * Preconditions:
 * @param hostname - char pointer to host name or IP address of server
 * @param port - char pointer to the server's port number
 * @param newSocket - pointer to an uninitialized int
 * Postconditions:
 * If a connection cannot be made, the program will terminate.
 * If connection to server is made, newSocket is created and can
 * be used for future communication with the server.
 * Source: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 *****************************************************************/
void connectToServer(char *hostname, char *port, int *newSocket) {
    int status;
    struct addrinfo hints, *servInfo, *p;

    memset(&hints, 0, sizeof hints);
    // IPv4
    hints.ai_family = AF_INET;
    // TCP
    hints.ai_socktype = SOCK_STREAM;

    // Get the address information for the server, which can be used to connect
    // Returns non-zero if fails
    if ((status = getaddrinfo(hostname, port, &hints, &servInfo)) != 0) {
        error("FAILED TO GET ADDRESS INFO", true);
    }

    // Loops through addresses until one that can be connected to is found
    for (p = servInfo; p != NULL; p = p->ai_next) {
        // Tries to create a socket using the info from getaddrinfo
        if ((*newSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        // If socket was created, tries to use it to connect to the server
        if (connect(*newSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(*newSocket);
            continue;
        }
        // Socket was successfully created and connection was made
        // Stop trying
        break;
    }

    // No connection was made
    if (p == NULL) {
        error("FAILED TO CREATE SOCKET", true);
    }

    freeaddrinfo(servInfo);
}

/*****************************************************************
 * Name: setUpUsername
 * Preconditions:
 * @param username - empty char array
 * Postconditions: The username will be set with input from the
 * user and appended with "> " to serve as a way to prompt for
 * message input
 *****************************************************************/
void setUpUsername(char *username) {
    char usernamePrompt[] = "Enter a username (10 characters or less): ";

    // Make sure the array is empty
    memset(username, 0, sizeof(username));

    getInput(12, usernamePrompt, username, true);
    strncat(username, "> ", 2);
}

/*****************************************************************
 * Name: sendMessage
 * Preconditions:
 * @param username - pointer to the username char array previously set
 * @param socket - pointer to the socket that is connected
 * Postconditions: Sends a user-provided message to the client.
 * If sending fails, prints an error message and returns 1 to
 * indicate the connection should be closed
 *****************************************************************/
int sendMessage(char *username, int *socket) {
    // User can input a message of up to 500 characters minus the
    // space taken by their username and "> "
    int spaceForMessage = MAXBUFFERSIZE - strlen(username);
    char totalMessage[MAXBUFFERSIZE];
    char userMessage[spaceForMessage];

    // Make sure the message array is empty
    memset(totalMessage, 0, MAXBUFFERSIZE);

    // Add the username to the beginning of the message
    strncpy(totalMessage, username, strlen(username));

    // Get the message from the user and add it to the array that
    // will be sent
    getInput(spaceForMessage, username, userMessage, true);
    strncat(totalMessage, userMessage, strlen(userMessage));

    // Try to send the message to the client
    // If fails, print an error and close the connection
    if (send(*socket, totalMessage, strlen(totalMessage), 0) == -1) {
        error("FAILURE SENDING TO SERVER", false);
        return 1;
    }
}

/*****************************************************************
 * Name: getInput
 * Preconditions:
 * @param maxLength - int representing how many characters the
 * user can enter
 * @param promptMessage - pointer to char holding string that will
 * be printed on screen to prompt user for input
 * @param input - pointer to char array that will hold user input
 * must have enough space for maxLength chars
 * @param stripNewline - boolean to indicate whether the newline
 * character should be stripped from the message
 * Postconditions: Loops until receives valid user input, if
 * stripNewline is true, it removes "\n", and finally input will
 * hold the user's input
 * Source for checking if user input is too long:
 * https://stackoverflow.com/a/40891999
 * Source for removing newline character:
 * https://stackoverflow.com/a/28462221
 *****************************************************************/
void getInput(int maxLength, char *promptMessage, char *input, bool stripNewline) {
    bool validInput = false;

    while (!validInput) {
        printf(promptMessage);

        if (fgets(input, maxLength, stdin) != NULL) {
            int inputLen = strlen(input);

            // If the last character is not "\n" it means it was too long
            if (input[inputLen - 1] != '\n') {
                // Clear the buffer
                while ((getchar()) != '\n');
                printf("\nThat was too long!\n");
            } else if (inputLen == 1) {
                printf("\nYou did not enter anything!\n");
            } else {
                if (stripNewline) {
                    // Remove newline character if unwanted
                    input[strcspn(input, "\n")] = 0;
                }
                validInput = true;
            }
        } else {
            printf("\nError getting input!\n");
        }
    }
}

/*****************************************************************
 * Name: receiveMessage
 * Preconditions:
 * @param socket - pointer to the socket that is connected
 * Postconditions:
 * If received "-1" from server, this indicates they have closed
 * the connection and so the chatclient should close theirs. If an
 * error occurs, the connection will be closed. If message is
 * successfully received, it is printed.
 *****************************************************************/
int receiveMessage(int *socket) {
    char inMessage[MAXBUFFERSIZE + 1];
    int numBytes;
    // Symbolizes connection is being closed by server
    char quitMessage[] = "-1";

    if ((numBytes = recv(*socket, inMessage, MAXBUFFERSIZE, 0)) == -1) {
        error("FAILURE RECEIVING FROM SERVER", false);
        return 1;
    } else if (strncmp(inMessage, quitMessage, 2) == 0) {
        // The server closed their side of the connection
        error("THE CHAT HAS ENDED", false);
        return 1;
    }

    inMessage[numBytes] = '\0';

    printf("%s\n", inMessage);
}

/*****************************************************************
 * Name: error
 * Preconditions:
 * @param errorMessage - string holding message that should be
 * printed describing the error
 * @param terminal - boolean indicating whether the entire program
 * should be terminated
 * Postconditions: The error message will be printed. If terminal
 * is true, the program will be terminated.
 *****************************************************************/
void error(char *errorMessage, bool terminal) {
    printf("\n\n***** %s *****\n", errorMessage);
    printf("***** CLOSING CONNECTION *****\n");

    if (terminal) {
        exit(1);
    }
}