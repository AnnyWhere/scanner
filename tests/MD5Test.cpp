#include <gtest/gtest.h>
#include "ScanCore.hpp"

class MD5Test : public ::testing::Test {
protected:
    DirectoryScanner scanner{".", "dummy_base.txt"};
};

TEST_F(MD5Test, MD5EmptyString) {
    std::string empty = "";
    std::string hash = scanner.getMD5Hash(empty);

    EXPECT_EQ(hash, "d41d8cd98f00b204e9800998ecf8427e");
}

TEST_F(MD5Test, MD5KnownValues) {
    std::string test1 = "hello";
    std::string test2 = "world";
    std::string test3 = "The quick brown fox jumps over the lazy dog";

    EXPECT_EQ(scanner.getMD5Hash(test1), "5d41402abc4b2a76b9719d911017c592");
    EXPECT_EQ(scanner.getMD5Hash(test2), "7d793037a0760186574b0282f2f435e7");
    EXPECT_EQ(scanner.getMD5Hash(test3), "9e107d9d372bb6826bd81d3542a419d6");
}

TEST_F(MD5Test, MD5Consistency) {
    std::string content = "consistent content for testing";
    std::string hash1 = scanner.getMD5Hash(content);
    std::string hash2 = scanner.getMD5Hash(content);

    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(hash1.length(), 32); 
}