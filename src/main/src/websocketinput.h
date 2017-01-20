#pragma once

#include "server_ws.h"
#include <map>
#include <vector>

using namespace std;

class WebSocketInput {

public:

    enum PlayerState {
        NOTHING = 0,
        UP,
        DOWN
    };

    WebSocketInput();
    ~WebSocketInput();

    std::vector<char> connectedPlayers() {
        auto ret = connected;
        connected.clear();
        return ret;
    }

    std::vector<char> disconnectedPlayers() {
        auto ret = disconnected;
        disconnected.clear();
        return ret;
    }

    PlayerState getPlayerState(char player) {
        return playerState[player];
    }

private:
    typedef SimpleWeb::SocketServer<SimpleWeb::WS> WsServer;
    WsServer server;
    thread* server_thread;
    std::map<size_t, char> players;
    std::map<char, PlayerState> playerState;
    std::vector<char> connected;
    std::vector<char> disconnected;
    char current_player = 'A';

};
