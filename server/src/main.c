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
    struct Endpoint *endpoints;
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

char *getFileByEndpoint(char *fileLocation, int fileSize)
{
    int isSuccess = 0;
    char *data = readFile(fileLocation, fileSize, &isSuccess);

    if (isSuccess == 0)
    {
        printf("File %s is not exist", fileLocation);
        return NULL;
    }

    // RESPONSE_200 + data
    char *res = (char *)calloc(strlen(RESPONSE_200) + fileSize, sizeof(char));

    // TODO: Change this ugly code
    for (int i = 0; i < strlen(RESPONSE_200); i++)
    {
        res[i] = RESPONSE_200[i];
    }

    for (int i = strlen(RESPONSE_200), j = 0; j < fileSize; i++, j++)
    {
        res[i] = data[j];
    }

    return res;
}

int main(int argc, char *argv[])
{
    char *rootRoot = getRoot(argv[0]);
    char *endpointsPath = getEndpointsPath(rootRoot);
    char *pathResources = getPublicPath(argv[0]);

    struct httpResponse kek = new_httpResponse();
    char *text = build_httpResponse(&kek);

    
    struct FileObject fo = new_FileObject("/Users/vladislavstupaev/Projects/Si-Major/public/index.html", 1);

    char *data = getFileObjectData(&fo);

    // INIT FILES ENDPOINTS
    struct Endpoint endpoints[MAX_ENDPOINTS];
    int endpointCount = 0;

    char *sep = "\n";
    int isSuccess = 0;
    char *endpointsData = readFile(endpointsPath, getFileSize(endpointsPath), &isSuccess);
    char *block = strtok(endpointsData, sep);

    if (!isSuccess)
    {
        // TODO: Write more
        printf("Error when reading file from endpoints.\n");
    }

    while (block != NULL)
    {
        char *filePath = copyAndAppend(pathResources, strtok(NULL, sep));
        int isDynRead = str2int(strtok(NULL, sep));
        endpoints[endpointCount++] = new_Endpoint(block, filePath, isDynRead);
        block = strtok(NULL, sep);
    }

    // END INIT FILES ENDPOINTS
    printf("Readed [enpdpoints] file\n");

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

    //setHttpHeader(httpHeader, pathResources); // Custom function to set header

    int clientSocket;

    // TODO: think about how to allocate memory more expediently
    char receiveDataBuffer[RECV_DATA_BUFFER_SIZE];

    for (int i = 0; i < endpointCount; i++)
    {
        endpoints[i].fileSize = getFileSize(endpoints[i].path);
    }

    // Mb use multi threading ?
    // https://stackoverflow.com/questions/2108961/sample-code-for-asynchronous-programming-in-c?lq=1

    // Wait for a connection, create a connected socket if a connection is pending
    while (1)
    {
        clientSocket = accept(serverSocket, NULL, NULL);

        parameter *p = malloc(sizeof(parameter));
        p->socket = clientSocket;
        p->endpoints = endpoints;
        p->endpointsCount = endpointCount;
        pthread_t helper;
        void *status;

        pthread_create(&helper, NULL, reqCallback, p);
    }

    return 0;
}

void *reqCallback(void *argument)
{
    char receiveDataBuffer[RECV_DATA_BUFFER_SIZE];

    parameter *p = (parameter *)argument;
    int clientSocket = p->socket;
    int endpointCount = p->endpointsCount;
    struct Endpoint *endpoints = p->endpoints;

    int totalSize = receiveBasic(clientSocket, receiveDataBuffer);

    if (totalSize <= 0)
    {
        printf("pthread ABOBA\n");
        close(clientSocket);
        pthread_exit(0);
    }

    struct httpRequest req = new_httpRequest(receiveDataBuffer);

    printf("[ %d ]\t%s\t%s\n", clientSocket, getHttpMethodByEnum(req.method), req.uri);

    // printf("%s", receiveDataBuffer);
    memset(receiveDataBuffer, 0, RECV_DATA_BUFFER_SIZE);

    for (int i = 0; i < endpointCount; i++)
    {
        if (!strcmp(endpoints[i].endpointStr, req.uri) && req.method == GET)
        {
            // TODO: Use cache
            char *response = getFileByEndpoint(endpoints[i].path, endpoints[i].fileSize);

            send(clientSocket, response, strlen(response), 0);
            free(response);
            free(req.headers.items);
            break;
        }
        else if (i == endpointCount - 1)
        {
            printf("[ Not Found]\n");
            send(clientSocket, RESPONSE_404, strlen(RESPONSE_404), 0);
        }
    }

    //send(clientSocket, httpHeader, sizeof(httpHeader), 0);
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
