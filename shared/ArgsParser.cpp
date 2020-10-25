#include "ArgsParser.h"

#include <iostream>
ArgsParser &ArgsParser::addOption(const std::string &name, const char letter, int arg_option)
{
    optString.push_back(letter);

    if(arg_option == required_argument)
        optString.push_back(':');

    opts.push_back({name.c_str(), arg_option, NULL, letter});

    return *this;
}

ArgsParser &ArgsParser::setHelpText(std::string &&text)
{
    std::swap(help_text, text);
    return *this;
}

int ArgsParser::parse(int argc, char *argv[])
{
    //--help by default
    opts.push_back({"help", no_argument, NULL, '?'});
    //end of opts
    opts.push_back({NULL, no_argument, NULL, 0});


    int longIndex = 0;
    int opt = getopt_long( argc, argv, optString.c_str(), opts.data(), &longIndex );
    while( opt != -1 ) {
        switch( opt ) {
        case '?':
            showHelp();
            return 1;

        default:
            if(optarg)
                opt_values.emplace(opt, optarg);
            else
                opt_values.emplace(opt, "true");
            break;
        }

        opt = getopt_long( argc, argv, optString.c_str(), opts.data(), &longIndex );
    }
    return 0;
}

bool ArgsParser::hasOption(char letter)
{
    return opt_values.find(letter) != std::end(opt_values);
}

std::string ArgsParser::getOption(char letter)
{
    return opt_values[letter];
}

void ArgsParser::showHelp()
{
    std::cout<<help_text<<std::endl;
}
