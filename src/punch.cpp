#include "punch.hpp"
#include "punch/read.hpp"
#include "punch/write.hpp"

#include <unordered_map>

int main(int argc, char * argv[])
{
    using namespace std;

    // initalize directory structure
    if (!fs::exists(paths::resources)) {
        fs::create_directory(paths::resources);
    }
    if (!fs::exists(paths::archives)) {
        fs::create_directory(paths::archives);
    }

    unordered_map<string, Command> argmap = {
        {"in", Command::In}, {"out", Command::Out}, {"log", Command::Log},
        {"clean", Command::Clean}, {"archive", Command::Archive}
    };

    // print usage if not used correctly
    string const usage = "punch <in|out|log|clean|archive>";
    if (argc < 2) {
        cout << usage << endl;
        return 1;
    }
    // process the arguments
    string const arg = argv[1];
    auto const command = map_get(argmap, arg, Command::Invalid);
    try {
        switch (command) {
        case Command::In:       punch_in();     break;
        case Command::Out:      punch_out();    break;
        case Command::Log:      printlogs();    break;
        case Command::Clean:    cleanlogs();    break;
        case Command::Archive:  archive();      break;
        default: cout << usage << endl; return 1;
        }
    }
    catch (invalid_argument e) {
        cout << "punch-card is ill formatted" << endl
             << "  " << e.what() << endl;
    }
    catch (runtime_error e) {
        cout << e.what() << endl;
    }
    catch (exception e) {
        cout << "something went wrong:" << endl
             << "  " << e.what() << endl;
        return 1;
    }
    return 0;
}
