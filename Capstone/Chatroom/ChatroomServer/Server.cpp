/*
https://github.com/pradeepselvakumar/CSC-284/blob/main/CapstoneProject/README.md

Part 2: Chatroom Server
Requirements:

    The server must accept a port number as input at startup.
    The server must:
        Handle multiple clients concurrently using threads.
        Maintain a list of available chatrooms.
        Support the following commands sent by clients:
            CREATE_ROOM <room_name>: Create a new room.
            JOIN_ROOM <room_name>: Join an existing room.
            LIST_ROOMS: List all available chatrooms.
            EXIT: Disconnect from the chatroom.
    The server should:
        Maintain room membership.
        Only deliver messages to users within the same room.
        Notify users when others join or leave the room.

Use separate classes to represent Users, Rooms,and the ChatServer itself, and add appropriate functionality to each.

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

