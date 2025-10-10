#include "ScanCore.hpp"
#include <iostream>
#include <fstream>

// Реализация методов FileScan
FileScan::FileScan(const std::string& path, const std::string& verdict)
    : filePath(path),
      fileContent(""),
      scanVerdict(verdict),
      fileSize(0),
      hash("") {
    this->setContent();
}

FileScan::~FileScan() = default;

const std::string& FileScan::getPath() const { return filePath; }
const std::string& FileScan::getContent() const { return fileContent; }
const std::string& FileScan::getVerdict() const { return scanVerdict; }
const std::string& FileScan::getHash() const { return hash; }
size_t FileScan::getSize() const { return fileSize; }

std::string FileScan::getFileName() const {
    size_t pos = filePath.find_last_of("/");
    return (pos != std::string::npos) ? filePath.substr(pos + 1) : filePath;
}

void FileScan::setContent() {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::string err = "Error: the file does not open ";
        err += filePath;
        throw err;
    }

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize > 0) {
        fileContent.resize(fileSize);
        file.read(&fileContent[0], fileSize);
    } else {
        fileContent.clear();
    }

    file.close();
}

void FileScan::setVerdict(const std::string& verdict) { scanVerdict = verdict; }
void FileScan::setHash(const std::string& hash_str) { hash = hash_str; }

bool FileScan::contains(const std::string& pattern) const {
    return fileContent.find(pattern) != std::string::npos;
}

bool FileScan::isEmpty() const {
    return fileContent.empty();
}

void FileScan::printInfo() const {
    std::cout << "File path: " << filePath << std::endl;
    std::cout << "File name: " << getFileName() << std::endl;
    std::cout << "Size: " << fileSize << " bytes" << std::endl;
    std::cout << "Hash: " << getHash() << std::endl;
    std::cout << "Verdict: " << getVerdict() << std::endl;
    std::cout << "---" << std::endl;
}