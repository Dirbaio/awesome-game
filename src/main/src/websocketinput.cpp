#include "websocketinput.h"
#include "assets.h"

using namespace std;

char WebSocketInput::findUnusedPlayerId() {
    std::lock_guard<std::mutex> guard(lock);
    vector<char> unused;
    for (char c = 'A'; c < 'A'+maxPlayers; c++) {

        bool found = false;
        for (auto p : players) {
            if (p.second == c) {
                found = true;
                break;
            }
        }
        if (!found) {
            unused.push_back(c);
        }
    }
    if (unused.empty()) return 0;

    int randIndex = rand() % unused.size();
    return unused[randIndex];
}


WebSocketInput::WebSocketInput(int _maxPlayers)
    : maxPlayers(_maxPlayers)
{
    server.config.port=8080;

    //  Test with the following JavaScript:
    //    var ws=new WebSocket("ws://localhost:8080/");
    //    ws.onmessage=function(evt){console.log(evt.data);};
    //    ws.send("test");
    auto& echo=server.endpoint["^/?$"];

    echo.on_message=[this](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::Message> message) {

        std::lock_guard<std::mutex> guard(lock);

        auto it = players.find((size_t)connection.get());
        if (it == players.end()) return;

        char player = it->second;

        auto message_str=message->string();
        cout << "Server: Message received: \"" << message_str << "\" from player: " << player << endl;
        if (message_str == "up") {
            playerState[player] = PlayerState::UP;
        } else if (message_str == "down") {
            playerState[player] = PlayerState::DOWN;
        } else if (message_str == "nup") {
            if (playerState[player] == PlayerState::UP) {
                playerState[player] = PlayerState::NOTHING;
            }
        } else if (message_str == "ndown") {
            if (playerState[player] == PlayerState::DOWN) {
                playerState[player] = PlayerState::NOTHING;
            }
        }
    };

    echo.on_open=[this](shared_ptr<WsServer::Connection> connection) {
        cout << "Server: Opened connection " << (size_t)connection.get() << endl;

        auto send_stream=make_shared<WsServer::SendStream>();

        char player = findUnusedPlayerId();
        if (!player) {
            *send_stream << ("Player limit reached :(");
        } else {
            std::lock_guard<std::mutex> guard(lock);
            players[(size_t)connection.get()] = player;
            connected.push_back(player);
            cout << "Server: Sending player id: " << player << endl;
            *send_stream << ("<img class='player' style='width:180px; height:180px;' src='/"+facePaths[faceIndex(player)]+"'>");
        }

        //server.send is an asynchronous function
        server.send(connection, send_stream, [](const boost::system::error_code& ec){
            if(ec) {
                //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                cout << "Server: Error sending message. " << "Error: " << ec << ", error message: " << ec.message() << endl;
            }
        });

    };

    //See RFC 6455 7.4.1. for status codes
    echo.on_close=[this](shared_ptr<WsServer::Connection> connection, int status, const string& /*reason*/) {
        cout << "Server: Closed connection " << (size_t)connection.get() << " with status code " << status << endl;
        std::lock_guard<std::mutex> guard(lock);
        auto it = players.find((size_t)connection.get());
        if (it != players.end()) {
            char player = it->second;
            players.erase((size_t)connection.get());
            disconnected.push_back(player);
        }
    };

    //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.on_error=[this](shared_ptr<WsServer::Connection> connection, const boost::system::error_code& ec) {
        cout << "Server: Error in connection " << (size_t)connection.get() << ". " <<
                "Error: " << ec << ", error message: " << ec.message() << endl;
        std::lock_guard<std::mutex> guard(lock);
        auto it = players.find((size_t)connection.get());
        if (it != players.end()) {
            char player = it->second;
            players.erase((size_t)connection.get());
            disconnected.push_back(player);
        }
    };

    server_thread = new thread([this](){
        server.start();
    });
}


WebSocketInput::~WebSocketInput() {
    server.stop();
    server_thread->join();
    delete server_thread;
}

