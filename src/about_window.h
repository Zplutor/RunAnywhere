#pragma once

#include <zaf/window/dialog.h>

class AboutWindow : public zaf::Dialog {
public:
    ZAF_DECLARE_REFLECTION_TYPE;

    static void ShowInstance();
};