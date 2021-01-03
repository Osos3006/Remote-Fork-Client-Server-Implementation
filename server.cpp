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
using namespace std;
//Server side
int main(int argc, char *argv[])
{
    //for the server, we only need to specify a port number
    if (argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    //grab the port number
    int port = atoi(argv[1]);
    //buffer to send and receive messages with
    char msg[1500];

    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr *)&servAddr,
                          sizeof(servAddr));
    if (bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if (newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
    //lets keep track of the session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;
    while (1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        bytesRead += recv(newSd, (char *)&msg, sizeof(msg), 0);

        string mnm = msg;
        if (mnm[0] == '-')
        {
            string serverIp;
            serverIp = msg;
            serverIp = serverIp.substr(1);
            bytesRead += recv(newSd, (char *)&msg, sizeof(msg), 0);
            string dumping = msg;
            system(dumping.c_str());
            close(newSd);

            //create a message buffer
            //setup a socket and connection tools
            struct hostent *host = gethostbyname(serverIp.c_str());
            sockaddr_in sendSockAddr;
            bzero((char *)&sendSockAddr, sizeof(sendSockAddr));
            sendSockAddr.sin_family = AF_INET;
            sendSockAddr.sin_addr.s_addr =
                inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
            sendSockAddr.sin_port = htons(port);
            int clientSd = socket(AF_INET, SOCK_STREAM, 0);
            //try to connect...
            int status = connect(clientSd,
                                 (sockaddr *)&sendSockAddr, sizeof(sendSockAddr));
            cout << serverIp << endl;
            if (status < 0)
            {
                cout << "Error connecting to socket!" << endl;
                break;
            }
            cout << "Connected to the server!" << endl;
            int bytesRead, bytesWritten = 0;
            struct timeval start1, end1;
            gettimeofday(&start1, NULL);
            system("zip -r files.zip distr main myfork.cpp myfork.h silly.cpp");

            while (1)
            {
                fstream file;
                file.open("files.zip", ios::in | ios::binary);
                std::string osamaaaaa((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                string zip_sz = to_string(osamaaaaa.length());
                strcpy(msg, zip_sz.c_str());
                cout << msg << endl;
                bytesWritten += send(clientSd, (char *)&msg, strlen(msg), 0);
                sleep(3);
                int cursor = 0;
                while (cursor < osamaaaaa.length())
                {
                    int chunk = send(clientSd, osamaaaaa.c_str(), osamaaaaa.length(), 0);
                    cursor += chunk;
                }
                break;
            }
            gettimeofday(&end1, NULL);
            close(clientSd);

            return 0;
        }
        else
        {

            string size = msg;
            cout << size << endl;
            int res_sz = atoi(size.c_str());
            int now_sz = 0;
            cout << res_sz << endl;
            char msg2[500 * 1024];
            memset(&msg2, 0, sizeof(msg2)); //clear the buffer
            while (now_sz < res_sz)
            {
                now_sz += recv(newSd, &msg2[now_sz], sizeof(msg2), 0);
            }
            cout << now_sz << endl;
            ofstream file("files.zip", ios::binary);
            for (int i = 0; i < now_sz; i++)
                file.put(msg2[i]);
            file.close();
            sleep(2);
            system("unzip -o files.zip");
            system("sudo criu restore -D ./distr --shell-job");
        }

        string data;
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if (data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char *)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
        bytesWritten += send(newSd, (char *)&msg, strlen(msg), 0);
        break;
    }
    //we need to close the socket descriptors after we're all done
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);

    return 0;
}