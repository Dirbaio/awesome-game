#include "webserver.h"

using namespace std;

#include "server_http.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>

using namespace std;

void WebServer::default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                           const shared_ptr<ifstream> &ifs) {
	//read and send 128 KB at a time
	static vector<char> buffer(131072); // Safe when server is running on one thread
	streamsize read_length;
	if((read_length=ifs->read(&buffer[0], buffer.size()).gcount())>0) {
		response->write(&buffer[0], read_length);
		if(read_length==static_cast<streamsize>(buffer.size())) {
			server.send(response, [this, &server, response, ifs](const boost::system::error_code &ec) {
				if(!ec)
					default_resource_send(server, response, ifs);
				else
					cerr << "Connection interrupted" << endl;
			});
		}
	}
}


WebServer::WebServer(map<string, string> paths) {
	server.config.port=8000;

	cout << "WebServer: Listening on port " << server.config.port << endl;
    server.default_resource["GET"]=[this,paths](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try {
            auto p = paths.find(request->path);
            if (p == paths.end()) {
                throw invalid_argument("invalid path");
            }
            string s = p->second;
			auto ifs=make_shared<ifstream>();
            ifs->open(s, ifstream::in | ios::binary | ios::ate);
			if(*ifs) {
				cout << "WebServer: Serving " << s << " to " << request->remote_endpoint_address << endl;

				auto length=ifs->tellg();
				ifs->seekg(0, ios::beg);

				*response << "HTTP/1.1 200 OK\r\n" << "Content-Length: " << length << "\r\n\r\n";
				default_resource_send(server, response, ifs);
			}
			else
				throw invalid_argument("could not read file");
		}
		catch(const exception &e) {
            string content="Could not open path "+request->path+": "+e.what();
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		}
	};

	server_thread = new thread([this](){
        server.start();
    });
}


WebServer::~WebServer() {
    server.stop();
    server_thread->join();
    delete server_thread;
}

