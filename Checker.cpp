#define NOMINMAX
#include <windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {

std::string lowerAscii(std::string value) {
    for (char& character : value) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return value;
}

void trimLine(std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }).base();
    if (first >= last) {
        value.clear();
        return;
    }
    value.assign(first, last);
}

std::string withoutComment(const std::string& line) {
    bool quoted = false;
    char quote = '\0';
    bool escaped = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char current = line[i];
        if (quoted) {
            if (escaped) {
                escaped = false;
            } else if (current == '\\') {
                escaped = true;
            } else if (current == quote) {
                quoted = false;
            }
            continue;
        }

        if (current == '"' || current == '\'') {
            quoted = true;
            quote = current;
            continue;
        }

        if (current == '/' && i + 1 < line.size() && line[i + 1] == '/') {
            return line.substr(0, i);
        }
    }

    return line;
}

bool isUiLibraryDirective(std::string line) {
    line = withoutComment(line);
    trimLine(line);
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line.erase(0, 3);
        trimLine(line);
    }

    std::size_t position = 0;
    while (position < line.size() &&
           std::isspace(static_cast<unsigned char>(line[position]))) {
        ++position;
    }

    const std::size_t commandStart = position;
    while (position < line.size()) {
        const unsigned char c = static_cast<unsigned char>(line[position]);
        if (!std::isalpha(c) && c != '_') break;
        ++position;
    }

    if (lowerAscii(line.substr(commandStart, position - commandStart)) != "add") {
        return false;
    }

    while (position < line.size() &&
           std::isspace(static_cast<unsigned char>(line[position]))) {
        ++position;
    }

    if (position >= line.size() || line[position] != '(') {
        return false;
    }

    const std::size_t close = line.find(')', position + 1);
    if (close == std::string::npos) {
        return false;
    }

    std::string library = line.substr(position + 1, close - position - 1);
    trimLine(library);
    library = lowerAscii(library);

    // Match the interpreter's scanLibraries() standalone-UI declarations.
    return library == "ui" || library == "animation" || library == "all";
}

bool sourceUsesUi(const std::filesystem::path& sourcePath) {
    std::ifstream source(sourcePath, std::ios::binary);
    if (!source) return false;

    std::string line;
    while (std::getline(source, line)) {
        if (isUiLibraryDirective(line)) return true;
    }
    return false;
}

void showError(const std::wstring& message) {
    MessageBoxW(nullptr, message.c_str(), L"TRG Checker", MB_OK | MB_ICONERROR);
}

} // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argumentCount = 0;
    LPWSTR* arguments = CommandLineToArgvW(GetCommandLineW(), &argumentCount);
    if (!arguments) {
        showError(L"Checker could not read the command line.");
        return 1;
    }

    if (argumentCount < 2 || !arguments[1] || !*arguments[1]) {
        LocalFree(arguments);
        showError(L"Open a .trg file through Windows File Explorer to run it.");
        return 2;
    }

    std::error_code pathError;
    std::filesystem::path sourcePath = std::filesystem::absolute(
        std::filesystem::path(arguments[1]), pathError);
    if (pathError) sourcePath = std::filesystem::path(arguments[1]);
    LocalFree(arguments);

    if (!std::filesystem::exists(sourcePath, pathError) || pathError ||
        !std::filesystem::is_regular_file(sourcePath, pathError) || pathError) {
        showError(L"Checker could not find the selected TRG file:\n\n" + sourcePath.wstring());
        return 1;
    }

    std::ifstream testFile(sourcePath, std::ios::binary);
    if (!testFile) {
        showError(L"Checker could not read the selected TRG file:\n\n" + sourcePath.wstring());
        return 1;
    }
    testFile.close();

    wchar_t checkerExecutable[32768] = {};
    const DWORD checkerLength = GetModuleFileNameW(
        nullptr, checkerExecutable, static_cast<DWORD>(std::size(checkerExecutable)));
    if (checkerLength == 0 || checkerLength >= std::size(checkerExecutable)) {
        showError(L"Checker could not locate its installation folder.");
        return 1;
    }

    const std::filesystem::path checkerPath(
        std::wstring(checkerExecutable, checkerLength));
    const std::filesystem::path installFolder = checkerPath.parent_path();
    const bool usesUi = sourceUsesUi(sourcePath);
    const std::filesystem::path appPath =
        installFolder / (usesUi ? L"UI-app.exe" : L"Console-app.exe");

    if (!std::filesystem::exists(appPath, pathError) || pathError) {
        showError(L"Checker could not find the required TRG application:\n\n" +
                  appPath.wstring() +
                  L"\n\nRun the TRG installer again to repair the installation.");
        return 1;
    }

    std::wstring commandLine = L"\"" + appPath.wstring() + L"\" \"" +
                               sourcePath.wstring() + L"\"";
    std::vector<wchar_t> mutableCommand(commandLine.begin(), commandLine.end());
    mutableCommand.push_back(L'\0');

    STARTUPINFOW startup = {};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process = {};

    std::filesystem::path workingDirectory = sourcePath.parent_path();
    if (workingDirectory.empty()) workingDirectory = std::filesystem::current_path();

    const BOOL started = CreateProcessW(
        appPath.c_str(),
        mutableCommand.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_UNICODE_ENVIRONMENT,
        nullptr,
        workingDirectory.c_str(),
        &startup,
        &process);

    if (!started) {
        const DWORD error = GetLastError();
        showError(L"Checker could not start the selected TRG application:\n\n" +
                  appPath.wstring() + L"\n\nWindows error: " + std::to_wstring(error));
        return 1;
    }

    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return 0;
}
