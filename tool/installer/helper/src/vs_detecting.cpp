#include "vs_detecting.h"
#include <Windows.h>
#include <atlbase.h>
#include <comutil.h>
#include <cwchar>
#include <Setup.Configuration.h>

namespace {

int GetMajorVersion(const wchar_t* version) {

    wchar_t* end{};
    auto major = std::wcstol(version, &end, 10);

    if (*end != L'.') {
        return 0;
    }

    return major;
}


VSInstallationInfo InnerGetInstallationInfo() {

    CComPtr<ISetupConfiguration> configuration;
    HRESULT hresult = CoCreateInstance(
        __uuidof(SetupConfiguration),
        nullptr, 
        CLSCTX_INPROC_SERVER, 
        __uuidof(ISetupConfiguration),
        reinterpret_cast<LPVOID*>(&configuration));

    if (FAILED(hresult)) {
        return {};
    }

    CComPtr<IEnumSetupInstances> instance_enumerator;
    hresult = configuration->EnumInstances(&instance_enumerator);
    if (FAILED(hresult)) {
        return {};
    }

    int current_major_version{};
    VSInstallationInfo installation_info;
    while (true) {

        CComPtr<ISetupInstance> instance;
        hresult = instance_enumerator->Next(1, &instance, nullptr);
        if (hresult != S_OK) {
            break;
        }

        bstr_t version{};
        hresult = instance->GetInstallationVersion(version.GetAddress());
        if (FAILED(hresult)) {
            continue;
        }

        int major_version = GetMajorVersion(version);
        if (major_version < 15) {
            //Not supported Visual studio installation.
            continue;
        }

        if (major_version <= current_major_version) {
            //Always get information from the latest installation.
            continue;
        }
        
        CComPtr<ISetupInstance2> instance2;
        hresult = instance->QueryInterface<ISetupInstance2>(&instance2);
        if (FAILED(hresult)) {
            continue;
        }

        bstr_t engine_path;
        hresult = instance2->GetEnginePath(engine_path.GetAddress());
        if (FAILED(hresult)) {
            continue;
        }

        //Reference: https://github.com/microsoft/vswhere/wiki/Find-VSIXInstaller
        installation_info.vsix_installer_path = static_cast<const wchar_t*>(engine_path);
        installation_info.vsix_installer_path /= "vsixinstaller.exe";

        installation_info.version_type =
            major_version >= 17 ? VSVersionType::VS2022OrNewer : VSVersionType::VS2019OrOlder;

        current_major_version = major_version;
    }

    return installation_info;
}


VSInstallationInfo GetInstallationInfo() {

    CoInitialize(nullptr);
    auto result = InnerGetInstallationInfo();
    CoUninitialize();
    return result;
}

}


const VSInstallationInfo& GetVSInstallationInfo() {

    static const VSInstallationInfo result = GetInstallationInfo();
    return result;
}