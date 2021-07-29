#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h> // for getnameinfo()

// Usual socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "networking/httpRequest.h"
#include "collections/dictionary.h"

#define CHUNK_SIZE 512
#define BACKLOG 10 // Passed to listen()
#define PORT 8003

#define RESPONSE_200 "HTTP/1.1 200 OK\r\n\n"

void report(struct sockaddr_in *serverAddress);
int receiveBasic(int socket, char *receivedData);

char *copyString(char *src)
{
    char *copied = (char *)calloc(strlen(src), sizeof(char));
    strcat(copied, src);
    return copied;
}

char *copyAndExpand(char *src, char *expand)
{
    char *copied = (char *)calloc(strlen(src) + strlen(expand), sizeof(char));
    strncpy(copied, src, strlen(src));
    return copied;
}

char *copyAndAppend(char *src, char *append)
{
    char *result = copyAndExpand(src, append);
    strcat(result, append);
    return result;
}

// TODO
void setHttpHeader(char *httpHeader, char *pathResources)
{
    // File object to return
    char *abobus = "/index.html";
    char *fileLocation = copyString(pathResources);
    strcat(fileLocation, abobus);

    FILE *htmlData = fopen(fileLocation, "r");

    char line[100];
    char responseData[8000];
    while (fgets(line, 100, htmlData) != 0)
    {
        strcat(responseData, line);
    }
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    strcat(httpHeader, responseData);
    fclose(htmlData);
}

char *getPublicPath(char *execPath)
{
    char separator = '/';
    char *path = "../../public";
    char *copied = copyAndExpand(execPath, path);

    for (int i = strlen(copied); i >= 0; i--)
    {
        if (copied[i] == separator)
            break;

        copied[i] = '\0';
    }

    strcat(copied, path);

    return copied;
}

int main(int argc, char *argv[])
{
    char *input = "POST /abobus HTTP/1.1\r\nAuthorization: Bearer token_\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.26.8\r\nAccept: */*\r\nPostman-Token: c4daa1d9-1bbc-4354-82ea-d888d6f971f0\r\nHost: 127.0.0.1:8003\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 27\r\n\r\n{\n    \"nickname\": \"admin\"\n}"; 
    
    struct httpRequest test = new_httpRequest(input);

    for (int i = 0; i < test.headers.count; i++)
    {
        printf("[%s] : [%s]\n", test.headers.items[i].key, test.headers.items[i].value);
    }
    
    char *pathResources = getPublicPath(argv[0]);

    char httpHeader[8000] = RESPONSE_200;

    // Socket setup: creates an endpoint for communication, returns a descriptor
    int serverSocket = socket(
        AF_INET,     // Domain: specifies protocol family
        SOCK_STREAM, // Type: specifies communication semantics
        0            // Protocol: 0 because there is a single protocol for the specified family
    );

    // Construct local address structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //inet_addr("127.0.0.1");

    // Bind socket to local address
    // bind() assigns the address specified by serverAddress to the socket
    // referred to by the file descriptor serverSocket.
    bind(
        serverSocket,                      // file descriptor referring to a socket
        (struct sockaddr *)&serverAddress, // Address to be assigned to the socket
        sizeof(serverAddress)              // Size (bytes) of the address structure
    );

    // Mark socket to listen for incoming connections
    int listening = listen(serverSocket, BACKLOG);
    if (listening < 0)
    {
        printf("Error: The server is not listening.\n");
        return 1;
    }

    report(&serverAddress); // Custom report function

    setHttpHeader(httpHeader, pathResources); // Custom function to set header

    int clientSocket;
    char recvChar;

    // TODO: think about how to allocate memory more expediently
    char data[65536]; // 64K

    // Wait for a connection, create a connected socket if a connection is pending
    while (1)
    {
        clientSocket = accept(serverSocket, NULL, NULL);
        send(clientSocket, httpHeader, sizeof(httpHeader), 0);
        receiveBasic(clientSocket, data);

        printf("%s", data);

        close(clientSocket);
    }

    return 0;
}

int receiveBasic(int socket, char *receivedData)
{
    int receivedSize = 0;
    int totalSize = 0;
    int chunkOffset = 0;

    char chunk[CHUNK_SIZE];

    // Data is received by chunks, for this you need to use an infinite loop
    while (1)
    {
        // Clearing the buffer at the beginning of each iteration
        memset(chunk, 0, CHUNK_SIZE);

        // The function returns the number of bytes received
        // or -1 in case of error
        receivedSize = recv(socket, chunk, CHUNK_SIZE, 0);
        if (receivedSize <= 0)
            return totalSize;

        // Merging all chunks
        memmove(receivedData + chunkOffset, chunk, CHUNK_SIZE);
        chunkOffset += CHUNK_SIZE;

        totalSize += receivedSize;

        // If the last byte of a chunk contains 0, then this is the last chunk
        // and the loop should be stopped
        if (totalSize > 0 && chunk[CHUNK_SIZE - 1] == 0)
            return totalSize;
    }
}

void report(struct sockaddr_in *serverAddress)
{
    char hostBuffer[INET6_ADDRSTRLEN];
    char serviceBuffer[NI_MAXSERV]; // defined in `<netdb.h>`
    socklen_t addr_len = sizeof(*serverAddress);

    int err = getnameinfo(
        (struct sockaddr *)serverAddress,
        addr_len,
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        NI_NUMERICHOST);

    if (err != 0)
        printf("It's not working!\n");

    printf("\n\n\tServer listening on http://%s:%s\n", hostBuffer, serviceBuffer);
}
