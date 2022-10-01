#include <gtest/gtest.h>
#include "module/tool/md5/md5_command.h"

using namespace ra::module::tool::md5;

TEST(MD5CommandTest, ParseSucceeded) {

    {
        ra::utility::CommandLine command_line(L"md5");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 \"");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 \"what is this\"");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result.string, L"what is this");
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /u8");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result.string, L"help");
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /u16");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF16);
        ASSERT_EQ(result.string, L"help");
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /c");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result.string, L"help");
        ASSERT_EQ(result.use_uppercase, true);
    }
}


/*
Move to unit test of ToolModule
TEST(MD5CommandTest, ParseFailed) {

    {
        ra::utility::CommandLine command_line(L"");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.has_value());
    }

    {
        ra::utility::CommandLine command_line(L"md55");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.has_value());
    }

    {
        ra::utility::CommandLine command_line(L"abc");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.has_value());
    }
}
*/