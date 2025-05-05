/*
https://github.com/pradeepselvakumar/CSC-284/blob/main/CapstoneProject/README.md

Part 2: Chatroom Server
/-1. The server must accept a port number as input at startup. // Not yet implemented 
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
#include <string>
#include <vector>
#include <map>
#pragma comment(lib, "ws2_32.lib")

//// Users, Rooms,and the ChatServer itself, and add appropriate functionality to each.
class Room {
public:
    std::string roomName;
    std::vector<SOCKET> users;  // Vector to store the sockets of users in the room

    // Constructors for Room class
    Room() : roomName("") {}
    Room(const std::string& name) : roomName(name) {}

    // Add a user to the room
    void addUser(SOCKET userSocket) {
        users.push_back(userSocket);    // Add the user's socket to the vector
    }

    // Remove a user from the room via their userSocket
    void removeUser(SOCKET userSocket) {
        // Iterate through the vector of user sockets
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (*it == userSocket) {    // If current socket matches the given userSocket
                users.erase(it);        // Remove the socket from the vector
                break;                  // Exit the loop
            }
        }
    }

    // Broadcast a message to all users in the room except the sender
    void broadcastMessage(const std::string& msg, SOCKET senderSocket) {
        for (SOCKET userSocket : users) {       // Iterate through the vector of user sockets
            if (userSocket != senderSocket) {   // If the current socket is not the sender's socket
                send(userSocket, msg.c_str(), msg.size(), 0);   // Send the message
            }
        }
    }

    // Get the number of users in the room
    int getUserCount() const {
        return users.size();
    }

    // Get the room name
    std::string getRoomName() const {
        return roomName;
    }
};

std::map<std::string, Room> rooms; // Global map to store rooms.
//std::map<SOCKET, User> userMap; // Map to store user objects, key is the socket.

////user name, chatcommands kind of go here
// Function to handle communication with a single client in a separate thread
void handleClient(SOCKET clientSocket) {
    char buffer[1024];  // Buffer to store data received from the client
    //ZeroMemory(buffer, sizeof(buffer)); // Clear buffer of leftover data

    // At this point, a client has connected. The requirements suggest:
    // - Creating a User object for this client (not yet implemented)
    // - Adding the user to a general "lobby" or initial connection state (not yet implemented)
        // create user
        // connected to server
        // add user to lobby

    while (true) {
        ZeroMemory(buffer, sizeof(buffer)); // Clear buffer of leftover data before each receive operation
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);  // Receive data from the client socket

        if (bytesReceived > 0) {
            std::string msg(buffer, bytesReceived); // Convert received bytes to a string
            std::cout << "Client says: " << msg << "\n";    // Print the message

            // Requirement: The server needs to parse this message to see if it's a command
            // (CREATE_ROOM, JOIN_ROOM, LIST_ROOMS, EXIT) or a regular chat message.
            // This parsing logic is not yet implemented.

            auto it = rooms.find("lobby");  // Find the "lobby" room in the rooms map
            if (it != rooms.end()) {    // If the "lobby" room is found
                Room& lobby = it->second;   // Get a reference to the "lobby" room object
                lobby.broadcastMessage(msg, clientSocket);  // Broadcast the message to all users
            }
        }
        else if (bytesReceived == 0) {
            std::cout << "Client disconnected gracefully.\n";   // Client closed the connection
            // Remove the user from the lobby upon disconnection
            auto it = rooms.find("lobby");  // Find the "lobby" room in the rooms map
            if (it != rooms.end()) {    // If the "lobby" room is found
                Room& lobby = it->second;
                lobby.removeUser(clientSocket); // Remove the user from the lobby
                std::string leaveNotice = "User left the room.\n"; // Create departure notice
                lobby.broadcastMessage(leaveNotice, clientSocket);  // Notify users
            }
            break;  // Exit the loop for this client
        } else {
            // Notify user that error occurred while receiving data & display error code
            std::cerr << "Error occurred while receiving data: " << WSAGetLastError() << "\n";  // recv failed with error
            break;  // Exit the loop for this client
        }
    }

    closesocket(clientSocket);  // Close the socket associated with this client
    // Requirement: When a client disconnects, the server should:
    // - Remove the user from any room they were in (not yet implemented)
    // - Notify other users in the room (not yet implemented)
}


int main(int argc, char* argv[]) {
    WSADATA wsaData;    // Structure for Windows Sockets startup info
    SOCKET serverSocket = INVALID_SOCKET, clientSocket = INVALID_SOCKET; // Socket handles initialized with INVALID_SOCKET upon declaration
    sockaddr_in serverAddr{}, clientAddr{};   // Structure for server's/client's address information
    //char buffer[1024];
    int clientSize = sizeof(clientAddr);    // Initialize size of clientAddr structure for accept()

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET: IPv4, Create a TCP socket w/default protocol
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    } else {
        std::cout << "----------WELCOME TO CELIA'S CHATROOM SERVER ----------\n"
            << "Socket for network communication created successfully.\n";
    }

    // Get port number from command-line argument
    if (argc != 2) {
        std::cerr << "Usage: chatroom_server <port_number>\n";
        closesocket(serverSocket);  // Close socket before cleanup
        WSACleanup();
        return 1;
    }
    int serverPort = atoi(argv[1]);   // Convert port number from string to int & save to "serverPort"

    if (serverPort <= 0 || serverPort > 65535) {
        std::cerr << "Invalid port number. Please use a port between 1 and 65535. (Hint: Try 54000.)\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;            // Use IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;    // Listen on all available network interfaces
    serverAddr.sin_port = htons(serverPort);          // Use the user's port number
    //inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Could be used to listen on a specific IP

    // Bind the socket to the server address
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, SOMAXCONN);    // SOMAXCONN: Maximum length of the pending connection queue
    std::cout << "Server listening on port " << serverPort << "...\n";

    // Create the "lobby" room
    rooms["lobby"] = Room("lobby"); // Add to the rooms map

    // Main loop/thread to accept incoming client connections
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);    // Accept a connection
        if (clientSocket != INVALID_SOCKET) {   // If a connection is accepted successfully
            std::cout << "Client connected.\n";
            rooms["lobby"].addUser(clientSocket);   // add new user to the lobby
            // create a new thread to handle communication with the newly connected client
            std::thread clientThread(handleClient, clientSocket);
            clientThread.detach(); // Detach thread so it runs independently in bg
        }
        // clientSocket is closed in handleClient thread
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    system("pause");
    return 0;
}