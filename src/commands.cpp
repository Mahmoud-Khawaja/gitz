#include "include/commands.h"
#include "include/object_utils.h"
#include "include/blob.h"
#include "include/tree.h"
#include "include/commit.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <stdexcept>


namespace gitz {
    std::map<std::string, std::string> readConfig(const std::filesystem::path& configPath) {
        std::map<std::string, std::string> config;
        // Check if the config file exists
        if (std::filesystem::exists(configPath)) {
            std::ifstream file(configPath);
            // heck open 
            if (!file.is_open()) throw std::runtime_error("Failed to open config file: " + configPath.string());

            std::string line;

            while (std::getline(file, line)) {
                size_t eqPos = line.find('=');

                if (eqPos != std::string::npos) {
                    std::string key = line.substr(0, eqPos);
                    std::string value = line.substr(eqPos + 1);
                    // Trim whitespace

                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);

                    //Only add non-empty keys to the config map
                    if (!key.empty()) config[key] = value;
                }
            }
        }

        return config;
    }
    
    
    void writeConfig(const std::filesystem::path& configPath, const std::map<std::string, std::string>& config) {
        std::ofstream file(configPath);
        if (!file.is_open()) throw std::runtime_error("Failed to write config file: " + configPath.string());
        for (const auto& [key, value] : config) file << key << "=" << value << "\n";
    }
    
    // config: Set or get global user configuration (nigga.name, nigga.email)
    void config(const std::vector<std::string>& args) {
        // TODO 
    }    

    // Helper to add a file to the index
    void addFileToIndex(const Repository& repo, Index& index, const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "omok btgely kol gom3a \n";
            std::cerr << "failed to open file: " << filePath << "\n";
            return;
        }
        // read file content into string
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        Blob blob(content);
        std::string hash = writeObject(blob, repo.getGitDir());
        std::filesystem::path relPath = std::filesystem::relative(filePath, repo.getRoot());
        if (index.isFileStaged(relPath) && index.getEntries().at(relPath.string()) == hash) return;  
        index.addFile(relPath, hash);
    }
    
    // aura: Add files to the staging area
    void aura(const Repository& repo, const std::vector<std::string>& paths) {
        // Create an Index object to manage the staging area
        Index index(repo.getGitDir());
        index.load();
        for (const auto& path : paths) {
            std::filesystem::path fullPath = repo.getRoot() / path;
            if (std::filesystem::is_directory(fullPath)) {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(fullPath)) {
                    if (entry.is_regular_file()) {
                        addFileToIndex(repo, index, entry.path());
                    }
                }
            } else if (std::filesystem::is_regular_file(fullPath)) {
                addFileToIndex(repo, index, fullPath);
            } else {
                std::cerr << "Invalid path: " << path << "\n";
            }
        }
        index.save();
        std::cout << "Added aura to the vibe: " << paths.size() << " path(s)\n";
    }
    
        std::string buildTree(const Repository& repo, const Index& index, const std::filesystem::path& dirPath) {
        std::vector<Tree::Entry> entries;
        std::map<std::string, std::string> subdirs;
        
        for (const auto& [path, hash] : index.getEntries()) {
            std::filesystem::path p = path;

            // If the file is in the current directory, add it as a tree entry
            if (p.parent_path() == dirPath) {
                Tree::Entry entry{"100644", "blob", hash, p.filename().string()};
                entries.push_back(entry);
            } 
            
            // If the file is in a subdirectory, store the subdirectory path
            else if (p.parent_path().string().find(dirPath.string()) == 0) {
                std::string subdir = p.parent_path().lexically_relative(dirPath).begin()->string();
                if (subdirs.find(subdir) == subdirs.end()) {
                    subdirs[subdir] = buildTree(repo, index, dirPath / subdir);
                }
            }
        }

        // Add subdirectories to the tree entries
        for (const auto& [subdir, treeHash] : subdirs) {
            Tree::Entry entry{"040000", "tree", treeHash, subdir};
            entries.push_back(entry);
        }
        
        std::sort(entries.begin(), entries.end(), [](const Tree::Entry& a, const Tree::Entry& b) {
            return a.name < b.name;
        });
        
        Tree tree(entries);
        return writeObject(tree, repo.getGitDir()); // serialize and write the tree to the repository
    }
    
    std::string getParentHash(const Repository& repo) {
        std::filesystem::path headPath = repo.getGitDir() / "HEAD";
        std::ifstream headFile(headPath);
        std::string headContent;
        std::getline(headFile, headContent);

        // If head points to a reference get the commit hash from that reference
        if (headContent.find("ref: ") == 0) {
            std::string refPath = headContent.substr(5);
            std::filesystem::path refFile = repo.getGitDir() / refPath;
            if (std::filesystem::exists(refFile)) {
                std::ifstream ref(refFile);
                std::string hash;
                std::getline(ref, hash);
                return hash;
            }
        }
        return "";
    }
    
    void updateHead(const Repository& repo, const std::string& commitHash) {
        std::filesystem::path headPath = repo.getGitDir() / "HEAD";
        std::ifstream headFile(headPath);
        std::string headContent;
        std::getline(headFile, headContent);

        // If head points to a reference update it with the new commit hash
        if (headContent.find("ref: ") == 0) {
            std::string refPath = headContent.substr(5);
            std::filesystem::path refFile = repo.getGitDir() / refPath;
            std::ofstream ref(refFile);
            ref << commitHash;
        }
    }
    
    // hawktuah: Commit staged changes
    void hawktuah(const Repository& repo, const std::string& message) {
        Index index(repo.getGitDir());
        index.load();
        if (index.getEntries().empty()) {
            std::cerr << "wtf nigga!, Nothing to hawktuah\n";
            return;
        }

        std::string treeHash = buildTree(repo, index, repo.getRoot());
        std::string parentHash = getParentHash(repo);
        // Default author and committer (could be from config in the future) : TODO 

        std::string author = "user <user@example.com>";
        std::string committer = "user <user@example.com>";


        Commit commit(treeHash, parentHash, author, committer, message);

        // write the commit object to the repository and update head
        std::string commitHash = writeObject(commit, repo.getGitDir());
        updateHead(repo, commitHash);


        std::cout << "good job mf! hawktuah’d: " << commitHash.substr(0, 7) << "\n";
    }
    
    // fineshyte: Show differences between working directory and staging area
    void fineshyte(const Repository& repo) {
        Index index(repo.getGitDir());
        index.load();

        // Iterate through files in the working directory
        for (const auto& entry : std::filesystem::recursive_directory_iterator(repo.getRoot())) {
            if (entry.is_regular_file()) {
                std::filesystem::path relPath = std::filesystem::relative(entry.path(), repo.getRoot());
                std::string pathStr = relPath.string();

                // Check if the file is staged and compare its hash with the working directory
                if (index.isFileStaged(relPath)) {

                    std::string stagedHash = index.getEntries().at(pathStr);
                    std::ifstream file(entry.path(), std::ios::binary);
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    Blob blob(content);
                    std::string currentHash = computeHash(blob.serialize());

                    // If the hash is different, print "Modified"
                    if (stagedHash != currentHash) {
                        std::cout << "Modified: " << pathStr << "\n";
                    }
                } 

                // If the file is not staged, print "Untracked"
                else {
                    std::cout << "Untracked: " << pathStr << "\n";
                }
            }
        }
    }
    
    // thesigma: Show repository status
    void thesigma(const Repository& repo) {
        Index index(repo.getGitDir());
        index.load();

        // Print the current branch
        std::cout << "On branch " << repo.getCurrentBranch() << "\n";
        std::cout << "Changes to be committed:\n";

        
        for (const auto& [path, hash] : index.getEntries()) {
            std::cout << "  " << path << "\n";
        }

        std::cout << "Changes not staged for commit:\n";
        fineshyte(repo); // changes not staged for commit
    }
    
    // log: Show commit history
    void log(const Repository& repo) {
        std::string commitHash = getParentHash(repo);

        // walk through the commit history
        while (!commitHash.empty()) {
            auto [type, content] = readObjectContent(commitHash, repo.getGitDir());
            Commit commit = Commit::fromContent(content);

            // printing details 
            std::cout << "Commit " << commitHash.substr(0, 7) << "\n";
            std::cout << "Author: " << commit.getAuthor() << "\n";
            std::cout << "Date: " << commit.getTimestamp();
            std::cout << "\n    " << commit.getMessage() << "\n\n";

            commitHash = commit.getParentHash(); // // move to the parent
        }
    }
    
    } 