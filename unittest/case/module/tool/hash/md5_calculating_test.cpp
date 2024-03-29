#include <gtest/gtest.h>
#include <zaf/application.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/crypto/md5.h>
#include "module/tool/hash/hash_calculating.h"

using namespace ra::mod;
using namespace ra::mod::tool::hash;

namespace {

zaf::crypto::HashAlgorithm MD5Creator() {
	return zaf::crypto::MD5{};
}

}

TEST(MD5CalculatingTest, File) {

	struct TestItem {
		const wchar_t* file_name;
		const wchar_t* md5;
	} test_items[] = {
	
		{ L"md5_test_file1", L"BE70C81C90E0AFFB615D34AD2BFEC874" },
		{ L"md5_test_file2", L"089B4F9ECD4DEFAA036039AD89044EA2" },
		{ L"md5_test_file3", L"F39F3B6A346D3C6C44A8E49A265D8126" },
	};

	std::filesystem::path directory_path{ __FILE__ };
	directory_path = directory_path.parent_path();

	for (const auto& each_item : test_items) {

		std::condition_variable cv;
		std::mutex lock;
		std::unique_lock<std::mutex> unique_lock{ lock };

		std::wstring md5;

		zaf::Application::Instance().Subscriptions() += 
			CalculateFileHash(directory_path / each_item.file_name, MD5Creator).Subscribe(
			[&md5](const HashResult& result) {
		
				md5 = result.result;
			}, 
			[&cv, &lock]() {

				std::scoped_lock<std::mutex> lock_guard(lock);
				cv.notify_all();
			}
		);

		cv.wait(unique_lock);

		ASSERT_EQ(md5, zaf::ToLowercased(each_item.md5));
	}
}


TEST(MD5CalculatingTest, String) {

	auto utf8_md5 = CalculateStringHash(L"CalculateStringMD5", TextEncoding::UTF8, MD5Creator);
	ASSERT_EQ(utf8_md5, zaf::ToLowercased(L"16135E49BA3E60456D5454B5B3E27B87"));

	auto utf16_md5 = CalculateStringHash(L"CalculateStringMD5", TextEncoding::UTF16, MD5Creator);
	ASSERT_EQ(utf16_md5, zaf::ToLowercased(L"F51CA9CDF4560B2D4EDC81A8682450E2"));
}