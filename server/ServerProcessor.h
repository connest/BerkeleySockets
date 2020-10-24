#pragma once
#include "IServerProcessor.h"

#include <vector>
class ServerProcessor : public IServerProcessor
{
public:

    // IServerProcessor interface
public:
    virtual std::string process(std::string &&buf) override;

private:
    std::vector<int> extractIntegerWords(std::string str);
};
