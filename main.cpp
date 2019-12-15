#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>

#include "bfshell.h"

void showHelp();

std::string readFile(std::string path);

void removeWhitespace(std::string& str);

void processBFCode(std::string code);

int main(int argc, const std::string argv[])
{
    int status = 0;
        
    if (argc == 1)
    {
        showHelp();
    }

    else
    {
        if (argv[1] == "-h" || argv[1] == "--help")
        {
            showHelp();
        }

        else if (argv[1] == "--shell")
        {
            BFShell shell;
            status = shell.run();
        }

        else if (argv[1] == "-f")
        {
            if (argv[2].empty())
            {
                std::cout << "Please provide the path to the file!\n";
            }

            else
            {
                std::string code = readFile(argv[2]);
                removeWhitespace(code);

                processBFCode(code);
            }
        }

        else
        {
            std::string code = std::accumulate(argv + 2, argv + argc);
        }
        
    }
    


    return status;
}

void showHelp()
{
    std::cout
        << "Brainfuck [--shell | -f <file> | <bf_code>]\n"
        << "--shell\t\tEnter shell mode\n"
        << "-f <file>\t\tOpen a <file> with bf code\n"
        << "<bf_code>\t\tInterprets a one-line bf code\n";
}

std::string readFile(std::string path)
{
    std::string contents = "";
    std::ifstream file(path, std::ios::in);
    if (file.is_open())
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        contents = ss.str();
        file.close();
    }

    else
    {
        std::cout << "Unable to open file: " << path << std::endl;
    }

    return contents;
}