#include "IClient.h"
#include "UDPClient.h"
#include "TCPClient.h"

#include "ClientFactory.h"

#include "../shared/ArgsParser.h"

#include <iostream>
bool validate(ArgsParser& parser, std::string& host, short& port)
{
    if(parser.hasOption('u') && parser.hasOption('t')) {
        std::cerr << "Invalid argument: cannot be UDP and TCP ogether" << std::endl;
        return false;
    }

    host = parser.getOption('h');
    if(host.empty())
        host.assign("127.0.0.1");


    std::string _port {parser.getOption('p')};
    if(_port.empty()) {
        std::cerr << "Invalid argument: port should not be empty" << std::endl;
        return false;
    }

    port = std::stoi( _port );

    return true;
}

int main(int argc, char *argv[]) {

    ArgsParser parser;

    parser  .addOption("host", 'h', required_argument)
            .addOption("port", 'p', required_argument)
            .addOption("UPD",  'u', no_argument)
            .addOption("TCP",  't', no_argument)
            .setHelpText(
                "Usage: Client -h <ip-address> -p <port> -t \r\n"
                " --host -h  - ip-address (default 127.0.0.1)\r\n"
                " --port -p  - port\r\n"
                " --TCP  -t  - use TCP (default)\r\n"
                " --UDP  -u  - use UDP\r\n"
                " --help -?  - show this message\r\n"
                );

    if(parser.parse(argc, argv))
        return 0; //Show help....

    std::string host;
    short port;
    bool is_valid = validate(parser, host, port);

    if(!is_valid) {
        parser.showHelp();
        return 1;
    }


    std::unique_ptr<IClient> client;

    if(parser.hasOption('u')) {
        client = ClientFactory::create<UDPClient>(std::move(host), std::move(port));

    } else { //defualt TCP
        client = ClientFactory::create<TCPClient>(std::move(host), std::move(port));

    }






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
}
