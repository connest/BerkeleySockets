#include "ServerProcessor.h"
#include "ServerFactory.h"

#include "TCPServer.h"
#include "UDPServer.h"

#include "../shared/ArgsParser.h"

#include <iostream>
#include <thread>

/**
 * @brief parse console arguments
 * @param argc arguments count
 * @param argv arguments vector
 * @return pair <port, success>
 */
std::pair<short, bool> parseArguments(int argc, char *argv[])
{
    ArgsParser parser;

    parser  .addOption("port", 'p', required_argument)
            .setHelpText(
                "Usage: Server -p <port>\r\n"
                " --port -p  - port\r\n"
                " --help -?  - show this message\r\n"
                );

    if(parser.parse(argc, argv)) {
        //if invalid, getopt_long will show help message
        return {0, false};
    }


    std::string _port {parser.getOption('p')};
    if(_port.empty()) {
        std::cerr << "Invalid argument: port should not be empty" << std::endl;
        parser.showHelp();
        return {0, false};
    }

    short port = std::stoi( _port );
    return {port, true};
}

/**
 * @brief run server
 * @param processor buisness logic request -> response
 * @param server server object
 */
void runServer(IServerProcessor* processor, IServer* server)
{
    int res = server->init(processor);
    if(res)
        return;

    server->start();
}

int main(int argc, char *argv[])
{
    auto result = parseArguments(argc, argv);
    if(!result.second)
        return 1;

    short TCPport = result.first;
    short UDPport = result.first;


    auto tcpServer = ServerFactory::create<TCPServer>(std::move(TCPport));
    auto udpServer = ServerFactory::create<UDPServer>(std::move(UDPport));


    auto processor = std::make_unique<ServerProcessor>();

    std::thread tcp_thread(runServer, processor.get(), tcpServer.get());

    // run UDP in current thread
    runServer(processor.get(), udpServer.get());

    tcp_thread.join();

    return 0;
}
