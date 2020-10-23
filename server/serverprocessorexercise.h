#pragma once
#include "serverprocessor.h"

#include <vector>
class ServerProcessorExercise : public IServerProcessor
{
public:
    ServerProcessorExercise();

    // IServerProcessor interface
public:
    virtual std::string process(std::string &&buf) override;

private:
    std::vector<int> extractIntegerWords(std::string str);
};
