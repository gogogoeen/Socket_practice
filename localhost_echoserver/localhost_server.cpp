# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <string>
# include <string.h>
# include <netdb.h>
# include <arpa/inet.h>


using namespace std;

class localhost_server {
   
public: 
    localhost_server () {
        client_addrlen = sizeof(clientInfo);    // not sure why need this...
        memset(hostname, 0, NI_MAXHOST);
        //memset(servicename, 0, NI_MAXSERVICE);
        
    };
    ~localhost_server () {
        if (serverSoc != -1) {
            close(serverSoc);
        }
    };

    bool run_server() {
        cout << "Start activating server..." << endl;

        // creating socket
        serverSoc = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSoc == -1) {
            cout << "Fail to Create a Server Socket" << endl;
            return false;
        }
        // bind up ip and port to socket
        serverInfo.sin_family = AF_INET;
        serverInfo.sin_port = htons(54000);
        serverInfo.sin_addr.s_addr = INADDR_ANY;
        
        if (bind(serverSoc, (sockaddr*)&serverInfo, sizeof(serverInfo)) == -1) {
            cout << "Fail to Bind the Socket" << endl;
            return false;
        }

        // listen for connections on a socket
        listen(serverSoc, 10);
        cout << "Waiting client to connect" << endl;
        
        
        // connect to a client socket, and the ip of the client
        connectSoc = accept(serverSoc, (sockaddr*)& clientInfo, &client_addrlen);
        if (connectSoc == -1) {
            cout << "Fail to connected to the client socket" << endl;
            return false;
        }

        inet_ntop(AF_INET, &clientInfo.sin_addr, hostname, NI_MAXHOST); // get the ip of client;
        cout << hostname << "connected on the server with port " << ntohs(clientInfo.sin_port) << endl;

        // receive the message from the client and echo back to client

        while (true) {
            memset(buf, 0, 4096);
            // received from client
            int bytereceived = recv(connectSoc, buf, sizeof(buf), 0);
            if (bytereceived == -1) {
                cout << "Fail to receive data from client";
                return false;
            }else if (bytereceived == 0) {
                cout << "Connection is closed" << endl;
                break;
            }
            cout << string(buf, 0, bytereceived) << endl;

            // Echo message back to client
            send(connectSoc, buf, bytereceived + 1, 0);
        }
        cout << "Closing the server..." << endl;
        // Close the socket
        close(serverSoc);
        close(connectSoc);

        return true;

    }
private:
    int serverSoc, connectSoc;  // file descriptor for sockets
    sockaddr_in serverInfo, clientInfo; // address information (server socket and connected client socket)
    socklen_t client_addrlen;
    char hostname[NI_MAXHOST];  // client's hostname
    //char servicename[NI_MAXSERV];   // the service (port) which client is connected on
    char buf[4096];

};



int main () {
    localhost_server server;
    if (!server.run_server()) {
        cout << "server crashed!" << endl;
    }

    return 0;
}