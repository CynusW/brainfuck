#include "bfshell.h"

#include <regex>
#include <iostream>
#include <algorithm>
#include <locale>
#include <limits>

BFLoop::BFLoop(size_t start, const std::string& code)
{
    this->start = start;

    std::stack<cell_size> temp;
    bool foundEnd = false;

    while (!foundEnd)
    {
        const char& c = code.at(start);
        if (c == '[')
        {
            temp.push('[');
        }

        else if (c ==']')
        {
            if (!temp.empty())
            {
                temp.pop();
                if (temp.empty())
                {
                    this->end = start;
                    foundEnd = true;
                }
            }
        }

        start++;
    }
}

BFShell::BFShell()
{
    index = 0;
    cells.push_back(BFCell(new cell_size(0)));
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

        else if (input == "show")
        {
            std::cout << index << " | " << (int)(*(cells.at(index))) << std::endl;
        }

        else 
        {
            execute(input);
        }
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
                index++;
                if (index >= cells.size())
                {
                    cells.push_back(BFCell(new cell_size(0)));
                }
            }
            break;
        case '<':
            {
                if (index == 0)
                {
                    cells.emplace(cells.begin(), BFCell(new cell_size(0)));
                }

                else index--;
            }
            break;
        case '+':
            *(cells[index]) += 1;
            break;
        case '-':
            *(cells[index]) -= 1;
            break;
        case '.':
            put();
            break;
        case ',':
            *(cells[index]) = get();
            break;
        case '[':
            {
                if (loops.empty() || loops.top().start != i)
                {
                    loops.push(BFLoop(i, code));
                }

                if (*(cells.at(index)) == 0)                
                {
                    i = loops.top().end;
                    loops.pop();
                }
            }
            break;
        case ']':
            if (*(cells.at(index)) != 0)
            {
                i = loops.top().start;

                continue;
            }

            else loops.pop();
            break;
        default:
            break;
        }

        i++;
    }

    std::cout << std::endl << std::endl;
    std::cout << index << " | " << (int)(*(cells.at(index))) << std::endl;
}

void BFShell::put()
{
    std::putchar((int)(*(cells.at(index))));
}

cell_size BFShell::get()
{
    cell_size c = getchar();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return c;
}