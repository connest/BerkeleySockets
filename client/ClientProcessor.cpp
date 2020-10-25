#include "ClientProcessor.h"

#include "UDPClient.h"
#include "TCPClient.h"
#include "ClientFactory.h"


#include <iostream>

std::pair<ArgsParser, bool> ClientProcessor::consoleParse(int argc, char *argv[])
{
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
    int result = parser.parse(argc, argv);

    return { std::move(parser), result == 0 };

}
bool ClientProcessor::getAddress(ArgsParser &parser)
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

std::unique_ptr<IClient> ClientProcessor::getClientObject(ArgsParser &parser)
{
    if(parser.hasOption('u')) {
        std::cout<<"UDP mode..." <<std::endl;
        return ClientFactory::create<UDPClient>(std::move(host), std::move(port));

    } else { //defualt TCP
        std::cout<<"TCP mode..." <<std::endl;
        return ClientFactory::create<TCPClient>(std::move(host), std::move(port));
    }
}
