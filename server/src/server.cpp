#include <string>
#include "json.hpp"
#include "./config.h"
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
    std::string host = argc > 1 ? argv[1] : "localhost";
    std::string port = argc > 2 ? argv[2] : "8080";
    TCPSocketOpts opts = { .backlog = 9999, .timeout = 5, .type = "server" };

    TCPSocket server(host.c_str(), port.c_str(), opts);

    std::map<std::string, HandlerOnFunc> method_handlers = {
        { "GET", HandlerOn::get },
        { "POST", HandlerOn::post },
        { "OPTIONS", HandlerOn::options },
        { "DELETE", HandlerOn::del },
        { "PUT", HandlerOn::put }
    };

    server.on("chatting", [&server, &method_handlers](const int& socket, const std::string& info) -> int {
        HttpRequestStruct http = Http::parse(info);
        HttpResponseStruct response;

        if (method_handlers.find(http.method) != method_handlers.end())
            response = method_handlers.at(http.method)(http);
        else
            response = Http::response(StatusCode::method_not_allowed, "Unknown method");

        if (std::stoi(response.headers.at("content-length")) > 1024)
            server.send_msg(socket, Http::to_send(response, 1024));
        else
            server.send_msg(socket, Http::to_send(response));

        if ((response.headers.find("connection") != response.headers.end())
                && (response.headers["connection"] == "close")) {
            return -1;
        }
        return 0;
    });

    server.start();

    return 0;
}
