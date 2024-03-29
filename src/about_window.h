#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/window/dialog.h>

namespace ra {

class AboutWindow : public zaf::Dialog {
public:
    ZAF_DECLARE_TYPE;

    static void ShowInstance();

protected:
    void AfterParse() override;

private:
    void InitializeGithubButton();
    void InitializeLicenseButton();

private:
    ZAF_BIND_CONTROL(zaf::Label, versionLabel);
    ZAF_BIND_CONTROL(zaf::Button, githubButton);
    ZAF_BIND_CONTROL(zaf::Button, licenseButton);
};

}