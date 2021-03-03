#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>

int main()
{
    using namespace std::chrono_literals;

    // Initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // Construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    // Prepare some static data for responses
    const std::string data{"World"};

    for (;;)
    {
        zmq::message_t request;

        // Receive a request from client
        if (socket.recv(request, zmq::recv_flags::none))
        {
            std::cout << "Received " << request.to_string() << std::endl;
        }
        else
        {
            std::cout << "FAILED to receive message on socket!" << std::endl;
        }

        // Simulate work
        std::this_thread::sleep_for(1s);

        // Send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    return 0;
}