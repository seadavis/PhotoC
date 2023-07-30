#include <gtest/gtest.h>
#include "camera.h"

class CameraParsingValidInputTest :
    public testing::TestWithParam<tuple<string, TimeLength>> {
};

INSTANTIATE_TEST_SUITE_P(CameraParsing,
                          CameraParsingValidInputTest,
                          testing::Values(
                              make_tuple("02:13:12", TimeLength(2, 13, 12)),
                              make_tuple("01:11:09", TimeLength(1, 11, 9)),
                              make_tuple("2:3:1", TimeLength(2, 3, 1)),
                              make_tuple("2:30:1", TimeLength(2, 30, 1))
                          ) );

class CameraParsingInvalidInputTest :
    public testing::TestWithParam<string> {
};

INSTANTIATE_TEST_SUITE_P(CameraParsing,
                          CameraParsingInvalidInputTest,
                          testing::Values(
                              "13:14",
                              "13",
                              ":12:11",
                              ":12:11aa:bb:cc",
                              "aa:bb:cc",
                              "aa:bb",
                              "aa",
                              ""
                          ) );


TEST_P(CameraParsingValidInputTest, ParsesValidInput)
{
    auto args = GetParam();

    const auto spec = get<0>(args);

    const auto expected = get<1>(args);
    const auto actual = ParseTimeLength(spec);

    EXPECT_EQ(expected, actual.value());
}

TEST_P(CameraParsingInvalidInputTest , ParsesInvalidInput)
{

    const auto spec = GetParam();
    const optional<TimeLength> expected = {};
    const auto actual = ParseTimeLength(spec);

    EXPECT_EQ(expected, actual);
}