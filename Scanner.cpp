#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "ScanCore.hpp"

namespace fs = std::filesystem;

void printHelp() {
        std::cout << "\nUsage:\n";
        std::cout << "  program [--base file] [--log logfile] [--path dir] [--help]\n\n";
        std::cout << "General Options:\n";
        std::cout << "  --base <file>     - a file with malicious file hashes.\n";
        std::cout << "  --path <dir>      - a directory that will be checked.\n";
        std::cout << "  --log <logfile>   - a file with logs about malicious files.\n";
        std::cout << "  --help            - Show help.\n\n";
    }


int main(int argc, char* argv[]) {
    std::string dataFile;
    std::string pathDir;
    std::string logFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            printHelp();
            return 0;
        }
        else if (arg == "--base" && i + 1 < argc) {
            dataFile = argv[++i];
        }
        else if (arg == "--path" && i + 1 < argc) {
            pathDir = argv[++i];
        }
        else if (arg == "--log" && i + 1 < argc) {
            logFile = argv[++i];
        }
        else {
            std::cout << "Unknown parameter: " << arg << std::endl;
            std::cout << "Use --help for usage information.\n";
            return 1;
        }
    }

    if (dataFile.empty() || pathDir.empty()) {
        std::cout << "The program was started without the necessary parameters.\n";
        std::cout << "Use --help for usage information.\n";
    }else if(!logFile.empty()){
        DirectoryScanner scanner(pathDir, dataFile, logFile);
        scanner.scanWithOutput();
    }else{
        DirectoryScanner scanner(pathDir, dataFile);
        scanner.scanWithOutput();
    }

    return 0;
}