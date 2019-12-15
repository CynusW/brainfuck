#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <locale>

#include "bfshell.h"

void showHelp();

std::string readFile(const std::string path);

void removeWhitespace(std::string& str);

int main(int argc, const char* cargv[])
{
    int status = 0;

    std::string* argv = new std::string[argc];
    std::transform(cargv, cargv + argc, argv, [](const char* cstr) { return std::string(cstr); });
        
    if (argc <= 1)
    {
        showHelp();
    }

    else
    {
        BFShell shell;

        if (argv[1] == "-h" || argv[1] == "--help")
        {
            showHelp();
        }

        else if (argv[1] == "--shell")
        {
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

                shell.execute(code);
            }
        }

        else
        {
            std::string code = std::accumulate(argv + 1, argv + argc, std::string());
            removeWhitespace(code);

            shell.execute(code);
        }
        
    }

    delete[] argv;

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

std::string readFile(const std::string path)
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

void removeWhitespace(std::string& str)
{
    std::string::iterator begin = str.begin();
    std::string::iterator end;

    while ((begin = std::find_if(
        begin,
        str.end(),
        [] (const char& c) { return isspace(c); }
    )) != str.end())
    {
        end = std::find_if(begin, str.end(), [](const char& c) { return !isspace(c); });
        begin = str.erase(begin, end);
    }
}