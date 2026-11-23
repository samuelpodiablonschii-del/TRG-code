#define NOMINMAX
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

#include "resource.h"

namespace {
constexpr int downloadButtonId = 301;
constexpr int progressBarId = 302;
constexpr int statusLabelId = 303;
constexpr int storageLabelId = 304;
constexpr UINT runtimeResourceId = 201;
constexpr UINT exampleResourceId = 202;
constexpr UINT readmeResourceId = 203;
constexpr UINT iconResourceId = 204;
constexpr UINT editor2DResourceId = 205;
constexpr UINT editor3DResourceId = 206;

HWND installerWindow = nullptr;
HWND installButton = nullptr;
HWND progressBar = nullptr;
HWND statusLabel = nullptr;
HWND storageLabel = nullptr;
std::filesystem::path installDirectory;
std::filesystem::path runtimePath;
std::filesystem::path examplePath;
std::filesystem::path shortcutPath;
std::uint64_t requiredStorage = 0;
std::uint64_t installerFileSize = 0;
std::uint64_t completedStorage = 0;
bool installed = false;

std::wstring systemMessage(const std::wstring& operation) {
    return operation + L" (Windows error " + std::to_wstring(GetLastError()) + L").";
}

std::uint64_t embeddedSize(UINT resourceId) {
    HRSRC resource = FindResourceW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(resourceId), MAKEINTRESOURCEW(10));
    if (!resource) throw std::runtime_error("An installation file is missing from this installer.");
    return SizeofResource(GetModuleHandleW(nullptr), resource);
}

bool matchesEmbeddedFile(UINT resourceId, const std::filesystem::path& path) {
    std::error_code fileError;
    if (!std::filesystem::exists(path, fileError)) {
        if (fileError) throw std::runtime_error("Could not check the existing Desktop example.");
        return false;
    }
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), MAKEINTRESOURCEW(10));
    if (!resource) throw std::runtime_error("The Desktop example is missing from this installer.");
    HGLOBAL loaded = LoadResource(module, resource);
    const auto* data = static_cast<const unsigned char*>(LockResource(loaded));
    const DWORD size = SizeofResource(module, resource);
    if (!loaded || !data) throw std::runtime_error("Could not read the Desktop example from this installer.");
    const std::uint64_t existingSize = std::filesystem::file_size(path, fileError);
    if (fileError) throw std::runtime_error("Could not read the existing Desktop example.");
    if (existingSize != size) return false;
    std::vector<unsigned char> existing(static_cast<std::size_t>(size));
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Could not read the existing Desktop example.");
    input.read(reinterpret_cast<char*>(existing.data()), static_cast<std::streamsize>(existing.size()));
    return input && std::equal(data, data + size, existing.begin());
}

std::filesystem::path desktopExamplePath(const std::filesystem::path& desktop) {
    std::filesystem::path candidate = desktop / L"TRG-Example.trg";
    if (matchesEmbeddedFile(exampleResourceId, candidate)) return candidate;
    std::error_code error;
    if (!std::filesystem::exists(candidate, error)) {
        if (error) throw std::runtime_error("Could not check the Desktop for an existing example.");
        return candidate;
    }
    for (unsigned suffix = 1; suffix <= 9999; ++suffix) {
        candidate = desktop / (L"TRG-Example (TRG " + std::to_wstring(suffix) + L").trg");
        if (!std::filesystem::exists(candidate, error)) {
            if (error) throw std::runtime_error("Could not choose a Desktop example filename.");
            return candidate;
        }
        if (matchesEmbeddedFile(exampleResourceId, candidate)) return candidate;
    }
    throw std::runtime_error("Could not choose a free filename for the Desktop example.");
}

std::filesystem::path desktopShortcutPath(const std::filesystem::path& desktop) {
    std::error_code error;
    std::filesystem::path candidate = desktop / L"TRG.lnk";
    if (!std::filesystem::exists(candidate, error)) {
        if (error) throw std::runtime_error("Could not check the Desktop for an existing TRG shortcut.");
        return candidate;
    }
    if (error) throw std::runtime_error("Could not check the Desktop for an existing TRG shortcut.");
    for (unsigned suffix = 1; suffix <= 9999; ++suffix) {
        candidate = desktop / (L"TRG (TRG " + std::to_wstring(suffix) + L").lnk");
        if (!std::filesystem::exists(candidate, error)) {
            if (error) throw std::runtime_error("Could not choose a Desktop shortcut filename.");
            return candidate;
        }
    }
    throw std::runtime_error("Could not choose a free filename for the Desktop shortcut.");
}

void createDesktopShortcut() {
    IShellLinkW* shellLink = nullptr;
    HRESULT result = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                                      IID_IShellLinkW, reinterpret_cast<void**>(&shellLink));
    if (FAILED(result)) throw std::runtime_error("Could not create the TRG Desktop shortcut.");

    result = shellLink->SetPath(runtimePath.c_str());
    if (SUCCEEDED(result)) result = shellLink->SetWorkingDirectory(installDirectory.c_str());
    if (SUCCEEDED(result)) result = shellLink->SetIconLocation((installDirectory / L"trg.ico").c_str(), 0);
    if (SUCCEEDED(result)) result = shellLink->SetDescription(L"Open TRG or run a .trg program");

    IPersistFile* persistFile = nullptr;
    if (SUCCEEDED(result)) result = shellLink->QueryInterface(IID_IPersistFile,
                                      reinterpret_cast<void**>(&persistFile));
    if (SUCCEEDED(result)) result = persistFile->Save(shortcutPath.c_str(), TRUE);

    if (persistFile) persistFile->Release();
    shellLink->Release();
    if (FAILED(result)) {
        throw std::runtime_error("Could not save the TRG Desktop shortcut (HRESULT " +
                                 std::to_string(static_cast<unsigned long>(result)) + ").");
    }
}

std::wstring formatMiB(std::uint64_t bytes) {
    const double mib = static_cast<double>(bytes) / (1024.0 * 1024.0);
    wchar_t result[64] = {};
    swprintf_s(result, L"%.2f MiB (%llu bytes)", mib, static_cast<unsigned long long>(bytes));
    return result;
}

void pumpMessages() {
    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

void updateProgress(const std::wstring& status) {
    if (statusLabel) SetWindowTextW(statusLabel, status.c_str());
    if (progressBar && requiredStorage) {
        const int position = static_cast<int>(completedStorage * 1000 / requiredStorage);
        SendMessageW(progressBar, PBM_SETPOS, static_cast<WPARAM>(position), 0);
    }
    if (installerWindow) {
        UpdateWindow(installerWindow);
        pumpMessages();
    }
}

void writeEmbeddedFile(UINT resourceId, const std::filesystem::path& destination,
                       const std::wstring& displayName) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), MAKEINTRESOURCEW(10));
    if (!resource) throw std::runtime_error("Could not find an embedded installation file.");
    HGLOBAL loaded = LoadResource(module, resource);
    const auto* data = static_cast<const unsigned char*>(LockResource(loaded));
    const DWORD size = SizeofResource(module, resource);
    if (!loaded || !data || size == 0) throw std::runtime_error("Could not read an embedded installation file.");

    updateProgress(L"Installing " + displayName + L"...");
    HANDLE output = CreateFileW(destination.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL, nullptr);
    if (output == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        throw std::runtime_error("Could not create installation file '" +
            destination.string() + "' (Windows error " + std::to_string(error) +
            "). Check folder permissions, antivirus protection, and available disk space.");
    }

    constexpr DWORD chunkSize = 64 * 1024;
    DWORD offset = 0;
    while (offset < size) {
        const DWORD count = (std::min)(chunkSize, size - offset);
        DWORD written = 0;
        if (!WriteFile(output, data + offset, count, &written, nullptr) || written != count) {
            const DWORD error = GetLastError();
            CloseHandle(output);
            throw std::runtime_error("Could not write installation file '" +
                destination.string() + "' (Windows error " + std::to_string(error) + ").");
        }
        offset += written;
        completedStorage += written;
        updateProgress(L"Installing " + displayName + L"...");
        Sleep(12);
    }

    if (!FlushFileBuffers(output)) {
        const DWORD error = GetLastError();
        CloseHandle(output);
        throw std::runtime_error("Could not finish writing installation file '" +
            destination.string() + "' (Windows error " + std::to_string(error) + ").");
    }
    CloseHandle(output);
}

[[maybe_unused]] void setRegistryString(HKEY root, const std::wstring& keyName, const std::wstring& valueName,
                       const std::wstring& value) {
    HKEY key = nullptr;
    const LONG openResult = RegCreateKeyExW(root, keyName.c_str(), 0, nullptr, 0,
        KEY_SET_VALUE | KEY_WOW64_64KEY, nullptr, &key, nullptr);
    if (openResult != ERROR_SUCCESS) throw std::runtime_error("Could not register .trg file opening.");
    const LONG setResult = RegSetValueExW(key, valueName.empty() ? nullptr : valueName.c_str(), 0,
        REG_SZ, reinterpret_cast<const BYTE*>(value.c_str()),
        static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t)));
    RegCloseKey(key);
    if (setResult != ERROR_SUCCESS) throw std::runtime_error("Could not register .trg file opening.");
}

void registerTrgFileType() {
#ifdef TRG_INSTALLER_TEST
    return;
#else
    const std::wstring applicationId = L"TRG.SourceFile";
    setRegistryString(HKEY_LOCAL_MACHINE, L"Software\\Classes\\.trg", L"", applicationId);
    setRegistryString(HKEY_LOCAL_MACHINE, L"Software\\Classes\\" + applicationId, L"", L"TRG source file");
    setRegistryString(HKEY_LOCAL_MACHINE,
        L"Software\\Classes\\" + applicationId + L"\\DefaultIcon", L"",
        L"\\\"" + runtimePath.wstring() + L"\\\",0");
    setRegistryString(HKEY_LOCAL_MACHINE,
        L"Software\\Classes\\" + applicationId + L"\\shell\\open\\command", L"",
        L"\\\"" + runtimePath.wstring() + L"\\\" \\\"%1\\\"");
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);
#endif
}

void installPackage() {
    EnableWindow(installButton, FALSE);
    completedStorage = 0;
    SendMessageW(progressBar, PBM_SETPOS, 0, 0);
    const HRESULT comResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool uninitializeCom = SUCCEEDED(comResult);
    if (FAILED(comResult) && comResult != RPC_E_CHANGED_MODE) {
        const std::wstring message = L"Could not initialize Windows shortcut support (HRESULT " +
            std::to_wstring(static_cast<unsigned long>(comResult)) + L").";
        updateProgress(L"Installation failed.");
        MessageBoxW(installerWindow, message.c_str(), L"TRG Installer error", MB_OK | MB_ICONERROR);
        EnableWindow(installButton, TRUE);
        return;
    }
    try {
        std::error_code error;
        std::filesystem::create_directories(installDirectory, error);
        if (error) throw std::runtime_error("Could not create the Program Files installation folder.");

        writeEmbeddedFile(runtimeResourceId, runtimePath, L"TRG.exe");
        writeEmbeddedFile(readmeResourceId, installDirectory / L"README.md", L"README.md");
        writeEmbeddedFile(iconResourceId, installDirectory / L"trg.ico", L"trg.ico");
        writeEmbeddedFile(editor2DResourceId, installDirectory / L"2DEditor.trg", L"2DEditor.trg");
        writeEmbeddedFile(editor3DResourceId, installDirectory / L"3DEditor.trg", L"3DEditor.trg");
        writeEmbeddedFile(exampleResourceId, examplePath,
            examplePath.filename().wstring() + L" on the Desktop");
        updateProgress(L"Creating the TRG Desktop shortcut...");
        createDesktopShortcut();
        registerTrgFileType();

        completedStorage = requiredStorage;
        SendMessageW(progressBar, PBM_SETPOS, 1000, 0);
        updateProgress(L"TRG was installed successfully. The app shortcut and interactive example are on your Desktop.");
        SetWindowTextW(installButton, L"Open Desktop Example");
        EnableWindow(installButton, TRUE);
        installed = true;
    } catch (const std::exception& error) {
        const std::wstring message(error.what(), error.what() + strlen(error.what()));
        updateProgress(L"Installation failed. Some files may have been installed; see the error details.");
        MessageBoxW(installerWindow, message.c_str(), L"TRG Installer error", MB_OK | MB_ICONERROR);
        EnableWindow(installButton, TRUE);
    }
    if (uninitializeCom) CoUninitialize();
}

void openExample() {
    const HINSTANCE result = ShellExecuteW(installerWindow, L"open", runtimePath.c_str(),
        (L"\\\"" + examplePath.wstring() + L"\\\"").c_str(), installDirectory.c_str(), SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) <= 32) {
        MessageBoxW(installerWindow, L"TRG was installed, but the example could not be opened.",
                    L"TRG Installer", MB_OK | MB_ICONERROR);
    }
}

bool isElevated() {
#ifdef TRG_INSTALLER_TEST
    return true;
#else
    HANDLE token = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) return false;
    TOKEN_ELEVATION elevation = {};
    DWORD returned = 0;
    const BOOL queried = GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &returned);
    CloseHandle(token);
    return queried && elevation.TokenIsElevated != 0;
#endif
}

LRESULT CALLBACK installerWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        installerWindow = window;
        CreateWindowExW(0, L"STATIC",
            L"TRG includes its runtime, purple icon, README, and interactive desktop example. "
            L"The installer does not include TRG.cpp or require an internet connection.",
            WS_CHILD | WS_VISIBLE | SS_LEFT, 24, 88, 532, 54, window, nullptr,
            GetModuleHandleW(nullptr), nullptr);
        storageLabel = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
            24, 147, 532, 45, window, reinterpret_cast<HMENU>(storageLabelId),
            GetModuleHandleW(nullptr), nullptr);
        progressBar = CreateWindowExW(0, PROGRESS_CLASSW, L"",
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 24, 198, 532, 24, window,
            reinterpret_cast<HMENU>(progressBarId), GetModuleHandleW(nullptr), nullptr);
        SendMessageW(progressBar, PBM_SETRANGE32, 0, 1000);
        statusLabel = CreateWindowExW(0, L"STATIC", L"Ready to install.",
            WS_CHILD | WS_VISIBLE | SS_LEFT, 24, 230, 532, 28, window,
            reinterpret_cast<HMENU>(statusLabelId), GetModuleHandleW(nullptr), nullptr);
        installButton = CreateWindowExW(0, L"BUTTON", L"Download / Install",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, 390, 266, 166, 36,
            window, reinterpret_cast<HMENU>(downloadButtonId), GetModuleHandleW(nullptr), nullptr);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == downloadButtonId) {
        if (installed) openExample();
        else installPackage();
        return 0;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(window, &paint);
        RECT client = {};
        GetClientRect(window, &client);
        HBRUSH background = CreateSolidBrush(RGB(248, 246, 252));
        FillRect(dc, &client, background);
        DeleteObject(background);
        RECT header = {0, 0, client.right, 72};
        HBRUSH purple = CreateSolidBrush(RGB(91, 52, 170));
        FillRect(dc, &header, purple);
        DeleteObject(purple);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(255, 255, 255));
        HFONT font = CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousFont = SelectObject(dc, font);
        RECT title = {24, 9, 340, 64};
        DrawTextW(dc, L"TRG Installer", -1, &title, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, previousFont);
        DeleteObject(font);
        EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand) {
    INITCOMMONCONTROLSEX controls = {sizeof(controls), ICC_PROGRESS_CLASS};
    InitCommonControlsEx(&controls);

    if (!isElevated()) {
        wchar_t executable[32768] = {};
        const DWORD length = GetModuleFileNameW(nullptr, executable, static_cast<DWORD>(std::size(executable)));
        if (length == 0 || length >= std::size(executable)) {
            MessageBoxW(nullptr, L"Could not locate the installer executable.",
                        L"TRG Installer error", MB_OK | MB_ICONERROR);
            return 1;
        }
        const HINSTANCE result = ShellExecuteW(nullptr, L"runas", executable, nullptr, nullptr, SW_SHOWNORMAL);
        if (reinterpret_cast<INT_PTR>(result) <= 32) {
            MessageBoxW(nullptr, L"Administrator permission is required to install TRG under Program Files.",
                        L"TRG Installer", MB_OK | MB_ICONERROR);
            return 1;
        }
        return 0;
    }

    std::filesystem::path programFiles;
    std::filesystem::path desktop;
#ifdef TRG_INSTALLER_TEST
    wchar_t temporary[MAX_PATH] = {};
    const DWORD temporaryLength = GetTempPathW(MAX_PATH, temporary);
    if (temporaryLength == 0 || temporaryLength >= MAX_PATH) {
        MessageBoxW(nullptr, L"Could not locate the temporary test folder.",
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    const std::filesystem::path testRoot = std::filesystem::path(temporary) / L"TRG-Installer-Test";
    programFiles = testRoot / L"Program Files";
    desktop = testRoot / L"Desktop";
    std::error_code testFolderError;
    std::filesystem::create_directories(desktop, testFolderError);
    if (testFolderError) {
        MessageBoxW(nullptr, L"Could not create the temporary installer test folder.",
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
#else
    wchar_t programFilesBuffer[MAX_PATH] = {};
    wchar_t desktopBuffer[MAX_PATH] = {};
    if (FAILED(SHGetFolderPathW(nullptr, CSIDL_PROGRAM_FILES, nullptr, SHGFP_TYPE_CURRENT, programFilesBuffer)) ||
        FAILED(SHGetFolderPathW(nullptr, CSIDL_DESKTOPDIRECTORY, nullptr, SHGFP_TYPE_CURRENT, desktopBuffer))) {
        MessageBoxW(nullptr, L"Could not locate Program Files or the Desktop folder.",
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    programFiles = programFilesBuffer;
    desktop = desktopBuffer;
#endif
    installDirectory = programFiles / L"TRG-code";
    runtimePath = installDirectory / L"TRG.exe";
    try {
        shortcutPath = desktopShortcutPath(desktop);
        examplePath = desktopExamplePath(desktop);
    } catch (const std::exception& error) {
        const std::wstring message(error.what(), error.what() + strlen(error.what()));
        MessageBoxW(nullptr, message.c_str(), L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    try {
        requiredStorage = embeddedSize(runtimeResourceId) + embeddedSize(readmeResourceId) +
            embeddedSize(iconResourceId) + embeddedSize(exampleResourceId) +
            embeddedSize(editor2DResourceId) + embeddedSize(editor3DResourceId) + 4096;
    } catch (const std::exception& error) {
        const std::wstring message(error.what(), error.what() + strlen(error.what()));
        MessageBoxW(nullptr, message.c_str(), L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }

    wchar_t installerExecutable[32768] = {};
    const DWORD installerPathLength = GetModuleFileNameW(nullptr, installerExecutable,
        static_cast<DWORD>(std::size(installerExecutable)));
    if (installerPathLength == 0 || installerPathLength >= std::size(installerExecutable)) {
        MessageBoxW(nullptr, L"Could not determine the installer file size.",
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    std::error_code fileSizeError;
    installerFileSize = std::filesystem::file_size(installerExecutable, fileSizeError);
    if (fileSizeError) {
        MessageBoxW(nullptr, L"Could not determine the installer file size.",
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ULARGE_INTEGER available = {};
    const std::wstring root = installDirectory.root_path().wstring();
    if (!GetDiskFreeSpaceExW(root.c_str(), &available, nullptr, nullptr)) {
        MessageBoxW(nullptr, systemMessage(L"Could not check free disk space").c_str(),
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    if (available.QuadPart < requiredStorage) {
        MessageBoxW(nullptr, L"There is not enough free space on the Program Files drive.",
                    L"TRG Installer", MB_OK | MB_ICONERROR);
        return 1;
    }

    const std::wstring storageText = L"Installer: " + formatMiB(installerFileSize) +
        L"; installed files: " + formatMiB(requiredStorage) +
        L"\r\nInstall folder: " + installDirectory.wstring();
    const wchar_t* className = L"TRGInstallerWindow";
    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = installerWindowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_TRG));
    windowClass.lpszClassName = className;
    if (!RegisterClassW(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        MessageBoxW(nullptr, systemMessage(L"Could not register the installer window").c_str(),
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    HWND window = CreateWindowExW(0, className, L"TRG Installer",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 350, nullptr, nullptr, instance, nullptr);
    if (!window) {
        MessageBoxW(nullptr, systemMessage(L"Could not create the installer window").c_str(),
                    L"TRG Installer error", MB_OK | MB_ICONERROR);
        return 1;
    }
    ShowWindow(window, showCommand);
    UpdateWindow(window);
    SetWindowTextW(storageLabel, storageText.c_str());

    MSG message;
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
