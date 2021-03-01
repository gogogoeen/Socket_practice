# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <string>
# include <string.h>
# include <netdb.h>
# include <arpa/inet.h>

using namespace std;
class localhost_client {
public:
    localhost_client () {};

    ~localhost_client() {};

    bool run_client () {
        cout << "Start activating client..." << endl;
        // creating socket
        clientSoc = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSoc == -1) {
            cout << "Fail to Create a Client Socket" << endl;
            return false;
        }

        // specify sockaddrinfo connected to
        clientInfo.sin_family = AF_INET;
        clientInfo.sin_port = htons(54000);
        clientInfo.sin_addr.s_addr = inet_addr("127.0.0.1");

        // connect to the server
        if (connect(clientSoc, (sockaddr*)& clientInfo, sizeof(clientInfo)) == -1) {
            cout << "Fail to Connect to Localhost Server" << endl;
            return false;
        }
        cout << "Connected to Localhost Server" << endl;

        while(true) {
            string input;
            cout << "Client>>" ;
            getline(cin, input);
            //send the message to server
            send(clientSoc, input.c_str(), input.size() + 1, 0);

            // received from server
            int bytereceived = recv(clientSoc, buf, sizeof(buf), 0);
            if (bytereceived == -1) {
                cout << "Fail to receive data from server";
                return false;
            }else if (bytereceived == 0) {
                cout << "Connection is closed" << endl;
                break;
            }
            cout << "Server>>" << string(buf, 0, bytereceived) << endl;
        }

        cout << "Closing the client..." << endl;
        close(clientSoc);
        return true;
    }
private:
    int clientSoc;
    sockaddr_in clientInfo;
    char buf[4096];


};

int main() {
    localhost_client client;
    if (!client.run_client()) {
        cout << "client crashed!" << endl;
    }
    
    return 0;
}