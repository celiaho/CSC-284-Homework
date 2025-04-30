/*
https://github.com/pradeepselvakumar/CSC-284/blob/main/CapstoneProject/README.md

Part 2: Chatroom Server
-1. The server must accept a port number as input at startup. // Not yet implemented 
The server must:
/-2. Handle multiple clients concurrently using threads. // Implemented using std::thread
3. Maintain a list of available chatrooms.         // Not yet implemented
-4. Support the following commands sent by clients:    // Not yet implemented
            CREATE_ROOM <room_name>: Create a new room.
            JOIN_ROOM <room_name>: Join an existing room.
            LIST_ROOMS: List all available chatrooms.
           -EXIT: Disconnect from the chatroom.
The server should:
5. Maintain room membership.                       // Not yet implemented
-6. Only deliver messages to users within the same room. // Not yet implemented
7. Notify users when others join or leave the room.    // Not yet implemented

8. Use separate classes to represent Users, Rooms,and the ChatServer itself, and add appropriate functionality to each. // Not yet implemented in this basic structure

HINTS:
/9. Use std::thread to handle multiple clients on the server. // Already being used
10. Use std::mutex to protect shared data structures like the room list and client registry. // Not yet implemented, but will be needed

Commit and push everytime something works
*/

/* Start with Sockets server code, when client connects have it spit out msg
Server: Welcome to the chatroom!Available commands :
CREATE <room> -Create a new room
JOIN <room> -Join an existing room
LEAVE - Leave the current room
LIST - List all rooms
EXIT - Disconnect

Create Room class

Commit and push everytime something works
*/

#include <iostream>
#include <winsock2.h>
#include <thread>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Function to handle communication with a single client in a separate thread
////user name, chatcommands kind of go here
void handleClient(SOCKET clientSocket) {
    char buffer[1024];  // Buffer to store data received from the client

    // At this point, a client has connected. The requirements suggest:
    // - Creating a User object for this client (not yet implemented)
    // - Adding the user to a general "lobby" or initial connection state (not yet implemented)
        // create user
        // connected to server
        // add user to lobby

    while (true) {
        // moved also below char buffer, clears any previous buffer from other programs
        ZeroMemory(buffer, sizeof(buffer)); // Clear the buffer before each receive operation to prevent leftover data
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);  // Receive data from the client socket

        if (bytesReceived > 0) {
            std::string msg(buffer, bytesReceived);
            std::cout << "Client says: " << msg << "\n";

            // Requirement: The server needs to parse this message to see if it's a command
            // (CREATE_ROOM, JOIN_ROOM, LIST_ROOMS, EXIT) or a regular chat message.
            // This parsing logic is not yet implemented.

            // For now, the server simply echoes back the received message to the same client.
            // Echo back
            send(clientSocket, buffer, bytesReceived, 0);
        }
        else if (bytesReceived == 0) {
            std::cout << "Client disconnected gracefully.\n";   // The client closed the connection
            break;  // Exit the loop for this client
        }
        else {
            std::cerr << "recv failed.\n";  // An error occurred during the receive operation
            break;
        }
    }

    closesocket(clientSocket);  // Close the socket associated with this client
    // Requirement: When a client disconnects, the server should:
    // - Remove the user from any room they were in (not yet implemented)
    // - Notify other users in the room (not yet implemented)
}


int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr{}, clientAddr{};
    char buffer[1024];
    int clientSize;
    //G notes
    //WSADATA wsaData;           // Structure to hold Windows Sockets startup information
    //SOCKET serverSocket = INVALID_SOCKET, clientSocket = INVALID_SOCKET; // Socket handles :  It's good practice to initialize socket handles to INVALID_SOCKET upon declaration
    //sockaddr_in serverAddr{};   // Structure to hold the server's address information
    //sockaddr_in clientAddr{};   // Structure to hold the client's address information (not directly used in accept in this basic form)
    //// char buffer[1024];      // Buffer is now used in handleClient
    //int clientSize = sizeof(clientAddr); // Size of the client address structure : Variable needs to be initialized to the size of the clientAddr structure before being passed to the accept() function. The accept() function uses this size to know how much memory is allocated for the client address. 

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET: IPv4, SOCK_STREAM: TCP, 0: default protocol
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;    // Use IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on any (available network) interface
    serverAddr.sin_port = htons(54000);      // Set the port number serverAddr.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Could be used to listen on a specific IP

    // Bind the socket to the server address
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, SOMAXCONN);    // SOMAXCONN: Maximum length of the pending connection queue
    std::cout << "Server listening on port 54000...\n";

    // Main loop/thread to accept incoming client connections
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);    // Accept a pending connection
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "Client connected.\n";
            // Create a new thread to handle communication with the newly connected client
            std::thread clientThread(handleClient, clientSocket);
            clientThread.detach(); // Let thread run independently in bg
        }
        // Note: We don't explicitly close clientSocket here in the main loop
        // because the detached thread is responsible for closing its own socket.
    }
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}