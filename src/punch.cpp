#include "punch.hpp"
#include "punch/read.hpp"
#include "punch/write.hpp"

#include <unordered_map>
#include <vector>
#include <algorithm>

std::string const usage = "punch <in|out|cards|clean|archive> [--help]";
std::string const program_help =
    "  in - start a new timecard\n"
    "  out - complete the current timecard\n"
    "  cards - list all current timecards\n"
    "  archive - archive the current timecards\n"
    "  clean - clean any current invalid timecard entries\n"
    "  help - show this message\n";

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
        {"in", Command::In}, {"out", Command::Out}, {"cards", Command::Cards},
        {"clean", Command::Clean}, {"archive", Command::Archive},
        {"help", Command::Help}
    };

    // print usage if not used correctly
    if (argc < 2) {
        cout << usage << endl;
        return 1;
    }
    // search for the help argument
    auto const arg_begin = argv;
    auto const arg_end = argv + argc;
    auto wants_help = [](string const & arg) { return arg == "--help"; };
    auto const help_search = find_if(arg_begin, arg_end, wants_help);

    if (help_search != arg_end) {
        cout << usage << endl
             << program_help << endl;
        return 0;
    }

    // process the arguments
    string const arg = argv[1];
    auto const command = map_get(argmap, arg, Command::Invalid);
    try {
        switch (command) {
        case Command::In:       punch_in();     break;
        case Command::Out:      punch_out();    break;
        case Command::Cards:    print_cards();  break;
        case Command::Clean:    clean_cards();  break;
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
