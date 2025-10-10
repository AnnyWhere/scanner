#include <gtest/gtest.h>
#include "ScanCore.hpp"
#include <fstream>
#include <filesystem>

class DirectoryScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = "test_scan_dir";
        cleanFile1 = testDir + "/file1.txt";
        cleanFile2 = testDir + "/file2.txt";
        subDir = testDir + "/subdir";
        subDirFile = subDir + "/file3.txt";

        std::filesystem::create_directories(subDir);

        std::ofstream f1(cleanFile1);
        f1 << "Clean file 1 content";
        f1.close();

        std::ofstream f2(cleanFile2);
        f2 << "Clean file 2 content";
        f2.close();

        std::ofstream f3(subDirFile);
        f3 << "Subdirectory file content";
        f3.close();

        // Создаем базу хешей
        baseFile = "test_base.txt";
        std::ofstream base(baseFile);
        base << "5d41402abc4b2a76b9719d911017c592;malicious\n";
        base << "7d793037a0760186574b0282f2f435e7;suspicious\n";
        base.close();

        maliciousFile = testDir + "/malicious.txt";
        std::ofstream mf(maliciousFile);
        mf << "hello"; // MD5: 5d41402abc4b2a76b9719d911017c592
        mf.close();
    }

    void TearDown() override {
        if (std::filesystem::exists(testDir)) {
            std::filesystem::remove_all(testDir);
        }
        if (std::filesystem::exists(baseFile)) {
            std::filesystem::remove(baseFile);
        }
        if (std::filesystem::exists("logs")) {
            std::filesystem::remove("logs");
        }
    }

    std::string testDir;
    std::string cleanFile1;
    std::string cleanFile2;
    std::string subDir;
    std::string subDirFile;
    std::string baseFile;
    std::string maliciousFile;
};

TEST_F(DirectoryScannerTest, ConstructorWithBaseFile) {
    EXPECT_NO_THROW({
        DirectoryScanner scanner(testDir, baseFile);
    });

    EXPECT_THROW({
        DirectoryScanner scanner(testDir, "nonexistent_base.txt");
    }, std::runtime_error);
}

TEST_F(DirectoryScannerTest, ScanDirectoryStructure) {
    DirectoryScanner scanner(testDir, baseFile);

    bool scanResult = scanner.scan();
    EXPECT_TRUE(scanResult);

    EXPECT_GT(scanner.getFilesCount(), 0);
    EXPECT_GE(scanner.getSuspiciousCount(), 0);
    EXPECT_GE(scanner.getErrorsCount(), 0);
}

TEST_F(DirectoryScannerTest, MaliciousFileDetection) {
    DirectoryScanner scanner(testDir, baseFile);
    scanner.scan();

    auto suspiciousFiles = scanner.getFilesByVerdict("malicious");
    auto cleanFiles = scanner.getFilesByVerdict("clean");

    EXPECT_GT(suspiciousFiles.size(), 0);

    EXPECT_GT(cleanFiles.size(), 0);
}

TEST_F(DirectoryScannerTest, FileSearchByName) {
    DirectoryScanner scanner(testDir, baseFile);
    scanner.scan();

    auto foundFiles = scanner.findFilesByName("file1");
    EXPECT_GT(foundFiles.size(), 0);

    auto notFoundFiles = scanner.findFilesByName("nonexistent");
    EXPECT_EQ(notFoundFiles.size(), 0);
}

TEST_F(DirectoryScannerTest, StatisticsOutput) {
    DirectoryScanner scanner(testDir, baseFile);
    scanner.scan();

    EXPECT_NO_THROW({
        scanner.printStatistics();
    });
}

TEST_F(DirectoryScannerTest, LogFileCreation) {
    std::string logFile = "test_log.txt";
    DirectoryScanner scanner(testDir, baseFile, logFile);
    scanner.scan();
    EXPECT_TRUE(std::filesystem::exists(logFile));

    if (std::filesystem::exists(logFile)) {
        std::filesystem::remove(logFile);
    }
}

TEST_F(DirectoryScannerTest, InvalidDirectoryScan) {
    DirectoryScanner scanner("nonexistent_directory", baseFile);

    bool scanResult = scanner.scan();
    EXPECT_FALSE(scanResult);
}

TEST_F(DirectoryScannerTest, BaseFileParsing) {
    std::string complexBase = "complex_base.txt";
    std::ofstream base(complexBase);
    base << "# This is a comment\n";
    base << "\n";
    base << "hash1;malware1\n";
    base << "hash2;malware2\n";
    base << "# Another comment\n";
    base.close();

    EXPECT_NO_THROW({
        DirectoryScanner scanner(testDir, complexBase);
    });

    std::filesystem::remove(complexBase);
}