#include "punch.hpp"
#include <iostream>
#include <unordered_map>

enum class Command {
    In, Out, Log, Invalid
};

int main(int argc, char * argv[])
{
    using namespace std;
    unordered_map<string, Command> argmap = {
        {"in", Command::In}, {"out", Command::Out}, {"log", Command::Log}
    };

    // print usage if not used correctly
    string const usage = "punch <in|out|log>";
    if (argc < 2) {
        cout << usage << endl;
        return 1;
    }

    // process arguments
    auto const command_search = argmap.find(argv[1]);
    auto command = Command::Invalid;
    if (command_search != argmap.end()) {
        command = command_search->second;
    }
    switch (command) {
    case Command::In:
        break;
    case Command::Out:
        break;
    case Command::Log:
        break;
    default:
        cout << usage << endl;
        return 1;
    }
    return 0;
}
