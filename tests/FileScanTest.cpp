#include <gtest/gtest.h>
#include "ScanCore.hpp"
#include <fstream>
#include <filesystem>

class FileScanTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFilePath = "test_file.txt";
        std::ofstream file(testFilePath);
        file << "This is a test file content for unit testing.";
        file.close();
    }

    void TearDown() override {
        if (std::filesystem::exists(testFilePath)) {
            std::filesystem::remove(testFilePath);
        }
    }

    std::string testFilePath;
};

TEST_F(FileScanTest, ConstructorAndGetters) {
    FileScan fileScan(testFilePath, "clean");

    EXPECT_EQ(fileScan.getPath(), testFilePath);
    EXPECT_EQ(fileScan.getVerdict(), "clean");
    EXPECT_FALSE(fileScan.getContent().empty());
    EXPECT_GT(fileScan.getSize(), 0);
}

TEST_F(FileScanTest, FileNameExtraction) {
    FileScan fileScan(testFilePath, "clean");

    std::string fileName = fileScan.getFileName();
    EXPECT_EQ(fileName, "test_file.txt");
}

TEST_F(FileScanTest, SetVerdictAndHash) {
    FileScan fileScan(testFilePath, "unknown");

    fileScan.setVerdict("malicious");
    fileScan.setHash("test_hash_123");

    EXPECT_EQ(fileScan.getVerdict(), "malicious");
    EXPECT_EQ(fileScan.getHash(), "test_hash_123");
}

TEST_F(FileScanTest, ContentOperations) {
    FileScan fileScan(testFilePath, "clean");

    EXPECT_FALSE(fileScan.isEmpty());
    EXPECT_TRUE(fileScan.contains("test file content"));
    EXPECT_FALSE(fileScan.contains("nonexistent content"));
}

TEST_F(FileScanTest, InvalidFileThrowsException) {
    EXPECT_THROW({
        FileScan invalidFile("nonexistent_file.txt", "unknown");
    }, std::string);
}

TEST_F(FileScanTest, PrintInfoNoCrash) {
    FileScan fileScan(testFilePath, "clean");

    EXPECT_NO_THROW({
        fileScan.printInfo();
    });
}