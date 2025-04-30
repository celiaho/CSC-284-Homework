/*
https://github.com/pradeepselvakumar/CSC-284/blob/main/CapstoneProject/README.md

Part 1: Chatroom Client
-1. The client must accept IP address and port number as command-line arguments. // Not yet implemented
After connecting to the server, the client must:
-/2. Allow the user to input chat commands (EXIT first) or messages. // Implemented using std::getline
3. Support the same commands defined in the server (CREATE_ROOM, JOIN_ROOM, etc.). // Not yet implemented - currently sends freeform text
-/4. Display messages received from the server in real time. // Implemented using a separate thread
-/5. The client should also allow users to type and send freeform messages to others in the room. // Implemented

You may use the sample ChatRoomServer.exe to program against while developing your client. Note that this ChatRoomServer.exe takes a mandatory command line arugment that specifies the port number. If executed directly by double clicking the file from Windows explorer, it will exit immediately.

The client program developed in class (refer to the multi-threaded version on Moodle) can be used to communicate with the ChatRoomServer if port 54000 is specified as a command line argument to it.

HINT: Use std::mutex to protect shared data structures like the room list and client registry. // Not applicable to the client in the same way as the server

Bonus (Optional):
    Allow the client to specify a username when connecting. If not specified, default to an anonymous ID. // Not yet implemented
    The server should use the username when displaying messages or notifications. // Relates to the server-side implementation

*/

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

// Function to receive messages from the server in a separate thread
void receiveMessages(SOCKET sock) {
    char buffer[1024];
    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            // Display the received message, adding a newline and the "Server:" prefix, and then the input prompt again
            std::cout << "\nServer: " << std::string(buffer, bytesReceived) << "\n> ";
        } else if (bytesReceived == 0) {
            std::cout << "Server disconnected.\n";
            break;
        } else {
            std::cerr << "recv failed.\n"; // An error occurred during receiving
            break;
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr{};
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;       // Use IPv4
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Set the server's IP address (currently hardcoded to localhost)
    serverAddr.sin_port = htons(54000);   // Set the server's port number (currently hardcoded)

    // Requirement: The IP address and port should be taken from command-line arguments.
    // This part needs to be implemented.

    // Connect to server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server. Type messages, or 'exit' to quit.\n";
    // Start the receiving thread
    std::thread receiver(receiveMessages, sock);
    receiver.detach();  // Allow the receiving thread to run independently

    // Send loop
    std::string input;
    while (true) {
        std::cout << "> ";// Prompt the user for input
        std::getline(std::cin, input); // Read a line of text from the user's input

        // Requirement: The client needs to recognize and handle commands
        // (CREATE_ROOM, JOIN_ROOM, LIST_ROOMS, EXIT) entered by the user.
        // Currently, it treats all input as a message to be sent.

        if (input == "exit") break; // Exit the sending loop if the user types "exit"

        send(sock, input.c_str(), input.size(), 0);// Send the user's input to the server
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}