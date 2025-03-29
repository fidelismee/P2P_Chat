#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <thread>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

// Handle each client session
void session(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        ws->accept();
        std::cout << "Client connected!" << std::endl;

        while (true) {
            beast::flat_buffer buffer;
            ws->read(buffer);  // Read incoming WebSocket message
            std::string message = beast::buffers_to_string(buffer.data());
            std::cout << "Received: " << message << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 9002));

        std::cout << "WebSocket server running on ws://localhost:9002" << std::endl;

        while (true) {
            auto socket = std::make_shared<tcp::socket>(ioc);
            acceptor.accept(*socket);  // Accept new TCP connection

            // Create a WebSocket stream from the accepted socket
            auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(*socket));

            // Start a new thread for each client
            std::thread(&session, ws).detach();
        }
    } catch (std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }

    return 0;
}
