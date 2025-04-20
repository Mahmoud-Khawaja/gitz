#include "repository.h"
#include "commands.h"
#include <iostream>
#include <vector>
#include <string>


void printHelp() {
    std::cout << "sub niggna, welcome to gitz! The getz version control system with aura.\n\n";
    std::cout << "Usage: gitz <command> [<args>]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  skibidi         (like git init)\n";
    std::cout << "  aura <file>...  (like git add)\n";
    std::cout << "  hawktuah -m \"msg\" (like git commit)\n";
    std::cout << "  fineshyte       (like git diff)\n";
    std::cout << "  thesigma        (like git status)\n";
    std::cout << "  log             (like git log)\n";
    std::cout << "  --help          Show this help message\n\n";
    std::cout << "Happy hawktuah!\n";
}

/*
gitz skibidi -> init repository

gitz aura <path> -> add to staging

gitz hawktuah -m "msg" → commit staged files

gitz fineshyte → show diff

gitz thesigma → show status

gitz log → hna 3ady b2a omal enta fakr eh 
*/

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "omak btgely\n";
        std::cerr << "Yo, usage: gitz <command> [<args>]\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "--help") {
        printHelp();
        return 0;
    }

    try {
        if (command == "skibidi") {
            gitz::Repository::init();
            return 0;
        }

        auto repoPath = gitz::Repository::findRepo();
        if (!repoPath) {
            std::cerr << "nigga wtf you’re not in a repo\n";
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