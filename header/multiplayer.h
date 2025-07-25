#pragma once
#include <SDL_net.h>
#include <iostream>
#include <string>

// MP: Data packet for game state synchronization
struct GameStatePacket {
    float ballX, ballY;
    int paddleLeftY, paddleRightY;
    int scoreLeft, scoreRight;
};

// MP: Data packet for client input
struct InputPacket {
    int paddleY;
};

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // MP: Server function to start listening
    bool Host(int port);
    // MP: Server function to check for incoming connections (non-blocking)
    bool CheckForClient();

    // MP: Client function to connect to a server
    bool Connect(const char* ip, int port);
    void CloseConnection();

    // MP: Generic send/receive functions
    bool SendData(const void* data, int size);
    bool ReceiveData(void* data, int size);

    bool IsConnected() const;
    bool IsServer() const { return isServer; }

private:
    IPaddress ip;
    TCPsocket serverSocket = nullptr;
    TCPsocket clientSocket = nullptr;
    SDLNet_SocketSet socketSet = nullptr; // MP: To check for incoming data without blocking
    bool isServer = false;
};