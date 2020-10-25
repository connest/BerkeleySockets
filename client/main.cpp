#include "IClient.h"
#include "UDPClient.h"
#include "TCPClient.h"

#include "ClientFactory.h"

#include "../shared/ArgsParser.h"

#include <iostream>


/**
 * @brief validate arguments
 * @param parser object, which contains values
 * @param host ip-address
 * @param port port
 * @return success flag
 */
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

/**
 * @brief parse console arguments
 * @param argc arguments count
 * @param argv arguments strings
 * @return pair <connection object, success>
 */
std::pair<std::unique_ptr<IClient>, bool>
consoleParse(int argc, char *argv[])
{
    std::unique_ptr<IClient> client;

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
        return {std::move(client), false};


    std::string host;
    short port;

    bool is_valid = validate(parser, host, port);

    if(!is_valid) {
        parser.showHelp();
        return {std::move(client), false};
    }


    if(parser.hasOption('u')) {
        std::cout<<"UDP mode..." <<std::endl;
        client = ClientFactory::create<UDPClient>(std::move(host), std::move(port));

    } else { //defualt TCP
        std::cout<<"TCP mode..." <<std::endl;
        client = ClientFactory::create<TCPClient>(std::move(host), std::move(port));

    }

    return {std::move(client), true};
}


int main(int argc, char *argv[]) {

    auto result = consoleParse(argc, argv);
    if(!result.second)
        return 1;

    std::unique_ptr<IClient> client = std::move(result.first);


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

    return 0;
}
