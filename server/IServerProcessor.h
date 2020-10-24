#pragma once

#include <string>

class IServerProcessor
{
public:
    /**
     * @brief process data
     *
     * business logic of the program
     * 1. extract numbers
     * 2. sort them
     * 3. sum them
     * 4. create response like
     *      <sorted numbers> \r\n
     *      <sum>
     *
     * @param buf request
     * @return response
     */
    virtual std::string process(std::string&& buf) = 0;
};
