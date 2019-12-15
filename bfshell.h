#include <string>
#include <memory>
#include <vector>
#include <stack>

typedef unsigned char cell_size;
typedef std::unique_ptr<cell_size> BFCell;

struct BFLoop
{
    size_t start;
    size_t end;
};

bool isValidBFCode(std::string code);

class BFShell
{
public:
    BFShell();
    virtual ~BFShell();

    int run();

    void execute(std::string code);

    void put();

    cell_size get();

private:
    size_t index;
    std::vector<BFCell> cells;
    std::stack<BFLoop> loops;
};
