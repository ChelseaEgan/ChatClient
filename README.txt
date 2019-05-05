# PROJECT 1
Name: Chelsea Egan
Class: CS 372-400
Last Modified: May 4, 2019

This is a chat system created for project 1 of the Intro to Networks class at Oregon State University. A server is started via a terminal and opens a socket for "clients" to connect to. The client is also started via a terminal (can be on the same or different host as the server) and connects to the server by providing its hostname and port number. They can then alternate sending messages until one sends the termination command. The server will remain open for future client connections.


## Installation
For chatserve.py, use the makefile to turn it into an executable file
```
make chatserve
```
For chatclient.c, use the makefile to compile
```
make chatclient
```
* NOTE: all files must be in the same directory and you must be in that directory when using these commands.


## Usage
First, start the server by providing a port number. For example:
```
./chatserve.py 30200
```
Second, start the client by providing a hostname and port number. For example:
```
./chatclient.exe flip1.engr.oregonstate.edu 30200
```


## Ending
Either server or client can enter "\quit" (without the quotes) when it is their turn to send a message. This will gracefully close their connection, but the server will remain running. At which point, entering Ctrl+C on the server will terminate its program.


## Notes
- I have tested using flip1 and flip3, alternating for both between server and client. It shouldn't matter which you use.
- I have been using port 30200 with success, but my program just asks for one between 1024 and 65535
- Servers and clients can send a maximum of 500 characters, but this includes their username and "> ", which are automatically added.


## Sources
PYTHON
General documentation used for chatserve
	https://docs.python.org/release/2.6.5/library/internet.html
	https://docs.python.org/3/library/socket.html
Setting up a TCP socket connection
	Computer Networking by Kurose & Ross, section 2.7.2
Input validation
	https://www.101computing.net/number-only/
Setting up the main method
	https://www.guru99.com/learn-python-main-function-with-examples-understand-main.html
Catching Ctrl+C
	https://stackoverflow.com/questions/1187970/how-to-exit-from-python-without-traceback

C
Checking if user input is too long
	https://stackoverflow.com/a/40891999
Removing newline characters from input
	https://stackoverflow.com/a/28462221
Setting up a TCP connection
	https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
