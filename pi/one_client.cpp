#include <iostream>

#define ASIO_DISABLE_THREADS
#define ASIO_STANDALONE   // Remove external dependency to Boost library
#include <asio.hpp>

using namespace std;
using namespace asio;
using asio::ip::udp;

int main(int argc, char*argv[]) {
  if (argc < 2) {
    std::cerr << "Missing hostname/IP" << endl;
    exit (0);
  }
  asio::io_service ios;
  /* create a UDP socket for IPv4 address, and port 0 */
  ip::udp::socket sock (ios, udp::endpoint(udp::v4(), 0));

  udp::resolver res (ios);

  /* find the endpoint of port 5000 on the localhost */
  udp::endpoint destination = *res.resolve ({udp::v4(), 
     "localhost", "5000"});
  char test[6] = "Hello";
  sock.send_to (asio::buffer(test, 5), destination);

  char response[1024];
  size_t resp_len;
  udp::endpoint response_endpoint;
  resp_len = sock.receive_from (asio::buffer (response, 1024), 
    response_endpoint);
  cout << "Response is ";
  cout.write (response, resp_len);
  cout << endl;
}
