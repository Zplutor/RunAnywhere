#pragma once

#include <zaf/base/none.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/rich_edit/ole_callback.h>
#include <zaf/rx/subject.h>
#include "main/text_block_object.h"
#include "utility/command_line.h"

namespace ra {

class CommandInputEdit : public zaf::RichEdit, public zaf::rich_edit::OLECallback {
public:
    ZAF_DECLARE_TYPE;

    utility::CommandLine GetInputCommandLine();

    //Use this event to observe input command changed event, rather than TextChangedEvent.
    zaf::Observable<zaf::None> CommandChangedEvent();

protected:
    void Initialize() override;
    void OnTextChanging(const zaf::TextChangingInfo& event_info) override;
    void OnTextChanged(const zaf::TextChangedInfo& event_info) override;
    void OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) override;

private:
    static bool ShouldInsertTextBlockObject(const std::wstring& text);

private:
    bool TryToInsertTextBlockObject();
    zaf::COMObject<TextBlockObject> InsertTextBlockObject(const std::wstring& text);
    void RaiseCommandChangedEvent();
    void InsertTextBlockObjectByKey();

    zaf::rich_edit::OperationResult GetClipboardData(
        zaf::rich_edit::ClipboardOperation operation,
        const zaf::TextRange& text_range,
        zaf::clipboard::DataObject& data_object) override;

private:
    zaf::Subject<zaf::None> command_changed_event_;
};

}