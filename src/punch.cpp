#include "punch.hpp"
#include "punch/read.hpp"
#include "punch/write.hpp"

#include <boost/program_options.hpp>

#include <unordered_map>
#include <vector>
#include <algorithm>

#include <stdexcept>
#include <exception>

namespace po = boost::program_options;

std::string const usage = "punch [--help] <in|out|cards|clean|archive>";

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

    // create the program description
    po::options_description program(usage);
    program.add_options()
        ("command", po::value<string>(), "which command to run")
        ("help", "the help message")
    ;
    // specify 1 positional command
    auto commands = po::positional_options_description{}.add("command", 1);

    // parse the top-level commands
    po::variables_map args;
    auto punch_parser = po::command_line_parser(argc, argv)
        .options(program)
        .positional(commands);
    po::store(punch_parser.run(), args);

    // print help if asked for
    if (args.count("help")) {
        cout << "Usage: " << usage << endl;
        return 0;
    }

    // if command not specified print usage
    if (!args.count("command")) {
        cout << "Usage: " << usage << endl;
        return 1;
    }

    // process the command
    try {
        switch (map_get(argmap, args["command"].as<string>(),
                        Command::Invalid)) {

        case Command::In:       punch_in();     break;
        case Command::Out:      punch_out();    break;
        case Command::Cards:    print_cards();  break;
        case Command::Archive:  archive();      break;
        case Command::Clean:    clean_cards();  break;
        default: cout << usage << endl; return 1;
        }
    }
    catch (invalid_argument e) {
        cout << "punch-card is ill formatted" << endl
             << "  " << e.what() << endl;
        return 1;
    }
    catch (runtime_error e) {
        cout << e.what() << endl;
        return 1;
    }
    catch (exception e) {
        cout << "something went wrong:" << endl
             << "  " << e.what() << endl;
        return 1;
    }
    return 0;
}
