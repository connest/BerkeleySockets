#include "ServerProcessor.h"
#include "ServerFactory.h"

#include "TCPServer.h"
#include "UDPServer.h"

#include "../shared/ArgsParser.h"

#include <iostream>
#include <thread>

void runServer(IServerProcessor* processor, IServer* server)
{
    server->init(processor);
    server->start();
}

int main(int argc, char *argv[])
{
    ArgsParser parser;

    parser  .addOption("port", 'p', required_argument)
            .setHelpText(
                "Usage: Server -p <port>\r\n"
                " --port -p  - port\r\n"
                " --help -?  - show this message\r\n"
                );

    if(parser.parse(argc, argv))
        return 0; //Show help....


    std::string _port {parser.getOption('p')};
    if(_port.empty()) {
        std::cerr << "Invalid argument: port should not be empty" << std::endl;
        parser.showHelp();
        return 1;
    }

    int port = std::stoi( _port );


    auto processor = std::make_unique<ServerProcessor>();
    auto tcpServer = ServerFactory::create<TCPServer>(port);
    auto udpServer = ServerFactory::create<UDPServer>(port);



    std::thread tcp_thread(runServer, processor.get(), tcpServer.get());

    runServer(processor.get(), udpServer.get());

    tcp_thread.join();

    return 0;
}
