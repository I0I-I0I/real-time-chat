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
        .timeout = 1,
    };
    Socket server("localhost",  "8080", opts);

    server.on("connection", [&server](int socket, const auto& _) -> void {
        for (;;) {
            std::string info = server.receive_msg(socket);
            if (info == "") return;
            HttpRequestStruct http = Http::parce(info);
            server.handle_received_data(socket, http.method, http);

            if (http.headers.find("connection") != http.headers.end()
                    && http.headers.at("connection") == "keep-alive") {
                continue;
            }
            break;
        }
        std::cout << "GoodBye!!!" << std::endl;
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
            std::string response;
            if (info.type() != typeid(HttpRequestStruct)) {
                response = Http::response( 400, "Something strange");
            } else {
                HttpRequestStruct http = std::any_cast<HttpRequestStruct>(info);
                response = handler(http);
            }
            server.send_msg(socket, response);
        });
    }

    server.start();

    return 0;
}
