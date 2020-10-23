#ifndef SERVERPROCESSOR_H
#define SERVERPROCESSOR_H

#include <string>
#include <algorithm>


class IServerProcessor
{
public:
    IServerProcessor();
    virtual std::string process(std::string&& buf) = 0;
};

#endif // SERVERPROCESSOR_H
