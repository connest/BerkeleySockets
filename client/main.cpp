#include <iostream>
#include "ClientProcessor.h"

int main(int argc, char *argv[]) {

    ClientProcessor pr;


    std::pair<ArgsParser, bool> result = pr.consoleParse(argc, argv);
    if(!result.second) {
        //if invalid, getopt_long will show help message
        return 1;
    }

    auto& parser = result.first;
    bool args_valid = pr.getAddress(parser);
    if(!args_valid) {
        parser.showHelp();
        return 2;
    }


    std::unique_ptr<IClient> client = pr.getClientObject(parser);

    int res = client->init();
    if(res) {
        std::cerr << "Client init problem..." << std::endl
                  << "Close" << std::endl;
        return 1;
    }


    std::cout<<"Input text:"<<std::endl;

    std::string request;
    std::getline(std::cin, request, '\n');


    client->send(std::move(request));

    char recvBuff[1024];
    client->recv(recvBuff, 1024);

    std::cout << recvBuff << std::endl;

    return  0;
}
