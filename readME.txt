Remote fork Implementation details all copyrights goes to © Mohammed Abuelwafa


used tools and libraries:
- standard c++ libraries and data structures including, but not limited to networking headers and file processing
- criu framework for process migration 



The following includes basically sums it up ...
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>




Design Details:
The code consists of three main files: 1) myfork.cpp 2)server.cpp 3)silly.cpp


- myfork.cpp 

a function that takes the ip and the port of the machine to fork at, then it 
forks returning a pid value. this function is responsible for returning an indictor
whether this was the parent of the child ( pid child always == 0 ).

then the myfork function appends a flag to the ip_address of the forked at node in order to 
inform the daemon process (server.cpp) in our case whether this is the client or the server node.



- server.cpp 

this file represents the daemon process, this process is always running on both the client and the server.
on the server side, it zips all the cpp files, the criu dump files, and exe files and sends them on the client node.

on the client side, it unzips the file and calls the criu restore in order to respawn the child process on the client node.



- silly.cpp

a simple c++ code that prints a loop from 0 to 9 with a label naming the printing process whether parent or child. " this .cpp file basically resembles the process to be forked"




How TO USE?

- on the server machine the following processes should run using the following commands:

1) daemon process (server.cpp )

g++ server.cpp -o daemon
./daemon

2) the silly.cpp ( process to be forked remotely)

// before running this process the remote process should have the daemon process running
// the object file should be named "main" due to some implementation restriction

g++ silly.cpp myfork.cpp -o main 
./main 

- on the client machine the following processes should run using the following:

1) daemon process (server.cpp)

g++ server.cpp -o daemon
./daemon





