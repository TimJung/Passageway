TODO: Put a description of what's what in here

The following two files require libasio-dev installed on Raspbian
one_client.cpp: a UDP client for sending data to port 44865
one_server.cpp: a UDP server for receiving data from port 44865

To compile the .cpp files:
   g++ -std=c++11 one_client.cpp -o one_client

To run it:
   ./one_client mobile.cis.gvsu.edu    # AFTER running the server on MASL


PServer.java: a UDP server for receiving data from port 44865

To compile the .java file:
   javac PServer.java

To run it:
   java -classpath . PServer
   
To run the pi.c file:
   gcc -Wall -o pi pi.c -lwiringpi -lparse
