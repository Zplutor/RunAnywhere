#include "module/tool/hex/hex_preview_control.h"
#include <fstream>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"
#include "utility/numeric_text_formatting.h"

namespace ra::mod::tool::hex {
namespace {

template<bool IsHistorical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr std::size_t LinesPerPage = 8;
};

template<>
struct StyleMetrics<true> {
    static constexpr std::size_t LinesPerPage = 2;
};

std::wstring FormatInteger(std::uint64_t integer) {
    auto result = std::to_wstring(integer);
    utility::InsertSeparatorToNumericText(result, 10, L',');
    return result;
}

}

ZAF_DEFINE_TYPE(HexPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/hex/hex_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void HexPreviewControl::OnStyleChanged() {

    binaryContent->ChangeStyle(Style());
    errorView->ChangeStyle(Style());
}


void HexPreviewControl::ShowContent(const GeneralInput& input, const zaf::Range& file_range) {

    auto update_guard = BeginUpdate();

    if (auto file_path = input.GetFile()) {
        ShowFileContent(*file_path, file_range);
    }
    else if (auto text = input.GetText()) {
        ShowTextContent(text->content, text->encoding);
    }
}


void HexPreviewControl::ShowTextContent(const std::wstring& text, TextEncoding encoding) {

    contentStatusBar->ShowText(text, encoding);

    auto text_binary = CreateTextBinary(text, encoding);
    ShowBinary(text_binary);
    ShowTextInfo(text, text_binary);
}


std::vector<std::byte> HexPreviewControl::CreateTextBinary(
    const std::wstring& text, 
    TextEncoding encoding) {

    std::vector<std::byte> result;
    if (encoding == TextEncoding::UTF8) {

        auto utf8_string = zaf::ToUTF8String(text);
        result.resize(utf8_string.size());
        std::memcpy(&result[0], utf8_string.data(), utf8_string.size());
    }
    else if (encoding == TextEncoding::UTF16) {

        auto size = text.size() * sizeof(wchar_t);
        result.resize(size);
        std::memcpy(&result[0], text.data(), size);
    }
    return result;
}


void HexPreviewControl::ShowTextInfo(
    const std::wstring& text, 
    const std::vector<std::byte>& binary) {

    std::wstring info_string;
    info_string += L"Text length: ";
    info_string += FormatInteger(text.length());

    info_string += L"    Byte count: ";
    info_string += FormatInteger(binary.size());

    infoLabel->SetText(info_string);
    infoLabel->SetIsVisible(true);
}


void HexPreviewControl::ShowFileContent(
    const std::filesystem::path& file_path,
    const zaf::Range& range) {

    contentStatusBar->ShowFile(file_path);

    FileContentInfo content_info;
    auto read_file_status = ReadFileContent(file_path, range, content_info);

    ShowFileInfo(read_file_status, content_info, range);
    ShowBinary(content_info.data);
    ShowMessage(read_file_status, content_info);
}


void HexPreviewControl::ShowFileInfo(
    ReadFileStatus status,
    const FileContentInfo& content_info,
    const zaf::Range& range) {

    if (status == ReadFileStatus::ReadFileFailed) {
        infoLabel->SetIsVisible(false);
        return;
    }

    infoLabel->SetIsVisible(true);

    std::wstring text;
    text += L"File size: ";
    text += FormatInteger(content_info.file_size);

    if (status == ReadFileStatus::OK) {

        auto range_end = range.index;
        if (!content_info.data.empty()) {
            range_end += content_info.data.size() - 1;
        }

        text += L"    Range: ";
        text += FormatInteger(range.index);
        text += L'-';
        text += FormatInteger(range_end);

        text += L"    Length: ";
        text += FormatInteger(content_info.data.size());
    }

    infoLabel->SetText(text);
}


void HexPreviewControl::ShowBinary(const std::vector<std::byte>& binary) {

    if (!binary.empty()) {
        binaryContent->SetBinary(binary);
        binaryContent->SetIsVisible(true);
    }
    else {
        binaryContent->SetIsVisible(false);
    }
}


const std::vector<std::byte>& HexPreviewControl::GetBinary() const {
    return binaryContent->GetBinary();
}


void HexPreviewControl::ShowMessage(
    ReadFileStatus status, 
    const FileContentInfo& content_info) {

    std::wstring message;
    switch (status) {
    case ReadFileStatus::InvalidPosition:
        message = L"Position exceeds file size";
        break;
    case ReadFileStatus::ReadFileFailed:
        message = ErrorMessages::UnableToReadFile;
        break;
    default:
        if (content_info.data.empty()) {
            message = ErrorMessages::NoContentToDisplay;
        }
        break;
    }

    if (!message.empty()) {
        errorView->ShowErrorText(message);
        errorView->SetIsVisible(true);
    }
    else {
        errorView->SetIsVisible(false);
    }
}


zaf::Frame HexPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.left = 0;
    result.right = 0;
    return result;
}


HexPreviewControl::ReadFileStatus HexPreviewControl::ReadFileContent(
    const std::filesystem::path& file_path,
    const zaf::Range& range,
    FileContentInfo& content_info) {

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file_stream.open(file_path, std::ifstream::in | std::ifstream::binary);

        file_stream.seekg(0, std::ifstream::end);
        content_info.file_size = file_stream.tellg();
        if (content_info.file_size == 0) {
            return ReadFileStatus::EmptyFile;
        }

        if (range.index >= content_info.file_size) {
            return ReadFileStatus::InvalidPosition;
        }

        auto can_read_length = 
            content_info.file_size - static_cast<std::streampos>(range.index);

        //Change read length to default length if it is 0, which has no meaning.
        auto expected_read_length = 
            range.length > 0 ? range.length : HexCommandParseResult::DefaultLength;

        auto buffer_length = std::min({ 
            static_cast<std::streampos>(can_read_length),
            static_cast<std::streampos>(expected_read_length),
            static_cast<std::streampos>(HexCommandParseResult::MaxLength)
        });

        //No need to read file if length is 0.
        if (buffer_length == 0) {
            return ReadFileStatus::OK;
        }

        content_info.data.resize(buffer_length);

        file_stream.seekg(range.index, std::ifstream::beg);
        file_stream.read(reinterpret_cast<char*>(&content_info.data[0]), buffer_length);

        return ReadFileStatus::OK;
    }
    catch (const std::ifstream::failure&) {
        return ReadFileStatus::ReadFileFailed;
    }
}

}