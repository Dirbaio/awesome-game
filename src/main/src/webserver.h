#pragma once

#include <string>
#include <thread>
#include <fstream>
#include <boost/filesystem.hpp>
#include <memory>

#include "server_http.hpp"

class WebServer {

public:
	WebServer(const std::string& file);
	~WebServer();

private:

	typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
	HttpServer server;
	std::thread* server_thread;

	void default_resource_send(const HttpServer &server, const std::shared_ptr<HttpServer::Response> &response, const std::shared_ptr<std::ifstream> &ifs);

};
