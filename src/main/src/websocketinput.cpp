#include "websocketinput.h"
#include "assets.h"

using namespace std;


WebSocketInput::WebSocketInput() {
    server.config.port=8080;

    //  Test with the following JavaScript:
    //    var ws=new WebSocket("ws://localhost:8080/");
    //    ws.onmessage=function(evt){console.log(evt.data);};
    //    ws.send("test");
    auto& echo=server.endpoint["^/?$"];

    echo.on_message=[this](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::Message> message) {
        auto message_str=message->string();
        std::lock_guard<std::mutex> guard(lock);
        char player = players[(size_t)connection.get()];
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

        std::lock_guard<std::mutex> guard(lock);

        char player = current_player;
        players[(size_t)connection.get()] = player;
        current_player++;

        connected.push_back(player);

        cout << "Server: Sending player id: " << player << endl;

        auto send_stream=make_shared<WsServer::SendStream>();
        *send_stream << ("<img class='player' style='width:180px; height:180px;' src='/"+facePaths[faceIndex(player)]+"'>");
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
        char player = players[(size_t)connection.get()];
        players.erase((size_t)connection.get());
        disconnected.push_back(player);
    };

    //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.on_error=[this](shared_ptr<WsServer::Connection> connection, const boost::system::error_code& ec) {
        cout << "Server: Error in connection " << (size_t)connection.get() << ". " <<
                "Error: " << ec << ", error message: " << ec.message() << endl;
        std::lock_guard<std::mutex> guard(lock);
        char player = players[(size_t)connection.get()];
        players.erase((size_t)connection.get());
        disconnected.push_back(player);
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

