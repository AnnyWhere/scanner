#include "ScanCore.hpp"
#include <iostream>
#include <fstream>
#include <memory>

// методы DirectoryScanner
std::string DirectoryScanner::md5(const std::string& data){
    int len = data.size();
    int remain = len % 64;
    int size = 0;
    if (remain < 56) {
        size = len - remain + 64;
    } else size = len - remain + 64 + 64;
    unsigned char *data_mas = new unsigned char[size];

    for (int i = 0; i < len; i++)
        data_mas[i] = data[i];
    data_mas[len] = 0x80;
    for (int i = len + 1; i < size - 8; i++)
        data_mas[i] = 0;
    unsigned long long b_len = len * 8;
    for (int i = 0; i<8; i++)
        data_mas[size - 8 + i] = b_len >> i * 8;
    std::string res = md5Core(data_mas, size);
    delete[] data_mas;
    return res;
}

unsigned int DirectoryScanner::rotateLeft(unsigned int val, unsigned int shift){
    return val << shift | val >> (32 - shift);
}

unsigned int DirectoryScanner::fullF(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
    unsigned int p, unsigned int v, unsigned int sh){
    return op + rotateLeft((op1 + ((x & y) | (~x & z)) + p + v), sh);
}

unsigned int DirectoryScanner::fullG(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
    unsigned int p, unsigned int v, unsigned int sh){
    return op + rotateLeft((op1 + ((x & z) | (~z & y)) + p + v), sh);
}

unsigned int DirectoryScanner::fullH(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
    unsigned int p, unsigned int v, unsigned int sh){
    return op + rotateLeft((op1 + (x ^ y ^ z) + p + v), sh);
}

unsigned int DirectoryScanner::fullI(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
    unsigned int p, unsigned int v, unsigned int sh){
    return op + rotateLeft((op1 + (y ^ (x | ~z)) + p + v), sh);
}

std::string DirectoryScanner::md5Core(unsigned char *data_mas, int size){
    unsigned int *part;
    part = (unsigned int *)data_mas;
    unsigned int T[64];
    for(int i = 0; i < 64; i++){
        T[i] = (unsigned int)(pow(2, 32) * fabs(sin(i + 1)));
    }
    unsigned int A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476;
    unsigned int A1{A}, B1{B}, C1{C}, D1{D};
    for (int i = 0; i < size; i+=64){
        A1 = A; B1 = B; C1 = C; D1 = D;

        A = fullF(B, A, B, C, D, part[0], T[0], 7), D = fullF(A, D, A, B, C, part[1], T[1], 12), C = fullF(D, C, D, A, B, part[2], T[2], 17), B = fullF(C, B, C, D, A, part[3], T[3], 22);
        A = fullF(B, A, B, C, D, part[4], T[4], 7), D = fullF(A, D, A, B, C, part[5], T[5], 12), C = fullF(D, C, D, A, B, part[6], T[6], 17), B = fullF(C, B, C, D, A, part[7], T[7], 22);
        A = fullF(B, A, B, C, D, part[8], T[8], 7), D = fullF(A, D, A, B, C, part[9], T[9], 12), C = fullF(D, C, D, A, B, part[10], T[10], 17), B = fullF(C, B, C, D, A, part[11], T[11], 22);
        A = fullF(B, A, B, C, D, part[12], T[12], 7), D = fullF(A, D, A, B, C, part[13], T[13], 12), C = fullF(D, C, D, A, B, part[14], T[14], 17), B = fullF(C, B, C, D, A, part[15], T[15], 22);

        A = fullG(B, A, B, C, D, part[1], T[16], 5), D = fullG(A, D, A, B, C, part[6], T[17], 9), C = fullG(D, C, D, A, B, part[11], T[18], 14), B = fullG(C, B, C, D, A, part[0], T[19], 20);
        A = fullG(B, A, B, C, D, part[5], T[20], 5), D = fullG(A, D, A, B, C, part[10], T[21], 9), C = fullG(D, C, D, A, B, part[15], T[22], 14), B = fullG(C, B, C, D, A, part[4], T[23], 20);
        A = fullG(B, A, B, C, D, part[9], T[24], 5), D = fullG(A, D, A, B, C, part[14], T[25], 9), C = fullG(D, C, D, A, B, part[3], T[26], 14), B = fullG(C, B, C, D, A, part[8], T[27], 20);
        A = fullG(B, A, B, C, D, part[13], T[28], 5), D = fullG(A, D, A, B, C, part[2], T[29], 9), C = fullG(D, C, D, A, B, part[7], T[30], 14), B = fullG(C, B, C, D, A, part[12], T[31], 20);

        A = fullH(B, A, B, C, D, part[5], T[32], 4), D = fullH(A, D, A, B, C, part[8], T[33], 11), C = fullH(D, C, D, A, B, part[11], T[34], 16), B = fullH(C, B, C, D, A, part[14], T[35], 23);
        A = fullH(B, A, B, C, D, part[1], T[36], 4), D = fullH(A, D, A, B, C, part[4], T[37], 11), C = fullH(D, C, D, A, B, part[7], T[38], 16), B = fullH(C, B, C, D, A, part[10], T[39], 23);
        A = fullH(B, A, B, C, D, part[13], T[40], 4), D = fullH(A, D, A, B, C, part[0], T[41], 11), C = fullH(D, C, D, A, B, part[3], T[42], 16), B = fullH(C, B, C, D, A, part[6], T[43], 23);
        A = fullH(B, A, B, C, D, part[9], T[44], 4), D = fullH(A, D, A, B, C, part[12], T[45], 11), C = fullH(D, C, D, A, B, part[15], T[46], 16), B = fullH(C, B, C, D, A, part[2], T[47], 23);

        A = fullI(B, A, B, C, D, part[0], T[48], 6), D = fullI(A, D, A, B, C, part[7], T[49], 10), C = fullI(D, C, D, A, B, part[14], T[50], 15), B = fullI(C, B, C, D, A, part[5], T[51], 21);
        A = fullI(B, A, B, C, D, part[12], T[52], 6), D = fullI(A, D, A, B, C, part[3], T[53], 10), C = fullI(D, C, D, A, B, part[10], T[54], 15), B = fullI(C, B, C, D, A, part[1], T[55], 21);
        A = fullI(B, A, B, C, D, part[8], T[56], 6), D = fullI(A, D, A, B, C, part[15], T[57], 10), C = fullI(D, C, D, A, B, part[6], T[58], 15), B = fullI(C, B, C, D, A, part[13], T[59], 21);
        A = fullI(B, A, B, C, D, part[4], T[60], 6), D = fullI(A, D, A, B, C, part[11], T[61], 10), C = fullI(D, C, D, A, B, part[2], T[62], 15), B = fullI(C, B, C, D, A, part[9], T[63], 21);
        A += A1, B += B1, C += C1, D += D1;
        part += 16;
    }
    std::vector<unsigned char> hash_arr(16);
    memcpy(&hash_arr[0], &A, 4);
    memcpy(&hash_arr[4], &B, 4);
    memcpy(&hash_arr[8], &C, 4);
    memcpy(&hash_arr[12], &D, 4);
    std::string symb = "0123456789abcdef";
    std::string res = "";
    for (size_t i = 0; i < hash_arr.size(); i++){
        res += symb[hash_arr[i] >> 4];
        res += symb[hash_arr[i] & 0x0F];
    }

    return res;
}

bool DirectoryScanner::isDirectory(const std::string& path) {
#ifdef _WIN32
    DWORD attributes = GetFileAttributesA(path.c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
#endif
}

void DirectoryScanner::traverseDirectory(const std::string& directoryPath, time_t start) {
#ifdef _WIN32
    WIN32_FIND_DATAA findFileData;
    std::string searchPath = directoryPath + "\\*";
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Couldn't find the directory: " << directoryPath << std::endl;
        return;
    }

    do {
        std::string entryName = findFileData.cFileName;

        if (entryName == "." || entryName == "..") {
            continue;
        }

        std::string fullPath = directoryPath;
        if (!fullPath.empty() && fullPath.back() != '\\') {
            fullPath += "\\";
        }
        fullPath += entryName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            traverseDirectory(fullPath, start);
        } else {
            try {
                FileScan file(fullPath);
                std::string verdict = analyzeFile(&file);
                scannedFiles.emplace_back(file);
            } catch (...) {
                errorsCount += 1;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
#else
    DIR* dir = opendir(directoryPath.c_str());
    if (!dir) {
        std::cerr << "Couldn't find the directory: " << directoryPath << std::endl;
        return;
    }

    std::unique_ptr<DIR, decltype(&closedir)> dirGuard(dir, closedir);
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string entryName = entry->d_name;

        if (entryName == "." || entryName == "..") {
            continue;
        }

        std::string fullPath = directoryPath;
        if (!fullPath.empty() && fullPath.back() != '/') {
            fullPath += "/";
        }
        fullPath += entryName;

        if (isDirectory(fullPath)) {
            traverseDirectory(fullPath, start);
        } else {
            try {
                FileScan file(fullPath);
                std::string verdict = analyzeFile(&file);
                scannedFiles.emplace_back(file);
            } catch (...) {
                errorsCount += 1;
            }
        }
    }
#endif

    time_t end;
    time(&end);
    programTime = difftime(end, start);
    resultsLog();
}

const std::string DirectoryScanner::analyzeFile(FileScan* file) {
    std::string result = "clean";
    std::string hash = md5(file->getContent());
    if (baseScan.count(hash)){
        result = baseScan[hash];
        suspFilesNum += 1;
    }

    file->setHash(hash);
    file->setVerdict(result);

    return result;
}

DirectoryScanner::DirectoryScanner(const std::string& path, const std::string& baseFile) : rootPath(path) {
    std::ifstream file(baseFile);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + baseFile);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string part;

        while (std::getline(ss, part, ';')) {
            parts.push_back(part);
        }
        if (parts.size() == 2) {
            baseScan[parts[0]] = parts[1];
        } else
            continue;
    }
    logFile = "./logs";
    suspFilesNum = 0;
    errorsCount = 0;
    file.close();
}

DirectoryScanner::DirectoryScanner(const std::string& path, const std::string& baseFile, const std::string& log) : rootPath(path), logFile(log) {
    std::ifstream file(baseFile);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + baseFile);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string part;

        while (std::getline(ss, part, ';')) {
            parts.push_back(part);
        }
        if (parts.size() == 2) {
            baseScan[parts[0]] = parts[1];
        } else
            continue;
    }
    suspFilesNum = 0;
    errorsCount = 0;
    file.close();
}

DirectoryScanner::~DirectoryScanner() = default;

bool DirectoryScanner::scan() {
    time_t start;
    time(&start);
    scannedFiles.clear();

    if (!isDirectory(rootPath)) {
        std::cerr << "The path is not a directory: " << rootPath << std::endl;
        return false;
    }

    traverseDirectory(rootPath, start);
    return true;
}

const std::vector<FileScan>& DirectoryScanner::getScannedFiles() const { return scannedFiles; }
const std::string& DirectoryScanner::getRootPath() const { return rootPath; }
size_t DirectoryScanner::getFilesCount() const { return scannedFiles.size(); }

void DirectoryScanner::scanWithOutput() {
    if (this->scan()){
        std::cout << "Scan results: " << rootPath << std::endl;
        std::cout << "Total files scanned: " << scannedFiles.size() << std::endl;
        std::cout << "Suspicious files found: " << suspFilesNum << std::endl;
        std::cout << "Number of errors: " << errorsCount << std::endl;
        std::cout << "Time: " << programTime << " s" << std::endl;
        std::cout << "=========================================" << std::endl;
        for (const auto& file : scannedFiles) {
            file.printInfo();
        }
        printStatistics();
    }
}

void DirectoryScanner::resultsLog() const {
    std::ofstream Log(logFile);
    Log << "Scan results: " << rootPath << "\n";
    Log << "Total files scanned: " << scannedFiles.size() << "\n";
    Log << "Suspicious files found: " << suspFilesNum << "\n";
    Log << "Number of errors: " << errorsCount << "\n";
    Log << "Time: " << programTime << " s\n";
    Log << "=========================================" << std::endl;

    for (const auto& file : scannedFiles) {
        if(file.getVerdict() != "clean"){
            Log << "File path: " << file.getPath() << "\n";
            Log << "File name: " << file.getFileName() << "\n";
            Log << "Size: " << file.getSize() << " bytes\n";
            Log << "Hash: " << file.getHash() << "\n";
            Log << "Verdict: " << file.getVerdict() << "\n";
            Log << "---\n";
        }
    }
    Log.close();
}

std::vector<FileScan> DirectoryScanner::getFilesByVerdict(const std::string& verdict) const {
    std::vector<FileScan> result;
    std::copy_if(scannedFiles.begin(), scannedFiles.end(),
                std::back_inserter(result),
                [verdict](const FileScan& file) {
                    return file.getVerdict() == verdict;
                });
    return result;
}

std::vector<FileScan> DirectoryScanner::findFilesByName(const std::string& fileName) const {
    std::vector<FileScan> result;
    std::copy_if(scannedFiles.begin(), scannedFiles.end(),
                std::back_inserter(result),
                [&fileName](const FileScan& file) {
                    return file.getFileName().find(fileName) != std::string::npos;
                });
    return result;
}

void DirectoryScanner::printStatistics() const {
    std::cout << "Scan statistic:" << std::endl;
    std::cout << "Total files: " << scannedFiles.size() << std::endl;

    auto cleanFiles = getFilesByVerdict("clean");

    std::cout << "Clean: " << cleanFiles.size() << std::endl;
    std::cout << "Suspicious: " << suspFilesNum << std::endl;
    std::cout << "Unknown: " << getFilesByVerdict("unknown").size() << std::endl;
}