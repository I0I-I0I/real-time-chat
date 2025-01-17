#include <functional>
#include <iostream>
#include <string>
#include "../lib/json.hpp"
#include "./config.h"
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

using json = nlohmann::json;

std::string create_response(
    std::function<std::string(const HttpRequestStruct&)> handler,
    const std::any& info
) {
    if (info.type() != typeid(HttpRequestStruct))
        return Http::response(
            400,
            "Something strange");
    HttpRequestStruct http = std::any_cast<HttpRequestStruct>(info);
    return handler(http);
}

int main() {
    SocketOpts opts = {
        .backlog = 5,
        .recv_timeout = 10000,
        .send_timeout = 10000,
    };
    Socket server("localhost",  "8080", opts);

    server.on("connection", [&server](int socket, const auto& _) -> void {
        std::string info = server.receive_msg(socket);
        if (info == "")
            return;
        HttpRequestStruct http = Http::parce(info);
        server.handle_received_data(socket, http.method, http);
    });

    server.on("close", [&server](int socket, const auto& info) -> void {
        std::cout << "Connection closed" << std::endl;
    });

    server.on("*", [&server](int socket, const auto& info) -> void {
        std::string response = Http::response(405, "Unknown method");
        server.send_msg(socket, response);
    });

    std::map<std::string, HandlerOnFunc> method_handlers = {
        { "GET", HandlerOn::get },
        { "POST", HandlerOn::post },
        { "OPTIONS", HandlerOn::options },
        { "DELETE", HandlerOn::del },
        { "PUT", HandlerOn::put }
    };

    for (const auto& [method, handler] : method_handlers) {
        server.on(method, [&server, &handler](int socket, const auto& info) -> void {
            std::string response = create_response(handler, info);
            server.send_msg(socket, response);
        });
    }

    server.start();

    return 0;
}
