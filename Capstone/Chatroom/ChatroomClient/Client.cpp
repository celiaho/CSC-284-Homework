/*
https://github.com/pradeepselvakumar/CSC-284/blob/main/CapstoneProject/README.md

Part 1: Chatroom Client
Requirements:

    The client must accept IP address and port number as command-line arguments.
    After connecting to the server, the client must:
        Allow the user to input chat commands or messages.
        Support the same commands defined in the server (CREATE_ROOM, JOIN_ROOM, etc.).
        Display messages received from the server in real time.
    The client should also allow users to type and send freeform messages to others in the room.

You may use the sample ChatRoomServer.exe to program against while developing your client.
*/

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

// Function to receive messages in a separate thread
void receiveMessages(SOCKET sock) {
    char buffer[1024];
    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << "\nServer: " << std::string(buffer, bytesReceived) << "\n> ";
        }
        else if (bytesReceived == 0) {
            std::cout << "Server disconnected.\n";
            break;
        }
        else {
            std::cerr << "recv failed.\n";
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

    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(54000);

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
    receiver.detach();

    // Send loop
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        send(sock, input.c_str(), input.size(), 0);
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}