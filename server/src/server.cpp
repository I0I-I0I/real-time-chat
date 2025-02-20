#include <string>
#include "../lib/json.hpp"
#include "./config.h"
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

using json = nlohmann::json;

int main() {
    SocketOpts opts = {
        .backlog = 5,
        .timeout = 5,
        .type = "server"
    };
    Socket server("localhost", "8080", opts);

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
            response = Http::response(404, "Unknown method");

        if (response.headers.find("content-length") == response.headers.end()) {
            response = Http::response(500, "Server Missing Content-Length");
            server.send_msg(socket, Http::to_send(response));
        }

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
