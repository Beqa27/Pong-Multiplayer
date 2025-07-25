// MP: This is a new file to implement the NetworkManager class.
#include "../header/multiplayer.h"

NetworkManager::NetworkManager() {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
    }
    socketSet = SDLNet_AllocSocketSet(1);
    if (!socketSet) {
        std::cerr << "SDLNet_AllocSocketSet failed: " << SDLNet_GetError() << std::endl;
    }
}

NetworkManager::~NetworkManager() {
    CloseConnection();
    if (socketSet) {
        SDLNet_FreeSocketSet(socketSet);
    }
    SDLNet_Quit();
}

bool NetworkManager::Host(int port) {
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
        std::cerr << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
        return false;
    }
    serverSocket = SDLNet_TCP_Open(&ip);
    if (!serverSocket) {
        std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
        return false;
    }
    isServer = true;
    std::cout << "Server hosted on port " << port << ". Waiting for client..." << std::endl;
    return true;
}

bool NetworkManager::CheckForClient() {
    if (!isServer || clientSocket) return false;

    clientSocket = SDLNet_TCP_Accept(serverSocket);
    if (clientSocket) {
        std::cout << "Client connected!" << std::endl;
        SDLNet_TCP_AddSocket(socketSet, clientSocket);
        return true;
    }
    return false;
}

bool NetworkManager::Connect(const char* host_ip, int port) {
    if (SDLNet_ResolveHost(&ip, host_ip, port) == -1) {
        std::cerr << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
        return false;
    }
    clientSocket = SDLNet_TCP_Open(&ip);
    if (!clientSocket) {
        std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
        return false;
    }
    std::cout << "Connected to server!" << std::endl;
    SDLNet_TCP_AddSocket(socketSet, clientSocket);
    isServer = false;
    return true;
}

void NetworkManager::CloseConnection() {
    if (clientSocket) {
        SDLNet_TCP_DelSocket(socketSet, clientSocket);
        SDLNet_TCP_Close(clientSocket);
        clientSocket = nullptr;
    }
    if (serverSocket) {
        SDLNet_TCP_Close(serverSocket);
        serverSocket = nullptr;
    }
}

bool NetworkManager::SendData(const void* data, int size) {
    if (!clientSocket) return false;
    int bytesSent = SDLNet_TCP_Send(clientSocket, data, size);
    if (bytesSent < size) {
        std::cerr << "SDLNet_TCP_Send failed: " << SDLNet_GetError() << std::endl;
        CloseConnection();
        return false;
    }
    return true;
}

bool NetworkManager::ReceiveData(void* data, int size) {
    if (!clientSocket) return false;
    // Check if there is data on the socket to be read without blocking
    if (SDLNet_CheckSockets(socketSet, 0) > 0 && SDLNet_SocketReady(clientSocket)) {
        int bytesReceived = SDLNet_TCP_Recv(clientSocket, data, size);
        if (bytesReceived <= 0) {
            std::cerr << "SDLNet_TCP_Recv failed or connection closed by peer." << std::endl;
            CloseConnection();
            return false;
        }
        return bytesReceived == size;
    }
    return false; // No data available right now
}

bool NetworkManager::IsConnected() const {
    return clientSocket != nullptr;
}