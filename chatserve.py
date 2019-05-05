#!/usr/bin/env python3

#######################################################################
# Name: Chelsea Egan
# Course: CS 372-400
# Program: chatserve.py
# Description: This program acts as the server to which clients can
# connect and chat. It opens a "welcoming" socket and listens for a
# connection. Once connected, the client and server can take turns
# sending messages until someone sends "\quit"
# Last Modified: May 4, 2019
#######################################################################

import socket
import sys

bot_name = 'John Henry'

# Can send 500 characters total
# 10 characters in name
# 2 characters for "> "
maxMsgChars = 488

######################################################
# Name: check_list
# Preconditions: (1) a list of 1+ strings, (2) a
# string of length 1+
# Postconditions: If the keyword is in the list, its
# index will be returned; if not found, -1 will be
# returned.
######################################################
def check_list(my_list, keyword):
    try:
        return my_list.index(keyword)
    except ValueError:
        return -1

######################################################
# Name: check_keywords
# Preconditions: string containing '> '
# Postconditions: If the string contains '\quit', -1
# will be returned, otherwise nothing will happen
######################################################
def check_keywords(message):
    # Getting the string right after '> '
    keyword_array = message.split('> ', 1)
    if(check_list(keyword_array, '\quit') > -1):
        return -1

######################################################
# Name: start_up
# Preconditions: User entered a number in the range of
# 1024-65535 on the command line on start up
# Postconditions: If user-provided port number is
# invalid, the program will terminate. Otherwise, a
# "welcoming" socket will be created and the server
# will listen for connection requests
# Source for input validation method:
# https://www.101computing.net/number-only/
######################################################
def start_up():
    hostname = socket.gethostname()
    host_address = socket.gethostbyname(hostname)
    
    try:
        # Get port from user input on command line
        server_port_num = int(sys.argv[1])
    except ValueError:
        print('Not an integer! Try again with an integer between 1024-65535.')
        sys.exit()
    else:
        # Range of ports that are not "well known"
        if server_port_num < 1024 or server_port_num > 65535:
            print('Out of range! Try again with an integer between 1024-65535.')
            sys.exit()
        else:
            # Create socket with the address family ipv4 and TCP protocol
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # Set the socket address and port according to the provided input
            server_socket.bind((host_address, server_port_num))
            server_socket.listen(1)
            print('The server is ready to receive on\nhostname: '
                  + socket.gethostname() + '\nport: ' + str(server_port_num))
            return server_socket

######################################################
# Name: close_connection
# Preconditions: string
# Postconditions: Prints the provided "goodbye"
# message then closes the socket connected to the
# client. This does NOT close the "welcoming" socket
######################################################
def close_connection(end_message, socket):
    print('\n\n****** ' + end_message + ' ******\n')
    socket.close()

######################################################
# Name: send_message
# Postconditions: Prompts the user for a message they
# want to send to the client. If they entered "\quit"
# then (1) sends "-1" to client to alert that chat is
# ending, (2) closes the connection with the client.
# If they entered too many characters, it will loop
# until a valid message is received.
# Else, sends the message to the client.
######################################################
def send_message(socket):
    while True:
        message = input(bot_name + '> ')
        if message == '\quit':
            message = '-1';
            send_to_client(message, socket)
            close_connection('YOU HAVE CLOSED THE CHAT', socket)
            return False
        elif len(message) > maxMsgChars:
            print('Your message was too long!')
            print('Please enter less than ' + str(maxMsgChars) + ' characters.')
        else:
            # Prepend the server's "name" and "> " to the
            # message so it's formatted on arrival
            message = bot_name + '> ' + message
            return send_to_client(message, socket)

######################################################
# Name: send_to_client
# Preconditions: string provided by user
# Postconditions: Attempts to send the message to the
# client. If an error is encountered, the connection
# with the client is ended.
######################################################
def send_to_client(message, socket):
    try:
        socket.send(message.encode())
        return True
    except:
        print('Error encountered sending message to chatclient!\n')
        close_connection('AN ERROR HAS CLOSED THE CHAT', socket)
        return False

######################################################
# Name: receive_message
# Postconditions: If client sent "\quit", (1) notify
# client that chat is ending, (2) close the connection
# with the client. If any errors are encountered, the
# connection with the client is ended. Else, prints
# the received message to the console.
######################################################
def receive_message(socket):
    try:
        # Will accept up to 500B of data
        client_message = socket.recv(4000).decode()

        # Checks message for "\quit" keyword
        if(check_keywords(client_message) == -1):
            send_to_client('-1', socket);
            close_connection('CLIENT CLOSED THE CHAT', socket)
            return False
        else:
            print(client_message)
            return True
    except:
        print('Error encountered receiving message from chatclient!\n')
        close_connection('AN ERROR HAS CLOSED THE CHAT', socket)
        return False

######################################################
# Name: send_and_receive
# Preconditions: A socket connection with a client
# Postconditions: Alternates between receiving and
# sending messages until connection is terminated via
# keyword, Ctrl+C, or an error.
######################################################
def send_and_receive(socket):
    print('\n\n***** SOMEONE HAS JOINED THE CHAT *****\n\n')
          
    socket_is_open = True
    
    while socket_is_open:
        if receive_message(socket) == False:
            break
        socket_is_open = send_message(socket)

######################################################
# MAIN FUNCTION
# (1) Upon startup, creates a socket using port number
#     provided in command line and listens
# (2) Accepts incoming connection request
# (3) Starts by receiving a message from the client
#     and printing it to the screen
# (4) Alternates sending and receiving messages
# (5) If someone sends "\quit", it closes the
#     connection, but continues to listen for new
#     connections.
# (6) Entering Ctrl+C will terminate the program
######################################################

def main():
    try:
        # "Welcoming socket" listening for client
        server_socket = start_up()

        while True:
            # Connection made with client
            connection_socket, addr = server_socket.accept()

            send_and_receive(connection_socket)
    # User enters Ctrl+C
    except KeyboardInterrupt:
        print ('\n\nGoodbye!')
    sys.exit(0)

if __name__=='__main__':
    main()
