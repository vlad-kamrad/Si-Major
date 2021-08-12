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

#include <pthread.h>

#include "networking/httpRequest.h"
#include "networking/httpResponse.h"
#include "collections/dictionary.h"
#include "utils/utils.h"
#include "file/file.h"
#include "file/endpoint.h"
#include "file/fileObject.h"

#define RECV_DATA_BUFFER_SIZE 65536
#define CHUNK_SIZE 512
#define BACKLOG 50 // Passed to listen()
#define PORT 8003
#define MAX_ENDPOINTS 50

void *reqCallback(void *argument);
void report(struct sockaddr_in *serverAddress);
int receiveBasic(int socket, char *receivedData);

typedef struct parameter
{
    int socket;
    struct EndpointObject *endpoints;
    int endpointsCount;

} parameter;

char *copyString(char *src)
{
    char *copied = (char *)calloc(strlen(src), sizeof(char));
    //strcat(copied, src);
    memmove(copied, src, strlen(src));
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

char *getRoot(char *source)
{
    char *cp = copyString(source);
    int size = strstr(cp, "/server") - cp;
    char *root = (char *)calloc(size, sizeof(char));
    memmove(root, cp, size);
    free(cp);
    return root;
}

char *getEndpointsPath(char *root)
{
    char *endpoints = "/endpoints";
    return copyAndAppend(root, endpoints);
}

int main(int argc, char *argv[])
{
    char *rootRoot = getRoot(argv[0]);
    char *endpointsPath = getEndpointsPath(rootRoot);
    char *pathResources = getPublicPath(argv[0]);

    // INIT FILES ENDPOINTS
    char *sep = "\n";
    int isSuccess = 0;
    char *endpointsData = readFile(endpointsPath, getFileSize(endpointsPath), &isSuccess);
    char *block = strtok(endpointsData, sep);

    if (!isSuccess)
    {
        // TODO: Write more
        printf("Error when reading file from endpoints.\n");
        return 1;
    }

    struct EndpointObject _endpoints[MAX_ENDPOINTS];
    int _endpointCount = 0;

    while (block != NULL)
    {
        char *location = copyAndAppend(pathResources, strtok(NULL, sep));
        int isDynRead = str2int(strtok(NULL, sep));
        _endpoints[_endpointCount++] = new_EndpointObject(block, location, isDynRead);
        block = strtok(NULL, sep);
    }

    printf("Endpoints file successfully readed\n");

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

    // TODO: think about how to allocate memory more expediently
    char receiveDataBuffer[RECV_DATA_BUFFER_SIZE];
    int clientSocket;

    // Wait for a connection, create a connected socket if a connection is pending
    while (1)
    {
        clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket < 0)
        {
            printf("client socket < 0\n");
            continue;
        }

        pthread_t helper;
        parameter *p = malloc(sizeof(parameter));
        p->socket = clientSocket;
        p->endpoints = _endpoints;
        p->endpointsCount = _endpointCount;

        pthread_create(&helper, NULL, reqCallback, p);
    }

    return 0;
}

struct EndpointObject *getRoute(char *uri, struct EndpointObject *endpoints, int endpointCount)
{
    for (int i = 0; i <= endpointCount; i++)
    {
        if (!strcmp(endpoints[i].endpoint, uri))
        {
            return &endpoints[i];
        }
    }

    return NULL;
}

int getRouteIndex(char *uri, struct EndpointObject *endpoints, int endpointCount)
{
    for (int i = 0; i < endpointCount; i++)
        if (!strcmp(endpoints[i].endpoint, uri))
            return i;

    return -1;
}

void *reqCallback(void *argument)
{
    char receiveDataBuffer[RECV_DATA_BUFFER_SIZE];

    parameter *p = (parameter *)argument;
    int clientSocket = p->socket;
    int endpointCount = p->endpointsCount;
    struct EndpointObject *endpoints = p->endpoints;

    int totalSize = receiveBasic(clientSocket, receiveDataBuffer);

    if (totalSize <= 0)
    {
        printf("pthread ABOBA\n");
        close(clientSocket);
        pthread_exit(0);
    }

    struct httpRequest req = new_httpRequest(receiveDataBuffer);

    printf("[ %d ]\t%s\t%s\n", clientSocket, getHttpMethodByEnum(req.method), req.uri);

    // memset(receiveDataBuffer, 0, RECV_DATA_BUFFER_SIZE);

    int routeIndex = getRouteIndex(req.uri, endpoints, endpointCount);
    // struct EndpointObject *end = getRoute(req.uri, endpoints, endpointCount);
    int isNF = 0;
    if (routeIndex < 0)
    {
        // TODO: Use Not Found Start line
        printf("[ Not Found]\n");
        routeIndex = getRouteIndex("/nfp\0", endpoints, endpointCount);
        isNF = 1;
    }

    if (routeIndex < 0)
    {
        printf("[ ERROR ]\n");
    }

    struct httpResponse resp = new_httpResponse(&endpoints[routeIndex].file);
    printf("[[ is Not Found = %d ]]\n", isNF);
    if (isNF)
    {
        resp.statusLine = RESPONSE_404;
    }

    char *output = build_httpResponse(&resp);

    // send(clientSocket, output, strlen(output), 0);
    write(clientSocket, output, sizeof(char[strlen(output)]));
    write(clientSocket, resp.body, resp.bodySize);

    // TODO: free dict resp
    // free(resp.body);
    // free(output);
    // free(req.headers.items);

    close(clientSocket);
    pthread_exit(NULL);
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
