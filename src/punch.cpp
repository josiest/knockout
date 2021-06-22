#include "punch.hpp"
#include <iostream>
#include <unordered_map>

enum class Command {
    In, Out, Log, Invalid
};

template<typename K, typename V>
V map_get(std::unordered_map<K, V> const & map, K const & key,
          V const & default_value)
{
    auto const search = map.find(key);
    if (search != map.end()) {
        return search->second;
    }
    return default_value;
}

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
    string arg = argv[1];
    switch (map_get(argmap, arg, Command::Invalid)) {
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
