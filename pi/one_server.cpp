#define ASIO_STANDALONE
#define ASIO_DISABLE_THREADS
#include <iostream>
#include <asio.hpp>

using namespace asio;
using asio::ip::udp;

int main() {
  asio::io_service ios;
  /* create a UDP socket for IPv4 address, port 5000 */
  ip::udp::socket sock (ios, udp::endpoint(udp::v4(), 5000));

  while (true) {
    char data[1024];
    udp::endpoint sender_ep;
    size_t len;
    len = sock.receive_from (asio::buffer(data, 1024), sender_ep);
    std::cout << "Received data fom " << sender_ep << " ";
    std::cout.write (data, len);
    std::cout << std::endl;
    sock.send_to(asio::buffer("OK", 2), sender_ep);
  }
}
