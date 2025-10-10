#ifndef SCANCORE_HPP
#define SCANCORE_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <cmath>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <cstring>

// Кроссплатформенные макросы для экспорта
#if defined(_WIN32) || defined(_WIN64)
    #ifdef SCANCORE_EXPORTS
        #define SCAN_API __declspec(dllexport)
    #else
        #define SCAN_API __declspec(dllimport)
    #endif
    #include <windows.h>
    #include <fileapi.h>
    #include <direct.h>
#else
    #ifdef SCANCORE_EXPORTS
        #define SCAN_API __attribute__((visibility("default")))
    #else
        #define SCAN_API
    #endif
    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

// Класс для представления сканированного файла
class SCAN_API FileScan {
private:
    std::string filePath;
    std::string fileContent;
    std::string scanVerdict;
    size_t fileSize;
    std::string hash;

public:
    FileScan(const std::string& path, const std::string& verdict = "unknown");
    virtual ~FileScan();

    const std::string& getPath() const;
    const std::string& getContent() const;
    const std::string& getVerdict() const;
    const std::string &getHash() const;
    size_t getSize() const;
    std::string getFileName() const;

    void setContent();
    void setVerdict(const std::string& verdict);
    void setHash(const std::string& hash_str);

    bool contains(const std::string& pattern) const;
    bool isEmpty() const;
    void printInfo() const;
};

// Сканнер с реализованной функцией хэширования md5
class SCAN_API DirectoryScanner {
private:
    std::string rootPath;
    std::map<std::string, std::string> baseScan;
    std::vector<FileScan> scannedFiles;
    std::string logFile;
    int suspFilesNum;
    int errorsCount;
    double programTime;

    std::string md5(const std::string& data);
    unsigned int rotateLeft(unsigned int val, unsigned int shift);
    unsigned int fullF(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
        unsigned int p, unsigned int v, unsigned int sh);
    unsigned int fullG(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
        unsigned int p, unsigned int v, unsigned int sh);
    unsigned int fullH(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
        unsigned int p, unsigned int v, unsigned int sh);
    unsigned int fullI(unsigned int op, unsigned int op1, unsigned int x, unsigned int y, unsigned int z,
        unsigned int p, unsigned int v, unsigned int sh);
    std::string md5Core(unsigned char *data_mas, int size);
    static bool isDirectory(const std::string& path);
    void traverseDirectory(const std::string& directoryPath, time_t start);
    const std::string analyzeFile(FileScan* file);

public:
    DirectoryScanner(const std::string& path, const std::string& baseFile);
    DirectoryScanner(const std::string& path, const std::string& baseFile, const std::string& log);
    virtual ~DirectoryScanner();

    bool scan();
    void scanWithOutput();
    void resultsLog() const;

    const std::vector<FileScan>& getScannedFiles() const;
    const std::string& getRootPath() const;
    size_t getFilesCount() const;
    int getSuspiciousCount() const { return suspFilesNum; }
    int getErrorsCount() const { return errorsCount; }
    double getScanTime() const { return programTime; }

    std::string getMD5Hash(const std::string& data) {
        return md5(data);
    }

    std::vector<FileScan> getFilesByVerdict(const std::string& verdict) const;
    std::vector<FileScan> findFilesByName(const std::string& fileName) const;
    void printStatistics() const;
};

#endif