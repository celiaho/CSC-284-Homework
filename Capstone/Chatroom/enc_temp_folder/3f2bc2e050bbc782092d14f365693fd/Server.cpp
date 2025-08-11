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
#include <mutex>
#include <algorithm>   // for std::transform
#include <cctype>      // for ::toupper
#include <atomic>      // for anonymousCounter
#pragma comment(lib, "ws2_32.lib")

// ------------------ Function Prototypes ------------------
/**
 * @brief Removes a user from their current room.
 * - Records lastRoom for messaging.
 * - Broadcasts a departure announcement to the room.
 * - Does not automatically add the user to a new room.
 *
 * @param clientSocket The socket of the user being removed.
 */
void removeUserFromRoom(SOCKET clientSocket, bool sendConfirmation = true);

/**
 * @brief Adds a user to the target room.
 * - Updates currentRoom.
 * - Broadcasts a join announcement to the room.
 * - If lastRoom is set, sends "You left X and joined Y" to the user.
 *
 * @param clientSocket The socket of the user being added.
 * @param targetRoom The name of the room to join.
 */
void addUserToRoom(SOCKET clientSocket, const std::string& targetRoom);

//// Rooms,Users, and the ChatServer itself, and add appropriate functionality to each.
// ------------------ Room Class ------------------
class Room {
public:
    std::string roomName;
    std::vector<SOCKET> users;          // Vector to store the sockets of users in the room

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
                int sent = send(userSocket, msg.c_str(), static_cast<int>(msg.size()), 0);
                if (sent == SOCKET_ERROR) {
                    // Optional: remove dead sockets if needed
                }
            }
        }
    }

    // Get the number of users in the room
    int getUserCount() const { return users.size(); }

    // Get the room name
    std::string getRoomName() const { return roomName; }
};

// ------------------ User Class ------------------
class User {
public:
    std::string username;
    SOCKET socket;
    std::string currentRoom;
    std::string lastRoom;

    // Set users' default room to Lobby
    User() : username("Anonymous"), socket(INVALID_SOCKET), currentRoom("Lobby") {}
    User(const std::string& name, SOCKET sock)
        : username(name), socket(sock), currentRoom("Lobby") {
    }
};

// ------------------ Globals ------------------
std::map<std::string, Room> rooms;      // Map to store rooms: roomName -> Room
std::map<SOCKET, User> userMap;         // Map to store user objects, key is the socket: socket -> User
std::mutex roomsMutex;                  // protect shared structures

std::atomic<int> anonymousCounter{1};   // Thread-safe counter for generating anonymous usernames

// ------------------ Username Assignment  ------------------
/**
 * @brief Assign a unique username, ensuring no duplicates (case-insensitive).
 * If requestedName is empty, assigns "Anonymous<N>" where N increments.
 * If the requested name is taken (case-insensitive), appends "_<N>" until unique.
 *
 * @param requestedName The name requested by the client (may be empty).
 * @return A unique username string.
 */
std::string assignUniqueUsername(const std::string& requestedUsername) {
    std::lock_guard<std::mutex> lock(roomsMutex);

    // Base username for unique username check: Use client's requested username or "Anonymous" if client did not enter a username
    std::string baseUsername = requestedUsername.empty() ? "Anonymous" : requestedUsername;
    std::string finalUsername = baseUsername;

    // If no name provided, assign Anonymous<N> immediately
       if (requestedUsername.empty()) {
        finalUsername += std::to_string(anonymousCounter++);
    }

    // Lambda to check if a name is already taken (case-insensitive)
    auto isUsernameTaken = [&](const std::string& requestedUsernameCandidate) {    // requestedUsername with appended numbers if necessary
        for (const auto& pair : userMap) {
            // Compare existing username with the candidate username, ignoring case.
            if (_stricmp(pair.second.username.c_str(), requestedUsernameCandidate.c_str()) == 0) {
                return true;    // Found a match (ignoring case)
            }
        }
        return false;   // No match found
        };

    // Append suffix until the name is unique
    int suffix = 1;
    while (isUsernameTaken(finalUsername)) {
        finalUsername = baseUsername + "_" + std::to_string(suffix++);
    }

    return finalUsername;
}

// ------------------ Utility Helper Functions ------------------
/**
 * @brief Removes trailing carriage return and newline characters from a string.
 * Typically used on incoming messages to strip any CR/LF characters from network
 * transmissions--ensures cleaner parsing.
 *
 * @param string The string to trim.
 * @return The trimmed string with no trailing \r or \n characters.
 */
static inline std::string trimCRLF(std::string string) {
    while (!string.empty() && (string.back() == '\r' || string.back() == '\n')) string.pop_back();
    return string;
}

// ------------------ Room-Change Helpers ------------------
/**
 * @brief Removes a user from their current room.
 * Saves the user's current room to lastRoom, removes them from that room's
 * user list, and broadcasts a departure announcement to the other users in
 * the room. Optionally sends a departure confirmation message to the user.
 *
 * @param clientSocket The socket of the user to remove.
 * @param sendConfirmation If true, sends a "You left X." message to the user.
 */
void removeUserFromRoom(SOCKET clientSocket, bool sendConfirmation) {
    std::lock_guard<std::mutex> lock(roomsMutex);

    User& user = userMap[clientSocket];
    const std::string& lastRoom = user.currentRoom;

    // Save the name of the room we're leaving
    user.lastRoom = lastRoom;
    Room& roomToLeave = rooms[user.currentRoom];

    // Remove user from that room
    roomToLeave.removeUser(clientSocket);

    // Room announcement: Broadcast user departure to room
    std::string departureAnnouncement = user.username + " has left the room.\n";
    roomToLeave.broadcastMessage(departureAnnouncement, clientSocket);

    // Optional user confirmation: Confirm departure with user
    if (sendConfirmation) {
        std::string departureConfirmation = "You left " + lastRoom + ".\n";
        send(clientSocket, departureConfirmation.c_str(), static_cast<int>(departureConfirmation.size()), 0);
    }
}

/**
 * @brief Adds a user to a target room.
 * Updates the user's currentRoom field, adds them to the room's user list,
 * and broadcasts a join announcement to the room. Also sends a confirmation
 * message directly to the user.
 *
 * @param clientSocket The socket of the user to add.
 * @param roomName The name of the room to join.
 */
void addUserToRoom(SOCKET clientSocket, const std::string& roomName) {
    std::lock_guard<std::mutex> lock(roomsMutex);

    User& user = userMap[clientSocket];

    // Ensure the room exists and is properly named before adding the user
    if (!rooms.count(roomName)) {
        rooms[roomName] = Room(roomName);
    }
    Room& roomToJoin = rooms[roomName];

    // Add the user and update currentRoom
    roomToJoin.addUser(clientSocket);
    user.currentRoom = roomName;

    // Room announcement: Broadcast user join to room
    std::string joinAnnouncement = user.username + " has joined the room.\n";
    roomToJoin.broadcastMessage(joinAnnouncement, clientSocket);

    // User confirmation: Confirm join with user
    std::string joinConfirmation = "You joined " + user.currentRoom + ".\n";
    send(clientSocket, joinConfirmation.c_str(), static_cast<int>(joinConfirmation.size()), 0);

    // Clear lastRoom now that the join is complete
    user.lastRoom.clear();
}

// ------------------ Command Functions ------------------
/**
 * @brief Sends the list of all available chatrooms to the requesting client.
 * Iterates through the global rooms map and builds a list showing each room's
 * name and the number of users currently in it. Sends the result to the
 * specified client socket.
 *
 * @param clientSocket The socket of the client requesting the room list.
 */
void listRooms(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    std::string roomList = "Available rooms:\n";
    for (auto& pair : rooms) {
        roomList += pair.first + " (" + std::to_string(pair.second.getUserCount()) + " users)\n";
    }
    send(clientSocket, roomList.c_str(), static_cast<int>(roomList.size()), 0);
}

/**
 * @brief Joins an existing room. If the room doesn't exist, notifies the user.
 * @param clientSocket The socket of the user issuing /JOIN_ROOM.
 * @param targetRoom   The name of the room to join.
 */
void joinRoom(SOCKET clientSocket, const std::string& roomName) {
    const std::string& currentRoom = userMap[clientSocket].currentRoom;

    // Check if given room name is empty
    if (roomName.empty()) {
        std::string emptyRoomNameMsg = "You must specify the room you want to join. The command syntax is: \"/JOIN_ROOM <room_name>\", e.g. \"/JOIN_ROOM New Room FTW\" (Do not include parentheses or arrow brackets). Try again.\n";
        send(clientSocket, emptyRoomNameMsg.c_str(), static_cast<int>(emptyRoomNameMsg.size()), 0);
        return;
    }
    // Check if already in room
    if (currentRoom == roomName) {
        std::string alreadyMsg = "You are already in " + roomName + ".\n";
        send(clientSocket, alreadyMsg.c_str(), static_cast<int>(alreadyMsg.size()), 0);
        return;
    }
    // Check that room exists before joining
    if (!rooms.count(roomName)) {
        std::string notFoundMsg = "\"" + roomName + "\" is not an existing room. To create and join it, type \"/CREATE_ROOM " + roomName + ".\n";
        send(clientSocket, notFoundMsg.c_str(), static_cast<int>(notFoundMsg.size()), 0);
        return;
    }

    // Move the user
    removeUserFromRoom(clientSocket, true);
    addUserToRoom(clientSocket, roomName);
}

/**
 * @brief Moves the user from their current room into the lobby.
 * - If the user is already in the lobby, sends a confirmation saying so.
 * - Otherwise, removes them from the current room and adds them to the lobby.
 *
 * @param clientSocket The socket of the user issuing /LEAVE_ROOM.
 */
void leaveRoom(SOCKET clientSocket) {
    const std::string& currentRoom = userMap[clientSocket].currentRoom;   // Get current room 

    // If already in Lobby, just confirm to the user
    if (currentRoom == "Lobby") {
        std::string alreadyInLobbyNotice = "You are already in the lobby. If you want to leave the lobby and exit this program, type \"\/EXIT\".\n";
        send(clientSocket, alreadyInLobbyNotice.c_str(), static_cast<int>(alreadyInLobbyNotice.size()), 0);
        return;
    }

    // Ensure Lobby exists
    if (!rooms.count("Lobby")) {
        rooms["Lobby"] = Room("Lobby");
    }

    // Move user to Lobby
    removeUserFromRoom(clientSocket, true);
    addUserToRoom(clientSocket, "Lobby");
}

/**
 * @brief Creates a new room and moves the user into it.
 * @param clientSocket The socket of the user issuing /CREATE_ROOM.
 * @param newRoomName  The name of the room to create.
 */
void createRoomAndJoin(SOCKET clientSocket, const std::string& roomName) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    // Check if given room name is empty
    if (roomName.empty()) {
        std::string emptyRoomNameMsg = "You must specify the room you want to join. The command syntax is: \"/CREATE_ROOM <room_name>\", e.g. \"/CREATE_ROOM New Room FTW\" (Do not include parentheses or arrow brackets). Try again.\n";
        send(clientSocket, emptyRoomNameMsg.c_str(), static_cast<int>(emptyRoomNameMsg.size()), 0);
        return;
    }
    // Check if room already exists
    if (rooms.count(roomName)) {
        std::string roomExistsMsg = "The room \"" + roomName + "\" already exists. Use \"/JOIN_ROOM " + roomName + "\" (without quotation marks) to join it.\n";
        send(clientSocket, roomExistsMsg.c_str(), static_cast<int>(roomExistsMsg.size()), 0);
        return;
    }

    // Create room
    rooms[roomName] = Room(roomName);

    // New room announcement: Broadcast room creation to users in Lobby
    std::string newRoomAnnouncement = "A new room was created: " + roomName + ".\n";
    rooms["Lobby"].broadcastMessage(newRoomAnnouncement, clientSocket);

    // Confirm new room creation with user
    std::string newRoomConfirmation = "You created " + roomName + ".\n";
    send(clientSocket, newRoomConfirmation.c_str(), static_cast<int>(newRoomConfirmation.size()), 0);

    // Move the user to the new room
    removeUserFromRoom(clientSocket, true);
    addUserToRoom(clientSocket, roomName);
}

// checked
void help(SOCKET clientSocket) {
    std::string helpText =
        "********** CHATTERVOX COMMANDS **********\n\n"
        "Type \"/\" followed by the command and any necessary arguments. For example, to create a new chatroom, type \"/CREATE_ROOM New Users\" (without the quotation marks).\n\n"
        "/LIST_ROOMS\t\tDisplay a list of all chatrooms on the server and the number of users in each room.\n"
        "/JOIN_ROOM <room name>\t\tMove to an existing chatroom.\n"
        "/LEAVE_ROOM\t\tLeave the chatroom you're in and move to the lobby.\n"
        "/CREATE_ROOM <room name>\tCreate a new chatroom and move to it.\n"
        "/HELP\t\t\tDisplay this command menu.\n"
        "/EXIT\t\t\tExit Chattervox.\n\n";
    send(clientSocket, helpText.c_str(), static_cast<int>(helpText.size()), 0);
}

/**
 * @brief Cleanly removes a user from the server and their current room.
 * @param clientSocket The socket of the user exiting.
 * @param sendGoodbye  If true, send a goodbye message to the user before removing them.
 */void exit(SOCKET clientSocket, bool sendGoodbye = false) {
    removeUserFromRoom(clientSocket, true); // Always send departure announcements to other users

    // Send goodbye message to user
    {
        std::lock_guard<std::mutex> lock(roomsMutex);

        if (sendGoodbye) {
            std::string exitMsg = "Goodbye, " + userMap[clientSocket].username + "—-visit again soon!\n";
            send(clientSocket, exitMsg.c_str(), static_cast<int>(exitMsg.size()), 0);
        }

        // Remove user from userMap
        userMap.erase(clientSocket);
    }
}

// ------------------ Client Handler ------------------
// Function to handle communication with a single client in a separate thread
void handleClient(SOCKET clientSocket) {
    char buffer[1024];  // Buffer to store data received from the client

    // On connection, create placeholder user entry
    {
        std::lock_guard<std::mutex> lock(roomsMutex);
        userMap[clientSocket] = User("", clientSocket);    // Use empty username temporarily
        rooms["Lobby"];     // Create Lobby if room does not exist (defensive guard)
    }

    // Username handshake: expect "USERNAME:<raw>\n"
    std::string requestedName;
    {
        char helloBuf[512];
        ZeroMemory(helloBuf, sizeof(helloBuf));
        int hb = recv(clientSocket, helloBuf, sizeof(helloBuf), 0);
        std::string helloLine(helloBuf, (hb > 0 ? hb : 0));
        // trim CRLF
        while (!helloLine.empty() && (helloLine.back() == '\r' || helloLine.back() == '\n')) helloLine.pop_back();

        const std::string tag = "USERNAME:";
        if (helloLine.rfind(tag, 0) == 0) {
            requestedName = helloLine.substr(tag.size()); // may be empty
        }
        else {
            requestedName.clear(); // treat as Anonymous if client didn't follow the protocol
        }
    }

    // Assign unique username (Anonymous<N> if none)
    std::string username = assignUniqueUsername(requestedName);
    {
        std::lock_guard<std::mutex> lock(roomsMutex);
        userMap[clientSocket].username = username;
    }

    // Confirm name to the client
    {
        std::string confirm = "USERNAME_CONFIRMED:" + username + "\n";
        send(clientSocket, confirm.c_str(), static_cast<int>(confirm.size()), 0);
    }

    // Place user into Lobby
    addUserToRoom(clientSocket, "Lobby");

    while (true) {
        ZeroMemory(buffer, sizeof(buffer)); // Clear buffer of leftover data before each receive operation
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);  // Receive data from the client socket

        if (bytesReceived > 0) {
            std::string msg(buffer, bytesReceived); // Convert received bytes to a string
            msg = trimCRLF(msg);                    // Trim trailing characters

            // Check if message is a command
            if (!msg.empty() && msg[0] == '/') {
                // Split into command and optional argument (works with or without a space)
                size_t spacePosition = msg.find(' ');
                std::string command = (spacePosition == std::string::npos) ? msg.substr(1) : msg.substr(1, spacePosition - 1);
                std::string argument = (spacePosition == std::string::npos) ? "" : msg.substr(spacePosition + 1);

                    // Capitalize command
                    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

                    // Handle commands
                    if (command == "LIST_ROOMS") {
                        listRooms(clientSocket);
                    } else if (command == "JOIN_ROOM") {
                        joinRoom(clientSocket, argument);
                    } else if (command == "LEAVE_ROOM") {
                        leaveRoom(clientSocket);
                    } else if (command == "CREATE_ROOM") {
                        createRoomAndJoin(clientSocket, argument);
                    } else if (command == "HELP") {
                        help(clientSocket);
                    } else if (command == "EXIT") {
                        exit(clientSocket, true);
                        break; // Leave loop to close socket/disconnect user
                    } else {
                        const std::string unknownCommandMsg = "Unknown command. Try \"/HELP\" (without the quotation marks).\n";
                        send(clientSocket, unknownCommandMsg.c_str(), static_cast<int>(unknownCommandMsg.size()), 0);
                    }
                } else {
                    // Regular chat message--broadcast to current room
                    std::string currentRoom, username, chatLine;
                    {
                        std::lock_guard<std::mutex> lock(roomsMutex);
                        if (!userMap.count(clientSocket)) continue;
                        currentRoom = userMap[clientSocket].currentRoom;
                        username = userMap[clientSocket].username;
                    }
                    chatLine = username + ": " + msg + "\n";
                    {
                        std::lock_guard<std::mutex> lock(roomsMutex);
                        if (rooms.count(currentRoom)) {
                            rooms[currentRoom].broadcastMessage(chatLine, clientSocket);
                        }
                    }
                }
            } else if (bytesReceived == 0) {    // Connection was closed by the client
                exit(clientSocket, false);
                break;  // Exit the receive loop for this client
            } else {
                // Notify user that error occurred while receiving data & display error code
                std::cerr << "Error occurred while receiving data: " << WSAGetLastError() << "\n";
                exit(clientSocket, false);
                break;  // Exit the receive loop for this client
            }
        }
    
    // Cleanup on disconnect
    closesocket(clientSocket);  // Close the socket associated with this client
}


// ------------------ Main ------------------
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
        std::cout << "----------WELCOME TO THE CHATTERVOX CHATROOM SERVER ----------\n"
            << "Socket for network communication created successfully.\n";
    }

    //// Get port number from command-line argument
    //if (argc != 2) {
    //    std::cerr << "Usage: chatroom_server <port_number>\n";
    //    closesocket(serverSocket);  // Close socket before cleanup
    //    WSACleanup();
    //    return 1;
    //}
    //int serverPort = atoi(argv[1]);   // Convert port number from string to int & save to "serverPort"

    // Determine server port
    int serverPort = 0;

    if (argc == 2) {
        // Use command-line argument
        serverPort = std::stoi(argv[1]);
    } else {
        // Use interactive prompt
        std::cout << "Enter server port number (e.g., 54000): ";
        std::string portStr;
        std::getline(std::cin, portStr);
        try {
            serverPort = std::stoi(portStr);
        }
        catch (...) {
            std::cerr << "Invalid port number format.\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
    }

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
    listen(serverSocket, SOMAXCONN);    // SOMAXCONN: Max length of the pending connection queue
    std::cout << "Server listening on port " << serverPort << "...\n";

    // Create the "lobby" room
    rooms["Lobby"] = Room("Lobby"); // Add to the rooms map

    // Main loop/thread to accept incoming client connections
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);    // Accept a connection
        if (clientSocket != INVALID_SOCKET) {   // If a connection is accepted successfully
            std::cout << "Client connected.\n";
            // Create a new thread to handle communication with the newly connected client
            std::thread clientThread(handleClient, clientSocket);
            clientThread.detach(); // Detach thread so it runs independently in bg
        }
        // ClientSocket is closed in handleClient thread
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    system("pause");
    return 0;
}