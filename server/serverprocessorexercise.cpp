#include "serverprocessorexercise.h"

#include <sstream>
#include <iostream>
ServerProcessorExercise::ServerProcessorExercise()
{
}

std::string ServerProcessorExercise::process(std::string &&buf)
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
    out.seekp(-1,out.cur);
    out << std::endl << sum;

//    //If no any numbers
//    if(out.str().empty())
//        return "\n0";

    return out.str();
}

std::vector<int> ServerProcessorExercise::extractIntegerWords(std::string str)
{
    // In case, when "text77other"
    // stringstream ignore 77
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
