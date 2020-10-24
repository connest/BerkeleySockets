#include "IClient.h"
#include "UDPClient.h"
#include "TCPClient.h"

#include "ClientFactory.h"
int main(int argc, char *argv[]) {



    //    //TODO create usage
    //    if(argc != 2) {
    //        std::cout<<"\n Usage: "<<argv[0]<<" <ip of server>" <<std::endl;
    //        return 1;
    //    }

    auto client = ClientFactory::create<UDPClient>("127.0.0.1", 8877);
    int res = client->init();
    if(res) {
        std::cerr << "Client init problem..." << std::endl
                  << "Close" << std::endl;
        return 1;
    }

    std::string request;

    std::cout<<"Input text:"<<std::endl;
    std::getline(std::cin, request, '\n');



    client->send(std::move(request));

    char recvBuff[1024];
    client->recv(recvBuff, 1024);

    std::cout << recvBuff << std::endl;

    return 0;




    return 0;
}
