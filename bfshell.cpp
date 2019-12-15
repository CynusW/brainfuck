#include "bfshell.h"

#include <regex>
#include <iostream>
#include <algorithm>
#include <locale>
#include <limits>

BFShell::BFShell()
{
    index = 0;
    cells.push_back(BFCell());
}

int BFShell::run()
{
    int status = 0;

    bool exit = false;
    std::string input;

    std::cout << "\nBrainfuck Shell\n";

    while (!exit)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (!input.empty())
        {
            std::transform(input.begin(), input.end(), input.begin(), [](const char& c) { return std::tolower(c); });
        }

        if (input == "exit")
        {
            exit = true;
            continue;
        }

        execute(input);
    }

    return status;
}

bool checkBracketsBalanced(const std::string code)
{
    std::stack<char> brackets;
    for (const char& c : code)
    {
        if (c == '[')
        {
            brackets.push('[');
        }

        else if (c == ']')
        {
            if (brackets.empty())
            {
                return false;
            }

            brackets.pop();
        }
    }

    return brackets.empty();
}

void BFShell::execute(const std::string code)
{
    if (checkBracketsBalanced(code) == false)
    {
        std::cout << "Warning: brackets not balanced!\n";
        return;
    }

    size_t i = 0;
    while (i < code.size())
    {
        const char c = code.at(i);

        switch (c)
        {
        case '>':
            {
                std::cout << "SHIFT RIGHT\n";

                index++;
                if (index >= cells.size())
                {
                    cells.push_back(BFCell());
                }
            }
            break;
        case '<':
            {
                std::cout << "SHIFT LEFT\n";

                index--;
                if (index < 0)
                {
                    cells.emplace(cells.begin(), BFCell());
                    index++;
                }
            }
            break;
        case '+':
            std::cout << "ADD\n";

            *(cells[index]) += 1;
            break;
        case '-':
            std::cout << "SUBSTRACT\n";

            *(cells[index]) -= 1;
            break;
        case '.':
            std::cout << "PUT\n";

            put();
            break;
        case ',':
            std::cout << "GET\n";

            *(cells[index]) = get();
            break;
        case '[':
            std::cout << "LOOP START\n";

            if (!loops.empty() && loops.top().start == i)
            {
                if (*(cells.at(index)) == 0)
                {
                    i = loops.top().end;
                    loops.pop();
                }

                break;
            }

            {
                BFLoop loop = { };
                loop.start = i;

                loops.push(loop);
            }
            break;
        case ']':
            std::cout << "LOOP END\n";

            if (*(cells.at(index)) != 0)
            {
                loops.top().end = i;
                i = loops.top().start;

                continue;
            }

            else loops.pop();
            break;
        default:
            std::cout << "DEFAULT\n";
            break;
        }

        i++;
    }
}

void BFShell::put()
{
    std::cout << *(cells.at(index));
}

cell_size BFShell::get()
{
    cell_size c = getchar();
    return c;
}