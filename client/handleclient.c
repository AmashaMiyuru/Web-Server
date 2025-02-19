#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "handleclient.h"
#include "../mime_utils/mime_utils.h"
#include "../server_config.h"

void handle_client(SOCKET client_fd) { // this fuction handle http requests from client
    char buffer[BUFFER_SIZE], method[10], path[512], protocol[10];
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytes_received <= 0) {
        closesocket(client_fd);
        return;
    }

    buffer[bytes_received] = '\0'; //add null terminator to mark the end of the string

    sscanf(buffer, "%s %s %s", method, path, protocol);// extract http method, path, protocol we use sscanf, it read from a string
    printf("Request: %s %s\n", method, path);


    char file_path[1024];
    const char *relative_path = (path[0] == '/') ? path + 1 : path;
    snprintf(file_path, sizeof(file_path), "%s/%s", WWW_ROOT, *relative_path ? relative_path : "index.html");


    FILE *file = fopen(file_path, "rb"); //open file in binary mode
    if (file) {
        char response_header[BUFFER_SIZE];
        fseek(file, 0, SEEK_END);  
        long file_size = ftell(file);
        rewind(file); //reset file pointer

        snprintf(response_header, sizeof(response_header), //snprintf function use to format a string and store it safetly 
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n\r\n",
                 get_mime_type(file_path), file_size);
        send(client_fd, response_header, strlen(response_header), 0);

        char file_buffer[BUFFER_SIZE];
        size_t bytes;
        while ((bytes = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0) {
            send(client_fd, file_buffer, bytes, 0);
        }
        fclose(file);
    } else {
        // File not found, serve 404.html
        char not_found_path[1024];
        snprintf(not_found_path, sizeof(not_found_path), "%s/404.html", WWW_ROOT);
    
        FILE *not_found_file = fopen(not_found_path, "rb");
        if (not_found_file) {
            fseek(not_found_file, 0, SEEK_END);
            long file_size = ftell(not_found_file);
            rewind(not_found_file);
    
            char response_header[BUFFER_SIZE];
            snprintf(response_header, sizeof(response_header),
                     "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %ld\r\n"
                     "Connection: close\r\n\r\n",
                     file_size);
            send(client_fd, response_header, strlen(response_header), 0);
    
            char file_buffer[BUFFER_SIZE];
            size_t bytes;
            while ((bytes = fread(file_buffer, 1, BUFFER_SIZE, not_found_file)) > 0) {
                send(client_fd, file_buffer, bytes, 0);
            }
            fclose(not_found_file);
        } else {
            // If 404.html is missing, send a simple 404 Not found as a response
            char *fallback_not_found = "HTTP/1.1 404 Not Found\r\n"
                                       "Content-Type: text/html\r\n"
                                       "Connection: close\r\n\r\n"
                                       "<h1>404 Not Found</h1>";
            send(client_fd, fallback_not_found, strlen(fallback_not_found), 0);
        }

    closesocket(client_fd);
}
}
