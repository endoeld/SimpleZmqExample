#include <string>
#include <iostream>

#include <zmq.hpp>

int main()
{
    // Initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // Construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // Set up some static data to send
    const std::string data{"Hello"};

    for (auto request_num = 0; request_num < 10; ++request_num)
    {
        // Send the request message
        std::cout << "Sending Hello " << request_num << "..." << std::endl;
        socket.send(zmq::buffer(data), zmq::send_flags::none);

        // Wait for reply from server
        zmq::message_t reply{};

        if (socket.recv(reply, zmq::recv_flags::none))
        {
            std::cout << "Received " << reply.to_string();
            std::cout << " (" << request_num << ")";
            std::cout << std::endl;
        }
        else
        {
            std::cout << "FAILED to receive message on socket!" << std::endl;
        }
    }

    return 0;
}