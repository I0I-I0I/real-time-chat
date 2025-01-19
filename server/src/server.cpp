#include <iostream>
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
        .timeout = 0,
    };
    Socket server("localhost",  "8080", opts);

    std::map<std::string, HandlerOnFunc> method_handlers = {
        { "GET", HandlerOn::get },
        { "POST", HandlerOn::post },
        { "OPTIONS", HandlerOn::options },
        { "DELETE", HandlerOn::del },
        { "PUT", HandlerOn::put }
    };

    server.on("connection", [&server, &method_handlers](const int& socket, const std::string& info) -> int {
        HttpRequestStruct http = Http::parce(info);
        HttpResponseStruct response;

        if (method_handlers.find(http.method) != method_handlers.end())
            response = method_handlers.at(http.method)(http);
        else
            response = Http::response(404, "Unknown method");

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
