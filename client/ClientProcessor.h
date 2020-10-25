#pragma once

#include "../shared/ArgsParser.h"

#include <memory>

#include "IClient.h"

class ClientProcessor
{
    std::string host;
    short port;

public:

    /**
     * @brief parse console arguments
     * @param argc arguments count
     * @param argv arguments vector
     * @return pair <parser object, success>
     */
    std::pair<ArgsParser, bool>
    consoleParse(int argc, char *argv[]);

    /**
     * @brief extract ip-adderss and port from arguments
     * @param parser parser object
     * @return arguments are valid
     */
    bool getAddress(ArgsParser& parser);

    /**
     * @brief create client object by arguments (--UDP or --TCP)
     * @param parser parser object
     * @return polymorfic object with interface IClient
     */
    std::unique_ptr<IClient> getClientObject(ArgsParser& parser);
};


