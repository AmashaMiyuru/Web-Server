#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include "./client/handleclient.h"

/*this program is a multi threaded TCP server that listens on port 8089
Spawns a new thread to handle each incoming client connection
*/

//handle_client_thread, is a Windows thread function designed to handle a single client connection
//DWORD WINAPI-this sepcifies that the function follows Windows API calling convention(WINAPI) and returns DWORD(unsigned 32 bit integer)
//LPVOID parameter - pointer to a void, used to pass data to the thread function, generic pointer to the client's socket, which will be casted to appropriate type inside the function
DWORD WINAPI handle_client_thread(LPVOID client_socket) { 
    SOCKET client_fd = *(SOCKET*)client_socket;
    handle_client(client_fd);
    closesocket(client_fd);
    free(client_socket);
    return 0;
}

int main() {
    WSADATA wsa; //WSADATA structure to hold winsock data(windows socket library data)
    SOCKET server_fd, *client_fd;
    struct sockaddr_in server, client; // structures for server and client address information
    int client_size = sizeof(struct sockaddr_in);

    WSAStartup(MAKEWORD(2,2), &wsa); // initialize the winsock library
    server_fd = socket(AF_INET, SOCK_STREAM, 0);  // create a TCP socket

    server.sin_family = AF_INET; //AF_INET specifies the address family(ipv4)
    server.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY accept connections on any network interface
    server.sin_port = htons(8089);

    bind(server_fd, (struct sockaddr*)&server, sizeof(server)); // associate with the 8089 port
    listen(server_fd, 5);
    
    printf("Server running on port 8089...\n");

    while (1) {
        client_fd = malloc(sizeof(SOCKET));
        *client_fd = accept(server_fd, (struct sockaddr*)&client, &client_size);
        if (*client_fd == INVALID_SOCKET) {
            free(client_fd);
            continue;
        }
        // create a thread for each client
        HANDLE thread = CreateThread(NULL, 0, handle_client_thread, client_fd, 0, NULL);
        CloseHandle(thread); // Free thread resource
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
