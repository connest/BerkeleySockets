#include "ServerProcessor.h"

#include <sstream>
#include <iostream>
#include <algorithm>
std::string ServerProcessor::process(std::string &&buf)
{
    std::vector<int> numbers = extractIntegerWords(buf);

    std::stable_sort(numbers.begin(), numbers.end());

    int sum = 0;
    std::stringstream out;
    for(const auto& number: numbers) {
        out << number << " ";
        sum += number;
    }

    //erase last space
    if(!numbers.empty())
        out.seekp(-1,out.cur);

    out << std::endl << sum;

    return out.str();
}

std::vector<int> ServerProcessor::extractIntegerWords(std::string str)
{
    // In case "text77other"
    // stringstream ignore 77
    // so, set all non-number text to spaces
    std::transform(str.begin(), str.end(), str.begin(), [](const auto& symbol) {
       if(symbol < '0' || symbol > '9')
          return ' ';

       return symbol;
    });

    std::vector<int> numbers;
    std::stringstream ss;

    ss << str;
    int found;

    while (!ss.eof()) {

        str.clear();
        ss >> str;

        if (std::stringstream(str) >> found)
            numbers.push_back(found);
    }

    return numbers;
}
