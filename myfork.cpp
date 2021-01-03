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
#include "myfork.h"
using namespace std;

string myfork(string ip_addr, int port)
{
    int pid = fork();
    if (pid == 0)
    {
        string serverIp = "192.168.1.75";
        //create a message buffer
        char msg[1500];
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
        if (status < 0)
        {
            cout << "Error connecting to socket!" << endl;
            return "1";
        }
        cout << "Connected to the server!" << endl;
        int bytesRead, bytesWritten = 0;
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        while (1)
        {
            cout << ">";
            string data;
            //getline(cin, data);
            data = string("-") + ip_addr;

            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, data.c_str());
            bytesWritten += send(clientSd, (char *)&msg, strlen(msg), 0);
            sleep(3);
            pid = getpid();
            string dumping = (string("sudo criu dump -t ") + to_string(pid).c_str() + string(" -D distr -s --shell-job")).c_str();
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, dumping.c_str());
            bytesWritten += send(clientSd, (char *)&msg, strlen(msg), 0);

            break;
        }
        gettimeofday(&end1, NULL);
        close(clientSd);
        pid = getpid();
        string dumping = (string("sudo criu dump -t ") + to_string(pid).c_str() + string(" -D distr -s --shell-job")).c_str();

        return "child";
    }
    else
    {
        return "parent";
    }
}