#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/chat_gpt/comm/chat_completion.h"

namespace ra::mod::chat_gpt::comm {

class OpenAIClientImpl;

/**
A module that communicates with OpenAI server via network.
*/
class OpenAIClient : zaf::NonCopyableNonMovable {
public:
    OpenAIClient();
    ~OpenAIClient();

    zaf::Observable<ChatCompletion> CreateChatCompletion(
        const std::vector<const Message*>& messages);

private:
    static zaf::Observable<ChatCompletion> CreateMockChatCompletion();

    static std::string CreateRequestBody(const std::vector<const Message*>& messages);
    static std::optional<ChatCompletion> ParseChatCompletion(const std::string& response);
    static std::string ParseErrorMessage(const std::string& response);

private:
    std::wstring api_key_;

    std::unique_ptr<OpenAIClientImpl> impl_;
};

}