#include <gtest/gtest.h>
#include <filesystem>
#include "files.h"

const std::string allowed_path = "./src/io/tests/allowedfolders";

class FileTest : public ::testing::Test {
 protected:
  void SetUp() override {
   
    auto savePath = fs::path(allowed_path);
    if(!fs::exists(savePath))
    {
        fs::create_directory(savePath);
    }

  }

  void TearDown() override{

    fs::remove_all(allowed_path);

  }

};

TEST_F(FileTest, Save)
{
    Mat expectedMat(Size(500, 500), CV_64FC1, Scalar(55, 55, 55));
    io::save(expectedMat, "./src/io/tests/allowedfolders/out.png");
    Mat result = imread("./src/io/tests/allowedfolders/out.png");

    ASSERT_TRUE(result.size() == expectedMat.size());
}