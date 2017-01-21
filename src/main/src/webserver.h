#pragma once

#include <string>
#include <thread>
#include <fstream>
#include <boost/filesystem.hpp>
#include <memory>
#include <map>

#include "server_http.hpp"

class WebServer {

public:
    WebServer(std::map<std::string, std::string> pathsToFiles);
	~WebServer();

private:

	typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
	HttpServer server;
	std::thread* server_thread;

	void default_resource_send(const HttpServer &server, const std::shared_ptr<HttpServer::Response> &response, const std::shared_ptr<std::ifstream> &ifs);

};
