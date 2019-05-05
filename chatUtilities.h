/*****************************************************************
 * Name: Chelsea Egan
 * Course: CS 372-400
 * Program: chatUtilities.h
 * Description: This file provides the declaration of functions
 * used by chatclient.c
 * Last Modified: May 4, 2019
*****************************************************************/

#ifndef CHAT_UTILITIES
#define CHAT_UTILITIES

#include <stdbool.h>

#define MAXBUFFERSIZE 500 // characters
#define MAXUNSIZE 10 // characters

void connectToServer(char *, char *, int *);
void setUpUsername(char *);
int sendMessage(char *, int *);
void getInput(int, char *, char *, bool);
int receiveMessage(int *);
void error(char *, bool);

#endif