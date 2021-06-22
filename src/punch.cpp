#include "punch.hpp"
#include "punch/read.hpp"
#include "punch/write.hpp"

#include <boost/program_options.hpp>

#include <unordered_map>
#include <vector>
#include <algorithm>

#include <stdexcept>
#include <exception>
#include <string>

namespace po = boost::program_options;
void parse_cards_command(std::vector<std::string> const & args);

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
    std::string const usage = "punch [--help] <in|out|cards> [<args>]";
    po::options_description program(usage);
    program.add_options()
        ("command", po::value<string>()->required(),
                    "which command to run")
        ("subargs", po::value<vector<string>>()->required(),
                    "subargs for command")
        ("help", "the help message")
    ;
    // specify 1 positional command
    po::positional_options_description pos;
    pos.add("command", 1).add("subargs", -1);

    // parse the top-level commands
    po::variables_map args;
    auto parsed = po::command_line_parser(argc, argv)
        .options(program)
        .positional(pos)
        .allow_unregistered().run();
    po::store(parsed, args);

    // print help if asked for
    if (args.count("help")) {
        cout << "Usage: " << usage << endl;
        return 0;
    }

    // if command not specified print usage
    if (!args.count("command")) {
        cout << "Usage: " << usage << endl;
        return 0;
    }

    // process the command
    vector<string> subargs =
        po::collect_unrecognized(parsed.options, po::include_positional);
    subargs.erase(subargs.begin()); // the first element is the command

    try {
        // try to get the enum associated with the command
        // if no enum then it's an invalid command
        switch (map_get(argmap, args["command"].as<string>(),
                        Command::Invalid)) {

        // call the appropriate function for each command
        case Command::In:
            punch_in(); break;

        case Command::Out:
            punch_out(); break;

        case Command::Cards:
            parse_cards_command(subargs);
            break;

        // the command was invalid, print usage
        default:
            cout << "Usage: " << usage << endl;
            return 0;
        }
    }
    // handle any errors that happened along the way
    catch (invalid_argument & e) {
        cout << "punch-card is ill formatted" << endl
             << "  " << e.what() << endl;
    }
    catch (runtime_error & e) {
        cout << e.what() << endl;
    }
    catch (exception & e) {
        cout << "something went wrong:" << endl
             << "  " << e.what() << endl;
    }
    return 1;
}

void parse_cards_command(std::vector<std::string> const & subargs)
{
    using namespace std;
    using namespace std::string_literals;

    // define the cards command and sub-args
    std::string const usage = "punch cards [--help] [list|archive|clean]";
    po::options_description cards_command("manipulate timecards");
    cards_command.add_options()
        ("command", po::value<string>()->default_value("list"),
                    "which command to run")
        ("help", "the cards help message")
    ;
    // define the positional arguments
    po::positional_options_description pos;
    pos.add("command", -1);

    // parse the sub-args
    po::variables_map args;
    auto parsed = po::command_line_parser(subargs)
        .options(cards_command)
        .positional(pos)
        .allow_unregistered().run();
    po::store(parsed, args);

    // print help if asked for"
    if (args.count("help")) {
        cout << "Usage: " << usage << endl;
        return;
    }

    // process the command

    // try to get the enum associated with the command
    // if no enum then it's an invalid command
    switch (map_get(cards_argmap, args["command"].as<string>(),
                    CardsCommand::Invalid)) {

    // call the appropraite function for each command
    case CardsCommand::List:
        print_cards(); break;

    case CardsCommand::Archive:
        archive(); break;

    case CardsCommand::Clean:
        clean_cards(); break;

    // the command was invalid, print usage
    default:
        cout << "Usage: " << usage << endl;
    }
}
