#pragma once

#include "server_ws.h"
#include <map>
#include <vector>
#include <mutex>

class WebSocketInput {

public:

    enum PlayerState {
        NOTHING = 0,
        UP,
        DOWN
    };

    WebSocketInput(int maxPlayers);
    ~WebSocketInput();

	std::vector<char> getPlayers() {
        std::lock_guard<std::mutex> guard(lock);
		std::vector<char> ret;
		for (auto p : players) {
			ret.push_back(p.second);
		}
		return ret;
	}

    std::vector<char> connectedPlayers() {
		std::lock_guard<std::mutex> guard(lock);
        auto ret = connected;
        connected.clear();
        return ret;
    }

    std::vector<char> disconnectedPlayers() {
		std::lock_guard<std::mutex> guard(lock);
        auto ret = disconnected;
        disconnected.clear();
        return ret;
    }

    PlayerState getPlayerState(char player) {
		std::lock_guard<std::mutex> guard(lock);
        return playerState[player];
    }

private:
    char findUnusedPlayerId();

    int maxPlayers;
	std::mutex lock;
    typedef SimpleWeb::SocketServer<SimpleWeb::WS> WsServer;
    WsServer server;
    std::thread* server_thread;
    std::map<size_t, char> players;
    std::map<char, PlayerState> playerState;
    std::vector<char> connected;
    std::vector<char> disconnected;

};
