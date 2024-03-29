#include <gtest/gtest.h>
#include "module/active_path/active_path_option_parsing.h"

using namespace ra::mod::active_path;

TEST(ActivePathOptionParsingTest, ParseInvalidTest) {

	auto option = ParseActivePathOption(L"");
	ASSERT_EQ(option.backward_level, 0);
	ASSERT_EQ(option.use_name, false);
	ASSERT_EQ(option.use_workspace_path, false);
	ASSERT_FALSE(option.overriding_path.has_value());

	option = ParseActivePathOption(L"??");
	ASSERT_EQ(option.backward_level, 0);
	ASSERT_EQ(option.use_name, false);
	ASSERT_EQ(option.use_workspace_path, false);
	ASSERT_FALSE(option.overriding_path.has_value());

	option = ParseActivePathOption(L"m");
	ASSERT_EQ(option.backward_level, 0);
	ASSERT_EQ(option.use_name, false);
	ASSERT_EQ(option.use_workspace_path, false);
	ASSERT_FALSE(option.overriding_path.has_value());
}


TEST(ActivePathOptionParsingTest, ParseUseWorkspace) {

	auto option = ParseActivePathOption(L"~");
	ASSERT_EQ(option.use_workspace_path, true);
}


TEST(ActivePathOptionParsingTest, ParseBackwardLevel) {

	auto option = ParseActivePathOption(L".");
	ASSERT_EQ(option.backward_level, 1);

	option = ParseActivePathOption(L"..");
	ASSERT_EQ(option.backward_level, 2);

	option = ParseActivePathOption(L"...");
	ASSERT_EQ(option.backward_level, 3);

	option = ParseActivePathOption(L"....");
	ASSERT_EQ(option.backward_level, 4);
}


TEST(ActivePathOptionParsingTest, ParseUseName) {

	auto option = ParseActivePathOption(L"n");
	ASSERT_EQ(option.use_name, true);
}


TEST(ActivePathOptionParsingTest, ParseOverridingPath) {

	auto option = ParseActivePathOption(L"=");
	ASSERT_EQ(option.overriding_path, L"");

	option = ParseActivePathOption(L"=1231231");
	ASSERT_EQ(option.overriding_path, L"1231231");
}


TEST(ActivePathOptionParsingTest, Combine) {

	auto option = ParseActivePathOption(L"~..n");
	ASSERT_EQ(option.backward_level, 2);
	ASSERT_EQ(option.use_name, true);
	ASSERT_EQ(option.use_workspace_path, true);
	ASSERT_FALSE(option.overriding_path.has_value());

	option = ParseActivePathOption(L"~...");
	ASSERT_EQ(option.backward_level, 3);
	ASSERT_EQ(option.use_name, false);
	ASSERT_EQ(option.use_workspace_path, true);
	ASSERT_FALSE(option.overriding_path.has_value());

	option = ParseActivePathOption(L"~n");
	ASSERT_EQ(option.backward_level, 0);
	ASSERT_EQ(option.use_name, true);
	ASSERT_EQ(option.use_workspace_path, true);
	ASSERT_FALSE(option.overriding_path.has_value());

	option = ParseActivePathOption(L".n");
	ASSERT_EQ(option.backward_level, 1);
	ASSERT_EQ(option.use_name, true);
	ASSERT_EQ(option.use_workspace_path, false);
	ASSERT_FALSE(option.overriding_path.has_value());
}