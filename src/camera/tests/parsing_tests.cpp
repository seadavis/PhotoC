#include <gtest/gtest.h>
#include "camera.h"

TEST(CameraParsing, TimeLengthOK)
{
    const auto spec = "02:13:12";

    const auto expected = TimeLength(2, 13, 12);
    const auto actual = ParseTimeLength(spec);

    EXPECT_EQ(expected, actual.value());
}

TEST(CameraParsing, TimeLengthNull)
{

    const auto spec = "13:14";
    const optional<TimeLength> expected = {};
    const auto actual = ParseTimeLength(spec);

    EXPECT_EQ(expected, actual);
}