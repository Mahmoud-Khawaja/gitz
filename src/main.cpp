#include "repository.h"
#include "commands.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Yo, usage: gitz <command> [<args>]\n";
        return 1;
    }

    std::string command = argv[1];

    try {
        if (command == "skibidi") {
            gitz::Repository::init();
            return 0;
        }

        auto repoPath = gitz::Repository::findRepo();
        if (!repoPath) {
            std::cerr << "Bruh, you’re not in a repo\n";
            return 1;
        }
        gitz::Repository repo(*repoPath);

        if (command == "aura") {
            // TODO
        } else if (command == "hawktuah") {
            // TODO
        } else if (command == "fineshyte") {
            // TODO
        } else if (command == "thesigma") {
            // TODO
        } else if (command == "log") {
            // TODO
        } else {
            std::cerr << "Wtf command is ‘" << command << "’?\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Big L: " << e.what() << "\n";
        return 1;
    }

    return 0;
}