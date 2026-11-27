#include "EditorSources.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <map>
#include <numeric>
#include <ctime>
#include <system_error>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <cwctype>
#include <variant>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <memory>
#include <array>
#include <deque>
#include <regex>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#endif

// The current runtime exposes Bluetooth extension declarations but does not
// call platform Bluetooth APIs directly. Avoid requiring platform-specific
// Bluetooth development headers just to build the interpreter.

#if defined(_WIN32)
#define TRG_NETWORK_DLL_EXPORT __declspec(dllexport)
#define TRG_BLUETOOTH_DLL_EXPORT __declspec(dllexport)
#else
#define TRG_NETWORK_DLL_EXPORT __attribute__((visibility("default")))
#define TRG_BLUETOOTH_DLL_EXPORT __attribute__((visibility("default")))
#endif

#include <string>

TRG_NETWORK_DLL_EXPORT std::string httpGet(const std::string& url, std::string& error);
TRG_NETWORK_DLL_EXPORT std::string httpPost(const std::string& url, const std::string& body, std::string& error);
extern TRG_NETWORK_DLL_EXPORT bool networkEnabled;

TRG_BLUETOOTH_DLL_EXPORT bool bluetoothConnect(std::string deviceAddress);
TRG_BLUETOOTH_DLL_EXPORT int bluetoothSend(std::string data);
TRG_BLUETOOTH_DLL_EXPORT std::string bluetoothReceive();
TRG_BLUETOOTH_DLL_EXPORT bool bluetoothAvailable();
TRG_BLUETOOTH_DLL_EXPORT std::string bluetoothGetLocalAddress();
TRG_BLUETOOTH_DLL_EXPORT bool bluetoothEnabled = false;
TRG_BLUETOOTH_DLL_EXPORT std::vector<std::string> bluetoothDevices;

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <gdiplus.h>
#include <richedit.h>
#include "resource.h"
#elif defined(__ANDROID__)
#include <jni.h>
#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_F1 0x70
#define VK_NUMPAD0 0x60
#define VK_BACK 0x08
#define VK_DELETE 0x2E
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_END 0x23
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_CAPITAL 0x14
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_SNAPSHOT 0x2C
#define VK_PAUSE 0x13
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_ADD 0x6B
#define VK_SUBTRACT 0x6D
#define VK_MULTIPLY 0x6A
#define VK_DIVIDE 0x6F
#define VK_DECIMAL 0x6E
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#endif

// Keep the interpreter's named keyboard constants available to non-Windows
// desktop builds too. These values follow the Windows virtual-key convention
// used by TRG scripts, so cross-platform scripts keep the same key identifiers.
#if !defined(_WIN32) && !defined(__ANDROID__)
#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_F1 0x70
#define VK_NUMPAD0 0x60
#define VK_BACK 0x08
#define VK_DELETE 0x2E
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_END 0x23
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_CAPITAL 0x14
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_SNAPSHOT 0x2C
#define VK_PAUSE 0x13
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_ADD 0x6B
#define VK_SUBTRACT 0x6D
#define VK_MULTIPLY 0x6A
#define VK_DIVIDE 0x6F
#define VK_DECIMAL 0x6E
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#endif

struct VariableValue;
struct DictionaryData;
class ProgramBlockIndex;
using VariableList = std::vector<std::shared_ptr<VariableValue>>;
struct VariableValue {
    std::variant<double, bool, std::string, std::vector<std::string>, std::shared_ptr<DictionaryData>, VariableList> value = std::string();
    VariableValue() = default;
    VariableValue(const std::string& text) : value(text) {}
    VariableValue(std::string&& text) : value(std::move(text)) {}
    VariableValue(const char* text) : value(std::string(text)) {}
    VariableValue(double number) : value(number) {}
    VariableValue(bool flag) : value(flag) {}
    VariableValue(const std::vector<std::string>& items) : value(items) {}
    VariableValue(std::vector<std::string>&& items) : value(std::move(items)) {}
    VariableValue(const std::shared_ptr<DictionaryData>& dictionary) : value(dictionary) {}
    VariableValue(const VariableList& items) : value(items) {}
    VariableValue(VariableList&& items) : value(std::move(items)) {}
    VariableValue& operator=(const VariableList& items) { value = items; return *this; }
    VariableValue& operator=(VariableList&& items) { value = std::move(items); return *this; }
    VariableValue& operator=(const std::string& text) { value = text; return *this; }
    VariableValue& operator=(std::string&& text) { value = std::move(text); return *this; }
    VariableValue& operator=(const char* text) { value = std::string(text); return *this; }
    VariableValue& operator=(double number) { value = number; return *this; }
    VariableValue& operator=(bool flag) { value = flag; return *this; }
    VariableValue& operator=(const std::vector<std::string>& items) { value = items; return *this; }
    VariableValue& operator=(std::vector<std::string>&& items) { value = std::move(items); return *this; }
    operator std::string() const {
        if (const auto* text = std::get_if<std::string>(&value)) return *text;
        if (const auto* number = std::get_if<double>(&value)) { std::ostringstream out; out << std::setprecision(15) << *number; return out.str(); }
        if (const auto* flag = std::get_if<bool>(&value)) return *flag ? "true" : "false";
        if (const auto* items = std::get_if<std::vector<std::string>>(&value)) {
            std::string joined;
            for (const auto& item : *items) {
                if (!joined.empty()) joined += ", ";
                joined += item;
            }
            return joined;
        }
        if (const auto* list = std::get_if<VariableList>(&value)) {
            std::string joined;
            for (const auto& item : *list) {
                if (!joined.empty()) joined += ", ";
                if (item) joined += static_cast<std::string>(*item);
            }
            return joined;
        }
        return "{dictionary}";
    }
    void clear() { value = std::string(); }
};
struct DictionaryData {
    std::unordered_map<std::string, VariableValue> values;
};
using Vars = std::unordered_map<std::string, VariableValue>;
using Types = std::unordered_map<std::string, std::string>;

struct Flow {
    bool broke = false;
    bool continued = false;
    bool stopped = false;
    bool returned = false;
    VariableValue returnValue;
};
struct TrgCallFrame {
    std::string functionName;
    std::size_t callLine = 0;
    std::size_t definitionLine = 0;
};
std::vector<TrgCallFrame> trgCallStack;

struct EveryTask {
    std::vector<std::string> body;
    std::shared_ptr<ProgramBlockIndex> blockIndex;
    std::chrono::milliseconds interval{1000};
    std::chrono::steady_clock::time_point nextRun;
    bool manualMode = false;
    bool uiMode = false;
};

struct CollisionWatch { std::string first; std::string second; std::string result; };

struct PhysicsBody {
    std::string name;
    double vx = 0;
    double vy = 0;
    double gravity = 0;
    double drag = 0;
    double restitution = 1.0;
    bool dynamic = true;
    bool solid = true;
    bool trigger = false;
    bool frozen = false;
    bool dragLaunch = false;
    bool dragLaunching = false;
    bool launched = false;
    double launchSpeedLimit = 70.0;
    double dragStartX = 0;
    double dragStartY = 0;
    std::string triggerKind;
    std::string gravityDirection = "none";
    double speedMultiplier = 1.0;
    double launchPowerMultiplier = 1.0;
    double ballSizeMultiplier = 1.0;
    bool midAirLaunch = false;
    bool immortal = false;
    int lives = 1;
    double spawnX = 0;
    double spawnY = 0;
    bool triggerStarts = false;
    bool triggerRepeats = false;
    bool triggerHasActivated = false;
    int bounceCount = 0;
    double angle = 0;
    bool stopsMomentum = false;
    bool noClip = false;
    bool isFlipped = false;
    bool isCircle = false;
};

struct AiSample { std::vector<double> inputs; double expected = 0; };
struct AiModel { std::vector<double> weights; double bias = 0; std::vector<AiSample> samples; };

struct UserFunction {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::string> body;
    std::size_t sourceLine = 0;
    std::shared_ptr<ProgramBlockIndex> blockIndex;
};

struct UiBlock {
    std::string name;
    std::string group;
    std::string color;
    std::string shape = "square";
    std::string glowColor = "";
    double glowIntensity = 0;
    int z = 0;
    double x = 0;
    double y = 0;
    double width = 100;
    double height = 100;
    double cornerRadius = 0;
    double opacity = 1.0;
    double angle = 0;
    double lineWidth = 1.0;
    bool stopsMomentum = false;
    bool noClip = false;
    bool isFlipped = false;
    bool visible = true;
};

struct Ui3DObject {
    std::string name;
    std::string group;
    std::string shape = "box";
    std::string color = "#8B5CF6";
    double x = 0, y = 0, z = 0;
    double width = 2, height = 2, depth = 2;
    double rotateX = 0, rotateY = 0, rotateZ = 0;
    double opacity = 1.0;
    bool visible = true;
    std::string texture = "smooth";
};

struct UiSprite {
    std::string name;
    std::string group;
    std::string path;
    double x = 0;
    double y = 0;
    double width = 10;
    double height = 10;
    double opacity = 1.0;
    bool widthAuto = true;
    bool heightAuto = true;
    bool visible = true;
};

struct StructField { std::string type; std::string initial; };
using StructDefinition = std::unordered_map<std::string, StructField>;

struct UiAnimation {
    std::string name;
    double startX = 0;
    double startY = 0;
    double dx = 0;
    double dy = 0;
    double startTime = 0;
    double duration = 0;
    int fps = 60;
    bool loop = false;
    double phase = 0;
};

struct UiButton {
    std::string name;
    std::string group;
    std::string label;
    double x = 0;
    double y = 0;
    double width = 20;
    double height = 10;
    std::string color = "#7043BE";
    std::string textColor = "#FFFFFF";
    std::string font = "sans";
    double opacity = 1.0;
    double cornerRadius = 7;
    double textSize = 0.34;
    bool textBold = true;
    bool visible = true;
    std::vector<std::string> body;
    std::shared_ptr<ProgramBlockIndex> blockIndex;
};

struct UiText {
    std::string name;
    std::string group;
    std::string value;
    std::string color;
    std::string font = "sans";
    std::string align = "left";
    double x = 0;
    double y = 0;
    double size = 5;
    double opacity = 1.0;
    bool bold = true;
    bool visible = true;
};

struct UiStyle {
    bool hasBackground = false;
    bool hasText = false;
    bool hasGlow = false;
    bool hasRadius = false;
    bool hasOpacity = false;
    bool hasFont = false;
    bool hasSize = false;
    bool hasBold = false;
    bool hasAlign = false;
    std::string background;
    std::string text;
    std::string glow;
    double glowIntensity = 0.5;
    double radius = 0;
    double opacity = 1.0;
    std::string font = "sans";
    double size = 5;
    bool bold = true;
    std::string align = "left";
};

struct UiGroup { std::string name; bool visible = false; std::string parent = "default"; };

using Defaults = std::unordered_map<std::string, std::string>;

std::unordered_map<std::string, UserFunction> userFunctions;
std::vector<UiBlock> uiBlocks;
std::vector<Ui3DObject> ui3DObjects;
int uiAutoNameCounter = 1;
int threeDAutoNameCounter = 1;
std::vector<UiSprite> uiSprites;
std::vector<UiAnimation> uiAnimations;
std::vector<EveryTask> everyTasks;
std::vector<CollisionWatch> collisionWatches;
std::unordered_map<std::string, PhysicsBody> physicsBodies;
std::unordered_map<std::string, AiModel> aiModels;
std::vector<UiButton> uiButtons;
std::vector<UiText> uiTexts;
std::unordered_map<std::string, UiStyle> uiStyles;
std::vector<UiGroup> uiGroups;
std::string currentUiGroup = "default";
std::unordered_map<std::string, StructDefinition> userStructs;
std::vector<std::string> uiProgram;
std::vector<std::string> preparedLaunchProgram;
bool preparedLaunchProgramReady = false;
std::filesystem::path projectRoot = std::filesystem::current_path();
bool filesEnabled = false;
bool jsonEnabled = false;
struct TrgDatabaseTable { std::vector<std::string> columns; std::vector<std::vector<std::string>> rows; };
std::unordered_map<std::string, TrgDatabaseTable> trgDatabaseTables;
std::filesystem::path trgDatabasePath;
bool databaseEnabled = false;
std::vector<std::string> groupStack;
std::vector<std::unordered_set<std::string>> groupSnapshots;
std::unordered_map<std::string, std::vector<std::string>> variableGroups;
std::unordered_set<std::string> executedFileKeys;
std::unordered_set<std::string> activeRunFiles;
Vars uiVariables;
Types uiTypes;
Defaults uiDefaults;
int trgScreenWidth = 1280;
int trgScreenHeight = 720;
#ifdef _WIN32
extern HWND mainWindow;
#endif
void refreshDisplayVariables(Vars& variables, Types& types) {
#ifdef _WIN32
    if (mainWindow) {
        RECT client = {};
        if (GetClientRect(mainWindow, &client)) {
            trgScreenWidth = std::max(1L, client.right - client.left);
            trgScreenHeight = std::max(1L, client.bottom - client.top);
        }
    }
#endif
    const int width = std::max(1, trgScreenWidth);
    const int height = std::max(1, trgScreenHeight);
    const int divisor = std::max(1, std::gcd(width, height));
    const int ratioWidth = width / divisor;
    const int ratioHeight = height / divisor;
    const std::string orientation = width > height ? "landscape" :
        (height > width ? "portrait" : "square");
    const std::string ratio = std::to_string(ratioWidth) + ":" + std::to_string(ratioHeight);
    variables["screen_width"] = static_cast<double>(width);
    variables["screen_height"] = static_cast<double>(height);
    variables["orientation"] = orientation;
    variables["screen_orientation"] = orientation;
    variables["landscape"] = width > height;
    variables["portrait"] = height > width;
    variables["is_landscape"] = width > height;
    variables["is_portrait"] = height > width;
    variables["aspect_width"] = static_cast<double>(ratioWidth);
    variables["aspect_height"] = static_cast<double>(ratioHeight);
    variables["aspect_ratio"] = ratio;
    types["screen_width"] = "num";
    types["screen_height"] = "num";
    types["orientation"] = "word";
    types["screen_orientation"] = "word";
    types["landscape"] = "toggle";
    types["portrait"] = "toggle";
    types["is_landscape"] = "toggle";
    types["is_portrait"] = "toggle";
    types["aspect_width"] = "num";
    types["aspect_height"] = "num";
    types["aspect_ratio"] = "word";
}
std::unordered_map<std::string, int> keyboardControls;
std::unordered_map<std::string, unsigned> controllerControls;
bool controlsEnabled = false;
bool webEnabled = false;
bool collisionEnabled = false;
bool physicsEnabled = false;
bool aiEnabled = false;
bool mathEnabled = false;
bool binaryEnabled = false;
bool regexEnabled = false;
bool compressionEnabled = false;
bool timeEnabled = false;
bool convertEnabled = false;
bool statsEnabled = false;
bool unitsEnabled = false;
bool styleEnabled = false;
bool threeDEnabled = false;
std::string webPageHtml;
std::vector<std::string> readProgram(std::istream& source);
std::size_t errorCount = 0;
std::chrono::steady_clock::time_point uiStart;
std::mt19937 trgRandomEngine(std::random_device{}());
bool uiEntityBounds(const std::string& name, double& x, double& y, double& width, double& height);
void refreshCollisionWatches(Vars& variables, Types& types);
void physicsStep(double seconds, Vars& variables, Types& types);
bool setUiEntityPosition(const std::string& name, double x, double y);

// ---------------------------------------------------------
// New features: particles, viewport, network, shapes, glows
// ---------------------------------------------------------
struct Particle {
    std::string name;
    double x = 0, y = 0;
    double vx = 0, vy = 0;
    double life = 0;
    double maxLife = 0;
    std::string color = "#FFFFFF";
    double size = 4;
    bool active = true;
};

std::vector<Particle> particles;
double viewportX = 0;
double viewportY = 0;
bool networkEnabled = false;
bool powershellEnabled = false;
bool raspberryEnabled = false;
bool accessibilityEnabled = false;

struct HttpRequest {
    std::string method;
    std::string url;
    std::string body;
    std::string resultVar;
};

struct AsyncHttpResult {
    bool complete = false;
    std::string response;
    std::string error;
};

std::vector<HttpRequest> pendingHttpRequests;
std::mutex asyncHttpResultsMutex;
std::unordered_map<std::string, AsyncHttpResult> asyncHttpResults;
std::atomic<std::uint64_t> asyncHttpNextId{1};

std::string httpGet(const std::string& url, std::string& error);
std::string httpPost(const std::string& url, const std::string& body, std::string& error);

// Shared by the Windows editor, Android editor bridge, and project-relative commands.
std::filesystem::path currentSourcePath;

bool uiGroupIsVisible(const std::string& name) {
    if (name.empty() || name == "default") return true;
    const auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) {
        return item.name == name;
    });
    return group != uiGroups.end() && group->visible && uiGroupIsVisible(group->parent);
}

bool uiNameIsVisible(const std::string& name) {
    for (const UiBlock& item : uiBlocks) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiSprite& item : uiSprites) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiButton& item : uiButtons) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiText& item : uiTexts) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    return false;
}

#ifdef __ANDROID__
extern std::string androidOutput;
extern jobject trgActivity;
extern std::unordered_map<std::string, std::string> androidMobileInputs;
extern std::atomic<bool> androidStopRequested;
extern std::mutex androidInputMutex;
std::string trgAskAndroid(const std::string& prompt, bool& accepted);
void trgStudioActionAndroid(const std::string& action);
#endif

#ifdef _WIN32
HWND mainWindow = nullptr;
HWND inputControl = nullptr;
HFONT openButtonFont = nullptr;
HFONT codeEditorFont = nullptr;
HMODULE richEditModule = nullptr;
bool inputFinished = false;
bool inputAccepted = false;
bool loadingEditorText = false;
bool highlightingEditor = false;
bool editorDirty = false;
bool restoreDesktopStateOnCreate = true;
bool previewMode = false;
bool standaloneAppMode = false;
bool standaloneTerminalMode = false;
std::string inputResult;
constexpr int openButtonId = 101;
constexpr int inputEditId = 201;
constexpr int inputButtonId = 202;
constexpr int saveButtonId = 102;
constexpr int runButtonId = 103;
constexpr int previewButtonId = 104;
constexpr int newButtonId = 105;
constexpr int buildButtonId = 106;
constexpr int dictionaryButtonId = 107;
constexpr int editor2DButtonId = 108;
constexpr int editor3DButtonId = 109;
constexpr int codeEditorId = 301;
HWND openButton = nullptr;
HWND saveButton = nullptr;
HWND runButton = nullptr;
HWND previewButton = nullptr;
HWND newButton = nullptr;
HWND buildButton = nullptr;
HWND dictionaryButton = nullptr;
HWND editor2DButton = nullptr;
HWND editor3DButton = nullptr;
HWND exportButton = nullptr;
HWND codeEditor = nullptr;
HWND outputControl = nullptr;
HWND terminalInputControl = nullptr;
HWND terminalSendButton = nullptr;
HWND dictionaryWindow = nullptr;
HWND dictionaryEdit = nullptr;
HFONT dictionaryFont = nullptr;
HBRUSH terminalInputBrush = nullptr;
HBRUSH outputControlBrush = nullptr;
HBRUSH dictionaryEditBrush = nullptr;
WNDPROC previousCodeEditorProc = nullptr;
WNDPROC previousTerminalInputProc = nullptr;
WNDPROC previousOutputProc = nullptr;
bool terminalInputActive = false;
std::wstring desktopDictionaryText;
constexpr int outputControlId = 302;
constexpr int terminalInputControlId = 303;
constexpr int terminalSendButtonId = 304;
constexpr int exportButtonId = 110;
constexpr int terminalInputHeight = 32;
std::filesystem::path desktopPreviousPath;
bool desktopPreviousDirty = false;
bool desktopVisualEditorActive = false;
std::wstring inputPrompt;
std::wstring outputBuffer;
ULONG_PTR gdiplusToken = 0;
std::unordered_map<std::string, std::unique_ptr<Gdiplus::Image>> desktopSpriteCache;
bool desktop3DEngineActive = false;
std::string desktop3DSelectedName;
double desktop3DCameraX = 0.0, desktop3DCameraY = 0.0, desktop3DCameraZ = -12.0;
double desktop3DCameraYaw = 0.0, desktop3DCameraPitch = 0.18;
double desktop3DZoom = 1.0;
double desktop3DWorldScale = 1.0;
std::vector<std::pair<std::string, RECT>> desktop3DProjectedHits;
RECT desktop3DViewportRect = {};
bool desktop3DDraggingObject = false;
bool desktop3DDraggingCamera = false;
int desktop3DAssetCandidate = -1;
bool desktop3DAssetDragging = false;
POINT desktop3DMouseDown = {};
double desktop3DDragStartX = 0.0, desktop3DDragStartY = 0.0, desktop3DDragStartZ = 0.0;
double desktop3DDragStartYaw = 0.0, desktop3DDragStartPitch = 0.0;
bool desktop3DContextOpen = false;
bool desktop3DRightClickMoved = false;
int desktop3DHierarchyScroll = 0;
POINT desktop3DContextPoint = {};
POINT desktop3DMouseCurrent = {};
RECT desktop3DContextRect = {};
RECT desktop3DToolbarRect = {};
RECT desktop3DHierarchyRect = {};
RECT desktop3DInspectorRect = {};
RECT desktop3DAssetShelfRect = {};
std::array<RECT, 13> desktop3DToolRects = {};
bool desktop3DPlayMode = false;
int desktop3DActiveTab = 0;
std::array<RECT, 5> desktop3DAssetRects = {};
std::array<RECT, 5> desktop3DTextureRects = {};
int desktop3DToolMode = 1;
bool desktop3DGridVisible = true;
double desktop3DDragStartWidth = 2.0, desktop3DDragStartHeight = 2.0, desktop3DDragStartDepth = 2.0;
double desktop3DDragStartRotX = 0.0, desktop3DDragStartRotY = 0.0;
LRESULT CALLBACK codeEditorWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK terminalInputWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK outputWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK dictionaryWindowProc(HWND, UINT, WPARAM, LPARAM);

bool runSourceFile(const std::filesystem::path& path);
bool loadSourceFile(const std::filesystem::path& path);
bool saveSourceFile();
void saveDesktopAutosave();
void restoreDesktopAutosave();
bool runEditorSource();
void applyEditorSyntaxHighlighting();
bool launchDesktopVisualEditor(const std::string& fileName);
bool returnFromDesktopVisualEditor();
void clearDesktopOutput();

int windowDpi(HWND window) {
    using GetDpiForWindowFunction = UINT (WINAPI*)(HWND);
    static const auto getDpiForWindow = reinterpret_cast<GetDpiForWindowFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForWindow"));
    return getDpiForWindow ? static_cast<int>(getDpiForWindow(window)) : 96;
}

int scaleForDpi(HWND window, int value) {
    return MulDiv(value, windowDpi(window), 96);
}

int systemDpi() {
    using GetDpiForSystemFunction = UINT (WINAPI*)();
    static const auto getDpiForSystem = reinterpret_cast<GetDpiForSystemFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForSystem"));
    if (getDpiForSystem) return static_cast<int>(getDpiForSystem());
    HDC screen = GetDC(nullptr);
    if (!screen) return 96;
    const int dpi = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(nullptr, screen);
    return dpi > 0 ? dpi : 96;
}

void enablePerMonitorDpiAwareness() {
    using SetProcessDpiAwarenessContextFunction = BOOL (WINAPI*)(HANDLE);
    static const auto setAwareness = reinterpret_cast<SetProcessDpiAwarenessContextFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetProcessDpiAwarenessContext"));
    if (setAwareness && setAwareness(reinterpret_cast<HANDLE>(static_cast<INT_PTR>(-4)))) return;
    SetProcessDPIAware();
}

void setOpenButtonFont(HWND window) {
    if (openButtonFont) DeleteObject(openButtonFont);
    openButtonFont = CreateFontW(scaleForDpi(window, 13), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    const HWND buttons[] = {openButton, saveButton, runButton, previewButton, newButton, buildButton, dictionaryButton, editor2DButton, editor3DButton, exportButton};
    if (openButtonFont) for (HWND button : buttons) if (button)
        SendMessageW(button, WM_SETFONT, reinterpret_cast<WPARAM>(openButtonFont), TRUE);
    if (codeEditorFont) DeleteObject(codeEditorFont);
    codeEditorFont = CreateFontW(-scaleForDpi(window, 15), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Consolas");
    if (codeEditor && codeEditorFont) SendMessageW(codeEditor, WM_SETFONT,
        reinterpret_cast<WPARAM>(codeEditorFont), TRUE);
}

bool openSourceDialog(std::filesystem::path& path, HWND owner) {
    wchar_t fileName[32768] = {};
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = owner;
    dialog.lpstrFilter = L"TRG files (*.trg)\0*.trg\0All files (*.*)\0*.*\0";
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = static_cast<DWORD>(sizeof(fileName) / sizeof(fileName[0]));
    dialog.lpstrDefExt = L"trg";
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!GetOpenFileNameW(&dialog)) return false;
    path = std::filesystem::path(fileName);
    return true;
}

bool saveSourceDialog(std::filesystem::path& path, HWND owner) {
    wchar_t fileName[32768] = {};
    if (!path.empty()) {
        std::wcsncpy(fileName, path.c_str(), 32767);
        fileName[32767] = L'\0';
    }
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = owner;
    dialog.lpstrFilter = L"TRG files (*.trg)\0*.trg\0Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = static_cast<DWORD>(sizeof(fileName) / sizeof(fileName[0]));
    dialog.lpstrDefExt = L"trg";
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
    if (!GetSaveFileNameW(&dialog)) return false;
    path = std::filesystem::path(fileName);
    return true;
}

void layoutOutputControls(HWND window) {
    RECT client = {};
    GetClientRect(window, &client);
    const LONG width = std::max<LONG>(0, client.right - client.left);
    const LONG height = std::max<LONG>(0, client.bottom - client.top);
    const LONG headerHeight = scaleForDpi(window, 84);
    const LONG gap = scaleForDpi(window, 5);
    const LONG margin = scaleForDpi(window, 12);

    if (standaloneAppMode) {
        const HWND controls[] = {openButton, saveButton, runButton, previewButton,
            newButton, buildButton, dictionaryButton, editor2DButton, editor3DButton,
            exportButton, codeEditor, outputControl, terminalInputControl, terminalSendButton};
        for (HWND control : controls) if (control) ShowWindow(control, SW_HIDE);
        return;
    }
    if (standaloneTerminalMode) {
        const HWND controls[] = {openButton, saveButton, runButton, previewButton,
            newButton, buildButton, dictionaryButton, editor2DButton, editor3DButton,
            exportButton, codeEditor};
        for (HWND control : controls) if (control) ShowWindow(control, SW_HIDE);
        const LONG terminalMargin = scaleForDpi(window, 16);
        const LONG header = scaleForDpi(window, 66);
        const LONG inputHeight = terminalInputActive ? scaleForDpi(window, terminalInputHeight + 8) : 0;
        if (outputControl) {
            MoveWindow(outputControl, terminalMargin, header + scaleForDpi(window, 12),
                std::max<LONG>(1, width - terminalMargin * 2),
                std::max<LONG>(1, height - header - terminalMargin - inputHeight - scaleForDpi(window, 12)),
                TRUE);
            ShowWindow(outputControl, SW_SHOW);
        }
        if (terminalInputControl && terminalSendButton) {
            const LONG inputWidth = std::min<LONG>(scaleForDpi(window, 560),
                std::max<LONG>(scaleForDpi(window, 150),
                    width - terminalMargin * 2 - scaleForDpi(window, 90)));
            const LONG inputY = height - terminalMargin - scaleForDpi(window, terminalInputHeight);
            MoveWindow(terminalInputControl, terminalMargin, inputY, inputWidth,
                scaleForDpi(window, 30), TRUE);
            MoveWindow(terminalSendButton, terminalMargin + inputWidth + scaleForDpi(window, 8),
                inputY, scaleForDpi(window, 76), scaleForDpi(window, 30), TRUE);
            ShowWindow(terminalInputControl, terminalInputActive ? SW_SHOW : SW_HIDE);
            ShowWindow(terminalSendButton, terminalInputActive ? SW_SHOW : SW_HIDE);
        }
        return;
    }

    if (previewMode) {
        const HWND controls[] = {openButton, saveButton, runButton, newButton, buildButton,
            dictionaryButton, editor2DButton, editor3DButton, exportButton};
        for (HWND control : controls) if (control) ShowWindow(control, SW_HIDE);
        if (previewButton) {
            if (standaloneAppMode) {
                ShowWindow(previewButton, SW_HIDE);
            } else {
                SetWindowTextW(previewButton, L"Code");
                const LONG buttonWidth = scaleForDpi(window, 76);
                const LONG buttonHeight = scaleForDpi(window, 34);
                MoveWindow(previewButton, std::max<LONG>(margin, width - margin - buttonWidth),
                    margin, buttonWidth, buttonHeight, TRUE);
                ShowWindow(previewButton, SW_SHOW);
                SetWindowPos(previewButton, HWND_TOP, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            }
        }
    } else {
        const HWND buttons[] = {openButton, saveButton, runButton, previewButton, newButton,
            buildButton, dictionaryButton, editor2DButton, editor3DButton, exportButton};
        const LONG toolbarLeft = scaleForDpi(window, 170);
        const LONG usableWidth = std::max<LONG>(scaleForDpi(window, 5 * 58),
            width - toolbarLeft - margin);
        const LONG buttonWidth = std::max<LONG>(scaleForDpi(window, 1),
            (usableWidth - gap * 4) / 5);
        const LONG buttonHeight = scaleForDpi(window, 30);
        const LONG rowGap = scaleForDpi(window, 5);
        for (std::size_t index = 0; index < std::size(buttons); ++index) {
            const LONG row = static_cast<LONG>(index / 5);
            const LONG column = static_cast<LONG>(index % 5);
            const LONG x = toolbarLeft + column * (buttonWidth + gap);
            const LONG y = scaleForDpi(window, 8) + row * (buttonHeight + rowGap);
            if (buttons[index]) {
                MoveWindow(buttons[index], x, y, buttonWidth, buttonHeight, TRUE);
                ShowWindow(buttons[index], SW_SHOW);
            }
        }
        if (previewButton) SetWindowTextW(previewButton, L"Preview");
    }
    if (codeEditor && previewMode) ShowWindow(codeEditor, SW_HIDE);

    LONG panelTop = 0;
    if (previewMode) {
        const LONG outputHeight = std::min<LONG>(
            std::max<LONG>(scaleForDpi(window, 116), height / 5),
            std::max<LONG>(scaleForDpi(window, 90), height / 3));
        panelTop = std::max<LONG>(scaleForDpi(window, 120), height - outputHeight);
    } else {
        const LONG editorMargin = scaleForDpi(window, 10);
        const LONG top = headerHeight + editorMargin;
        const LONG outputMinimum = scaleForDpi(window, 112);
        const LONG canvasMinimum = scaleForDpi(window, 100);
        const LONG panelMinimum = std::min(height, headerHeight + canvasMinimum);
        const LONG panelMaximum = std::max(panelMinimum, height - outputMinimum);
        panelTop = std::clamp<LONG>(height * 65 / 100, panelMinimum, panelMaximum);
        if (codeEditor) {
            const LONG bottom = std::max<LONG>(top + 1, panelTop - editorMargin);
            MoveWindow(codeEditor, editorMargin, top,
                std::max<LONG>(1, width - editorMargin * 2), bottom - top, TRUE);
            ShowWindow(codeEditor, SW_SHOW);
        }
    }
    const LONG outputHeaderHeight = scaleForDpi(window, 25);
    const LONG inputHeight = terminalInputActive ? scaleForDpi(window, terminalInputHeight + 5) : 0;
    const LONG outputTop = panelTop + outputHeaderHeight;
    const LONG outputBottom = std::max<LONG>(outputTop + 1, height - margin - inputHeight);
    if (outputControl) {
        MoveWindow(outputControl, margin, outputTop, std::max<LONG>(1, width - margin * 2),
            outputBottom - outputTop, TRUE);
        ShowWindow(outputControl, SW_SHOW);
    }
    if (terminalInputControl && terminalSendButton) {
        const LONG inputWidth = std::min<LONG>(scaleForDpi(window, 430),
            std::max<LONG>(scaleForDpi(window, 150), width - margin * 2 - scaleForDpi(window, 82)));
        const LONG inputY = height - margin - scaleForDpi(window, terminalInputHeight);
        MoveWindow(terminalInputControl, margin, inputY, inputWidth, scaleForDpi(window, 28), TRUE);
        MoveWindow(terminalSendButton, margin + inputWidth + scaleForDpi(window, 7), inputY,
            scaleForDpi(window, 72), scaleForDpi(window, 28), TRUE);
        ShowWindow(terminalInputControl, terminalInputActive ? SW_SHOW : SW_HIDE);
        ShowWindow(terminalSendButton, terminalInputActive ? SW_SHOW : SW_HIDE);
    }
}

std::wstring wideFromUtf8(const std::string& value) {
    if (value.empty()) return std::wstring();
    const int size = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (size <= 0) return std::wstring(value.begin(), value.end());
    std::wstring result(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), &result[0], size);
    return result;
}

std::string utf8FromWide(const std::wstring& value) {
    if (value.empty()) return std::string();
    const int size = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (size <= 0) return std::string(value.begin(), value.end());
    std::string result(static_cast<std::size_t>(size), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), &result[0], size, nullptr, nullptr);
    return result;
}


LRESULT CALLBACK codeEditorWindowProc(HWND control, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_KEYDOWN && wParam == VK_TAB) {
        SendMessageW(control, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(L"    "));
        return 0;
    }
    if (message == WM_CHAR) {
        DWORD selectionStart = 0, selectionEnd = 0;
        SendMessageW(control, EM_GETSEL, reinterpret_cast<WPARAM>(&selectionStart),
                     reinterpret_cast<LPARAM>(&selectionEnd));
        const int textLength = GetWindowTextLengthW(control);
        std::wstring text(static_cast<std::size_t>(textLength) + 1, L'\0');
        GetWindowTextW(control, text.data(), textLength + 1);
        text.resize(static_cast<std::size_t>(textLength));
        if (selectionStart == selectionEnd &&
            (wParam == L')' || wParam == L'"' || wParam == L'\'') &&
            selectionStart < text.size() && text[selectionStart] == static_cast<wchar_t>(wParam)) {
            SendMessageW(control, EM_SETSEL, selectionStart + 1, selectionStart + 1);
            return 0;
        }
        if (selectionStart != selectionEnd &&
            (wParam == L'(' || wParam == L'"' || wParam == L'\'')) {
            const std::wstring selected = text.substr(selectionStart, selectionEnd - selectionStart);
            const wchar_t closing = wParam == L'(' ? L')' : static_cast<wchar_t>(wParam);
            std::wstring wrapped;
            wrapped.reserve(selected.size() + 2);
            wrapped.push_back(static_cast<wchar_t>(wParam));
            wrapped += selected;
            wrapped.push_back(closing);
            SendMessageW(control, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(wrapped.c_str()));
            SendMessageW(control, EM_SETSEL, selectionStart + 1,
                selectionStart + 1 + static_cast<DWORD>(selected.size()));
            return 0;
        }
        if (selectionStart == selectionEnd &&
            (wParam == L'(' || wParam == L'"' || wParam == L'\'')) {
            const wchar_t pair[] = {static_cast<wchar_t>(wParam),
                wParam == L'(' ? L')' : static_cast<wchar_t>(wParam), L'\0'};
            SendMessageW(control, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(pair));
            SendMessageW(control, EM_SETSEL, selectionStart + 1, selectionStart + 1);
            return 0;
        }
        if (wParam == L'\r' && selectionStart == selectionEnd) {
            const std::wstring before = text.substr(0, selectionStart);
            const std::size_t lineStart = before.find_last_of(L"\r\n") == std::wstring::npos
                ? 0 : before.find_last_of(L"\r\n") + 1;
            const std::wstring line = before.substr(lineStart);
            std::size_t indent = 0;
            while (indent < line.size() && (line[indent] == L' ' || line[indent] == L'\t'))
                indent += line[indent] == L'\t' ? 4 : 1;
            std::size_t first = indent;
            while (first < line.size() && std::iswspace(line[first])) ++first;
            const std::wstring trimmed = line.substr(first);
            const bool betweenPair = selectionStart > 0 && selectionStart < text.size() &&
                text[selectionStart - 1] == L'(' && text[selectionStart] == L')';
            std::wstring insertion;
            std::size_t caretOffset = 0;
            if (betweenPair) {
                const std::wstring outer(indent, L' ');
                const std::wstring inner(indent + 4, L' ');
                insertion = L"\r\n" + inner + L"\r\n" + outer;
                caretOffset = 2 + inner.size();
            } else {
                std::size_t nextIndent = indent;
                if (!trimmed.empty() && trimmed.back() == L'(') nextIndent += 4;
                else if (trimmed.empty() && selectionStart < text.size() &&
                         text[selectionStart] == L')' && nextIndent >= 4) nextIndent -= 4;
                insertion = L"\r\n" + std::wstring(nextIndent, L' ');
                caretOffset = insertion.size();
            }
            SendMessageW(control, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(insertion.c_str()));
            const DWORD caret = selectionStart + static_cast<DWORD>(caretOffset);
            SendMessageW(control, EM_SETSEL, caret, caret);
            return 0;
        }
    }
    return previousCodeEditorProc
        ? CallWindowProcW(previousCodeEditorProc, control, message, wParam, lParam)
        : DefWindowProcW(control, message, wParam, lParam);
}

LRESULT CALLBACK terminalInputWindowProc(HWND control, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_KEYDOWN && wParam == VK_RETURN) {
        if (mainWindow) SendMessageW(mainWindow, WM_COMMAND,
            MAKEWPARAM(terminalSendButtonId, BN_CLICKED), reinterpret_cast<LPARAM>(terminalSendButton));
        return 0;
    }
    return previousTerminalInputProc
        ? CallWindowProcW(previousTerminalInputProc, control, message, wParam, lParam)
        : DefWindowProcW(control, message, wParam, lParam);
}

LRESULT CALLBACK dictionaryWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        dictionaryEditBrush = CreateSolidBrush(RGB(8, 7, 12));
        dictionaryEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", desktopDictionaryText.c_str(),
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE |
                ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL,
            0, 0, 100, 100, window, nullptr, GetModuleHandleW(nullptr), nullptr);
        dictionaryFont = CreateFontW(-15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            FIXED_PITCH | FF_MODERN, L"Consolas");
        if (dictionaryEdit && dictionaryFont)
            SendMessageW(dictionaryEdit, WM_SETFONT, reinterpret_cast<WPARAM>(dictionaryFont), TRUE);
        return 0;
    }
    if (message == WM_CTLCOLOREDIT && reinterpret_cast<HWND>(lParam) == dictionaryEdit) {
        HDC dc = reinterpret_cast<HDC>(wParam);
        SetTextColor(dc, RGB(243, 238, 250));
        SetBkColor(dc, RGB(8, 7, 12));
        return reinterpret_cast<LRESULT>(dictionaryEditBrush);
    }
    if (message == WM_SIZE) {
        if (dictionaryEdit) MoveWindow(dictionaryEdit, 10, 10,
            std::max(1, LOWORD(lParam) - 20), std::max(1, HIWORD(lParam) - 20), TRUE);
        return 0;
    }
    if (message == WM_CLOSE) {
        DestroyWindow(window);
        return 0;
    }
    if (message == WM_DESTROY) {
        dictionaryWindow = nullptr;
        dictionaryEdit = nullptr;
        if (dictionaryEditBrush) DeleteObject(dictionaryEditBrush);
        if (dictionaryFont) DeleteObject(dictionaryFont);
        dictionaryEditBrush = nullptr;
        dictionaryFont = nullptr;
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

void showDesktopDictionary(HWND owner) {
    desktopDictionaryText =
        L"TRG LANGUAGE REFERENCE\r\n"
        L"=====================\r\n\r\n"
        L"PROGRAM STRUCTURE\r\n"
        L"  TRG executes source from top to bottom. Commands are case-insensitive.\r\n"
        L"  // comment\r\n"
        L"  Blocks use parentheses: if score > 0 ( ... )\r\n"
        L"  Libraries are declared at file scope: add (math), add (ui), add (3d)\r\n\r\n"
        L"OUTPUT AND INPUT\r\n"
        L"  say VALUE...              Join values; use / to insert a space\r\n"
        L"  print VALUE...            Alias for say\r\n"
        L"  show VALUE...             Alias for say\r\n"
        L"  ask PROMPT VARIABLE       Read a value from the terminal input line\r\n\r\n"
        L"VARIABLES AND TYPES\r\n"
        L"  var NAME = VALUE          Declare a value\r\n"
        L"  num NAME = EXPRESSION     Number\r\n"
        L"  word NAME = TEXT          String\r\n"
        L"  code NAME = COMMAND       Store an executable TRG command\r\n"
        L"  toggle NAME = true|false  Boolean\r\n"
        L"  color NAME = #RRGGBB      Color value\r\n"
        L"  list NAME                 Create a list\r\n"
        L"  set NAME VALUE            Update a variable\r\n"
        L"  increase NAME [AMOUNT]    Add to a number\r\n"
        L"  decrease NAME [AMOUNT]    Subtract from a number\r\n"
        L"  NAME.field                Read or update a record field\r\n\r\n"
        L"EXPRESSIONS AND MATH\r\n"
        L"  Operators: +  -  *  /  %  ^  ( )  <  >  <=  >=  ==  !=  &&  ||\r\n"
        L"  random MIN MAX RESULT     Generate a random value\r\n"
        L"  sqrt(VALUE)               Square root\r\n"
        L"  add (math)                Enables sin, cos, tan, asin, acos, atan,\r\n"
        L"                            sqrt, abs, floor, ceil, round, min, max,\r\n"
        L"                            clamp, distance, and related math helpers\r\n\r\n"
        L"FLOW CONTROL\r\n"
        L"  if CONDITION ( ... )\r\n"
        L"  else ( ... )\r\n"
        L"  loop CONDITION ( ... )\r\n"
        L"  break | continue | stop\r\n"
        L"  wait 250ms | wait 2s\r\n"
        L"  every 100ms ( ... )      Repeating timer block\r\n"
        L"  group NAME ( ... )       Group variables and UI objects\r\n"
        L"  define NAME PARAMS ( ... )   Define a function\r\n"
        L"  call NAME ARGUMENTS      Call a function\r\n"
        L"  return [VALUE]            Return from a function\r\n\r\n"
        L"LISTS AND COLLECTIONS\r\n"
        L"  push LIST VALUE          Append an item\r\n"
        L"  insert LIST INDEX VALUE  Insert an item (zero-based index)\r\n"
        L"  get LIST INDEX RESULT    Read an item\r\n"
        L"  set LIST INDEX VALUE     Replace an item\r\n"
        L"  remove LIST INDEX        Remove an item\r\n"
        L"  pop LIST RESULT          Remove the final item\r\n"
        L"  join LIST SEPARATOR RESULT\r\n"
        L"  split TEXT SEPARATOR RESULT\r\n"
        L"  record TYPE ( ... )      Declare a record type\r\n"
        L"  struct TYPE ( ... )      Alias for record\r\n"
        L"  class TYPE ( ... )       Alias for record\r\n"
        L"  new TYPE VARIABLE        Create a record instance\r\n\r\n"
        L"TEXT HELPERS\r\n"
        L"  upper TEXT RESULT | lower TEXT RESULT | trim TEXT RESULT\r\n"
        L"  length TEXT RESULT | contains TEXT SEARCH RESULT\r\n"
        L"  index_of TEXT SEARCH RESULT\r\n"
        L"  replace TEXT SEARCH WITH RESULT\r\n"
        L"  substring TEXT START COUNT RESULT\r\n"
        L"  starts_with TEXT PREFIX RESULT | ends_with TEXT SUFFIX RESULT\r\n\r\n"
        L"FILES AND PROJECTS  (add (files))\r\n"
        L"  read_file PATH VARIABLE\r\n"
        L"  write_file PATH TEXT... | append_file PATH TEXT...\r\n"
        L"  file_exists PATH VARIABLE | delete_file PATH\r\n"
        L"  add (plugin.trg) | run (file.trg)\r\n"
        L"  add global ( ... ) | add connect (file.trg)\r\n\r\n"
        L"JSON  (add (json))\r\n"
        L"  json GROUP clean RESULT\r\n"
        L"  json GROUP compressed RESULT\r\n"
        L"  json read (PATH)\r\n"
        L"  create FILE (LOCATION) RESULT\r\n\r\n"
        L"DATABASE  (add (database))\r\n"
        L"  database open PATH | database save\r\n"
        L"  table create NAME COLUMN... | table delete NAME | table count NAME RESULT\r\n"
        L"  record add TABLE FIELD VALUE...\r\n"
        L"  record get TABLE INDEX FIELD RESULT\r\n"
        L"  record update TABLE INDEX FIELD VALUE | record delete TABLE INDEX\r\n"
        L"  query TABLE FIELD OP VALUE RESULT\r\n"
        L"  sql select from TABLE where FIELD OP VALUE into RESULT\r\n\r\n"
        L"UI AND 2D  (add (ui))\r\n"
        L"  text NAME VALUE x X y Y size SIZE color COLOR\r\n"
        L"  button NAME LABEL x X y Y w W h H color COLOR ( ... )\r\n"
        L"  block z Z x X y Y w W h H color COLOR name NAME\r\n"
        L"  sprite NAME FILE x X y Y w W h H\r\n"
        L"  ui create block NAME x X y Y w W h H color COLOR shape SHAPE\r\n"
        L"  ui duplicate SOURCE NAME | ui rename OLD NEW | ui delete NAME\r\n"
        L"  ui move NAME DX DY | ui resize NAME DW DH\r\n"
        L"  ui color NAME COLOR | ui shape NAME SHAPE\r\n"
        L"  animate NAME x DX y DY fps FPS time SECONDS\r\n\r\n"
        L"REUSABLE STYLES  (add (style))\r\n"
        L"  style name NAME ( ... )       Define a reusable visual style\r\n"
        L"  style apply OBJECT NAME       Apply a style to a UI object\r\n"
        L"  Properties: background, text, glow COLOR [INTENSITY], radius, opacity\r\n"
        L"  Text properties: font sans|serif|mono, size, bold, align left|center|right\r\n\r\n"
        L"3D SCENES  (add (3d))\r\n"
        L"  3d box NAME x X y Y z Z w W h H d D color COLOR\r\n"
        L"  3d sphere NAME x X y Y z Z size SIZE color COLOR\r\n"
        L"  3d camera NAME x X y Y z Z w W h H d D color COLOR\r\n"
        L"  3d spawn NAME x X y Y z Z size SIZE color COLOR\r\n"
        L"  3d plane NAME x X y Y z Z w W h H d D color COLOR\r\n"
        L"  3d duplicate SOURCE NAME | 3d rename OLD NEW | 3d delete NAME\r\n"
        L"  3d move NAME DX DY DZ | 3d rotate NAME RX RY RZ\r\n"
        L"  3d scale NAME FACTOR | 3d color NAME COLOR\r\n"
        L"  3d texture NAME smooth|grid|metal|wood|stone\r\n"
        L"  3d save FILE.trgscene | 3d load FILE.trgscene\r\n"
        L"  Desktop engine: WASD move, Q/E rise/drop, arrow keys look,\r\n"
        L"  drag empty viewport to orbit, drag assets to add scene objects,\r\n"
        L"  drag models to move, right-click a model for its properties.\r\n\r\n"
        L"COLLISION AND CONTROLS\r\n"
        L"  add (collision) | collides A B RESULT | watch_collision A B RESULT\r\n"
        L"  add (controls) | add (animation)\r\n"
        L"  move_ui OBJECT x DX y DY\r\n\r\n"
        L"WEB, MEDIA, AND AI\r\n"
        L"NETWORK  (add (network))\r\n"
        L"  http_get URL RESULT | http_post URL BODY RESULT  (synchronous)\r\n"
        L"  http_get_async URL REQUEST_ID | http_post_async URL BODY REQUEST_ID\r\n"
        L"  http_result REQUEST_ID RESPONSE STATUS  (pending/complete/error/unknown)\r\n"
        L"  http_forget REQUEST_ID  Release a request result\r\n\r\n"
        L"  add (web): web_page, web_heading, web_text, web_button, web_image, web_show\r\n"
        L"  play_audio FILE | show_image FILE | play_video FILE\r\n"
        L"  add (ai): ai create, teach, train, predict, classify\r\n"
        L"  add (all) enables the built-in libraries.\r\n\r\n"
        L"DEFAULTS AND MANUAL REGISTERS\r\n"
        L"  default num|word|toggle|color VALUE  Change a type's default value\r\n"
        L"  add (manual): register NAME [num|word|toggle|color]\r\n"
        L"  registry NAME [TYPE]        Alias for register\r\n"
        L"  move NAME VALUE             Assign a manual register\r\n"
        L"  add|subtract|multiply|divide NAME VALUE  Numeric register arithmetic\r\n"
        L"  Arithmetic register commands require add (manual).\r\n\r\n"
        L"FUNCTIONS AND ITERATION\r\n"
        L"  define NAME PARAMS ( ... )  Define a reusable function\r\n"
        L"  call NAME ARGUMENTS         Call a function\r\n"
        L"  return [VALUE]               Return from a function\r\n"
        L"  for each ITEM in LIST ( ... )  Iterate through a list\r\n"
        L"  group NAME ( ... )           Run code in a named UI group\r\n"
        L"  hide NAME | show NAME        Hide or show a UI group\r\n\r\n"
        L"DICTIONARIES\r\n"
        L"  dictionary NAME              Create a key/value dictionary\r\n"
        L"  dict_set NAME KEY VALUE      Set a value\r\n"
        L"  dict_get NAME KEY RESULT     Read a value\r\n"
        L"  dict_has NAME KEY RESULT     Test whether a key exists\r\n"
        L"  dict_remove NAME KEY         Remove a key\r\n"
        L"  dict_keys NAME RESULT        Get all keys\r\n"
        L"  dict_count NAME RESULT      Count entries\r\n\r\n"
        L"MATH AND STATISTICS\r\n"
        L"  add (math): math sin|cos|tan|abs|floor|ceil|round|sqrt VALUE RESULT\r\n"
        L"  math min|max A B RESULT | math clamp VALUE MIN MAX RESULT\r\n"
        L"  math distance X1 Y1 X2 Y2 RESULT\r\n"
        L"  add (stats): stats sum|average|min|max|median|range LIST RESULT\r\n\r\n"
        L"CONVERSIONS AND UNITS\r\n"
        L"  add (convert): convert number|text|bool VALUE RESULT\r\n"
        L"  add (units): units c_to_f|f_to_c|km_to_mi|mi_to_km VALUE RESULT\r\n"
        L"  units m_to_ft|ft_to_m|kg_to_lb|lb_to_kg VALUE RESULT\r\n"
        L"  add (time): time unix|milliseconds|year|month|day|hour|minute|second RESULT\r\n\r\n"
        L"BINARY, BITS AND CHECKSUMS\r\n"
        L"  add (binary): binary to_bin|from_bin|to_hex|from_hex VALUE RESULT\r\n"
        L"  binary base64_encode|base64_decode VALUE RESULT\r\n"
        L"  bit and|or|xor A B RESULT | bit not A RESULT\r\n"
        L"  bit shift_left|shift_right VALUE AMOUNT RESULT\r\n"
        L"  checksum crc32|fnv1a TEXT RESULT\r\n\r\n"
        L"TEXT PATTERNS AND COMPRESSION\r\n"
        L"  add (regex): regex match|search TEXT PATTERN RESULT\r\n"
        L"  regex replace TEXT PATTERN REPLACEMENT RESULT\r\n"
        L"  add (compression): compress encode|decode TEXT RESULT\r\n"
        L"  add (files): read_file PATH RESULT | write_file PATH TEXT...\r\n"
        L"  append_file PATH TEXT... | file_exists PATH RESULT | delete_file PATH\r\n\r\n"
        L"NETWORK AND WEBASSEMBLY\r\n"
        L"  add (network): http_get URL RESULT | http_post URL BODY RESULT\r\n"
        L"  add (web): run_wasm FILE.wasm\r\n"
        L"  web_page TITLE | web_heading TEXT | web_text TEXT | web_color COLOR\r\n"
        L"  web_button LABEL [MESSAGE] | web_image FILE | web_show\r\n\r\n"
        L"PHYSICS AND COLLISION\r\n"
        L"  add (physics) and add (ui) to register UI objects as bodies\r\n"
        L"  physics body|solid|trigger OBJECT\r\n"
        L"  physics velocity BODY VX VY | physics position BODY X Y\r\n"
        L"  physics gravity BODY VALUE | physics drag BODY VALUE | physics bounce BODY VALUE\r\n"
        L"  physics drag_launch BODY MAX_SPEED | physics step SECONDS\r\n"
        L"  physics freeze|unfreeze BODY | physics reset BODY | physics remove BODY\r\n"
        L"  add (collision): collides A B RESULT | watch_collision A B RESULT\r\n\r\n"
        L"PARTICLES, VIEWPORT AND MEDIA\r\n"
        L"  particle NAME X Y COLOR SPEED LIFE [SIZE]\r\n"
        L"  scroll X Y                 Move the UI viewport\r\n"
        L"  set_text NAME VALUE | set_color NAME #RRGGBB\r\n"
        L"  play FILE | play_audio FILE | show_image FILE | play_video FILE\r\n"
        L"  animate NAME x DX y DY fps FPS time SECONDS\r\n"
        L"  image NAME FILE x X y Y w W h H | sprite NAME FILE x X y Y w W h H\r\n"
        L"  run (FILE.trg)              Run another file in the current scope\r\n"
        L"  add global ( ... )          Share declarations across project files\r\n"
        L"  add connect (FILE.trg)      Connect variables across project files\r\n"
        L"  add (ai): ai create, teach, train, predict, classify\r\n"
        L"  add (accessibility): enable accessibility helpers\r\n"
        L"  add (powershell): PowerShell integration on supported Windows builds\r\n"
        L"  add (raspberry): Raspberry Pi integration on supported builds\r\n\r\n"
        L"DESKTOP IDE AND TERMINAL\r\n"
        L"  Tab inserts four spaces. Parentheses and quotes pair automatically.\r\n"
        L"  Enter between () creates a blank body line with four-space indentation;\r\n"
        L"  the matching ) stays on its own line at the parent indentation level.\r\n"
        L"  Typing a closing bracket or quote skips over its existing pair.\r\n"
        L"  Select text and type (, \" or ' to wrap the selected text.\r\n"
        L"  The code editor highlights comments, strings, numbers and TRG commands.\r\n"
        L"  Build EXE creates a self-contained Windows TRG runtime package.\r\n"
        L"  Export TRG saves the current source as a .trg file.\r\n"
        L"  Program output has vertical/horizontal scrollbars; select text and use\r\n"
        L"  Ctrl+C or right-click for Copy, Select All and Clear Output.\r\n"
        L"  ask PROMPT VARIABLE writes its prompt here and accepts the answer in\r\n"
        L"  the terminal input line at the bottom of the output panel.\r\n\r\n"
        L"DESKTOP 3D ENGINE CONTROLS\r\n"
        L"  Drag BOX, SPHERE, CAMERA, SPAWN or PLANE from the Content Browser.\r\n"
        L"  Click an asset card to create it in front of the editor camera.\r\n"
        L"  Select, Move, Rotate and Scale choose the active transform tool.\r\n"
        L"  WASD moves the editor camera; hold Shift to move faster.\r\n"
        L"  Q/E move vertically; arrow keys look around; right-drag orbits.\r\n"
        L"  Mouse wheel zooms the viewport; wheel over the hierarchy scrolls it.\r\n"
        L"  Right-click an object for Rename, Duplicate, Delete, Texture,\r\n"
        L"  Focus and Position actions. Save Scene and Load Scene use\r\n"
        L"  scene.trgscene in the current project folder.\r\n"
        L"  Material cards apply Smooth, Grid, Metal, Wood or Stone textures.\r\n\r\n"
        L"LIBRARY LIST\r\n"
        L"  add (files) (json) (database) (web) (collision) (physics) (3d)\r\n"
        L"  add (manual) (ui) (animation) (controls) (network) (ai) (math)\r\n"
        L"  add (binary) (regex) (compression) (powershell) (raspberry)\r\n"
        L"  add (accessibility) (time) (convert) (stats) (units) (all)\r\n"
        L"  Most library declarations apply to the whole file and belong at the top.\r\n\r\n"
        L"COMMAND INDEX\r\n"
        L"  var let num word code toggle color list default register registry dictionary\r\n"
        L"  if else loop for each every wait break continue stop define call return\r\n"
        L"  say print show ask add run create set increase decrease move\r\n"
        L"  http_get http_post http_get_async http_post_async http_result http_forget\r\n"
        L"  add subtract multiply divide random math stats units convert time\r\n"
        L"  upper lower trim ttrim length contains index_of find find_position\r\n"
        L"  replace replace_text count_text starts_with ends_with substring\r\n"
        L"  push insert get remove pop join split dict_set dict_get dict_has\r\n"
        L"  dict_remove dict_keys dict_count json database table record query sql\r\n"
        L"  read_file write_file append_file file_exists delete_file\r\n"
        L"  binary bit checksum regex compress http_get http_post run_wasm\r\n"
        L"  text button block sprite image ui animate group show hide scroll\r\n"
        L"  set_text set_color collides watch_collision physics particle\r\n"
        L"  3d play play_audio show_image play_video web_page web_heading web_text\r\n"
        L"  web_color web_button web_image web_show studio\r\n\r\n"
        L"COLORS accept #RRGGBB or 0xRRGGBB.";
    const wchar_t* className = L"TRGDictionaryWindow";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = dictionaryWindowProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = className;
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return;
    if (!dictionaryWindow) {
        dictionaryWindow = CreateWindowExW(WS_EX_TOOLWINDOW, className,
            L"TRG Command Dictionary", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, scaleForDpi(owner, 760), scaleForDpi(owner, 680),
            owner, nullptr, wc.hInstance, nullptr);
    }
    if (dictionaryWindow) {
        ShowWindow(dictionaryWindow, SW_SHOW);
        SetForegroundWindow(dictionaryWindow);
    }
}


void updateEditorWindowTitle() {
    if (!mainWindow) return;
    std::wstring title = L"TRG Editor - ";
    title += currentSourcePath.empty() ? L"Untitled.trg" : currentSourcePath.filename().wstring();
    if (editorDirty) title += L" *";
    SetWindowTextW(mainWindow, title.c_str());
}

bool confirmDiscardEditorChanges(HWND owner) {
    if (!editorDirty) return true;
    return MessageBoxW(owner, L"Discard unsaved changes to this TRG file?", L"Unsaved TRG code",
                       MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDYES;
}

void appendOutput(const std::string& text) {
#ifdef TRG_TEST_CONSOLE
    std::cout << text;
#else
    const std::wstring addition = wideFromUtf8(text);
    outputBuffer += addition;
    bool trimmed = false;
    if (outputBuffer.size() > 30000) {
        const std::size_t cutoff = outputBuffer.find(L'\n', outputBuffer.size() - 22000);
        if (cutoff != std::wstring::npos) {
            outputBuffer.erase(0, cutoff + 1);
            trimmed = true;
        }
    }
    if (outputControl) {
        if (trimmed) {
            SetWindowTextW(outputControl, outputBuffer.c_str());
        } else {
            SendMessageW(outputControl, EM_SETSEL, static_cast<WPARAM>(-1), static_cast<LPARAM>(-1));
            SendMessageW(outputControl, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(addition.c_str()));
        }
        SendMessageW(outputControl, EM_SETSEL, static_cast<WPARAM>(-1), static_cast<LPARAM>(-1));
        SendMessageW(outputControl, EM_SCROLLCARET, 0, 0);
        SendMessageW(outputControl, WM_VSCROLL, SB_BOTTOM, 0);
    }
    if (mainWindow) InvalidateRect(mainWindow, nullptr, FALSE);
#endif
}
void clearDesktopOutput() {
    outputBuffer.clear();
    if (outputControl) SetWindowTextW(outputControl, L"");
}
#else
void appendOutput(const std::string& text) {
#ifdef __ANDROID__
    androidOutput += text;
#else
    std::cout << text;
#endif
}
#endif

#ifdef _WIN32
LRESULT CALLBACK outputWindowProc(HWND control, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x8000)) {
        if (wParam == 'A') {
            SendMessageW(control, EM_SETSEL, 0, -1);
            return 0;
        }
        if (wParam == 'C') {
            SendMessageW(control, WM_COPY, 0, 0);
            return 0;
        }
    }
    if (message == WM_CONTEXTMENU) {
        POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        if (point.x == -1 && point.y == -1) GetCursorPos(&point);
        HMENU menu = CreatePopupMenu();
        if (menu) {
            AppendMenuW(menu, MF_STRING, 1, L"Copy");
            AppendMenuW(menu, MF_STRING, 2, L"Select all");
            AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
            AppendMenuW(menu, MF_STRING, 3, L"Clear output");
            const UINT selected = TrackPopupMenu(menu,
                TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY,
                point.x, point.y, 0, GetParent(control), nullptr);
            DestroyMenu(menu);
            if (selected == 1) SendMessageW(control, WM_COPY, 0, 0);
            else if (selected == 2) SendMessageW(control, EM_SETSEL, 0, -1);
            else if (selected == 3) clearDesktopOutput();
        }
        return 0;
    }
    return previousOutputProc
        ? CallWindowProcW(previousOutputProc, control, message, wParam, lParam)
        : DefWindowProcW(control, message, wParam, lParam);
}
#endif


std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::string htmlEscape(const std::string& value) {
    std::string result;
    for (char c : value) {
        if (c == '&') result += "&amp;";
        else if (c == '<') result += "&lt;";
        else if (c == '>') result += "&gt;";
        else if (c == '"') result += "&quot;";
        else if (c == '\'') result += "&#39;";
        else result += c;
    }
    return result;
}

std::string base64Encode(const std::string& input) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    unsigned value = 0;
    int bits = -6;
    for (unsigned char byte : input) {
        value = (value << 8) | byte;
        bits += 8;
        while (bits >= 0) {
            output.push_back(alphabet[(value >> bits) & 0x3f]);
            bits -= 6;
        }
    }
    if (bits > -6) output.push_back(alphabet[((value << 8) >> (bits + 8)) & 0x3f]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

std::string base64Decode(const std::string& input, bool& valid) {
    static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output; unsigned value = 0; int bits = -8; bool padding = false; valid = true;
    for (unsigned char c : input) {
        if (std::isspace(c)) continue;
        if (c == '=') { padding = true; continue; }
        if (padding) { valid = false; return {}; }
        const std::size_t index = alphabet.find(static_cast<char>(c));
        if (index == std::string::npos) { valid = false; return {}; }
        value = (value << 6) | static_cast<unsigned>(index); bits += 6;
        if (bits >= 0) { output.push_back(static_cast<char>((value >> bits) & 0xff)); bits -= 8; }
    }
    if (bits >= 0) valid = false;
    return valid ? output : std::string();
}
std::string hexEncode(const std::string& input) {
    static const char digits[] = "0123456789ABCDEF"; std::string output; output.reserve(input.size()*2);
    for (unsigned char byte : input) { output.push_back(digits[byte >> 4]); output.push_back(digits[byte & 15]); }
    return output;
}
bool hexDecode(const std::string& input, std::string& output) {
    auto digit=[](char c)->int { if(c>='0'&&c<='9')return c-'0'; c=static_cast<char>(std::toupper(static_cast<unsigned char>(c))); return c>='A'&&c<='F'?c-'A'+10:-1; };
    std::string clean; for(unsigned char c:input)if(!std::isspace(c))clean.push_back(static_cast<char>(c));
    if(clean.size()%2)return false; output.clear();
    for(std::size_t i=0;i<clean.size();i+=2){int h=digit(clean[i]),l=digit(clean[i+1]);if(h<0||l<0)return false;output.push_back(static_cast<char>((h<<4)|l));}
    return true;
}
std::uint32_t trgCrc32(const std::string& input) {
    std::uint32_t crc=0xffffffffu; for(unsigned char byte:input){crc^=byte;for(int bit=0;bit<8;++bit)crc=(crc>>1)^(0xedb88320u&(0u-(crc&1u)));} return ~crc;
}
std::string rleEncode(const std::string& input) {
    std::string out; for(std::size_t i=0;i<input.size();){std::size_t n=1;while(i+n<input.size()&&input[i+n]==input[i]&&n<255)++n;if(!out.empty())out.push_back(';');out+=std::to_string(n)+":"+hexEncode(input.substr(i,1));i+=n;}return out;
}
bool rleDecode(const std::string& input,std::string& output) {
    output.clear();if(input.empty())return true;std::stringstream stream(input);std::string part;
    while(std::getline(stream,part,';')){auto colon=part.find(':');if(colon==std::string::npos)return false;unsigned n=0;
        try{auto countText=part.substr(0,colon);std::size_t used=0;n=static_cast<unsigned>(std::stoul(countText,&used));if(used!=countText.size()||n==0||n>255)return false;}catch(...){return false;}
        std::string byte;if(!hexDecode(part.substr(colon+1),byte)||byte.size()!=1||output.size()+n>16*1024*1024)return false;output.append(n,byte[0]);}
    return true;
}

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}


std::string stripComment(const std::string& line) {
    bool inQuotes = false;
    for (std::size_t i = 0; i + 1 < line.size(); ++i) {
        if (line[i] == '\\' && inQuotes) { ++i; continue; }
        if (line[i] == '"') inQuotes = !inQuotes;
        else if (!inQuotes && line[i] == '/' && line[i + 1] == '/') return line.substr(0, i);
    }
    return line;
}

bool validName(const std::string& name) {
    if (name.empty()) return false;
    if (!(std::isalpha(static_cast<unsigned char>(name[0])) || name[0] == '_')) return false;
    for (char c : name) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_')) return false;
    }
    return true;
}

bool validVariablePath(const std::string& name) {
    const std::size_t dot = name.find('.');
    return dot == std::string::npos ? validName(name) :
        name.find('.', dot + 1) == std::string::npos &&
        validName(name.substr(0, dot)) && validName(name.substr(dot + 1));
}

void reportError(std::size_t lineNumber, const std::string& reason, const std::string& part = "") {
    const std::size_t sourceLine = lineNumber +
        (trgCallStack.empty() ? 0 : trgCallStack.back().definitionLine);
    std::string msg = "line " + std::to_string(sourceLine) + ": " + reason;
    if (!part.empty()) msg += " \"" + part + "\"";
    for (auto frame = trgCallStack.rbegin(); frame != trgCallStack.rend(); ++frame) {
        msg += "\n  at " + frame->functionName + " (called from line " +
               std::to_string(frame->callLine) + ")";
    }
    ++errorCount;
    appendOutput("ERROR " + msg + "\r\n");
}

class ProgramBlockIndex {
public:
    struct Pair { std::size_t openLine=0, openColumn=0, closeLine=0; };
    ProgramBlockIndex() = default;
    explicit ProgramBlockIndex(const std::vector<std::string>& program) { build(program); }
    void build(const std::vector<std::string>& program) {
        pairs.clear();
        struct Opening { char closer; std::size_t line; std::size_t column; };
        std::vector<Opening> stack;
        for(std::size_t lineNumber=0;lineNumber<program.size();++lineNumber) {
            const std::string& line=program[lineNumber];
            char quote='\0';
            for(std::size_t column=0;column<line.size();++column) {
                const char ch=line[column];
                if(quote!='\0') {
                    if(ch=='\\' && column+1<line.size()) ++column;
                    else if(ch==quote) quote='\0';
                    continue;
                }
                if(ch=='"' || ch=='\'') { quote=ch; continue; }
                if(ch=='/' && column+1<line.size() && line[column+1]=='/') break;
                if(ch=='(' || ch=='{') stack.push_back({ch=='('?')':'}',lineNumber,column});
                else if((ch==')' || ch=='}') && !stack.empty() && ch==stack.back().closer) {
                    const Opening opening=stack.back();
                    stack.pop_back();
                    pairs.push_back({opening.line,opening.column,lineNumber});
                }
            }
        }
        std::sort(pairs.begin(),pairs.end(),[](const Pair& a,const Pair& b) {
            if(a.openLine!=b.openLine) return a.openLine<b.openLine;
            return a.openColumn<b.openColumn;
        });
    }
    bool find(const std::vector<std::string>& program,std::size_t start,std::size_t end,
              std::size_t& open,std::size_t& close) const {
        if(start>=program.size() || start>=end) return false;
        const std::string& line=program[start];
        std::size_t last=line.size();
        while(last>0 && std::isspace(static_cast<unsigned char>(line[last-1]))) --last;
        std::size_t queryLine=start,queryColumn=0;
        if(last>0 && (line[last-1]=='(' || line[last-1]=='{')) queryColumn=last-1;
        else {
            const std::string header=lower(trim(line));
            const bool isHeader=
                header=="if" || header.rfind("if ",0)==0 ||
                header=="else" || header.rfind("else ",0)==0 ||
                header=="loop" || header.rfind("loop ",0)==0 ||
                header=="every" || header.rfind("every ",0)==0 ||
                header=="group" || header.rfind("group ",0)==0 ||
                header=="record" || header.rfind("record ",0)==0 ||
                header.rfind("struct ",0)==0 || header.rfind("class ",0)==0 ||
                header=="define" || header.rfind("define ",0)==0 ||
                header=="add" || header.rfind("add ",0)==0;
            if(isHeader) ++queryLine;
        }
        const auto key=std::make_pair(queryLine,queryColumn);
        const auto found=std::lower_bound(pairs.begin(),pairs.end(),key,
            [](const Pair& candidate,const std::pair<std::size_t,std::size_t>& target) {
                if(candidate.openLine!=target.first) return candidate.openLine<target.first;
                return candidate.openColumn<target.second;
            });
        if(found==pairs.end() || found->closeLine>=end) return false;
        open=found->openLine; close=found->closeLine; return true;
    }
private:
    std::vector<Pair> pairs;
};

bool findBlock(const std::vector<std::string>& program,std::size_t start,
               std::size_t end,std::size_t& open,std::size_t& close,
               const ProgramBlockIndex* index=nullptr) {
    if(index && index->find(program,start,end,open,close)) return true;
    std::vector<char> expectedClosers;
    open=std::string::npos;
    bool opened=false;
    for(close=start;close<end;++close) {
        const std::string& line=program[close];
        std::size_t firstCharacter=0;
        if(close==start) {
            std::size_t lastCharacter=line.size();
            while(lastCharacter>0 && std::isspace(static_cast<unsigned char>(line[lastCharacter-1]))) --lastCharacter;
            if(lastCharacter>0 && (line[lastCharacter-1]=='(' || line[lastCharacter-1]=='{')) {
                const char opener=line[lastCharacter-1];
                open=close; expectedClosers.push_back(opener=='('?')':'}');
                opened=true; firstCharacter=lastCharacter;
            } else {
                const std::string header=lower(trim(line));
                if(header=="if" || header.rfind("if ",0)==0 ||
                   header=="else" || header.rfind("else ",0)==0 ||
                   header=="loop" || header.rfind("loop ",0)==0 ||
                   header=="every" || header.rfind("every ",0)==0 ||
                   header=="group" || header.rfind("group ",0)==0 ||
                   header=="record" || header.rfind("record ",0)==0 ||
                   header.rfind("struct ",0)==0 || header.rfind("class ",0)==0 ||
                   header=="define" || header.rfind("define ",0)==0 ||
                   header=="add" || header.rfind("add ",0)==0) firstCharacter=line.size();
            }
        }
        char quote='\0';
        for(std::size_t i=firstCharacter;i<line.size();++i) {
            const char ch=line[i];
            if(quote!='\0') {
                if(ch=='\\' && i+1<line.size()) ++i;
                else if(ch==quote) quote='\0';
                continue;
            }
            if(ch=='"' || ch=='\'') { quote=ch; continue; }
            if(ch=='/' && i+1<line.size() && line[i+1]=='/') break;
            if(ch=='(' || ch=='{') {
                if(!opened) { open=close; opened=true; }
                expectedClosers.push_back(ch=='('?')':'}');
            } else if((ch==')' || ch=='}') && opened && !expectedClosers.empty() && ch==expectedClosers.back()) {
                expectedClosers.pop_back();
                if(expectedClosers.empty()) return true;
            }
        }
    }
    return false;
}

Defaults standardDefaults() {
    return {{"num", "0"}, {"word", "none"}, {"color", "#222222"}, {"toggle", "false"}};
}

std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> result;
    result.reserve(text.size() / 2 + 1);
    std::string token;
    token.reserve(text.size());
    bool quoted = false;
    char quote = '\0';
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (quoted) {
            if (c == '\\' && i + 1 < text.size()) {
                token += text[++i];
            } else if (c == quote) {
                quoted = false;
            } else {
                token += c;
            }
        } else if (c == '"' || c == '\'') {
            quoted = true;
            quote = c;
        } else if (std::isspace(static_cast<unsigned char>(c)) || c == ',') {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) result.push_back(token);
    return result;
}

std::string valueFor(const std::string& word, const Vars& variables) {
    const auto it = variables.find(word);
    return it == variables.end() ? word : static_cast<std::string>(it->second);
}




std::size_t positionAfterArgument(const std::string& line, const std::string& token) {
    const std::size_t commandEnd = line.find_first_of(" \t");
    if (commandEnd == std::string::npos) return std::string::npos;
    const std::size_t position = line.find(token, commandEnd + 1);
    return position == std::string::npos ? std::string::npos : position + token.size();
}

std::string unbraced(const std::string& value) {
    if (value.size() >= 2 && value.front() == '{' && value.back() == '}') {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

bool parseNumber(const std::string& text, double& value) {
    std::istringstream input(text);
    input >> value;
    return input && (input >> std::ws).eof();
}

bool parseDurationMilliseconds(const std::string& source, long long& milliseconds) {
    std::string value = lower(trim(source));
    bool useMilliseconds = false;
    if (value.size() >= 2 && value.substr(value.size() - 2) == "ms") {
        useMilliseconds = true;
        value.resize(value.size() - 2);
    } else if (!value.empty() && value.back() == 's') {
        value.pop_back();
    }
    double duration = 0;
    if (!parseNumber(value, duration) || duration < 0) return false;
    const double result = useMilliseconds ? duration : duration * 1000.0;
    if (result > static_cast<double>(std::numeric_limits<long long>::max())) return false;
    milliseconds = static_cast<long long>(result);
    return true;
}

std::filesystem::path projectFilePath(const std::string& name, bool& valid) {
    const std::filesystem::path root = std::filesystem::absolute(projectRoot).lexically_normal();
    const std::filesystem::path requested = std::filesystem::u8path(name);
    
    
    if (name.empty() || requested.is_absolute() || requested.has_root_name() || requested.has_root_directory()) {
        valid = false;
        return {};
    }
    const std::filesystem::path target = (root / requested).lexically_normal();
    const std::filesystem::path relative = target.lexically_relative(root);
    valid = !relative.empty() && relative != "." && *relative.begin() != "..";
    return target;
}

std::filesystem::path canonicalProjectPath(const std::filesystem::path& path) {
    std::error_code error;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(path, error);
    return error ? std::filesystem::absolute(path).lexically_normal() : canonical;
}



std::filesystem::path projectExistingFile(const std::string& name, bool& valid) {
    const std::filesystem::path path = projectFilePath(name, valid);
    if (!valid) return {};
    std::error_code error;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(path, error);
    if (error) { valid = false; return {}; }
    const std::filesystem::path root = std::filesystem::weakly_canonical(projectRoot, error);
    if (error) { valid = false; return {}; }
    const std::filesystem::path relative = canonical.lexically_relative(root);
    valid = !relative.empty() && relative != "." && *relative.begin() != "..";
    return canonical;
}

std::string numberString(double value) {
    std::ostringstream output;
    output << std::setprecision(15) << value;
    return output.str();
}

bool parseColor(const std::string& color, unsigned& red, unsigned& green, unsigned& blue) {
    std::string hex = color;
    const bool blueRedGreenFormat = hex.size() >= 2 && hex[0] == '#' && hex[1] == '#';
    if (blueRedGreenFormat) hex.erase(0, 2);
    else if (!hex.empty() && hex[0] == '#') hex.erase(0, 1);
    else if (hex.size() >= 8 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex.erase(0, 2);
        if (hex.size() == 8) hex.erase(0, 2);
    }
    else return false;
    if (hex.size() != 6 || !std::all_of(hex.begin(), hex.end(), [](unsigned char c) { return std::isxdigit(c) != 0; })) return false;
    try {
        if (blueRedGreenFormat) {
            blue = static_cast<unsigned>(std::stoul(hex.substr(0, 2), nullptr, 16));
            red = static_cast<unsigned>(std::stoul(hex.substr(2, 2), nullptr, 16));
            green = static_cast<unsigned>(std::stoul(hex.substr(4, 2), nullptr, 16));
        } else {
            red = static_cast<unsigned>(std::stoul(hex.substr(0, 2), nullptr, 16));
            green = static_cast<unsigned>(std::stoul(hex.substr(2, 2), nullptr, 16));
            blue = static_cast<unsigned>(std::stoul(hex.substr(4, 2), nullptr, 16));
        }
        return true;
    } catch (const std::exception&) {
        return false;
    }
}




bool evaluateExpression(const std::string& expr, Vars& variables, double& result) {
    std::string t = trim(expr);
    if (t.empty()) return false;
    // Avoid constructing a temporary operator string for every character.
    const auto isOperator = [](char c) noexcept {
        switch (c) {
            case '(' : case ')' : case '+' : case '-' : case '*' :
            case '/' : case '%' : case '^' : case ',' : return true;
            default: return false;
        }
    };
    std::vector<std::string> tokens;
    tokens.reserve(t.size() / 2 + 1);
    for (std::size_t i = 0; i < t.size();) {
        if (std::isspace(static_cast<unsigned char>(t[i]))) { ++i; continue; }
        if (isOperator(t[i])) {
            tokens.emplace_back(1, t[i++]);
            continue;
        }
        const std::size_t start = i;
        while (i < t.size() && !std::isspace(static_cast<unsigned char>(t[i])) &&
               !isOperator(t[i])) ++i;
        tokens.push_back(t.substr(start, i - start));
    }
    std::size_t position = 0;
    std::function<bool(double&)> parseSum, parseProduct, parseUnary, parsePower, parsePrimary;
    parsePrimary = [&](double& value) {
        if (position >= tokens.size()) return false;
        const std::string token = tokens[position++];
        if (token == "(") {
            if (!parseSum(value) || position >= tokens.size() || tokens[position++] != ")") return false;
            return true;
        }
        if (lower(token) == "sqrt") {
            const bool parenthesized = position < tokens.size() && tokens[position] == "(";
            if (parenthesized) {
                ++position;
                if (!parseSum(value) || position >= tokens.size() || tokens[position++] != ")") return false;
            } else if (!parseUnary(value)) return false;
            if (value < 0) return false;
            value = std::sqrt(value);
            return true;
        }
        const auto variable = variables.find(token);
        return parseNumber(variable == variables.end() ? token : static_cast<std::string>(variable->second), value);
    };
    parsePower = [&](double& value) {
        if (!parsePrimary(value)) return false;
        if (position < tokens.size() && tokens[position] == "^") {
            ++position;
            double exponent = 0;
            if (!parseUnary(exponent)) return false;
            value = std::pow(value, exponent);
        }
        return std::isfinite(value);
    };
    parseUnary = [&](double& value) {
        if (position < tokens.size() && (tokens[position] == "+" || tokens[position] == "-")) {
            const bool negative = tokens[position++] == "-";
            if (!parseUnary(value)) return false;
            if (negative) value = -value;
            return true;
        }
        return parsePower(value);
    };
    parseProduct = [&](double& value) {
        if (!parseUnary(value)) return false;
        while (position < tokens.size() && (tokens[position] == "*" || tokens[position] == "/" || tokens[position] == "%")) {
            const std::string op = tokens[position++];
            double right = 0;
            if (!parseUnary(right) || ((op == "/" || op == "%") && right == 0)) return false;
            if (op == "*") value *= right;
            else if (op == "/") value /= right;
            else value = std::fmod(value, right);
        }
        return std::isfinite(value);
    };
    parseSum = [&](double& value) {
        if (!parseProduct(value)) return false;
        while (position < tokens.size() && (tokens[position] == "+" || tokens[position] == "-")) {
            const std::string op = tokens[position++];
            double right = 0;
            if (!parseProduct(right)) return false;
            value = op == "+" ? value + right : value - right;
        }
        return std::isfinite(value);
    };
    return parseSum(result) && position == tokens.size();
}

bool conditionIsTrue(const std::string& cond, Vars& variables) {
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    if (controlsEnabled) {
        for (const auto& control : keyboardControls) {
            variables[control.first] = (GetAsyncKeyState(control.second) & 0x8000) ? "press" : "released";
        }

        struct ControllerState {
            unsigned long packetNumber;
            unsigned short buttons;
            unsigned char leftTrigger;
            unsigned char rightTrigger;
            short thumbLX;
            short thumbLY;
            short thumbRX;
            short thumbRY;
        };
        using GetControllerState = unsigned long (WINAPI*)(unsigned long, ControllerState*);
        static HMODULE controllerModule = nullptr;
        static GetControllerState getControllerState = nullptr;
        static bool controllerLookupDone = false;
        if (!controllerLookupDone) {
            controllerLookupDone = true;
            const wchar_t* modules[] = {L"xinput1_4.dll", L"xinput1_3.dll", L"xinput9_1_0.dll"};
            for (const wchar_t* module : modules) {
                controllerModule = LoadLibraryW(module);
                if (controllerModule) {
                    const FARPROC procedure = GetProcAddress(controllerModule, "XInputGetState");
                    static_assert(sizeof(procedure) == sizeof(getControllerState), "Windows function pointers must have equal size");
                    std::memcpy(&getControllerState, &procedure, sizeof(procedure));
                    if (getControllerState) break;
                    FreeLibrary(controllerModule);
                    controllerModule = nullptr;
                }
            }
        }
        ControllerState state = {};
        const bool connected = getControllerState && getControllerState(0, &state) == 0;
        for (const auto& control : controllerControls) {
            bool pressed = false;
            if (connected) {
                if (control.second == 0x10000) pressed = state.leftTrigger >= 30;
                else if (control.second == 0x20000) pressed = state.rightTrigger >= 30;
                else pressed = (state.buttons & control.second) != 0;
            }
            variables[control.first] = pressed ? "press" : "released";
        }
        if (connected) {
            variables["controller_left_stick_x"] = std::clamp(state.thumbLX / 32767.0, -1.0, 1.0);
            variables["controller_left_stick_y"] = std::clamp(state.thumbLY / 32767.0, -1.0, 1.0);
            variables["controller_right_stick_x"] = std::clamp(state.thumbRX / 32767.0, -1.0, 1.0);
            variables["controller_right_stick_y"] = std::clamp(state.thumbRY / 32767.0, -1.0, 1.0);
        }
    }
#elif defined(__ANDROID__)
    if (controlsEnabled) {
        std::lock_guard<std::mutex> lock(androidInputMutex);
        for (const auto& control : androidMobileInputs) {
            variables[control.first] = control.second;
        }
    }
#endif
    std::string t = trim(cond);
    const auto variable = variables.find(t);
    if (variable != variables.end()) t = static_cast<std::string>(variable->second);
    if (lower(t) == "true") return true;
    if (lower(t) == "false") return false;

    const std::vector<std::string> operators = {"==", "!=", "<=", ">=", "=", "<", ">"};
    for (const std::string& op : operators) {
        const std::size_t pos = t.find(op);
        if (pos == std::string::npos) continue;
        const std::string leftText = trim(t.substr(0, pos));
        const std::string rightText = trim(t.substr(pos + op.size()));
        double left = 0, right = 0;
        if (evaluateExpression(leftText, variables, left) && evaluateExpression(rightText, variables, right)) {
            if (op == "==") return left == right;
            if (op == "!=") return left != right;
            if (op == "<=") return left <= right;
            if (op == ">=") return left >= right;
            if (op == "=") return left == right;
            if (op == "<") return left < right;
            return left > right;
        }
        // Decode quoted string literals before comparing them with variables.
        // tokenize() removes their surrounding quotes and preserves spaces.
        const auto resolveConditionValue = [&](const std::string& expression) {
            const std::string raw = trim(expression);
            const std::vector<std::string> parts = tokenize(raw);
            if (parts.size() == 1) {
                const bool quotedLiteral = raw.size() >= 2 &&
                    ((raw.front() == '"' && raw.back() == '"') ||
                     (raw.front() == '\'' && raw.back() == '\''));
                return quotedLiteral ? parts.front() : valueFor(parts.front(), variables);
            }
            return raw;
        };
        const std::string leftValue = resolveConditionValue(leftText);
        const std::string rightValue = resolveConditionValue(rightText);
        return (op == "==" || op == "=") ? leftValue == rightValue :
            op == "!=" ? leftValue != rightValue : false;
    }
    double numeric = 0;
    if (parseNumber(t, numeric)) return numeric != 0;
    return false;
}

bool askForInput(const std::string& prompt, std::string& answer);

void initializeControls(Vars& variables, Types& types) {
    controlsEnabled = true;
    keyboardControls.clear();
    controllerControls.clear();
    const std::pair<const char*, int> keys[] = {
        {"key_a", 'A'}, {"key_b", 'B'}, {"key_c", 'C'}, {"key_d", 'D'},
        {"key_e", 'E'}, {"key_f", 'F'}, {"key_g", 'G'}, {"key_h", 'H'},
        {"key_i", 'I'}, {"key_j", 'J'}, {"key_k", 'K'}, {"key_l", 'L'},
        {"key_m", 'M'}, {"key_n", 'N'}, {"key_o", 'O'}, {"key_p", 'P'},
        {"key_q", 'Q'}, {"key_r", 'R'}, {"key_s", 'S'}, {"key_t", 'T'},
        {"key_u", 'U'}, {"key_v", 'V'}, {"key_w", 'W'}, {"key_x", 'X'},
        {"key_y", 'Y'}, {"key_z", 'Z'}, {"key_0", '0'}, {"key_1", '1'},
        {"key_2", '2'}, {"key_3", '3'}, {"key_4", '4'}, {"key_5", '5'},
        {"key_6", '6'}, {"key_7", '7'}, {"key_8", '8'}, {"key_9", '9'},
        {"key_space", VK_SPACE}, {"key_enter", VK_RETURN}, {"key_escape", VK_ESCAPE},
        {"key_tab", VK_TAB}, {"key_shift", VK_SHIFT}, {"key_control", VK_CONTROL},
        {"key_alt", VK_MENU}, {"key_left", VK_LEFT}, {"key_right", VK_RIGHT},
        {"key_up", VK_UP}, {"key_down", VK_DOWN}
    };
    for (const auto& key : keys) {
        keyboardControls[key.first] = key.second;
        variables[key.first] = "released";
        types[key.first] = "word";
    }
    const std::pair<const char*, unsigned> buttons[] = {
        {"controller_a", 0x1000}, {"controller_b", 0x2000},
        {"controller_x", 0x4000}, {"controller_y", 0x8000},
        {"controller_lb", 0x0100}, {"controller_rb", 0x0200},
        {"controller_lt", 0x10000}, {"controller_rt", 0x20000},
        {"controller_left_stick", 0x0040}, {"controller_right_stick", 0x0080},
        {"controller_start", 0x0010}, {"controller_back", 0x0020},
        {"controller_dpad_up", 0x0001}, {"controller_dpad_down", 0x0002},
        {"controller_dpad_left", 0x0004}, {"controller_dpad_right", 0x0008}
    };
    for (const auto& button : buttons) {
        controllerControls[button.first] = button.second;
        variables[button.first] = "released";
        types[button.first] = "word";
    }
    for (int functionKey = 1; functionKey <= 24; ++functionKey) {
        const std::string name = "key_f" + std::to_string(functionKey);
        keyboardControls[name] = VK_F1 + functionKey - 1;
        variables[name] = "released";
        types[name] = "word";
    }
    for (int digit = 0; digit <= 9; ++digit) {
        const std::string name = "key_numpad" + std::to_string(digit);
        keyboardControls[name] = VK_NUMPAD0 + digit;
        variables[name] = "released";
        types[name] = "word";
    }
    const std::pair<const char*, int> extraKeys[] = {
        {"key_backspace", VK_BACK}, {"key_delete", VK_DELETE}, {"key_insert", VK_INSERT},
        {"key_home", VK_HOME}, {"key_end", VK_END}, {"key_page_up", VK_PRIOR},
        {"key_page_down", VK_NEXT}, {"key_caps_lock", VK_CAPITAL},
        {"key_num_lock", VK_NUMLOCK}, {"key_scroll_lock", VK_SCROLL},
        {"key_print_screen", VK_SNAPSHOT}, {"key_pause", VK_PAUSE},
        {"key_left_windows", VK_LWIN}, {"key_right_windows", VK_RWIN},
        {"key_apps", VK_APPS}, {"key_numpad_add", VK_ADD},
        {"key_numpad_subtract", VK_SUBTRACT}, {"key_numpad_multiply", VK_MULTIPLY},
        {"key_numpad_divide", VK_DIVIDE}, {"key_numpad_decimal", VK_DECIMAL},
        {"key_semicolon", VK_OEM_1}, {"key_equals", VK_OEM_PLUS},
        {"key_comma", VK_OEM_COMMA}, {"key_minus", VK_OEM_MINUS},
        {"key_period", VK_OEM_PERIOD}, {"key_slash", VK_OEM_2},
        {"key_backtick", VK_OEM_3}, {"key_left_bracket", VK_OEM_4},
        {"key_backslash", VK_OEM_5}, {"key_right_bracket", VK_OEM_6},
        {"key_apostrophe", VK_OEM_7}
    };
    for (const auto& key : extraKeys) {
        keyboardControls[key.first] = key.second;
        variables[key.first] = "released";
        types[key.first] = "word";
    }
    variables["pointer_x"] = 0.0;
    variables["pointer_y"] = 0.0;
    variables["pointer_down"] = "released";
    types["pointer_x"] = "num";
    types["pointer_y"] = "num";
    types["pointer_down"] = "word";
    const char* mobileControls[] = {
        "mobile_tap", "mobile_hold", "mobile_swipe_left", "mobile_swipe_right",
        "mobile_swipe_up", "mobile_swipe_down", "mobile_button_1", "mobile_button_2",
        "mobile_button_3", "mobile_button_4", "mobile_button_5", "mobile_button_6",
        "mobile_button_7", "mobile_button_8"
    };
    for (const char* control : mobileControls) {
#ifdef __ANDROID__
        variables[control] = "released";
#else
        variables[control] = "unavailable";
#endif
        types[control] = "word";
    }
#ifdef __ANDROID__
    for (const auto& key : keyboardControls) variables[key.first] = "unavailable";
    for (const auto& button : controllerControls) variables[button.first] = "unavailable";
    {
        std::lock_guard<std::mutex> lock(androidInputMutex);
        androidMobileInputs.clear();
        for (const char* control : mobileControls) androidMobileInputs[control] = "released";
        androidMobileInputs["pointer_x"] = "0";
        androidMobileInputs["pointer_y"] = "0";
        androidMobileInputs["pointer_down"] = "released";
    }
#endif
    const char* controllerAxes[] = {
        "controller_left_stick_x", "controller_left_stick_y",
        "controller_right_stick_x", "controller_right_stick_y"
    };
    for (const char* axis : controllerAxes) {
        variables[axis] = 0.0;
        types[axis] = "num";
    }
}




std::string jsonEscape(const std::string& text) {
    std::string out;
    out.reserve(text.size() + 8);
    for (unsigned char c : text) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    char buffer[8];
                    std::snprintf(buffer, sizeof(buffer), "\\u%04x", static_cast<unsigned>(c));
                    out += buffer;
                } else {
                    out += static_cast<char>(c);
                }
        }
    }
    return out;
}

struct JsonNode {
    enum Kind { Null, Number, Boolean, String, Array, Object } kind = Null;
    double number = 0;
    bool boolean = false;
    std::string text;
    std::vector<JsonNode> items;
    std::vector<std::pair<std::string, JsonNode>> members;
};

void appendUtf8(std::string& out, unsigned codePoint) {
    if (codePoint <= 0x7F) out += static_cast<char>(codePoint);
    else if (codePoint <= 0x7FF) {
        out += static_cast<char>(0xC0 | (codePoint >> 6));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        out += static_cast<char>(0xE0 | (codePoint >> 12));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (codePoint >> 18));
        out += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    }
}

class JsonParser {
public:
    explicit JsonParser(const std::string& source) : source(source) {}
    bool parse(JsonNode& result) {
        skipSpace();
        if (!parseValue(result)) return false;
        skipSpace();
        return position == source.size();
    }
private:
    const std::string& source;
    std::size_t position = 0;

    void skipSpace() {
        while (position < source.size() && std::isspace(static_cast<unsigned char>(source[position]))) ++position;
    }
    bool parseValue(JsonNode& result) {
        skipSpace();
        if (position >= source.size()) return false;
        const char c = source[position];
        if (c == '{') return parseObject(result);
        if (c == '[') return parseArray(result);
        if (c == '"') {
            result.kind = JsonNode::String;
            return parseString(result.text);
        }
        if (source.compare(position, 4, "true") == 0) {
            position += 4; result.kind = JsonNode::Boolean; result.boolean = true; return true;
        }
        if (source.compare(position, 5, "false") == 0) {
            position += 5; result.kind = JsonNode::Boolean; result.boolean = false; return true;
        }
        if (source.compare(position, 4, "null") == 0) {
            position += 4; result.kind = JsonNode::Null; return true;
        }
        return parseNumber(result);
    }
    bool parseString(std::string& result) {
        if (position >= source.size() || source[position] != '"') return false;
        ++position;
        result.clear();
        while (position < source.size()) {
            const char c = source[position++];
            if (c == '"') return true;
            if (c != '\\') { result += c; continue; }
            if (position >= source.size()) return false;
            const char escape = source[position++];
            switch (escape) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                case 'u': {
                    if (position + 4 > source.size()) return false;
                    unsigned codePoint = 0;
                    for (int digit = 0; digit < 4; ++digit) {
                        const char hex = source[position++];
                        codePoint <<= 4;
                        if (hex >= '0' && hex <= '9') codePoint |= static_cast<unsigned>(hex - '0');
                        else if (hex >= 'a' && hex <= 'f') codePoint |= static_cast<unsigned>(hex - 'a' + 10);
                        else if (hex >= 'A' && hex <= 'F') codePoint |= static_cast<unsigned>(hex - 'A' + 10);
                        else return false;
                    }
                    appendUtf8(result, codePoint);
                    break;
                }
                default: return false;
            }
        }
        return false;
    }
    bool parseNumber(JsonNode& result) {
        const std::size_t start = position;
        if (position < source.size() && source[position] == '-') ++position;
        while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        if (position < source.size() && source[position] == '.') {
            ++position;
            while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        }
        if (position < source.size() && (source[position] == 'e' || source[position] == 'E')) {
            ++position;
            if (position < source.size() && (source[position] == '+' || source[position] == '-')) ++position;
            while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        }
        if (position == start) return false;
        const std::string number = source.substr(start, position - start);
        char* end = nullptr;
        const double value = std::strtod(number.c_str(), &end);
        if (!end || *end != '\0') return false;
        result.kind = JsonNode::Number;
        result.number = value;
        return true;
    }
    bool parseArray(JsonNode& result) {
        ++position;
        result.kind = JsonNode::Array;
        skipSpace();
        if (position < source.size() && source[position] == ']') { ++position; return true; }
        while (position < source.size()) {
            JsonNode item;
            if (!parseValue(item)) return false;
            result.items.push_back(std::move(item));
            skipSpace();
            if (position < source.size() && source[position] == ',') { ++position; continue; }
            if (position < source.size() && source[position] == ']') { ++position; return true; }
            return false;
        }
        return false;
    }
    bool parseObject(JsonNode& result) {
        ++position;
        result.kind = JsonNode::Object;
        skipSpace();
        if (position < source.size() && source[position] == '}') { ++position; return true; }
        while (position < source.size()) {
            skipSpace();
            std::string key;
            if (!parseString(key)) return false;
            skipSpace();
            if (position >= source.size() || source[position] != ':') return false;
            ++position;
            JsonNode value;
            if (!parseValue(value)) return false;
            result.members.emplace_back(std::move(key), std::move(value));
            skipSpace();
            if (position < source.size() && source[position] == ',') { ++position; continue; }
            if (position < source.size() && source[position] == '}') { ++position; return true; }
            return false;
        }
        return false;
    }
};

std::string jsonNodeCompact(const JsonNode& node) {
    switch (node.kind) {
        case JsonNode::Null: return "null";
        case JsonNode::Number: return numberString(node.number);
        case JsonNode::Boolean: return node.boolean ? "true" : "false";
        case JsonNode::String: return "\"" + jsonEscape(node.text) + "\"";
        case JsonNode::Array: {
            std::string out = "[";
            for (std::size_t i = 0; i < node.items.size(); ++i) {
                if (i) out += ',';
                out += jsonNodeCompact(node.items[i]);
            }
            out += ']';
            return out;
        }
        case JsonNode::Object: {
            std::string out = "{";
            for (std::size_t i = 0; i < node.members.size(); ++i) {
                if (i) out += ',';
                out += "\"" + jsonEscape(node.members[i].first) + "\":" + jsonNodeCompact(node.members[i].second);
            }
            out += '}';
            return out;
        }
    }
    return "null";
}

std::string jsonValueText(const VariableValue& value) {
    if (const auto* number = std::get_if<double>(&value.value)) return numberString(*number);
    if (const auto* flag = std::get_if<bool>(&value.value)) return *flag ? "true" : "false";
    if (const auto* text = std::get_if<std::string>(&value.value)) return "\"" + jsonEscape(*text) + "\"";
    const auto& items = std::get<std::vector<std::string>>(value.value);
    std::string out = "[";
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i) out += ',';
        out += "\"" + jsonEscape(items[i]) + "\"";
    }
    out += ']';
    return out;
}

std::string serializeJsonObject(const std::vector<std::pair<std::string, VariableValue>>& entries, bool clean) {
    std::vector<std::pair<std::string, VariableValue>> sorted = entries;
    
    std::sort(sorted.begin(), sorted.end(), [](const auto& left, const auto& right) {
        if (left.first.size() != right.first.size()) return left.first.size() > right.first.size();
        return left.first < right.first;
    });
    if (sorted.empty()) return "{}";
    std::string out;
    if (clean) {
        out = "{\n";
        for (std::size_t i = 0; i < sorted.size(); ++i) {
            out += "  \"" + jsonEscape(sorted[i].first) + "\": " + jsonValueText(sorted[i].second);
            if (i + 1 < sorted.size()) out += ',';
            out += '\n';
        }
        out += '}';
    } else {
        out = "{";
        for (std::size_t i = 0; i < sorted.size(); ++i) {
            if (i) out += ',';
            out += "\"" + jsonEscape(sorted[i].first) + "\":" + jsonValueText(sorted[i].second);
        }
        out += '}';
    }
    return out;
}

void flattenJson(const JsonNode& node, const std::string& name, Vars& variables, Types& types) {
    if (node.kind == JsonNode::Object) {
        for (const auto& member : node.members) {
            const std::string key = name.empty() ? member.first : name + "." + member.first;
            flattenJson(member.second, key, variables, types);
        }
        return;
    }
    if (name.empty()) return;
    switch (node.kind) {
        case JsonNode::Number:
            variables[name] = node.number;
            types[name] = "num";
            break;
        case JsonNode::Boolean:
            variables[name] = node.boolean;
            types[name] = "toggle";
            break;
        case JsonNode::Null:
            variables[name] = std::string();
            types[name] = "word";
            break;
        case JsonNode::String:
            variables[name] = node.text;
            types[name] = "word";
            break;
        case JsonNode::Array: {
            std::vector<std::string> items;
            for (const auto& item : node.items) {
                if (item.kind == JsonNode::String) items.push_back(item.text);
                else items.push_back(jsonNodeCompact(item));
            }
            variables[name] = std::move(items);
            types[name] = "list";
            break;
        }
        default:
            break;
    }
}

bool readJsonIntoVariables(const std::filesystem::path& path, Vars& variables, Types& types, std::string& error) {
    std::ifstream input(path, std::ios::binary);
    if (!input) { error = "could not read JSON file"; return false; }
    std::ostringstream content;
    content << input.rdbuf();
    std::string source = content.str();
    if (source.size() >= 3 && static_cast<unsigned char>(source[0]) == 0xEF &&
        static_cast<unsigned char>(source[1]) == 0xBB && static_cast<unsigned char>(source[2]) == 0xBF)
        source.erase(0, 3);
    JsonNode root;
    JsonParser parser(source);
    if (!parser.parse(root)) { error = "invalid JSON"; return false; }
    flattenJson(root, "", variables, types);
    return true;
}

// ---------------------------------------------------------
// HTTP / network implementation (add (network))
// ---------------------------------------------------------
#ifdef _WIN32
std::string httpGet(const std::string& url, std::string& error) {
    error.clear();
    std::string result;
    std::string scheme, host, path;
    std::size_t schemeEnd = url.find("://");
    if (schemeEnd != std::string::npos) {
        scheme = url.substr(0, schemeEnd);
        std::size_t pathStart = url.find('/', schemeEnd + 3);
        if (pathStart != std::string::npos) {
            host = url.substr(schemeEnd + 3, pathStart - schemeEnd - 3);
            path = url.substr(pathStart);
        } else {
            host = url.substr(schemeEnd + 3);
            path = "/";
        }
    } else {
        error = "invalid URL";
        return {};
    }

    HINTERNET session = WinHttpOpen(L"TRG/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { error = "could not open WinHTTP session"; return {}; }
    WinHttpSetTimeouts(session, 5000, 5000, 10000, 10000);

    std::wstring whost = wideFromUtf8(host);
    std::wstring wpath = wideFromUtf8(path);
    INTERNET_PORT port = (lower(scheme) == "https") ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    DWORD flag = (lower(scheme) == "https") ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET connect = WinHttpConnect(session, whost.c_str(), port, 0);
    if (!connect) { WinHttpCloseHandle(session); error = "could not connect"; return {}; }

    HINTERNET request = WinHttpOpenRequest(connect, L"GET", wpath.c_str(),
                                           nullptr, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES, flag);
    if (!request) { WinHttpCloseHandle(connect); WinHttpCloseHandle(session); error = "could not open request"; return {}; }

    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "send request failed: " + std::to_string(err);
        return {};
    }
    if (!WinHttpReceiveResponse(request, nullptr)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "receive response failed: " + std::to_string(err);
        return {};
    }

    DWORD size = 0;
    do {
        size = 0;
        if (!WinHttpQueryDataAvailable(request, &size)) break;
        if (size == 0) break;
        std::vector<char> buffer(size + 1);
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), size, &read)) break;
        buffer[read] = '\0';
        result += buffer.data();
    } while (size > 0);

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return result;
}
#else
std::string httpGet(const std::string& url, std::string& error) { error = "network only on Windows"; return {}; }
#endif

#ifdef _WIN32
std::string httpPost(const std::string& url, const std::string& body, std::string& error) {
    error.clear();
    std::string result;
    std::string scheme, host, path;
    std::size_t schemeEnd = url.find("://");
    if (schemeEnd != std::string::npos) {
        scheme = url.substr(0, schemeEnd);
        std::size_t pathStart = url.find('/', schemeEnd + 3);
        if (pathStart != std::string::npos) {
            host = url.substr(schemeEnd + 3, pathStart - schemeEnd - 3);
            path = url.substr(pathStart);
        } else {
            host = url.substr(schemeEnd + 3);
            path = "/";
        }
    } else {
        error = "invalid URL";
        return {};
    }

    HINTERNET session = WinHttpOpen(L"TRG/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { error = "could not open WinHTTP session"; return {}; }
    WinHttpSetTimeouts(session, 5000, 5000, 10000, 10000);

    std::wstring whost = wideFromUtf8(host);
    std::wstring wpath = wideFromUtf8(path);
    INTERNET_PORT port = (lower(scheme) == "https") ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    DWORD flag = (lower(scheme) == "https") ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET connect = WinHttpConnect(session, whost.c_str(), port, 0);
    if (!connect) { WinHttpCloseHandle(session); error = "could not connect"; return {}; }

    HINTERNET request = WinHttpOpenRequest(connect, L"POST", wpath.c_str(),
                                           nullptr, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES, flag);
    if (!request) { WinHttpCloseHandle(connect); WinHttpCloseHandle(session); error = "could not open request"; return {}; }

    std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    std::wstring wheaders = wideFromUtf8(headers);

    if (!WinHttpSendRequest(request, wheaders.c_str(), (DWORD)wheaders.size(),
                            (LPVOID)body.data(), (DWORD)body.size(), (DWORD)body.size(), 0)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "send request failed: " + std::to_string(err);
        return {};
    }
    if (!WinHttpReceiveResponse(request, nullptr)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "receive response failed: " + std::to_string(err);
        return {};
    }

    DWORD size = 0;
    do {
        size = 0;
        if (!WinHttpQueryDataAvailable(request, &size)) break;
        if (size == 0) break;
        std::vector<char> buffer(size + 1);
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), size, &read)) break;
        buffer[read] = '\0';
        result += buffer.data();
    } while (size > 0);

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return result;
}

#else
std::string httpPost(const std::string&, const std::string&, std::string& error) {
    error = "network only on Windows";
    return {};
}
#endif

// Asynchronous HTTP worker pool. The legacy http_get/http_post commands stay
// synchronous for backwards compatibility; the *_async commands do not block
// the interpreter/UI update thread while network requests are running.
class HttpWorkerPool {
public:
    HttpWorkerPool() = default;
    HttpWorkerPool(const HttpWorkerPool&) = delete;
    HttpWorkerPool& operator=(const HttpWorkerPool&) = delete;
    ~HttpWorkerPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stopping = true;
        }
        ready.notify_all();
        for (std::thread& worker : workers) if (worker.joinable()) worker.join();
    }
    void submit(std::function<void()> job) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopping) throw std::runtime_error("HTTP worker pool is stopping");
            if (workers.empty()) {
                workers.emplace_back([this]() { workerLoop(); });
                workers.emplace_back([this]() { workerLoop(); });
            }
            jobs.push_back(std::move(job));
        }
        ready.notify_one();
    }
private:
    void workerLoop() {
        while (true) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mutex);
                ready.wait(lock, [this]() { return stopping || !jobs.empty(); });
                if (stopping && jobs.empty()) return;
                job = std::move(jobs.front());
                jobs.pop_front();
            }
            try { job(); } catch (...) { /* Keep the worker alive. */ }
        }
    }
    std::mutex mutex;
    std::condition_variable ready;
    std::deque<std::function<void()>> jobs;
    std::vector<std::thread> workers;
    bool stopping = false;
};

HttpWorkerPool& httpWorkerPool() {
    static HttpWorkerPool pool;
    return pool;
}

std::string queueAsyncHttpRequest(const std::string& method, const std::string& url,
                                  const std::string& body, std::string& error) {
    error.clear();
    const std::string requestId = "http_" +
        std::to_string(asyncHttpNextId.fetch_add(1, std::memory_order_relaxed));
    {
        std::lock_guard<std::mutex> lock(asyncHttpResultsMutex);
        if (asyncHttpResults.size() >= 512) {
            for (auto item = asyncHttpResults.begin();
                 item != asyncHttpResults.end() && asyncHttpResults.size() >= 512;) {
                if (item->second.complete) item = asyncHttpResults.erase(item);
                else ++item;
            }
        }
        if (asyncHttpResults.size() >= 512) {
            error = "too many outstanding HTTP requests; release completed requests with http_forget";
            return {};
        }
        asyncHttpResults.emplace(requestId, AsyncHttpResult{});
    }
    try {
        httpWorkerPool().submit([requestId, method, url, body]() {
            std::string requestError;
            std::string response;
            try {
                response = method == "POST"
                    ? httpPost(url, body, requestError)
                    : httpGet(url, requestError);
            } catch (const std::exception& exception) {
                requestError = std::string("HTTP worker error: ") + exception.what();
            } catch (...) {
                requestError = "HTTP worker error";
            }
            std::lock_guard<std::mutex> lock(asyncHttpResultsMutex);
            const auto request = asyncHttpResults.find(requestId);
            if (request != asyncHttpResults.end()) {
                request->second.response = std::move(response);
                request->second.error = std::move(requestError);
                request->second.complete = true;
            }
        });
    } catch (const std::exception& exception) {
        std::lock_guard<std::mutex> lock(asyncHttpResultsMutex);
        asyncHttpResults.erase(requestId);
        error = std::string("could not queue HTTP request: ") + exception.what();
        return {};
    }
    return requestId;
}

bool readAsyncHttpResult(const std::string& requestId, AsyncHttpResult& result) {
    std::lock_guard<std::mutex> lock(asyncHttpResultsMutex);
    const auto request = asyncHttpResults.find(requestId);
    if (request == asyncHttpResults.end()) return false;
    result = request->second;
    return true;
}

void forgetAsyncHttpResult(const std::string& requestId) {
    std::lock_guard<std::mutex> lock(asyncHttpResultsMutex);
    asyncHttpResults.erase(requestId);
}

// ---------------------------------------------------------

// PowerShell library (add (powershell))
// ---------------------------------------------------------
bool psIsSafeCommand(const std::string& cmd) {
    // Whitelist of safe PowerShell commands/verbs-nouns
    static const std::unordered_set<std::string> safeCommands = {
        "Write-Host", "Write-Verbose", "Write-Debug",
        "Get-Help", "Get-Member", "Get-Command",
        "Set-Variable", "Get-Variable", "New-Variable",
        "Clear-Variable", "Remove-Variable",
        "Select-Object", "Where-Object",
        "Measure-Object", "Sort-Object",
        "Export-Csv", "ConvertTo-Csv",
        "Get-Process", "Get-Service",
        "Start-Process", "Stop-Process",
        "Test-Path", "Get-ChildItem",
        "Write-Output", "Read-Host"
    };
    // Check if cmd starts with any safe verb-noun
    for (const auto& safe : safeCommands) {
        if (cmd.rfind(safe, 0) == 0) return true;
    }
    return false;
}

std::string psExecuteSafe(const std::string& code, std::string& error) {
    error.clear();
    // Simple whitelist-based approach: only allow known safe commands
    // Extract first token (command name)
    std::string firstLine = code;
    std::size_t nl = firstLine.find('\n');
    if (nl != std::string::npos) firstLine = firstLine.substr(0, nl);
    firstLine = trim(firstLine);
    if (firstLine.empty()) return "";
    
    std::istringstream iss(firstLine);
    std::string cmd;
    iss >> cmd;
    
    if (!psIsSafeCommand(cmd)) {
        error = "powershell command not in whitelist: " + cmd;
        return {};
    }
    
    // For whitelisted commands, we'll just log/append output what would run
    // In a real implementation, you'd use PowerShell API or invoke with constraints
    // Here we just append the command to output as a safe placeholder
    return "POWERSHELL EXECUTED (whitelisted): " + code;
}

struct LibraryState {
    bool files = false;
    bool json = false;
    bool database = false;
    bool web = false;
    bool collision = false;
    bool manual = false;
    bool ui = false;
    bool controls = false;
    bool network = false;
    bool ai = false;
    bool math = false;
    bool binary = false;
    bool regex = false;
    bool compression = false;
    bool powershell = false;
    bool raspberry = false;
    bool accessibility = false;
    bool time = false;
    bool convert = false;
    bool stats = false;
    bool units = false;
    bool style = false;
    bool physics = false;
    bool threeD = false;
    bool bouncyballs = false;
};

std::string parenContent(const std::string& line) {
    
    
    bool quoted = false;
    char quote = '\0';
    std::size_t open = std::string::npos;
    int depth = 0;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (quoted) {
            if (c == '\\' && i + 1 < line.size()) ++i;
            else if (c == quote) quoted = false;
            continue;
        }
        if (c == '"' || c == '\'') { quoted = true; quote = c; continue; }
        if (c == '(') {
            if (open == std::string::npos) { open = i; depth = 1; }
            else ++depth;
        } else if (c == ')' && open != std::string::npos) {
            if (--depth == 0) return trim(line.substr(open + 1, i - open - 1));
        }
    }
    return "";
}

bool addDirectiveKind(const std::vector<std::string>& tokens, std::string& kind) {
    if (tokens.size() < 2 || lower(tokens[0]) != "add") return false;
    const std::string second = lower(tokens[1]);
    if (second == "global" || second == "(global)") { kind = "global"; return true; }
    if (second == "connect" || second == "(connect)") { kind = "connect"; return true; }
    return false;
}



void stripProjectDirectives(const std::vector<std::string>& program,
                            std::vector<std::string>& kept,
                            std::vector<std::string>& globals,
                            std::vector<std::string>& connects,
                            bool report,
                            std::unordered_set<std::string>* seenGlobals = nullptr) {
    for (std::size_t i = 0; i < program.size(); ++i) {
        const std::string line = trim(program[i]);
        const std::vector<std::string> tokens = tokenize(line);
        std::string kind;
        if (!addDirectiveKind(tokens, kind)) {
            kept.push_back(program[i]);
            continue;
        }
        if (kind == "global") {
            std::size_t open = 0, close = 0;
            if (findBlock(program, i, program.size(), open, close)) {
                std::string signature;
                for (std::size_t row = open + 1; row < close; ++row) signature += program[row] + '\n';
                if (seenGlobals && !seenGlobals->insert(signature).second) {
                    i = close;
                    continue;
                }
                for (std::size_t row = open + 1; row < close; ++row) globals.push_back(program[row]);
                i = close;
            } else if (report) {
                reportError(i + 1, "add global needs a ( ... ) block");
            }
            continue;
        }
        std::string target;
        if (lower(tokens[1]) == "(connect)") {
            if (tokens.size() >= 3) target = tokens[2];
        } else {
            target = parenContent(line);
        }
        if (target.empty() && tokens.size() >= 3) target = tokens[2];
        if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
            target = target.substr(1, target.size() - 2);
        if (!target.empty()) connects.push_back(target);
        else if (report) reportError(i + 1, "add connect needs a file name");
    }
}

void scanLibraries(const std::vector<std::string>& program, LibraryState& libraries) {
    for (const auto& raw : program) {
        const std::vector<std::string> tokens = tokenize(trim(raw));
        if (tokens.size() < 2 || lower(tokens[0]) != "add") continue;
        const std::string second = lower(tokens[1]);
        if (second == "global" || second == "(global)" || second == "connect" || second == "(connect)") continue;
        if (second.size() < 3 || second.front() != '(' || second.back() != ')') continue;
        const std::string name = lower(second.substr(1, second.size() - 2));
        if (name == "all") {
            libraries.files = libraries.json = libraries.database = libraries.web = libraries.collision = true;
            libraries.manual = libraries.ui = libraries.controls = libraries.network = libraries.ai = libraries.math = true;
            libraries.binary = libraries.regex = libraries.compression = true;
            libraries.powershell = libraries.raspberry = libraries.accessibility = true;
            libraries.stats = libraries.units = true;
            libraries.physics = true;
            libraries.style = true;
            libraries.threeD = true;
            libraries.bouncyballs = true;
        } else if (name == "files") libraries.files = true;
        else if (name == "json") libraries.json = true;
        else if (name == "database" || name == "db") libraries.database = true;
        else if (name == "web") libraries.web = true;
        else if (name == "collision") libraries.collision = true;
        else if (name == "physics") libraries.physics = true;
        else if (name == "3d") libraries.threeD = true;
        else if (name == "bouncyballs") libraries.bouncyballs = true;
        else if (name == "manual") libraries.manual = true;
        else if (name == "ui" || name == "animation") libraries.ui = true;
        else if (name == "controls") libraries.controls = true;
        else if (name == "network") libraries.network = true;
        else if (name == "ai") libraries.ai = true;
        else if (name == "math") libraries.math = true;
        else if (name == "binary") libraries.binary = true;
        else if (name == "regex") libraries.regex = true;
        else if (name == "compression") libraries.compression = true;
        else if (name == "powershell") libraries.powershell = true;
        else if (name == "raspberry") libraries.raspberry = true;
        else if (name == "accessibility") libraries.accessibility = true;
        else if (name == "time") libraries.time = true;
        else if (name == "convert") libraries.convert = true;
        else if (name == "stats") libraries.stats = true;
        else if (name == "units") libraries.units = true;
        else if (name == "style") libraries.style = true;
    }
}

void collectTrgFiles(const std::filesystem::path& root, std::vector<std::filesystem::path>& files) {
    std::error_code error;
    const std::filesystem::path canonicalRoot = std::filesystem::weakly_canonical(root, error);
    std::filesystem::recursive_directory_iterator iterator(root,
        std::filesystem::directory_options::skip_permission_denied, error);
    const std::filesystem::recursive_directory_iterator end;
    static const std::unordered_set<std::string> ignoredDirectories = {
        ".git", ".svn", ".hg", ".idea", ".vs", ".gradle", ".cache",
        "node_modules", "vendor", "build", "dist", "target", "out",
        "bin", "obj", "debug", "release", ".venv", "venv", "__pycache__"
    };
    while (!error && iterator != end) {
        std::error_code entryError;
        if (iterator->is_directory(entryError) && !entryError) {
            const std::string directoryName = lower(iterator->path().filename().u8string());
            if ((!directoryName.empty() && directoryName.front() == '.') ||
                ignoredDirectories.count(directoryName)) iterator.disable_recursion_pending();
        } else if (iterator->is_regular_file(entryError) && !entryError &&
            lower(iterator->path().extension().u8string()) == ".trg") {
            
            std::error_code canonicalError;
            const std::filesystem::path canonical =
                std::filesystem::weakly_canonical(iterator->path(), canonicalError);
            bool inside = !canonicalError;
            if (inside) {
                const std::filesystem::path relative = canonical.lexically_relative(canonicalRoot);
                inside = !relative.empty() && relative != "." && *relative.begin() != "..";
            }
            if (inside) files.push_back(iterator->path());
        }
        iterator.increment(error);
    }
    std::sort(files.begin(), files.end());
}





std::vector<std::string> mergePrograms(const std::vector<std::string>& mainProgram,
                                       const std::filesystem::path& mainPath,
                                       bool includeProjectGlobals, bool report,
                                       const std::filesystem::path& excludePath = {}) {
    struct ProjectNode {
        std::filesystem::path path;
        std::vector<std::string> program;
        std::vector<std::string> connects;
    };

    std::vector<std::string> globals;
    std::vector<std::string> keptMain;
    std::vector<std::string> mainConnects;
    std::unordered_set<std::string> seenGlobals;
    stripProjectDirectives(mainProgram, keptMain, globals, mainConnects, report, &seenGlobals);

    const std::filesystem::path mainCanonical = mainPath.empty() ? std::filesystem::path() : canonicalProjectPath(mainPath);
    const std::string mainKey = mainCanonical.empty() ? "__main__" : lower(mainCanonical.u8string());

    std::unordered_map<std::string, ProjectNode> nodes;
    // Only project-wide globals require directory discovery. Explicitly connected
    // modules are loaded lazily below, so ordinary scripts avoid scanning folders.
    if (includeProjectGlobals) {
        std::vector<std::filesystem::path> files;
        collectTrgFiles(projectRoot, files);
        for (const auto& file : files) {
            const std::filesystem::path canonical = canonicalProjectPath(file);
            const std::string key = lower(canonical.u8string());
            if (key == mainKey) continue;
            std::ifstream input(file, std::ios::binary);
            if (!input) continue;
            std::vector<std::string> kept, localGlobals, connects;
            stripProjectDirectives(readProgram(input), kept, localGlobals, connects, false, &seenGlobals);
            globals.insert(globals.end(), localGlobals.begin(), localGlobals.end());
            nodes[key] = {canonical, std::move(kept), std::move(connects)};
        }
    }
    nodes[mainKey] = {mainCanonical, keptMain, mainConnects};

    std::unordered_map<std::string, std::vector<std::string>> adjacency;
    std::unordered_map<std::string, std::vector<std::string>> connectionErrors;
    std::unordered_set<std::string> processed;
    std::deque<std::string> pending;
    for (const auto& item : nodes) pending.push_back(item.first);

    auto loadConnectedNode = [&](const std::string& raw, const std::string& owner) -> std::string {
        bool valid = false;
        const std::filesystem::path path = projectExistingFile(raw, valid);
        if (!valid) {
            connectionErrors[owner].push_back("connect target must stay inside the TRG project folder: \"" + raw + "\"");
            return {};
        }
        const std::filesystem::path canonical = canonicalProjectPath(path);
        const std::string key = lower(canonical.u8string());
        if (nodes.count(key)) return key;
        std::ifstream input(canonical, std::ios::binary);
        if (!input) {
            connectionErrors[owner].push_back("could not open connected file: \"" + canonical.u8string() + "\"");
            return {};
        }
        std::vector<std::string> kept, localGlobals, connects;
        stripProjectDirectives(readProgram(input), kept, localGlobals, connects, false, &seenGlobals);
        if (includeProjectGlobals) globals.insert(globals.end(), localGlobals.begin(), localGlobals.end());
        nodes[key] = {canonical, std::move(kept), std::move(connects)};
        return key;
    };

    while (!pending.empty()) {
        const std::string key = pending.front();
        pending.pop_front();
        if (!processed.insert(key).second) continue;
        for (const auto& connect : nodes[key].connects) {
            const std::string target = loadConnectedNode(connect, key);
            if (target.empty() || target == key) continue;
            adjacency[key].push_back(target);
            adjacency[target].push_back(key);
            if (!processed.count(target)) pending.push_back(target);
        }
    }

    
    std::vector<std::string> order;
    std::unordered_set<std::string> visited;
    std::deque<std::string> queue;
    queue.push_back(mainKey);
    visited.insert(mainKey);
    while (!queue.empty()) {
        const std::string key = queue.front();
        queue.pop_front();
        order.push_back(key);
        const auto edges = adjacency.find(key);
        if (edges == adjacency.end()) continue;
        for (const auto& next : edges->second) {
            if (visited.insert(next).second) queue.push_back(next);
        }
    }

    if (report) {
        for (const auto& key : order) {
            const auto errors = connectionErrors.find(key);
            if (errors == connectionErrors.end()) continue;
            for (const auto& message : errors->second) {
                ++errorCount;
                appendOutput("ERROR " + message + "\r\n");
            }
        }
    }

    std::string excludeKey;
    if (!excludePath.empty()) excludeKey = lower(canonicalProjectPath(excludePath).u8string());

    std::vector<std::string> merged;
    merged.insert(merged.end(), globals.begin(), globals.end());
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (*it == mainKey || *it == excludeKey) continue;
        if (executedFileKeys.count(*it)) continue;
        const auto node = nodes.find(*it);
        if (node == nodes.end()) continue;
        merged.insert(merged.end(), node->second.program.begin(), node->second.program.end());
    }
    merged.insert(merged.end(), keptMain.begin(), keptMain.end());
    for (const auto& key : order) executedFileKeys.insert(key);
    return merged;
}




void registerFunctions(const std::vector<std::string>& program, bool clearFirst = true) {
    if (clearFirst) userFunctions.clear();
    for (std::size_t i = 0; i < program.size(); ++i) {
        std::string line = trim(program[i]);
        if (lower(line).rfind("define ", 0) != 0) continue;

        std::istringstream in(line);
        std::string kw, name;
        in >> kw >> name;
        if (!validName(name)) continue;

        std::vector<std::string> params;
        std::string p;
        while (in >> p) {
            if (p == "(") break; 
            if (p != ",") params.push_back(p);
        }

        std::size_t open, close;
        if (findBlock(program, i, program.size(), open, close)) {
            UserFunction function;
            function.name = name;
            function.params = params;
            function.body.assign(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                                 program.begin() + static_cast<std::ptrdiff_t>(close));
            function.sourceLine = i + 1;
            function.blockIndex = std::make_shared<ProgramBlockIndex>(function.body);
            userFunctions[lower(name)] = std::move(function);
        }
    }
}


std::string databaseEncode(const std::string& value) { return value.empty() ? "-" : hexEncode(value); }
bool databaseDecode(const std::string& encoded, std::string& value) {
    if (encoded == "-") { value.clear(); return true; }
    return hexDecode(encoded, value);
}
std::size_t databaseColumnIndex(const TrgDatabaseTable& table, const std::string& name) {
    const auto found = std::find(table.columns.begin(), table.columns.end(), name);
    return found == table.columns.end() ? table.columns.size() : static_cast<std::size_t>(found - table.columns.begin());
}
bool saveTrgDatabase(std::string& error) {
    error.clear();
    if (trgDatabasePath.empty()) { error = "open a database first"; return false; }
    std::ofstream out(trgDatabasePath, std::ios::binary | std::ios::trunc);
    if (!out) { error = "could not write database file"; return false; }
    out << "TRGDB1\n";
    std::vector<std::string> names;
    for (const auto& item : trgDatabaseTables) names.push_back(item.first);
    std::sort(names.begin(), names.end());
    for (const auto& name : names) {
        const auto& table = trgDatabaseTables.at(name);
        out << "T\t" << databaseEncode(name) << "\t" << table.columns.size() << "\t" << table.rows.size() << "\n";
        for (const auto& column : table.columns) out << "C\t" << databaseEncode(column) << "\n";
        for (const auto& row : table.rows) {
            out << "R";
            for (const auto& cell : row) out << "\t" << databaseEncode(cell);
            out << "\n";
        }
    }
    if (!out) { error = "database write failed"; return false; }
    return true;
}
bool loadTrgDatabase(const std::filesystem::path& path, std::string& error) {
    error.clear();
    std::ifstream in(path, std::ios::binary);
    if (!in) { error = "could not read database file"; return false; }
    std::string line;
    if (!std::getline(in, line) || line != "TRGDB1") { error = "not a supported TRG database file"; return false; }
    std::unordered_map<std::string, TrgDatabaseTable> loaded;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> parts;
        std::istringstream stream(line);
        std::string part;
        while (std::getline(stream, part, '\t')) parts.push_back(part);
        if (parts.size() != 4 || parts[0] != "T") { error = "corrupt database table header"; return false; }
        std::string name;
        if (!databaseDecode(parts[1], name) || name.empty()) { error = "corrupt database table name"; return false; }
        std::size_t columns = 0, rows = 0;
        try { columns = static_cast<std::size_t>(std::stoull(parts[2])); rows = static_cast<std::size_t>(std::stoull(parts[3])); }
        catch (...) { error = "corrupt database dimensions"; return false; }
        if (columns > 1024 || rows > 1000000) { error = "database dimensions exceed safety limits"; return false; }
        TrgDatabaseTable table;
        for (std::size_t c = 0; c < columns; ++c) {
            if (!std::getline(in, line) || line.rfind("C\t", 0) != 0) { error = "corrupt database column"; return false; }
            std::string value;
            if (!databaseDecode(line.substr(2), value) || value.empty() ||
                std::find(table.columns.begin(), table.columns.end(), value) != table.columns.end()) { error = "invalid or duplicate database column"; return false; }
            table.columns.push_back(std::move(value));
        }
        for (std::size_t r = 0; r < rows; ++r) {
            if (!std::getline(in, line)) { error = "incomplete database records"; return false; }
            std::istringstream cellsStream(line);
            std::string cell;
            if (!std::getline(cellsStream, cell, '\t') || cell != "R") { error = "corrupt database record"; return false; }
            std::vector<std::string> values;
            while (std::getline(cellsStream, cell, '\t')) {
                std::string value;
                if (!databaseDecode(cell, value)) { error = "corrupt database value"; return false; }
                values.push_back(std::move(value));
            }
            if (values.size() != columns) { error = "database record has the wrong number of fields"; return false; }
            table.rows.push_back(std::move(values));
        }
        if (!loaded.emplace(name, std::move(table)).second) { error = "duplicate database table"; return false; }
    }
    trgDatabaseTables = std::move(loaded);
    trgDatabasePath = path;
    return true;
}

void executeBlock(const std::vector<std::string>& program, std::size_t begin, std::size_t end,
                  Vars& variables, Types& types, Defaults& defaults, Flow& flow,
                  bool manualMode = false, bool uiMode = false,
                  const ProgramBlockIndex* blockIndex = nullptr) {
    ProgramBlockIndex localBlockIndex;
    if (!blockIndex) {
        localBlockIndex.build(program);
        blockIndex = &localBlockIndex;
    }
    for (std::size_t i = begin; i < end; ++i) {
#ifdef __ANDROID__
        if (androidStopRequested.load()) {
            flow.stopped = true;
            return;
        }
#endif
        if (flow.stopped || flow.broke || flow.continued || flow.returned) return;
        
        std::string line = trim(program[i]);
        if (line.empty() || line == "(" || line == ")") continue;
        if (line == "{" || line == "}") {
            reportError(i + 1, "TRG blocks use ( and ); braces are not supported");
            continue;
        }
        
        std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty()) continue;

        // A code variable stores a TRG command. Calling it expands that command
        // and appends any arguments supplied after the variable name.
        const auto codeVariable = variables.find(tokens[0]);
        if (codeVariable != variables.end() && types.count(tokens[0]) && types[tokens[0]] == "code") {
            const std::string codeSource = static_cast<std::string>(codeVariable->second);
            const std::size_t argumentStart = positionAfterArgument(line, tokens[0]);
            const std::string arguments = argumentStart == std::string::npos ? "" : trim(line.substr(argumentStart));
            line = codeSource;
            if (!arguments.empty()) {
                if (!line.empty() && line.back() != '\n' && line.back() != '\r') line += ' ';
                line += arguments;
            }
            tokens = tokenize(line);
            if (tokens.empty()) continue;
        }

        std::string cmd = lower(tokens[0]);
        if (cmd == "print" || cmd == "show") cmd = "say";
        if (cmd == "ttrim") cmd = "trim";


        
        
        if (cmd == "set") {
            if (tokens.size() < 3 || !validVariablePath(tokens[1])) {
                reportError(i + 1, "use set VARIABLE VALUE", "set");
                continue;
            }
            const std::size_t namePosition = positionAfterArgument(line, tokens[1]);
            if (namePosition == std::string::npos) { reportError(i + 1, "set needs a valid variable", tokens[1]); continue; }
            std::string raw = trim(line.substr(namePosition));
            if (!raw.empty() && raw.front() == '=') raw = trim(raw.substr(1));
            const std::string type = types.count(tokens[1]) ? types[tokens[1]] : "word";
            if (type == "num") {
                double number = 0;
                if (!evaluateExpression(raw, variables, number)) { reportError(i + 1, "set needs a valid number", raw); continue; }
                variables[tokens[1]] = number;
            } else {
                const auto parts = tokenize(raw);
                std::string value;
                for (std::size_t part = 0; part < parts.size(); ++part) {
                    if (part) value += ' ';
                    value += valueFor(parts[part], variables);
                }
                if (type == "toggle") {
                    const std::string flag = lower(value);
                    if (flag != "true" && flag != "false" && flag != "yes" && flag != "no" && value != "1" && value != "0") {
                        reportError(i + 1, "set toggle needs true or false", value); continue;
                    }
                    variables[tokens[1]] = flag == "true" || flag == "yes" || value == "1";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "set color needs a valid color", value); continue; }
                    variables[tokens[1]] = value;
                } else if (type == "list") {
                    reportError(i + 1, "use push or remove to change list values", tokens[1]);
                    continue;
                } else variables[tokens[1]] = value;
            }
            if (!types.count(tokens[1])) types[tokens[1]] = type;
            continue;
        }
        if (cmd == "increase" || cmd == "decrease") {
            if ((tokens.size() != 2 && tokens.size() != 3) || !validVariablePath(tokens[1]) ||
                !variables.count(tokens[1]) || !types.count(tokens[1]) || types[tokens[1]] != "num") {
                reportError(i + 1, "use increase/decrease NUMBER_VARIABLE [AMOUNT]");
                continue;
            }
            const std::string amountText = tokens.size() == 3 ? tokens[2] : "1";
            double amount = 0, current = 0;
            if (!evaluateExpression(amountText, variables, amount) || !parseNumber(static_cast<std::string>(variables[tokens[1]]), current)) {
                reportError(i + 1, "increase/decrease needs numeric values", tokens[1]);
                continue;
            }
            variables[tokens[1]] = current + (cmd == "increase" ? amount : -amount);
            continue;
        }

        
        
        if (cmd == "record" || cmd == "struct" || cmd == "class") {
            std::size_t open = 0, close = 0;
            const bool headerShape = tokens.size() == 2 || (tokens.size() == 3 && tokens[2] == "(");
            if (!headerShape || !validName(tokens[1]) || !findBlock(program, i, end, open, close, blockIndex)) {
                reportError(i + 1, "use record NAME ( word field; num count = 0 )", tokens.size() > 1 ? tokens[1] : "");
                continue;
            }
            StructDefinition definition;
            for (std::size_t row = open + 1; row < close; ++row) {
                const auto field = tokenize(trim(program[row]));
                if (field.empty()) continue;
                const std::string fieldType = lower(field[0]);
                if (field.size() < 2 || !validName(field[1]) ||
                    (fieldType != "word" && fieldType != "num" && fieldType != "toggle" && fieldType != "color")) {
                    reportError(row + 1, "record fields use word, num, toggle, or color", field[0]);
                    continue;
                }
                std::string initial;
                const std::size_t equals = trim(program[row]).find('=');
                if (equals != std::string::npos) {
                    const auto initialTokens = tokenize(trim(program[row]).substr(equals + 1));
                    for (std::size_t part = 0; part < initialTokens.size(); ++part) {
                        if (part) initial += ' ';
                        initial += initialTokens[part];
                    }
                }
                definition[field[1]] = {fieldType, initial};
            }
            userStructs[tokens[1]] = std::move(definition);
            i = close;
            continue;
        }
        if (cmd == "new") {
            if (tokens.size() != 3 || !validName(tokens[2]) || !userStructs.count(tokens[1])) {
                reportError(i + 1, "use new TYPE instanceName (define TYPE with record first)");
                continue;
            }
            for (const auto& field : userStructs[tokens[1]]) {
                const std::string key = tokens[2] + "." + field.first;
                const std::string raw = field.second.initial;
                const std::string initial = raw.empty() ? defaults[field.second.type] : valueFor(raw, variables);
                if (field.second.type == "num") {
                    double number = 0;
                    if (!evaluateExpression(raw.empty() ? initial : raw, variables, number)) number = 0;
                    variables[key] = number;
                } else if (field.second.type == "toggle") variables[key] = lower(initial) == "true";
                else variables[key] = initial;
                types[key] = field.second.type;
            }
            continue;
        }

        
        if (tokens.size() == 3 && validName(tokens[0]) &&
            (std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& block) { return block.name == tokens[0]; }) ||
             std::any_of(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& sprite) { return sprite.name == tokens[0]; }) ||
             std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& button) { return button.name == tokens[0]; }) ||
             std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& text) { return text.name == tokens[0]; }))) {
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == tokens[0]; });
            auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == tokens[0]; });
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == tokens[0]; });
            auto text = std::find_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == tokens[0]; });
            const std::string property = lower(tokens[1]);
            const std::string raw = valueFor(tokens[2], variables);
            if (text != uiTexts.end() && (property == "text" || property == "value")) { text->value = raw; continue; }
            if (button != uiButtons.end() && (property == "label" || property == "text")) { button->label = raw; continue; }
            if (property == "visible" || property == "show") {
                const bool visible = lower(raw) == "true" || lower(raw) == "yes" || raw == "1";
                if (block != uiBlocks.end()) block->visible = visible;
                else if (sprite != uiSprites.end()) sprite->visible = visible;
                else if (button != uiButtons.end()) button->visible = visible;
                else text->visible = visible;
                continue;
            }
            if (text != uiTexts.end() && property == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid text color", raw);
                else text->color = raw;
                continue;
            }
            if (text != uiTexts.end() && (property == "font" || property == "align")) {
                const std::string value = lower(raw);
                if (property == "font" && value != "sans" && value != "serif" && value != "mono" && value != "monospace")
                    reportError(i + 1, "font must be sans, serif, or mono", raw);
                else if (property == "align" && value != "left" && value != "center" && value != "right")
                    reportError(i + 1, "text alignment must be left, center, or right", raw);
                else if (property == "font") text->font = value == "monospace" ? "mono" : value;
                else text->align = value;
                continue;
            }
            if (property == "edges" || property == "shape") {
                if (lower(raw) != "square" && lower(raw) != "round" && lower(raw) != "rounded") {
                    reportError(i + 1, "UI edges must be round or square", raw); continue;
                }
                if (block != uiBlocks.end()) block->cornerRadius = lower(raw) == "square" ? 0 : 10;
                else if (button != uiButtons.end()) button->cornerRadius = lower(raw) == "square" ? 0 : 7;
                continue;
            }
            if (property == "color" && (block != uiBlocks.end() || button != uiButtons.end())) {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid UI color", raw);
                else if (block != uiBlocks.end()) block->color = raw;
                else button->color = raw;
                continue;
            }
            if (property == "opacity" || property == "size") {
                double number = 0;
                if (!parseNumber(raw, number) || number < 0 || number > (property == "opacity" ? 1.0 : 100.0)) {
                    reportError(i + 1, "opacity must be 0..1 and text size must be 0..100", raw); continue;
                }
                if (block != uiBlocks.end() && property == "opacity") block->opacity = number;
                else if (sprite != uiSprites.end() && property == "opacity") sprite->opacity = number;
                else if (button != uiButtons.end() && property == "opacity") button->opacity = number;
                else if (text != uiTexts.end() && property == "opacity") text->opacity = number;
                else if (text != uiTexts.end()) text->size = number;
                else { reportError(i + 1, "opacity or size is not supported by this UI object", property); continue; }
                continue;
            }
            if (text != uiTexts.end() && property == "bold") { text->bold = lower(raw) == "true" || raw == "1"; continue; }
            if (button != uiButtons.end() && property == "font") {
                const std::string value = lower(raw);
                if (value == "sans" || value == "serif" || value == "mono" || value == "monospace") button->font = value == "monospace" ? "mono" : value;
                else reportError(i + 1, "button font must be sans, serif, or mono", raw);
                continue;
            }
            if (button != uiButtons.end() && property == "text_color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid button text color", raw);
                else button->textColor = raw;
                continue;
            }
            if (button != uiButtons.end() && property == "bold") { button->textBold = lower(raw) == "true" || raw == "1"; continue; }
            if (property == "x" || property == "y" || property == "w" || property == "h" || property == "radius") {
                double number = 0;
                const bool relative = raw.size() > 1 && (raw[0] == '+' || raw[0] == '-');
                if (!parseNumber(raw, number)) { reportError(i + 1, "UI position and size need a number", raw); continue; }
                if (block != uiBlocks.end()) {
                    if (property == "x") block->x = relative ? block->x + number : number;
                    else if (property == "y") block->y = relative ? block->y + number : number;
                    else if (property == "w") block->width = number;
                    else if (property == "h") block->height = number;
                    else block->cornerRadius = std::max(0.0, number);
                } else if (sprite != uiSprites.end()) {
                    if (property == "x") sprite->x = relative ? sprite->x + number : number;
                    else if (property == "y") sprite->y = relative ? sprite->y + number : number;
                    else if (property == "w") sprite->width = number;
                    else if (property == "h") sprite->height = number;
                } else if (button != uiButtons.end()) {
                    if (property == "x") button->x = relative ? button->x + number : number;
                    else if (property == "y") button->y = relative ? button->y + number : number;
                    else if (property == "w") button->width = number;
                    else if (property == "h") button->height = number;
                    else button->cornerRadius = std::max(0.0, number);
                } else if (text != uiTexts.end()) {
                    if (property == "x") text->x = relative ? text->x + number : number;
                    else if (property == "y") text->y = relative ? text->y + number : number;
                    else { reportError(i + 1, "text only supports x, y, size, and opacity", property); continue; }
                }
                refreshCollisionWatches(variables, types);
                continue;
            }
        }
        if (tokens.size() == 3 && std::any_of(uiButtons.begin(), uiButtons.end(),
                [&](const UiButton& button) { return button.name == tokens[0]; })) {
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(),
                [&](const UiButton& item) { return item.name == tokens[0]; });
            const std::string property = lower(tokens[1]);
            const std::string raw = valueFor(tokens[2], variables);
            if (property == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid UI button color", raw);
                else button->color = raw;
            }
            else if (property == "edges" || property == "shape") {
                if (lower(raw) != "square" && lower(raw) != "round" && lower(raw) != "rounded") {
                    reportError(i + 1, "UI edges must be round or square", raw); continue;
                }
                button->cornerRadius = lower(raw) == "square" ? 0 : 7;
            }
            else {
                double number = 0;
                const bool relative = raw.size() > 1 && (raw[0] == '+' || raw[0] == '-');
                if (!parseNumber(raw, number)) { reportError(i + 1, "UI button property needs a number or color", raw); continue; }
                if (property == "x") button->x = relative ? button->x + number : number;
                else if (property == "y") button->y = relative ? button->y + number : number;
                else if (property == "w") button->width = number;
                else if (property == "h") button->height = number;
                else if (property == "radius") button->cornerRadius = std::max(0.0, number);
                else if (property == "opacity" && number >= 0 && number <= 1) button->opacity = number;
                else { reportError(i + 1, "unknown UI button property", property); continue; }
            }
            refreshCollisionWatches(variables, types);
            continue;
        }

        if ((cmd == "show" || cmd == "hide") && tokens.size() == 2) {
            const bool visible = cmd == "show";
            auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) { return item.name == tokens[1]; });
            if (group != uiGroups.end()) { group->visible = visible; continue; }
            bool found = false;
            for (UiBlock& item : uiBlocks) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiSprite& item : uiSprites) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiButton& item : uiButtons) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiText& item : uiTexts) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (Ui3DObject& item : ui3DObjects) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            if (!found) reportError(i + 1, "unknown UI group or object", tokens[1]);
            continue;
        }

        
        auto ufIt = userFunctions.find(cmd);
        if (ufIt != userFunctions.end()) {
            const auto& uf = ufIt->second;
            std::size_t resultArrow = tokens.size();
            for (std::size_t tokenIndex = 1; tokenIndex < tokens.size(); ++tokenIndex) {
                if (tokens[tokenIndex] == "->") { resultArrow = tokenIndex; break; }
            }
            const std::string resultName = resultArrow + 1 < tokens.size() ? tokens[resultArrow + 1] : "";
            if (resultArrow < tokens.size() &&
                (resultArrow + 2 != tokens.size() || !validName(resultName))) {
                reportError(i + 1, "function result syntax is FUNCTION ARGUMENTS -> RESULT_VARIABLE");
                continue;
            }

            // Function frames inherit the caller's values, but declarations made
            // inside the function stay local. Assignments to pre-existing globals
            // remain visible, while parameters always shadow caller variables.
            Vars callerVariables = variables;
            Types callerTypes = types;
            Defaults localDefaults = defaults;
            Vars localVariables = variables;
            Types localTypes = types;
            std::unordered_set<std::string> declaredLocals;
            for (const std::string& bodyLine : uf.body) {
                const auto declaration = tokenize(trim(bodyLine));
                if (declaration.size() >= 2 &&
                    (declaration[0] == "let" || declaration[0] == "var" || declaration[0] == "word" ||
                     declaration[0] == "num" || declaration[0] == "toggle" ||
                     declaration[0] == "color" || declaration[0] == "list" ||
                     declaration[0] == "dictionary")) {
                    declaredLocals.insert(declaration[1]);
                }
            }
            for (std::size_t p = 0; p < uf.params.size() && p + 1 < resultArrow; ++p) {
                const std::string& argument = tokens[p + 1];
                const std::string& parameter = uf.params[p];
                if (callerVariables.count(argument)) {
                    localVariables[parameter] = callerVariables.at(argument);
                    const auto argumentType = callerTypes.find(argument);
                    if (argumentType != callerTypes.end()) localTypes[parameter] = argumentType->second;
                    else if (std::holds_alternative<double>(localVariables[parameter].value)) localTypes[parameter] = "num";
                    else if (std::holds_alternative<bool>(localVariables[parameter].value)) localTypes[parameter] = "toggle";
                    else if (std::holds_alternative<VariableList>(localVariables[parameter].value)) localTypes[parameter] = "list";
                    else if (std::holds_alternative<std::shared_ptr<DictionaryData>>(localVariables[parameter].value)) localTypes[parameter] = "dictionary";
                    else localTypes[parameter] = "word";
                } else {
                    const std::string raw = valueFor(argument, callerVariables);
                    double numeric = 0;
                    if (lower(raw) == "true" || lower(raw) == "false") {
                        localVariables[parameter] = lower(raw) == "true";
                        localTypes[parameter] = "toggle";
                    } else if (parseNumber(raw, numeric)) {
                        localVariables[parameter] = numeric;
                        localTypes[parameter] = "num";
                    } else {
                        localVariables[parameter] = raw;
                        localTypes[parameter] = "word";
                    }
                }
            }
            Flow inner;
            const std::size_t callSiteLine = i + 1 +
                (trgCallStack.empty() ? 0 : trgCallStack.back().definitionLine);
            trgCallStack.push_back({uf.name, callSiteLine, uf.sourceLine});
            executeBlock(uf.body, 0, uf.body.size(), localVariables, localTypes, localDefaults,
                         inner, manualMode, uiMode, uf.blockIndex.get());
            trgCallStack.pop_back();

            for (const auto& existing : callerVariables) {
                if (declaredLocals.count(existing.first) ||
                    std::find(uf.params.begin(), uf.params.end(), existing.first) != uf.params.end()) continue;
                const auto updated = localVariables.find(existing.first);
                if (updated != localVariables.end()) variables[existing.first] = updated->second;
            }
            for (const auto& existing : callerTypes) {
                if (declaredLocals.count(existing.first) ||
                    std::find(uf.params.begin(), uf.params.end(), existing.first) != uf.params.end()) continue;
                const auto updated = localTypes.find(existing.first);
                if (updated != localTypes.end()) types[existing.first] = updated->second;
            }

            if (inner.stopped) { flow.stopped = true; return; }
            if (inner.returned && !resultName.empty()) {
                variables[resultName] = inner.returnValue;
                if (std::holds_alternative<double>(inner.returnValue.value)) types[resultName] = "num";
                else if (std::holds_alternative<bool>(inner.returnValue.value)) types[resultName] = "toggle";
                else if (std::holds_alternative<std::vector<std::string>>(inner.returnValue.value) ||
                         std::holds_alternative<VariableList>(inner.returnValue.value)) types[resultName] = "list";
                else if (std::holds_alternative<std::shared_ptr<DictionaryData>>(inner.returnValue.value)) types[resultName] = "dictionary";
                else types[resultName] = "word";
            }
            continue;
        }
        if (cmd == "define") {
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close, blockIndex)) {
                reportError(i + 1, "function definition needs a code block");
                continue;
            }
            i = close;
            continue;
        }

        
        if (cmd == "say") {
            std::string message;
            bool separateNext = false;
            for (std::size_t a = 1; a < tokens.size(); ++a) {
                if (tokens[a] == "/") {
                    separateNext = true;
                    continue;
                }
                if (separateNext && !message.empty()) message += ' ';
                message += valueFor(tokens[a], variables);
                separateNext = false;
            }
            appendOutput(message + "\r\n");
            continue;
        }
        if (cmd == "wait") {
            if (tokens.size() != 2) { reportError(i + 1, "use wait 10s or wait 100ms"); continue; }
            std::string duration = lower(valueFor(tokens[1], variables));
            std::chrono::milliseconds delay(0);
            try {
                if (duration.size() > 2 && duration.substr(duration.size() - 2) == "ms") {
                    double value = 0;
                    if (!parseNumber(duration.substr(0, duration.size() - 2), value) || value < 0) throw std::invalid_argument("duration");
                    delay = std::chrono::milliseconds(static_cast<long long>(value));
                } else {
                    if (!duration.empty() && duration.back() == 's') duration.pop_back();
                    double value = 0;
                    if (!parseNumber(duration, value) || value < 0) throw std::invalid_argument("duration");
                    delay = std::chrono::milliseconds(static_cast<long long>(value * 1000));
                }
            } catch (...) { reportError(i + 1, "wait duration must look like 100ms or 10s", tokens[1]); continue; }
#ifdef __ANDROID__
            const auto until = std::chrono::steady_clock::now() + delay;
            while (!androidStopRequested.load() && std::chrono::steady_clock::now() < until)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
#else
            std::this_thread::sleep_for(delay);
#endif
            continue;
        }
        if (cmd == "play") {
            if (tokens.size() < 2) { reportError(i + 1, "use play FILE"); continue; }
            std::string fileName = valueFor(tokens[1], variables);
            bool pathValid = false;
            const std::filesystem::path mediaPath = projectFilePath(fileName, pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(mediaPath)) {
                reportError(i + 1, "media file not found inside project folder", fileName); continue;
            }
            std::string ext = lower(mediaPath.extension().u8string());
            if (ext == ".m4a" || ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".aac") {
#ifdef _WIN32
                std::wstring wpath = mediaPath.wstring();
                if (!PlaySoundW(wpath.c_str(), nullptr, SND_FILENAME | SND_ASYNC)) {
                    reportError(i + 1, "could not play sound", fileName);
                }
#else
                reportError(i + 1, "play sound is Windows-only", fileName);
#endif
            } else if (ext == ".mp4" || ext == ".webm" || ext == ".avi" || ext == ".mov") {
#ifdef _WIN32
                ShellExecuteW(mainWindow, L"open", mediaPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#else
                reportError(i + 1, "play video is Windows-only", fileName);
#endif
            } else {
                reportError(i + 1, "unknown media type", ext);
            }
            continue;
        }
        if (cmd == "scroll") {
            if (tokens.size() != 3) { reportError(i + 1, "use scroll X Y"); continue; }
            double vx = 0, vy = 0;
            if (!evaluateExpression(valueFor(tokens[1], variables), variables, vx) ||
                !evaluateExpression(valueFor(tokens[2], variables), variables, vy)) {
                reportError(i + 1, "scroll needs numeric X Y"); continue;
            }
            viewportX = vx;
            viewportY = vy;
            continue;
        }
        if (cmd == "http_get") {
            if (!networkEnabled) { reportError(i + 1, "http_get needs add (network)"); continue; }
            if (tokens.size() != 3 || !validName(tokens[2])) { reportError(i + 1, "use http_get URL RESULT"); continue; }
            std::string url = valueFor(tokens[1], variables);
            std::string error;
            std::string response = httpGet(url, error);
            if (!error.empty()) { reportError(i + 1, error, url); continue; }
            variables[tokens[2]] = response;
            types[tokens[2]] = "word";
            continue;
        }
        if (cmd == "http_post") {
            if (!networkEnabled) { reportError(i + 1, "http_post needs add (network)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) { reportError(i + 1, "use http_post URL BODY RESULT"); continue; }
            std::string url = valueFor(tokens[1], variables);
            std::string body = valueFor(tokens[2], variables);
            std::string error;
            std::string response = httpPost(url, body, error);
            if (!error.empty()) { reportError(i + 1, error, url); continue; }
            variables[tokens[3]] = response;
            types[tokens[3]] = "word";
            continue;
        }

        if (cmd == "http_get_async" || cmd == "http_post_async") {
            if (!networkEnabled) { reportError(i + 1, "asynchronous HTTP needs add (network)"); continue; }
            const bool isPost = cmd == "http_post_async";
            const std::size_t expectedTokens = isPost ? 4 : 3;
            const std::size_t resultIndex = isPost ? 3 : 2;
            if (tokens.size() != expectedTokens || !validName(tokens[resultIndex])) {
                reportError(i + 1, isPost
                    ? "use http_post_async URL BODY REQUEST_ID"
                    : "use http_get_async URL REQUEST_ID");
                continue;
            }
            const std::string url = valueFor(tokens[1], variables);
            const std::string body = isPost ? valueFor(tokens[2], variables) : std::string();
            std::string error;
            const std::string requestId = queueAsyncHttpRequest(isPost ? "POST" : "GET", url, body, error);
            if (!error.empty()) { reportError(i + 1, error, url); continue; }
            variables[tokens[resultIndex]] = requestId;
            types[tokens[resultIndex]] = "word";
            continue;
        }

        if (cmd == "http_result") {
            if (!networkEnabled) { reportError(i + 1, "http_result needs add (network)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[2]) || !validName(tokens[3])) {
                reportError(i + 1, "use http_result REQUEST_ID RESPONSE STATUS");
                continue;
            }
            AsyncHttpResult request;
            const std::string requestId = valueFor(tokens[1], variables);
            if (!readAsyncHttpResult(requestId, request)) {
                variables[tokens[2]] = "unknown HTTP request";
                variables[tokens[3]] = "unknown";
            } else if (!request.complete) {
                variables[tokens[3]] = "pending";
            } else if (!request.error.empty()) {
                variables[tokens[2]] = request.error;
                variables[tokens[3]] = "error";
            } else {
                variables[tokens[2]] = request.response;
                variables[tokens[3]] = "complete";
            }
            types[tokens[2]] = "word";
            types[tokens[3]] = "word";
            continue;
        }

        if (cmd == "http_forget") {
            if (!networkEnabled) { reportError(i + 1, "http_forget needs add (network)"); continue; }
            if (tokens.size() != 2) { reportError(i + 1, "use http_forget REQUEST_ID"); continue; }
            forgetAsyncHttpResult(valueFor(tokens[1], variables));
            continue;
        }
        if (cmd == "particle") {
            // particle NAME X Y COLOR SPEED LIFE SIZE
            if (tokens.size() < 7) { reportError(i + 1, "use particle NAME X Y COLOR SPEED LIFE SIZE"); continue; }
            Particle p;
            p.name = tokens[1];
            if (!evaluateExpression(valueFor(tokens[2], variables), variables, p.x)) p.x = 0;
            if (!evaluateExpression(valueFor(tokens[3], variables), variables, p.y)) p.y = 0;
            p.color = valueFor(tokens[4], variables);
            if (!evaluateExpression(valueFor(tokens[5], variables), variables, p.vx)) p.vx = 0;
            p.vy = p.vx;
            if (!evaluateExpression(valueFor(tokens[6], variables), variables, p.maxLife)) p.maxLife = 1;
            p.life = p.maxLife;
            p.size = 4;
            if (tokens.size() >= 8) {
                if (!evaluateExpression(valueFor(tokens[7], variables), variables, p.size)) p.size = 4;
            }
            particles.push_back(p);
            continue;
        }
        if (cmd == "studio") {
#ifdef __ANDROID__
            if (tokens.size() != 2) {
                reportError(i + 1, "use studio ACTION");
            } else {
                trgStudioActionAndroid(tokens[1]);
            }
#else
#ifdef _WIN32
            if (tokens.size() == 2 && lower(tokens[1]) == "code" && desktopVisualEditorActive) {
                returnFromDesktopVisualEditor();
            } else {
                reportError(i + 1, "desktop studio supports studio code while a built-in visual editor is open");
            }
#else
            reportError(i + 1, "studio actions are only available in TRG Studio");
#endif
#endif
            continue;
        }
        if (cmd == "ask") {
            if (tokens.size() < 3 || !validName(tokens.back())) {
                reportError(i + 1, "ask needs a prompt and valid variable name");
                continue;
            }
            const std::size_t targetIndex = tokens.size() - 1;
            std::string prompt;
            for (std::size_t a = 1; a < targetIndex; ++a) {
                if (a > 1) prompt += ' ';
                prompt += valueFor(tokens[a], variables);
            }
            std::string answer;
            if (!askForInput(prompt, answer)) {
                reportError(i + 1, "input was cancelled");
                // Do not retry failed input inside a loop: that can spin forever
                // when the standalone terminal has no active input control.
                flow.stopped = true;
                return;
            }
            const std::string target = tokens[targetIndex];
            if (!types.count(target)) types[target] = "word";
            if (types[target] == "num") {
                double numeric = 0;
                if (!parseNumber(answer, numeric)) { reportError(i + 1, "input must be a number", answer); continue; }
                variables[target] = numeric;
            } else if (types[target] == "toggle") {
                const std::string toggleValue = lower(answer);
                if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "input must be true or false", answer); continue; }
                variables[target] = toggleValue == "true";
            } else variables[target] = answer;
            continue;
        }

        
        
        
        if (cmd == "add" && tokens.size() >= 2) {
            const std::string second = lower(tokens[1]);

            
            if (second == "global" || second == "(global)") {
                std::size_t open = 0, close = 0;
                if (!findBlock(program, i, end, open, close, blockIndex)) {
                    reportError(i + 1, "add global needs a ( ... ) block");
                    continue;
                }
                Flow inner;
                executeBlock(program, open + 1, close, variables, types, defaults, inner, manualMode, uiMode, blockIndex);
                if (inner.stopped) { flow.stopped = true; return; }
                if (inner.returned) { flow.returned = true; flow.returnValue = inner.returnValue; return; }
                i = close;
                continue;
            }

            
            if (second == "connect" || second == "(connect)") {
                std::string target;
                if (second == "(connect)") {
                    if (tokens.size() >= 3) target = tokens[2];
                } else {
                    target = parenContent(line);
                }
                if (target.empty() && tokens.size() >= 3) target = tokens[2];
                if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                    target = target.substr(1, target.size() - 2);
                if (target.empty()) { reportError(i + 1, "use add connect (file.trg)"); continue; }
                bool pathValid = false;
                const std::filesystem::path connectedPath = projectExistingFile(target, pathValid);
                if (!pathValid) { reportError(i + 1, "connected file must stay inside the TRG project folder", target); continue; }
                if (executedFileKeys.count(lower(canonicalProjectPath(connectedPath).u8string()))) continue;
                static thread_local unsigned connectDepth = 0;
                if (connectDepth >= 16) { reportError(i + 1, "connect depth exceeded", target); continue; }
                std::ifstream connectedFile(connectedPath, std::ios::binary);
                if (!connectedFile) { reportError(i + 1, "could not open connected file", target); continue; }
                const std::vector<std::string> connectedProgram = mergePrograms(readProgram(connectedFile), connectedPath, false, true, currentSourcePath);
                LibraryState connectedLibraries;
                scanLibraries(connectedProgram, connectedLibraries);
                filesEnabled = filesEnabled || connectedLibraries.files;
                jsonEnabled = jsonEnabled || connectedLibraries.json;
                databaseEnabled = databaseEnabled || connectedLibraries.database;
                webEnabled = webEnabled || connectedLibraries.web;
                collisionEnabled = collisionEnabled || connectedLibraries.collision;
                physicsEnabled = physicsEnabled || connectedLibraries.physics;
                aiEnabled = aiEnabled || connectedLibraries.ai;
                mathEnabled = mathEnabled || connectedLibraries.math;
            binaryEnabled = binaryEnabled || connectedLibraries.binary;
            regexEnabled = regexEnabled || connectedLibraries.regex;
            compressionEnabled = compressionEnabled || connectedLibraries.compression;
                powershellEnabled = powershellEnabled || connectedLibraries.powershell;
                raspberryEnabled = raspberryEnabled || connectedLibraries.raspberry;
                accessibilityEnabled = accessibilityEnabled || connectedLibraries.accessibility;
                timeEnabled = timeEnabled || connectedLibraries.time;
                convertEnabled = convertEnabled || connectedLibraries.convert;
                statsEnabled = statsEnabled || connectedLibraries.stats;
                unitsEnabled = unitsEnabled || connectedLibraries.units;
                if (connectedLibraries.controls) initializeControls(variables, types);
                registerFunctions(connectedProgram, false);
                ++connectDepth;
                Flow inner;
                executeBlock(connectedProgram, 0, connectedProgram.size(), variables, types, defaults, inner,
                             manualMode || connectedLibraries.manual, uiMode || connectedLibraries.ui);
                --connectDepth;
                if (inner.stopped) { flow.stopped = true; return; }
                if (inner.returned) { flow.returned = true; flow.returnValue = inner.returnValue; return; }
                continue;
            }

            
            if (tokens.size() == 2 && tokens[1].size() >= 6 &&
                tokens[1].front() == '(' && tokens[1].back() == ')') {
                const std::string moduleName = tokens[1].substr(1, tokens[1].size() - 2);
                const std::string moduleLower = lower(moduleName);
                if (moduleLower.size() >= 4 && moduleLower.compare(moduleLower.size() - 4, 4, ".trg") == 0) {
                    static thread_local unsigned includeDepth = 0;
                    if (includeDepth >= 16) { reportError(i + 1, "module include depth exceeded", tokens[1]); continue; }
                    bool pathValid = false;
                    const std::filesystem::path modulePath = projectExistingFile(moduleName, pathValid);
                    if (!pathValid) { reportError(i + 1, "module must be inside the TRG project folder", moduleName); continue; }
                    std::ifstream module(modulePath, std::ios::binary);
                    if (!module) { reportError(i + 1, "could not open TRG module", moduleName); continue; }
                    const std::vector<std::string> moduleProgram = readProgram(module);
                    LibraryState moduleLibraries;
                    scanLibraries(moduleProgram, moduleLibraries);
                    filesEnabled = filesEnabled || moduleLibraries.files;
                    jsonEnabled = jsonEnabled || moduleLibraries.json;
                    databaseEnabled = databaseEnabled || moduleLibraries.database;
                    webEnabled = webEnabled || moduleLibraries.web;
                    collisionEnabled = collisionEnabled || moduleLibraries.collision;
                    physicsEnabled = physicsEnabled || moduleLibraries.physics;
                    aiEnabled = aiEnabled || moduleLibraries.ai;
                    mathEnabled = mathEnabled || moduleLibraries.math;
                    binaryEnabled = binaryEnabled || moduleLibraries.binary;
                    regexEnabled = regexEnabled || moduleLibraries.regex;
                    compressionEnabled = compressionEnabled || moduleLibraries.compression;
                    powershellEnabled = powershellEnabled || moduleLibraries.powershell;
                    raspberryEnabled = raspberryEnabled || moduleLibraries.raspberry;
                    accessibilityEnabled = accessibilityEnabled || moduleLibraries.accessibility;
                    timeEnabled = timeEnabled || moduleLibraries.time;
                    convertEnabled = convertEnabled || moduleLibraries.convert;
                    statsEnabled = statsEnabled || moduleLibraries.stats;
                    unitsEnabled = unitsEnabled || moduleLibraries.units;
                    if (moduleLibraries.controls) initializeControls(variables, types);
                    registerFunctions(moduleProgram, false);
                    ++includeDepth;
                    Flow moduleFlow;
                    executeBlock(moduleProgram, 0, moduleProgram.size(), variables, types, defaults, moduleFlow,
                                 manualMode || moduleLibraries.manual, uiMode || moduleLibraries.ui);
                    --includeDepth;
                    if (moduleFlow.stopped) { flow.stopped = true; return; }
                if (moduleFlow.returned) { flow.returned = true; flow.returnValue = moduleFlow.returnValue; return; }
                    continue;
                }
            }

            
            if (tokens.size() == 2 && tokens[1].size() >= 3 &&
                tokens[1].front() == '(' && tokens[1].back() == ')') {
                const std::string library = lower(tokens[1]);
                if (library == "(files)") { filesEnabled = true; appendOutput("TRG files library enabled.\r\n"); continue; }
                if (library == "(json)") { jsonEnabled = true; appendOutput("TRG JSON library enabled.\r\n"); continue; }
                if (library == "(database)" || library == "(db)") { databaseEnabled = true; appendOutput("TRG Database library enabled.\r\n"); continue; }
                if (library == "(web)") { webEnabled = true; appendOutput("TRG WebAssembly commands enabled.\r\n"); continue; }
                if (library == "(collision)") { collisionEnabled = true; appendOutput("TRG collision helpers enabled.\r\n"); continue; }
                if (library == "(physics)") { physicsEnabled = true; appendOutput("TRG physics library enabled.\r\n"); continue; }
                if (library == "(3d)") { threeDEnabled = true; appendOutput("TRG 3D library enabled.\r\n"); continue; }
                if (library == "(manual)") { manualMode = true; continue; }
                if (library == "(ui)" || library == "(animation)") { uiMode = true; continue; }
                if (library == "(controls)") { initializeControls(variables, types); continue; }
                if (library == "(network)") { networkEnabled = true; appendOutput("TRG network library enabled.\r\n"); continue; }
                if (library == "(ai)") { aiEnabled = true; appendOutput("TRG AI library enabled.\r\n"); continue; }
                if (library == "(math)") { mathEnabled = true; appendOutput("TRG math library enabled.\r\n"); continue; }
                if (library == "(binary)") { binaryEnabled = true; appendOutput("TRG binary library enabled.\r\n"); continue; }
                if (library == "(regex)") { regexEnabled = true; appendOutput("TRG regex library enabled.\r\n"); continue; }
                if (library == "(compression)") { compressionEnabled = true; appendOutput("TRG compression library enabled.\r\n"); continue; }
                if (library == "(powershell)") { powershellEnabled = true; appendOutput("TRG PowerShell library enabled.\r\n"); continue; }
                if (library == "(raspberry)") { raspberryEnabled = true; appendOutput("TRG Raspberry library enabled.\r\n"); continue; }
                if (library == "(accessibility)") { accessibilityEnabled = true; appendOutput("TRG accessibility library enabled.\r\n"); continue; }
                if (library == "(time)") { timeEnabled = true; appendOutput("TRG time library enabled.\r\n"); continue; }
                if (library == "(convert)") { convertEnabled = true; appendOutput("TRG convert library enabled.\r\n"); continue; }
                if (library == "(stats)") { statsEnabled = true; appendOutput("TRG stats library enabled.\r\n"); continue; }
                if (library == "(units)") { unitsEnabled = true; appendOutput("TRG units library enabled.\r\n"); continue; }
                if (library == "(style)") { styleEnabled = true; appendOutput("TRG style library enabled.\r\n"); continue; }
                if (library == "(all)") {
                    filesEnabled = jsonEnabled = databaseEnabled = webEnabled = collisionEnabled = physicsEnabled = networkEnabled = aiEnabled = mathEnabled = true;
                    binaryEnabled = regexEnabled = compressionEnabled = true;
                    powershellEnabled = raspberryEnabled = accessibilityEnabled = true;
                    statsEnabled = unitsEnabled = true;
                    styleEnabled = true;
                    initializeControls(variables, types);
                    manualMode = true;
                    uiMode = true;
                    appendOutput("All built-in TRG libraries enabled.\r\n");
                    continue;
                }
                reportError(i + 1, "unknown library", tokens[1]);
                continue;
            }

            
            if (!line.empty() && (line.back() == '(' || line.back() == '{')) {
                if (line.back() == '(') {
                    std::size_t open = 0, close = 0;
                    if (findBlock(program, i, end, open, close, blockIndex)) i = close;
                }
                reportError(i + 1, "libraries apply to the whole file; use add (json) without a block");
                continue;
            }
        }

        
        if (cmd == "run") {
            std::string target = parenContent(line);
            if (target.empty() && tokens.size() >= 2) {
                target = tokens[1];
                if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                    target = target.substr(1, target.size() - 2);
            }
            if (target.empty()) { reportError(i + 1, "use run (file.trg)"); continue; }
            bool pathValid = false;
            const std::filesystem::path runPath = projectExistingFile(target, pathValid);
            if (!pathValid) { reportError(i + 1, "run target must stay inside the TRG project folder", target); continue; }
            const std::string runKey = lower(canonicalProjectPath(runPath).u8string());
            
            
            
            if (activeRunFiles.count(runKey)) continue;
            static thread_local unsigned runDepth = 0;
            if (runDepth >= 16) { reportError(i + 1, "run depth exceeded", target); continue; }
            std::ifstream runFile(runPath, std::ios::binary);
            if (!runFile) { reportError(i + 1, "could not open file to run", target); continue; }
            const std::vector<std::string> source = readProgram(runFile);
            std::vector<std::string> runProgram;
            if (executedFileKeys.count(runKey)) {
                
                
                std::vector<std::string> kept, globals, connects;
                std::unordered_set<std::string> seen;
                stripProjectDirectives(source, kept, globals, connects, true, &seen);
                runProgram = std::move(kept);
            } else {
                runProgram = mergePrograms(source, runPath, false, true, currentSourcePath);
            }
            LibraryState runLibraries;
            scanLibraries(runProgram, runLibraries);
            filesEnabled = filesEnabled || runLibraries.files;
            jsonEnabled = jsonEnabled || runLibraries.json;
            databaseEnabled = databaseEnabled || runLibraries.database;
            webEnabled = webEnabled || runLibraries.web;
            collisionEnabled = collisionEnabled || runLibraries.collision;
            if (runLibraries.controls) initializeControls(variables, types);
            registerFunctions(runProgram, false);
            ++runDepth;
            activeRunFiles.insert(runKey);
            Flow inner;
            executeBlock(runProgram, 0, runProgram.size(), variables, types, defaults, inner,
                         manualMode || runLibraries.manual, uiMode || runLibraries.ui);
            activeRunFiles.erase(runKey);
            --runDepth;
            if (inner.stopped) { flow.stopped = true; return; }
                if (inner.returned) { flow.returned = true; flow.returnValue = inner.returnValue; return; }
            continue;
        }

        
        if (cmd == "database" || cmd == "db" || cmd == "table" || cmd == "record" || cmd == "query" || cmd == "sql") {
            if (!databaseEnabled) { reportError(i + 1, "database commands need add (database)"); continue; }
            const std::string action = tokens.size() > 1 ? lower(tokens[1]) : "";
            std::string dbError;
            if (cmd == "database" || cmd == "db") {
                if (action == "open" && tokens.size() == 3) {
                    bool validPath = false;
                    const auto path = projectFilePath(valueFor(tokens[2], variables), validPath);
                    if (!validPath) { reportError(i + 1, "database path must stay inside the TRG project folder", tokens[2]); continue; }
                    if (std::filesystem::exists(path)) {
                        if (!loadTrgDatabase(path, dbError)) { reportError(i + 1, dbError, tokens[2]); continue; }
                    } else {
                        trgDatabaseTables.clear(); trgDatabasePath = path;
                        if (!saveTrgDatabase(dbError)) { reportError(i + 1, dbError, tokens[2]); continue; }
                    }
                    appendOutput("Database opened: " + path.filename().u8string() + "\r\n");
                } else if (action == "save" && tokens.size() == 2) {
                    if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                    else appendOutput("Database saved.\r\n");
                } else if (action == "close" && tokens.size() == 2) {
                    trgDatabaseTables.clear(); trgDatabasePath.clear(); appendOutput("Database closed.\r\n");
                } else if (action == "tables" && tokens.size() == 2) {
                    std::vector<std::string> names;
                    for (const auto& item : trgDatabaseTables) names.push_back(item.first);
                    std::sort(names.begin(), names.end());
                    for (const auto& name : names) appendOutput(name + "\r\n");
                    variables["database_table_count"] = static_cast<double>(names.size()); types["database_table_count"] = "num";
                } else reportError(i + 1, "use database open FILE, database save, database close, or database tables");
                continue;
            }
            if (trgDatabasePath.empty()) { reportError(i + 1, "open a database first with database open FILE"); continue; }
            if (cmd == "table") {
                if (action == "create" && tokens.size() >= 4) {
                    const std::string name = valueFor(tokens[2], variables);
                    if (!validName(name) || trgDatabaseTables.count(name)) { reportError(i + 1, "table name is invalid or already exists", name); continue; }
                    TrgDatabaseTable table; bool valid = true;
                    for (std::size_t n = 3; n < tokens.size(); ++n) {
                        const std::string column = valueFor(tokens[n], variables);
                        if (!validName(column) || std::find(table.columns.begin(), table.columns.end(), column) != table.columns.end()) { valid = false; break; }
                        table.columns.push_back(column);
                    }
                    if (!valid) { reportError(i + 1, "table columns must be unique valid names"); continue; }
                    trgDatabaseTables.emplace(name, std::move(table));
                    if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                    else appendOutput("Created table " + name + ".\r\n");
                } else if (action == "delete" && tokens.size() == 3) {
                    if (!trgDatabaseTables.erase(valueFor(tokens[2], variables))) reportError(i + 1, "unknown table", tokens[2]);
                    else if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                } else if (action == "count" && tokens.size() == 4 && validName(tokens[3])) {
                    const auto found = trgDatabaseTables.find(valueFor(tokens[2], variables));
                    if (found == trgDatabaseTables.end()) { reportError(i + 1, "unknown table", tokens[2]); continue; }
                    variables[tokens[3]] = static_cast<double>(found->second.rows.size()); types[tokens[3]] = "num";
                } else reportError(i + 1, "use table create NAME COLUMN... | table delete NAME | table count NAME RESULT");
                continue;
            }
            if (cmd == "record") {
                if (action == "add" && tokens.size() >= 5 && (tokens.size() - 3) % 2 == 0) {
                    const auto found = trgDatabaseTables.find(valueFor(tokens[2], variables));
                    if (found == trgDatabaseTables.end()) { reportError(i + 1, "unknown table", tokens[2]); continue; }
                    std::vector<std::string> row(found->second.columns.size()); bool valid = true;
                    for (std::size_t n = 3; n + 1 < tokens.size(); n += 2) {
                        const auto col = databaseColumnIndex(found->second, valueFor(tokens[n], variables));
                        if (col == found->second.columns.size()) { valid = false; break; }
                        row[col] = valueFor(tokens[n + 1], variables);
                    }
                    if (!valid) { reportError(i + 1, "record contains an unknown field", tokens[2]); continue; }
                    found->second.rows.push_back(std::move(row));
                    if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                    else { variables["last_record"] = static_cast<double>(found->second.rows.size() - 1); types["last_record"] = "num"; }
                } else if (action == "get" && tokens.size() == 6 && validName(tokens[5])) {
                    const auto found = trgDatabaseTables.find(valueFor(tokens[2], variables)); double index = 0;
                    if (found == trgDatabaseTables.end() || !parseNumber(valueFor(tokens[3], variables), index) || index < 0 ||
                        index >= (found == trgDatabaseTables.end() ? 0.0 : static_cast<double>(found->second.rows.size())) || std::floor(index) != index) {
                        reportError(i + 1, "record get needs a valid table and row index"); continue;
                    }
                    const auto col = databaseColumnIndex(found->second, valueFor(tokens[4], variables));
                    if (col == found->second.columns.size()) { reportError(i + 1, "unknown record field", tokens[4]); continue; }
                    variables[tokens[5]] = found->second.rows[static_cast<std::size_t>(index)][col]; types[tokens[5]] = "word";
                } else if (action == "update" && tokens.size() == 6) {
                    const auto found = trgDatabaseTables.find(valueFor(tokens[2], variables)); double index = 0;
                    if (found == trgDatabaseTables.end() || !parseNumber(valueFor(tokens[3], variables), index) || index < 0 ||
                        index >= (found == trgDatabaseTables.end() ? 0.0 : static_cast<double>(found->second.rows.size())) || std::floor(index) != index) {
                        reportError(i + 1, "record update needs a valid table and row index"); continue;
                    }
                    const auto col = databaseColumnIndex(found->second, valueFor(tokens[4], variables));
                    if (col == found->second.columns.size()) { reportError(i + 1, "unknown record field", tokens[4]); continue; }
                    found->second.rows[static_cast<std::size_t>(index)][col] = valueFor(tokens[5], variables);
                    if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                } else if (action == "delete" && tokens.size() == 4) {
                    const auto found = trgDatabaseTables.find(valueFor(tokens[2], variables)); double index = 0;
                    if (found == trgDatabaseTables.end() || !parseNumber(valueFor(tokens[3], variables), index) || index < 0 ||
                        index >= (found == trgDatabaseTables.end() ? 0.0 : static_cast<double>(found->second.rows.size())) || std::floor(index) != index) {
                        reportError(i + 1, "record delete needs a valid table and row index"); continue;
                    }
                    found->second.rows.erase(found->second.rows.begin() + static_cast<std::ptrdiff_t>(index));
                    if (!saveTrgDatabase(dbError)) reportError(i + 1, dbError);
                } else reportError(i + 1, "use record add TABLE FIELD VALUE... | record get TABLE INDEX FIELD RESULT | record update TABLE INDEX FIELD VALUE | record delete TABLE INDEX");
                continue;
            }
            if (cmd == "query" || cmd == "sql") {
                const bool sqlMode = cmd == "sql";
                const std::size_t tableAt = sqlMode ? 3 : 1;
                const std::size_t fieldAt = sqlMode ? 5 : 2;
                const std::size_t opAt = fieldAt + 1, valueAt = fieldAt + 2, resultAt = sqlMode ? 9 : 5;
                if ((sqlMode && (tokens.size() != 10 || action != "select" || lower(tokens[2]) != "from" ||
                                 lower(tokens[4]) != "where" || lower(tokens[8]) != "into")) ||
                    (!sqlMode && tokens.size() != 6) || resultAt >= tokens.size() || !validName(tokens[resultAt])) {
                    reportError(i + 1, sqlMode ? "use sql select from TABLE where FIELD OP VALUE into RESULT" : "use query TABLE FIELD OP VALUE RESULT"); continue;
                }
                const auto found = trgDatabaseTables.find(valueFor(tokens[tableAt], variables));
                if (found == trgDatabaseTables.end()) { reportError(i + 1, "unknown table", tokens[tableAt]); continue; }
                const auto col = databaseColumnIndex(found->second, valueFor(tokens[fieldAt], variables));
                if (col == found->second.columns.size()) { reportError(i + 1, "unknown query field", tokens[fieldAt]); continue; }
                const std::string op = tokens[opAt], wanted = valueFor(tokens[valueAt], variables);
                if (op != "=" && op != "!=" && op != ">" && op != "<" && op != ">=" && op != "<=") {
                    reportError(i + 1, "unsupported query operator", op); continue;
                }
                std::vector<std::string> matches;
                for (std::size_t n = 0; n < found->second.rows.size(); ++n) {
                    const std::string actual = found->second.rows[n][col]; double a = 0, b = 0;
                    const bool numeric = parseNumber(actual, a) && parseNumber(wanted, b);
                    const bool match = op == "=" ? actual == wanted : op == "!=" ? actual != wanted :
                        op == ">" ? (numeric ? a > b : actual > wanted) : op == "<" ? (numeric ? a < b : actual < wanted) :
                        op == ">=" ? (numeric ? a >= b : actual >= wanted) : (numeric ? a <= b : actual <= wanted);
                    if (match) matches.push_back(std::to_string(n));
                }
                const std::size_t count = matches.size();
                variables[tokens[resultAt]] = std::move(matches); types[tokens[resultAt]] = "list";
                variables["query_count"] = static_cast<double>(count); types["query_count"] = "num";
                continue;
            }
        }

        if (cmd == "json") {
            if (!jsonEnabled) { reportError(i + 1, "JSON commands need add (json)"); continue; }
            if (tokens.size() >= 2 && lower(tokens[1]) == "read") {
                std::string target = parenContent(line);
                if (target.empty() && tokens.size() >= 3) {
                    target = tokens[2];
                    if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                        target = target.substr(1, target.size() - 2);
                }
                if (target.empty()) { reportError(i + 1, "use json read (file.json)"); continue; }
                bool pathValid = false;
                const std::filesystem::path jsonPath = projectExistingFile(target, pathValid);
                if (!pathValid) { reportError(i + 1, "JSON file must stay inside the TRG project folder", target); continue; }
                std::string jsonError;
                if (!readJsonIntoVariables(jsonPath, variables, types, jsonError)) {
                    reportError(i + 1, jsonError, target);
                    continue;
                }
                continue;
            }
            if (tokens.size() < 4) { reportError(i + 1, "use json GROUP clean|compressed RESULT"); continue; }
            const std::string groupName = tokens[1];
            const std::string format = lower(tokens[2]);
            const std::string resultName = tokens.back();
            if (format != "clean" && format != "compressed") {
                reportError(i + 1, "json format must be clean or compressed", tokens[2]);
                continue;
            }
            if (!validName(resultName)) {
                reportError(i + 1, "json needs a valid result variable", resultName);
                continue;
            }
            std::vector<std::pair<std::string, VariableValue>> entries;
            std::unordered_set<std::string> included;
            auto include = [&](const std::string& name) {
                if (!included.insert(name).second) return;
                const auto it = variables.find(name);
                if (it != variables.end()) entries.push_back({it->first, it->second});
            };
            for (const auto& item : variables) {
                const auto membership = variableGroups.find(item.first);
                if (membership == variableGroups.end()) continue;
                if (std::find(membership->second.begin(), membership->second.end(), groupName) == membership->second.end()) continue;
                include(item.first);
            }
            
            
            for (std::size_t level = 0; level < groupStack.size() && level < groupSnapshots.size(); ++level) {
                if (groupStack[level] != groupName) continue;
                for (const auto& item : variables)
                    if (!groupSnapshots[level].count(item.first)) include(item.first);
            }
            variables[resultName] = serializeJsonObject(entries, format == "clean");
            types[resultName] = "word";
            continue;
        }

        
        if (cmd == "create") {
            if (!filesEnabled && !jsonEnabled) { reportError(i + 1, "create needs add (files) or add (json)"); continue; }
            if (tokens.size() < 3) { reportError(i + 1, "use create FILE (LOCATION) RESULT"); continue; }
            const std::string fileName = tokens[1];
            const std::string location = parenContent(line);
            const std::string resultName = tokens.back();
            if (!validName(resultName)) { reportError(i + 1, "create needs a valid result variable", resultName); continue; }
            const auto result = variables.find(resultName);
            if (result == variables.end()) { reportError(i + 1, "create needs an existing variable holding JSON", resultName); continue; }
            const std::string content = static_cast<std::string>(result->second);
            const std::string relative = location.empty() ? fileName : location + "/" + fileName;
            bool pathValid = false;
            const std::filesystem::path outputPath = projectFilePath(relative, pathValid);
            if (!pathValid) { reportError(i + 1, "file must stay inside the TRG project folder", relative); continue; }
            std::error_code directoryError;
            if (!outputPath.parent_path().empty())
                std::filesystem::create_directories(outputPath.parent_path(), directoryError);
            std::ofstream output(outputPath, std::ios::binary | std::ios::trunc);
            if (!output) { reportError(i + 1, "could not write JSON file", relative); continue; }
            output << content;
            continue;
        }

        if (cmd == "ai") {
            if (!aiEnabled) { reportError(i + 1, "AI commands need add (ai)"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "use ai create, teach, train, predict, or classify"); continue; }
            const std::string action = lower(tokens[1]);
            if (action == "create") {
                if (tokens.size() != 4 || !validName(tokens[2])) {
                    reportError(i + 1, "use ai create MODEL INPUT_COUNT");
                    continue;
                }
                double inputCount = 0;
                if (!parseNumber(tokens[3], inputCount) || inputCount < 1 || inputCount > 64 ||
                    std::floor(inputCount) != inputCount) {
                    reportError(i + 1, "AI input count must be a whole number from 1 to 64", tokens[3]);
                    continue;
                }
                AiModel model;
                model.weights.resize(static_cast<std::size_t>(inputCount), 0.0);
                std::uniform_real_distribution<double> initialWeight(-0.05, 0.05);
                for (double& weight : model.weights) weight = initialWeight(trgRandomEngine);
                aiModels[tokens[2]] = std::move(model);
                appendOutput("Created AI model " + tokens[2] + " with " + tokens[3] + " inputs.\r\n");
                continue;
            }
            if (action == "teach") {
                if (tokens.size() != 5 || !aiModels.count(tokens[2])) {
                    reportError(i + 1, "use ai teach MODEL INPUTS EXPECTED (create the model first)");
                    continue;
                }
                std::vector<double> inputValues;
                std::stringstream inputStream(tokens[3]);
                std::string inputPart;
                bool validInputs = true;
                while (std::getline(inputStream, inputPart, ';')) {
                    double value = 0;
                    if (!parseNumber(trim(inputPart), value) || !std::isfinite(value)) { validInputs = false; break; }
                    inputValues.push_back(value);
                }
                double expected = 0;
                AiModel& model = aiModels[tokens[2]];
                if (!validInputs || inputValues.size() != model.weights.size() ||
                    !parseNumber(tokens[4], expected) || (expected != 0.0 && expected != 1.0)) {
                    reportError(i + 1, "AI examples need the correct number of semicolon-separated inputs and an output of 0 or 1");
                    continue;
                }
                model.samples.push_back({std::move(inputValues), expected});
                appendOutput("Added training example to " + tokens[2] + ".\r\n");
                continue;
            }
            if (action == "train") {
                if ((tokens.size() != 4 && tokens.size() != 5) || !aiModels.count(tokens[2])) {
                    reportError(i + 1, "use ai train MODEL EPOCHS [LEARNING_RATE]");
                    continue;
                }
                AiModel& model = aiModels[tokens[2]];
                double epochsValue = 0, learningRate = 0.1;
                if (model.samples.empty() || !parseNumber(tokens[3], epochsValue) ||
                    epochsValue < 1 || epochsValue > 100000 || std::floor(epochsValue) != epochsValue ||
                    (tokens.size() == 5 && !parseNumber(tokens[4], learningRate)) ||
                    learningRate <= 0 || learningRate > 1) {
                    reportError(i + 1, "training needs examples, 1-100000 epochs, and a learning rate above 0 and at most 1");
                    continue;
                }
                for (int epoch = 0; epoch < static_cast<int>(epochsValue); ++epoch) {
                    for (const AiSample& sample : model.samples) {
                        double total = model.bias;
                        for (std::size_t index = 0; index < model.weights.size(); ++index)
                            total += model.weights[index] * sample.inputs[index];
                        total = std::clamp(total, -30.0, 30.0);
                        const double prediction = 1.0 / (1.0 + std::exp(-total));
                        const double error = prediction - sample.expected;
                        for (std::size_t index = 0; index < model.weights.size(); ++index)
                            model.weights[index] -= learningRate * error * sample.inputs[index];
                        model.bias -= learningRate * error;
                    }
                }
                appendOutput("Trained " + tokens[2] + " using " + std::to_string(model.samples.size()) +
                    " examples for " + tokens[3] + " epochs.\r\n");
                continue;
            }
            if (action == "predict" || action == "classify") {
                if (tokens.size() != 5 || !validName(tokens[4]) || !aiModels.count(tokens[2])) {
                    reportError(i + 1, "use ai predict MODEL INPUTS RESULT or ai classify MODEL INPUTS RESULT");
                    continue;
                }
                AiModel& model = aiModels[tokens[2]];
                std::vector<double> inputValues;
                std::stringstream inputStream(tokens[3]);
                std::string inputPart;
                bool validInputs = true;
                while (std::getline(inputStream, inputPart, ';')) {
                    double value = 0;
                    if (!parseNumber(trim(inputPart), value) || !std::isfinite(value)) { validInputs = false; break; }
                    inputValues.push_back(value);
                }
                if (!validInputs || inputValues.size() != model.weights.size()) {
                    reportError(i + 1, "AI input count does not match the model");
                    continue;
                }
                double total = model.bias;
                for (std::size_t index = 0; index < model.weights.size(); ++index)
                    total += model.weights[index] * inputValues[index];
                total = std::clamp(total, -30.0, 30.0);
                const double prediction = 1.0 / (1.0 + std::exp(-total));
                if (action == "predict") {
                    variables[tokens[4]] = prediction;
                    types[tokens[4]] = "num";
                } else {
                    variables[tokens[4]] = prediction >= 0.5;
                    types[tokens[4]] = "toggle";
                }
                continue;
            }
            reportError(i + 1, "unknown AI action; use create, teach, train, predict, or classify", action);
            continue;
        }

        if (cmd == "find" || cmd == "find_position" || cmd == "replace_text" ||
            cmd == "count_text" || cmd == "starts_with" || cmd == "ends_with") {
            const std::string source = tokens.size() > 1 ? valueFor(tokens[1], variables) : std::string();
            const std::string search = tokens.size() > 2 ? valueFor(tokens[2], variables) : std::string();
            if (cmd == "replace_text") {
                if (tokens.size() != 5 || !validName(tokens[4])) { reportError(i + 1, "use replace_text TEXT OLD_TEXT NEW_TEXT RESULT"); continue; }
                std::string result = source;
                if (!search.empty()) {
                    const std::string replacement = valueFor(tokens[3], variables);
                    std::size_t position = 0;
                    while ((position = result.find(search, position)) != std::string::npos) {
                        result.replace(position, search.size(), replacement);
                        position += replacement.size();
                    }
                }
                variables[tokens[4]] = result; types[tokens[4]] = "word";
            } else {
                if (tokens.size() != 4 || !validName(tokens[3])) {
                    reportError(i + 1, "use find TEXT SEARCH RESULT, find_position TEXT SEARCH RESULT, count_text TEXT SEARCH RESULT, starts_with TEXT PREFIX RESULT, or ends_with TEXT SUFFIX RESULT");
                    continue;
                }
                if (cmd == "find") { variables[tokens[3]] = source.find(search) != std::string::npos; types[tokens[3]] = "toggle"; }
                else if (cmd == "find_position") { const auto p = source.find(search); variables[tokens[3]] = p == std::string::npos ? -1.0 : static_cast<double>(p); types[tokens[3]] = "num"; }
                else if (cmd == "count_text") {
                    std::size_t count = 0, position = 0;
                    if (!search.empty()) while ((position = source.find(search, position)) != std::string::npos) { ++count; position += search.size(); }
                    variables[tokens[3]] = static_cast<double>(count); types[tokens[3]] = "num";
                } else if (cmd == "starts_with") { variables[tokens[3]] = source.rfind(search, 0) == 0; types[tokens[3]] = "toggle"; }
                else { variables[tokens[3]] = source.size() >= search.size() && source.compare(source.size() - search.size(), search.size(), search) == 0; types[tokens[3]] = "toggle"; }
            }
            continue;
        }

        if (cmd == "add_numbers" || cmd == "subtract_numbers" || cmd == "multiply_numbers" ||
            cmd == "divide_numbers" || cmd == "square_root" || cmd == "round_number" ||
            cmd == "absolute_number" || cmd == "minimum" || cmd == "maximum" ||
            cmd == "distance_between") {
            const bool two = cmd == "add_numbers" || cmd == "subtract_numbers" ||
                cmd == "multiply_numbers" || cmd == "divide_numbers" || cmd == "minimum" || cmd == "maximum";
            const bool distance = cmd == "distance_between";
            const std::size_t expected = distance ? 6u : (two ? 4u : 3u);
            if (tokens.size() != expected || !validName(tokens.back())) {
                reportError(i + 1, "check the numbers and result name in this command");
                continue;
            }
            std::vector<double> values;
            bool valid = true;
            for (std::size_t index = 1; index + 1 < tokens.size(); ++index) {
                double value = 0;
                if (!evaluateExpression(valueFor(tokens[index], variables), variables, value)) { valid = false; break; }
                values.push_back(value);
            }
            if (!valid) { reportError(i + 1, "this command needs valid numbers"); continue; }
            double result = 0;
            if (cmd == "add_numbers") result = values[0] + values[1];
            else if (cmd == "subtract_numbers") result = values[0] - values[1];
            else if (cmd == "multiply_numbers") result = values[0] * values[1];
            else if (cmd == "divide_numbers") {
                if (values[1] == 0) { reportError(i + 1, "cannot divide by zero"); continue; }
                result = values[0] / values[1];
            } else if (cmd == "square_root") {
                if (values[0] < 0) { reportError(i + 1, "square_root needs a number of zero or greater"); continue; }
                result = std::sqrt(values[0]);
            } else if (cmd == "round_number") result = std::round(values[0]);
            else if (cmd == "absolute_number") result = std::abs(values[0]);
            else if (cmd == "minimum") result = (std::min)(values[0], values[1]);
            else if (cmd == "maximum") result = (std::max)(values[0], values[1]);
            else result = std::hypot(values[2] - values[0], values[3] - values[1]);
            if (!std::isfinite(result)) { reportError(i + 1, "the answer is outside the supported number range"); continue; }
            variables[tokens.back()] = result; types[tokens.back()] = "num";
            continue;
        }

        if (cmd == "binary") {
            if (!binaryEnabled) { reportError(i + 1, "binary commands need add (binary)"); continue; }
            if (tokens.size()!=4) { reportError(i+1,"use binary to_bin|from_bin|to_hex|from_hex|base64_encode|base64_decode VALUE RESULT"); continue; }
            const std::string action=lower(tokens[1]), source=valueFor(tokens[2],variables);
            if(!validName(tokens[3])){reportError(i+1,"binary needs a valid result variable");continue;}
            if(action=="to_bin"){double n=0;if(!parseNumber(source,n)||n<0||n>9007199254740991.0||std::floor(n)!=n){reportError(i+1,"to_bin needs a whole number from 0 to 9007199254740991");continue;}std::uint64_t v=static_cast<std::uint64_t>(n);std::string out;do{out.insert(out.begin(),(v&1u)?'1':'0');v>>=1;}while(v);variables[tokens[3]]=out;types[tokens[3]]="word";}
            else if(action=="from_bin"){if(source.empty()||source.size()>53||source.find_first_not_of("01")!=std::string::npos){reportError(i+1,"from_bin needs 1 to 53 binary digits");continue;}std::uint64_t v=0;for(char b:source)v=(v<<1)|static_cast<unsigned>(b-'0');variables[tokens[3]]=static_cast<double>(v);types[tokens[3]]="num";}
            else if(action=="to_hex"){variables[tokens[3]]=hexEncode(source);types[tokens[3]]="word";}
            else if(action=="from_hex"){std::string out;if(!hexDecode(source,out)){reportError(i+1,"from_hex needs pairs of hexadecimal digits");continue;}variables[tokens[3]]=out;types[tokens[3]]="word";}
            else if(action=="base64_encode"){variables[tokens[3]]=base64Encode(source);types[tokens[3]]="word";}
            else if(action=="base64_decode"){bool ok=false;std::string out=base64Decode(source,ok);if(!ok){reportError(i+1,"invalid Base64 input");continue;}variables[tokens[3]]=out;types[tokens[3]]="word";}
            else reportError(i+1,"unknown binary action",action);
            continue;
        }
        if (cmd == "bit") {
            if(!binaryEnabled){reportError(i+1,"bit commands need add (binary)");continue;}
            if(tokens.size()<4||tokens.size()>5){reportError(i+1,"use bit and|or|xor|not|shift_left|shift_right A [B] RESULT");continue;}
            const std::string action=lower(tokens[1]);const bool unary=action=="not";
            if(tokens.size()!=(unary?4u:5u)||!validName(tokens.back())){reportError(i+1,"invalid bit operation arguments");continue;}
            auto read=[&](const std::string& t,std::uint32_t& v){double n=0;if(!parseNumber(valueFor(t,variables),n)||n<0||n>4294967295.0||std::floor(n)!=n)return false;v=static_cast<std::uint32_t>(n);return true;};
            std::uint32_t a=0,b=0;if(!read(tokens[2],a)||(!unary&&!read(tokens[3],b))){reportError(i+1,"bit operations need unsigned 32-bit whole numbers");continue;}
            std::uint32_t out=0;if(action=="and")out=a&b;else if(action=="or")out=a|b;else if(action=="xor")out=a^b;else if(action=="not")out=~a;
            else if(action=="shift_left"||action=="shift_right"){if(b>31){reportError(i+1,"shift amount must be 0 to 31");continue;}out=action=="shift_left"?static_cast<std::uint32_t>(a<<b):static_cast<std::uint32_t>(a>>b);}
            else{reportError(i+1,"unknown bit operation",action);continue;}variables[tokens.back()]=static_cast<double>(out);types[tokens.back()]="num";continue;
        }
        if (cmd == "checksum") {
            if(!binaryEnabled){reportError(i+1,"checksum needs add (binary)");continue;}
            if(tokens.size()!=4||!validName(tokens[3])){reportError(i+1,"use checksum crc32|fnv1a TEXT RESULT");continue;}
            const std::string action=lower(tokens[1]),source=valueFor(tokens[2],variables);std::uint32_t h=0;
            if(action=="crc32")h=trgCrc32(source);else if(action=="fnv1a"){h=2166136261u;for(unsigned char c:source){h^=c;h*=16777619u;}}else{reportError(i+1,"use crc32 or fnv1a",action);continue;}
            variables[tokens[3]]=static_cast<double>(h);types[tokens[3]]="num";continue;
        }
        if (cmd == "regex") {
            if(!regexEnabled){reportError(i+1,"regex commands need add (regex)");continue;}
            if(tokens.size()<5||tokens.size()>6||!validName(tokens.back())){reportError(i+1,"use regex match|search TEXT PATTERN RESULT or regex replace TEXT PATTERN REPLACEMENT RESULT");continue;}
            const std::string action=lower(tokens[1]),source=valueFor(tokens[2],variables),pattern=valueFor(tokens[3],variables);
            try{const std::regex re(pattern);
                if(action=="match"&&tokens.size()==5){variables[tokens[4]]=std::regex_search(source,re);types[tokens[4]]="toggle";}
                else if(action=="search"&&tokens.size()==5){std::smatch m;variables[tokens[4]]=std::regex_search(source,m,re)?static_cast<double>(m.position()):-1.0;types[tokens[4]]="num";}
                else if(action=="replace"&&tokens.size()==6){variables[tokens[5]]=std::regex_replace(source,re,valueFor(tokens[4],variables));types[tokens[5]]="word";}
                else reportError(i+1,"invalid regex action or arguments");
            }catch(const std::regex_error&){reportError(i+1,"invalid regular expression",pattern);}continue;
        }
        if (cmd == "compress") {
            if(!compressionEnabled){reportError(i+1,"compression commands need add (compression)");continue;}
            if(tokens.size()!=4||!validName(tokens[3])){reportError(i+1,"use compress encode|decode TEXT RESULT");continue;}
            const std::string action=lower(tokens[1]),source=valueFor(tokens[2],variables);
            if(action=="encode"){variables[tokens[3]]=rleEncode(source);types[tokens[3]]="word";}
            else if(action=="decode"){std::string out;if(!rleDecode(source,out)){reportError(i+1,"invalid or oversized RLE data");continue;}variables[tokens[3]]=out;types[tokens[3]]="word";}
            else reportError(i+1,"unknown compression action",action);continue;
        }

        if (cmd == "math") {
            if (!mathEnabled) { reportError(i + 1, "math commands need add (math)"); continue; }
            if (tokens.size() < 4) {
                reportError(i + 1, "use math sin/cos/tan/abs/floor/ceil/round/sqrt VALUE RESULT, min/max A B RESULT, clamp VALUE MIN MAX RESULT, or distance X1 Y1 X2 Y2 RESULT");
                continue;
            }
            const std::string operation = lower(tokens[1]);
            const std::size_t expected = operation == "min" || operation == "max" ? 5 :
                (operation == "distance" ? 7 : (operation == "clamp" ? 6 : 4));
            if (tokens.size() != expected || !validName(tokens.back())) {
                reportError(i + 1, "invalid arguments for math " + operation);
                continue;
            }
            std::vector<double> values;
            bool validValues = true;
            for (std::size_t index = 2; index + 1 < tokens.size(); ++index) {
                double value = 0;
                if (!evaluateExpression(valueFor(tokens[index], variables), variables, value)) {
                    validValues = false;
                    break;
                }
                values.push_back(value);
            }
            if (!validValues) { reportError(i + 1, "math needs valid numeric inputs"); continue; }
            double result = 0;
            if (operation == "sin") result = std::sin(values[0]);
            else if (operation == "cos") result = std::cos(values[0]);
            else if (operation == "tan") result = std::tan(values[0]);
            else if (operation == "abs") result = std::abs(values[0]);
            else if (operation == "floor") result = std::floor(values[0]);
            else if (operation == "ceil") result = std::ceil(values[0]);
            else if (operation == "round") result = std::round(values[0]);
            else if (operation == "sqrt") {
                if (values[0] < 0) { reportError(i + 1, "sqrt needs a value of zero or greater"); continue; }
                result = std::sqrt(values[0]);
            } else if (operation == "min") result = (std::min)(values[0], values[1]);
            else if (operation == "max") result = (std::max)(values[0], values[1]);
            else if (operation == "clamp") {
                if (values[1] > values[2]) { reportError(i + 1, "clamp minimum must not exceed maximum"); continue; }
                result = std::clamp(values[0], values[1], values[2]);
            } else if (operation == "distance") {
                result = std::hypot(values[2] - values[0], values[3] - values[1]);
            } else {
                reportError(i + 1, "unknown math operation", operation);
                continue;
            }
            if (!std::isfinite(result)) { reportError(i + 1, "math result is outside the supported numeric range"); continue; }
            variables[tokens.back()] = result;
            types[tokens.back()] = "num";
            continue;
        }

        if (cmd == "time") {
            if (!timeEnabled) { reportError(i + 1, "time commands need add (time)"); continue; }
            if (tokens.size() != 3 || !validName(tokens[2])) { reportError(i + 1, "use time unix|milliseconds|year|month|day|hour|minute|second RESULT"); continue; }
            const auto now = std::chrono::system_clock::now();
            const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            const std::time_t rawTime = std::chrono::system_clock::to_time_t(now); std::tm parts{};
            #ifdef _WIN32
            localtime_s(&parts, &rawTime);
            #else
            localtime_r(&rawTime, &parts);
            #endif
            const std::string action = lower(tokens[1]);
            if (action == "unix") variables[tokens[2]] = static_cast<double>(milliseconds / 1000);
            else if (action == "milliseconds") variables[tokens[2]] = static_cast<double>(milliseconds);
            else if (action == "year") variables[tokens[2]] = static_cast<double>(parts.tm_year + 1900);
            else if (action == "month") variables[tokens[2]] = static_cast<double>(parts.tm_mon + 1);
            else if (action == "day") variables[tokens[2]] = static_cast<double>(parts.tm_mday);
            else if (action == "hour") variables[tokens[2]] = static_cast<double>(parts.tm_hour);
            else if (action == "minute") variables[tokens[2]] = static_cast<double>(parts.tm_min);
            else if (action == "second") variables[tokens[2]] = static_cast<double>(parts.tm_sec);
            else { reportError(i + 1, "unknown time action", action); continue; }
            types[tokens[2]] = "num"; continue;
        }

        if (cmd == "convert") {
            if (!convertEnabled) { reportError(i + 1, "convert commands need add (convert)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) { reportError(i + 1, "use convert number|text|bool VALUE RESULT"); continue; }
            const std::string action = lower(tokens[1]), source = valueFor(tokens[2], variables);
            if (action == "number") { double number = 0; if (!parseNumber(source, number)) { reportError(i + 1, "value cannot be converted to a number", source); continue; } variables[tokens[3]] = number; types[tokens[3]] = "num"; }
            else if (action == "text") { variables[tokens[3]] = source; types[tokens[3]] = "word"; }
            else if (action == "bool") { const std::string value = lower(trim(source)); if (value == "true" || value == "1" || value == "yes" || value == "on") variables[tokens[3]] = true; else if (value == "false" || value == "0" || value == "no" || value == "off") variables[tokens[3]] = false; else { reportError(i + 1, "value cannot be converted to bool", source); continue; } types[tokens[3]] = "toggle"; }
            else { reportError(i + 1, "unknown conversion", action); }
            continue;
        }
        if (cmd == "stats") {
            if (!statsEnabled) { reportError(i + 1, "stats commands need add (stats)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3]) || !variables.count(tokens[2]) || types[tokens[2]] != "list") { reportError(i + 1, "use stats sum|average|min|max|median|range LIST RESULT"); continue; }
            const auto* items = std::get_if<std::vector<std::string>>(&variables[tokens[2]].value);
            if (!items || items->empty()) { reportError(i + 1, "stats needs a non-empty list"); continue; }
            std::vector<double> values; values.reserve(items->size());
            for (const auto& item : *items) { double number = 0; if (!parseNumber(valueFor(item, variables), number)) { reportError(i + 1, "stats list must contain only numbers", item); values.clear(); break; } values.push_back(number); }
            if (values.empty()) continue;
            const std::string action = lower(tokens[1]); double result = 0;
            if (action == "sum") result = std::accumulate(values.begin(), values.end(), 0.0);
            else if (action == "average") result = std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(values.size());
            else if (action == "min") result = *std::min_element(values.begin(), values.end());
            else if (action == "max") result = *std::max_element(values.begin(), values.end());
            else if (action == "range") result = *std::max_element(values.begin(), values.end()) - *std::min_element(values.begin(), values.end());
            else if (action == "median") { std::sort(values.begin(), values.end()); const std::size_t middle = values.size() / 2; result = values.size() % 2 ? values[middle] : (values[middle - 1] + values[middle]) / 2.0; }
            else { reportError(i + 1, "unknown stats operation", action); continue; }
            variables[tokens[3]] = result; types[tokens[3]] = "num"; continue;
        }

        if (cmd == "units") {
            if (!unitsEnabled) { reportError(i + 1, "units commands need add (units)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) { reportError(i + 1, "use units c_to_f|f_to_c|km_to_mi|mi_to_km|m_to_ft|ft_to_m|kg_to_lb|lb_to_kg VALUE RESULT"); continue; }
            double value = 0; if (!evaluateExpression(tokens[2], variables, value)) { reportError(i + 1, "units needs a numeric value", tokens[2]); continue; }
            const std::string action = lower(tokens[1]); double result = 0;
            if (action == "c_to_f") result = value * 9.0 / 5.0 + 32.0;
            else if (action == "f_to_c") result = (value - 32.0) * 5.0 / 9.0;
            else if (action == "km_to_mi") result = value * 0.621371192237334;
            else if (action == "mi_to_km") result = value / 0.621371192237334;
            else if (action == "m_to_ft") result = value * 3.28083989501312;
            else if (action == "ft_to_m") result = value / 3.28083989501312;
            else if (action == "kg_to_lb") result = value * 2.20462262184878;
            else if (action == "lb_to_kg") result = value / 2.20462262184878;
            else { reportError(i + 1, "unknown unit conversion", action); continue; }
            variables[tokens[3]] = result; types[tokens[3]] = "num"; continue;
        }
        if (cmd == "random") {
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use random MIN MAX RESULT");
                continue;
            }
            double minimum = 0, maximum = 0;
            if (!evaluateExpression(tokens[1], variables, minimum) ||
                !evaluateExpression(tokens[2], variables, maximum) || minimum > maximum) {
                reportError(i + 1, "random needs numeric bounds with MIN no greater than MAX");
                continue;
            }
            if (std::floor(minimum) == minimum && std::floor(maximum) == maximum &&
                minimum >= static_cast<double>(std::numeric_limits<int>::min()) &&
                maximum <= static_cast<double>(std::numeric_limits<int>::max())) {
                std::uniform_int_distribution<int> distribution(static_cast<int>(minimum), static_cast<int>(maximum));
                variables[tokens[3]] = static_cast<double>(distribution(trgRandomEngine));
            } else {
                std::uniform_real_distribution<double> distribution(minimum, maximum);
                variables[tokens[3]] = distribution(trgRandomEngine);
            }
            types[tokens[3]] = "num";
            continue;
        }

        if (cmd == "collides" || cmd == "touching") {
            if (!collisionEnabled) { reportError(i + 1, "collision helpers need add (collision)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use collides OBJECT_A OBJECT_B RESULT");
                continue;
            }
            double ax = 0, ay = 0, aw = 0, ah = 0, bx = 0, by = 0, bw = 0, bh = 0;
            if (!uiEntityBounds(tokens[1], ax, ay, aw, ah) || !uiEntityBounds(tokens[2], bx, by, bw, bh)) {
                reportError(i + 1, "collision checks need two named UI blocks, buttons, or sprites");
                continue;
            }
            const bool overlap = ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
            variables[tokens[3]] = overlap;
            types[tokens[3]] = "toggle";
            continue;
        }
        if (cmd == "watch_collision") {
            if (!collisionEnabled) { reportError(i + 1, "collision helpers need add (collision)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use watch_collision OBJECT_A OBJECT_B RESULT");
                continue;
            }
            double x = 0, y = 0, w = 0, h = 0;
            if (!uiEntityBounds(tokens[1], x, y, w, h) || !uiEntityBounds(tokens[2], x, y, w, h)) {
                reportError(i + 1, "watch_collision needs two named UI blocks, buttons, or sprites");
                continue;
            }
            collisionWatches.push_back({tokens[1], tokens[2], tokens[3]});
            types[tokens[3]] = "toggle";
            refreshCollisionWatches(variables, types);
            continue;
        }
        if (cmd == "move_ui") {
            if (!collisionEnabled) { reportError(i + 1, "game movement helpers need add (collision)"); continue; }
            if (tokens.size() != 6 || lower(tokens[2]) != "x" || lower(tokens[4]) != "y") {
                reportError(i + 1, "use move_ui OBJECT x DX y DY");
                continue;
            }
            double dx = 0, dy = 0;
            if (!parseNumber(valueFor(tokens[3], variables), dx) || !parseNumber(valueFor(tokens[5], variables), dy)) {
                reportError(i + 1, "move_ui needs numeric x and y amounts");
                continue;
            }
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == tokens[1]; });
            auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == tokens[1]; });
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == tokens[1]; });
            if (block != uiBlocks.end()) { block->x += dx; block->y += dy; }
            else if (sprite != uiSprites.end()) { sprite->x += dx; sprite->y += dy; }
            else if (button != uiButtons.end()) { button->x += dx; button->y += dy; }
            else { reportError(i + 1, "unknown game object", tokens[1]); continue; }
            refreshCollisionWatches(variables, types);
            continue;
        }

        if (cmd == "set_text" || cmd == "set_color") {
            if (!uiMode || tokens.size() != 3) { reportError(i + 1, "use set_text NAME VALUE or set_color NAME #RRGGBB"); continue; }
            const std::string value = valueFor(tokens[2], variables);
            bool changed = false;
            if (cmd == "set_text") {
                for (UiText& item : uiTexts) if (item.name == tokens[1]) { item.value = value; changed = true; }
                for (UiButton& item : uiButtons) if (item.name == tokens[1]) { item.label = value; changed = true; }
            } else {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(value, red, green, blue)) { reportError(i + 1, "set_color needs a valid color", value); continue; }
                for (UiBlock& item : uiBlocks) if (item.name == tokens[1]) { item.color = value; changed = true; }
                for (UiButton& item : uiButtons) if (item.name == tokens[1]) { item.color = value; changed = true; }
            }
            if (!changed) reportError(i + 1, "unknown UI element", tokens[1]);
            continue;
        }


        if (cmd == "physics") {
            if (!physicsEnabled) { reportError(i + 1, "physics commands need add (physics)"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "use physics body|solid|trigger|velocity|gravity|drag|bounce|position|drag_launch|reset|freeze|unfreeze|remove|step"); continue; }
            const std::string action = lower(tokens[1]);
            if (action == "body" || action == "solid" || action == "trigger") {
                if (tokens.size() != 3 || !validName(tokens[2])) {
                    reportError(i + 1, "use physics body|solid|trigger OBJECT");
                    continue;
                }
                double x = 0, y = 0, w = 0, h = 0;
                if (!uiEntityBounds(tokens[2], x, y, w, h)) {
                    reportError(i + 1, "physics needs an existing named UI block, button, or sprite", tokens[2]);
                    continue;
                }
                PhysicsBody body;
                body.name = tokens[2];
                body.dynamic = action == "body";
                body.solid = action == "solid" || action == "body";
                body.trigger = action == "trigger";
                physicsBodies[body.name] = body;
                continue;
            }
            if (action == "reset") {
                if (tokens.size() != 3 || !physicsBodies.count(tokens[2])) {
                    reportError(i + 1, "use physics reset BODY");
                    continue;
                }
                PhysicsBody& body = physicsBodies[tokens[2]];
                body.vx = body.vy = 0;
                body.frozen = false;
                body.dragLaunching = false;
                body.launched = false;
                body.bounceCount = 0;
                continue;
            }
            if (action == "drag_launch") {
                if (tokens.size() != 4 || !physicsBodies.count(tokens[2]) || !physicsBodies[tokens[2]].dynamic) {
                    reportError(i + 1, "use physics drag_launch BODY MAX_SPEED");
                    continue;
                }
                double limit = 0;
                if (!evaluateExpression(tokens[3], variables, limit) || !std::isfinite(limit) || limit <= 0) {
                    reportError(i + 1, "drag launch speed limit must be greater than zero");
                    continue;
                }
                PhysicsBody& body = physicsBodies[tokens[2]];
                body.dragLaunch = true;
                body.launchSpeedLimit = limit;
                body.launched = false;
                body.dragLaunching = false;
                body.vx = body.vy = 0;
                continue;
            }
            if (action == "freeze" || action == "unfreeze") {
                if (tokens.size() != 3 || !physicsBodies.count(tokens[2])) {
                    reportError(i + 1, "use physics freeze|unfreeze BODY");
                    continue;
                }
                PhysicsBody& body = physicsBodies[tokens[2]];
                body.frozen = action == "freeze";
                if (body.frozen) body.vx = body.vy = 0;
                continue;
            }
            if (action == "remove") {
                if (tokens.size() != 3) { reportError(i + 1, "use physics remove OBJECT"); continue; }
                physicsBodies.erase(tokens[2]);
                continue;
            }
            if (action == "position") {
                if (tokens.size() != 5 || !physicsBodies.count(tokens[2])) {
                    reportError(i + 1, "use physics position BODY X Y");
                    continue;
                }
                double x = 0, y = 0;
                if (!evaluateExpression(tokens[3], variables, x) || !evaluateExpression(tokens[4], variables, y) ||
                    !setUiEntityPosition(tokens[2], x, y)) {
                    reportError(i + 1, "physics position needs a registered UI body and numeric coordinates");
                    continue;
                }
                refreshCollisionWatches(variables, types);
                continue;
            }
            if (action == "step" || action == "update") {
                if (tokens.size() != 3) { reportError(i + 1, "use physics step SECONDS"); continue; }
                double seconds = 0;
                if (!evaluateExpression(tokens[2], variables, seconds) || !std::isfinite(seconds) || seconds <= 0 || seconds > 0.1) {
                    reportError(i + 1, "physics step needs a time greater than 0 and no more than 0.1 seconds");
                    continue;
                }
                physicsStep(seconds, variables, types);
                continue;
            }
            if (tokens.size() < 4 || !physicsBodies.count(tokens[2])) {
                reportError(i + 1, "physics setting needs a registered body; use physics body or physics solid first", tokens.size() > 2 ? tokens[2] : "");
                continue;
            }
            PhysicsBody& body = physicsBodies[tokens[2]];
            if (action == "velocity") {
                if (tokens.size() != 5 || !body.dynamic) { reportError(i + 1, "use physics velocity BODY VX VY on a dynamic body"); continue; }
                if (!evaluateExpression(tokens[3], variables, body.vx) || !evaluateExpression(tokens[4], variables, body.vy)) {
                    reportError(i + 1, "physics velocity needs numeric VX and VY"); continue;
                }
            } else if (action == "gravity" || action == "drag" || action == "bounce") {
                if (tokens.size() != 4) { reportError(i + 1, "use physics gravity|drag|bounce BODY VALUE"); continue; }
                double value = 0;
                if (!evaluateExpression(tokens[3], variables, value) || !std::isfinite(value)) {
                    reportError(i + 1, "physics setting needs a finite number"); continue;
                }
                if (action == "gravity") body.gravity = value;
                else if (action == "drag" && value >= 0) body.drag = value;
                else if (action == "bounce" && value >= 0) body.restitution = value;
                else { reportError(i + 1, "physics drag and bounce must be zero or greater"); continue; }
            } else {
                reportError(i + 1, "unknown physics operation", action);
            }
            continue;
        }

        if (cmd == "dictionary") {
            if (tokens.size() != 2 || !validName(tokens[1])) {
                reportError(i + 1, "use dictionary NAME");
                continue;
            }
            variables[tokens[1]] = std::make_shared<DictionaryData>();
            types[tokens[1]] = "dictionary";
            continue;
        }
        if (cmd == "dict_set" || cmd == "dict_get" || cmd == "dict_has" ||
            cmd == "dict_remove" || cmd == "dict_keys" || cmd == "dict_count") {
            const std::size_t expected = cmd == "dict_set" ? 4 :
                (cmd == "dict_get" || cmd == "dict_has" ? 4 :
                 (cmd == "dict_remove" ? 3 : 3));
            if (tokens.size() != expected || !variables.count(tokens[1]) ||
                types[tokens[1]] != "dictionary") {
                reportError(i + 1, "use dictionary NAME, then dict_set/get/has/remove/keys/count");
                continue;
            }
            auto* dictionaryPointer = std::get_if<std::shared_ptr<DictionaryData>>(&variables[tokens[1]].value);
            if (!dictionaryPointer || !*dictionaryPointer) {
                reportError(i + 1, "dictionary value is invalid", tokens[1]);
                continue;
            }
            auto& entries = (*dictionaryPointer)->values;
            if (cmd == "dict_set") {
                const std::string key = valueFor(tokens[2], variables);
                const std::string raw = valueFor(tokens[3], variables);
                if (variables.count(tokens[3])) entries[key] = variables[tokens[3]];
                else if (lower(raw) == "true" || lower(raw) == "false") entries[key] = (lower(raw) == "true");
                else {
                    double numeric = 0;
                    if (parseNumber(raw, numeric)) entries[key] = numeric;
                    else entries[key] = raw;
                }
            } else if (cmd == "dict_get") {
                const std::string key = valueFor(tokens[2], variables);
                const auto found = entries.find(key);
                if (found == entries.end()) { reportError(i + 1, "dictionary key was not found", key); continue; }
                if (!validName(tokens[3])) { reportError(i + 1, "dict_get needs a destination variable", tokens[3]); continue; }
                variables[tokens[3]] = found->second;
                if (std::holds_alternative<double>(found->second.value)) types[tokens[3]] = "num";
                else if (std::holds_alternative<bool>(found->second.value)) types[tokens[3]] = "toggle";
                else if (std::holds_alternative<std::vector<std::string>>(found->second.value) ||
                         std::holds_alternative<VariableList>(found->second.value)) types[tokens[3]] = "list";
                else if (std::holds_alternative<std::shared_ptr<DictionaryData>>(found->second.value)) types[tokens[3]] = "dictionary";
                else types[tokens[3]] = "word";
            } else if (cmd == "dict_has") {
                if (!validName(tokens[3])) { reportError(i + 1, "dict_has needs a result variable", tokens[3]); continue; }
                variables[tokens[3]] = entries.count(valueFor(tokens[2], variables)) != 0;
                types[tokens[3]] = "toggle";
            } else if (cmd == "dict_remove") {
                entries.erase(valueFor(tokens[2], variables));
            } else if (cmd == "dict_count") {
                if (!validName(tokens[2])) { reportError(i + 1, "dict_count needs a result variable", tokens[2]); continue; }
                variables[tokens[2]] = static_cast<double>(entries.size());
                types[tokens[2]] = "num";
            } else {
                if (!validName(tokens[2])) { reportError(i + 1, "dict_keys needs a destination list", tokens[2]); continue; }
                VariableList keys;
                keys.reserve(entries.size());
                std::vector<std::string> sortedKeys;
                for (const auto& entry : entries) sortedKeys.push_back(entry.first);
                std::sort(sortedKeys.begin(), sortedKeys.end());
                for (const auto& key : sortedKeys) keys.push_back(std::make_shared<VariableValue>(key));
                variables[tokens[2]] = std::move(keys);
                types[tokens[2]] = "list";
            }
            continue;
        }

        if (cmd == "list") {
            if (tokens.size() < 2 || !validName(tokens[1])) { reportError(i + 1, "use list NAME [VALUE, VALUE]", "list"); continue; }
            VariableList items;
            const std::size_t namePosition = positionAfterArgument(line, tokens[1]);
            if (namePosition == std::string::npos) { reportError(i + 1, "list needs a valid name", tokens[1]); continue; }
            std::string initial = trim(line.substr(namePosition));
            if (!initial.empty() && initial.front() == '=') {
                initial = trim(initial.substr(1));
                for (const std::string& item : tokenize(initial)) {
                    if (variables.count(item)) items.push_back(std::make_shared<VariableValue>(variables[item]));
                    else if (lower(item) == "true" || lower(item) == "false")
                        items.push_back(std::make_shared<VariableValue>(lower(item) == "true"));
                    else {
                        double numeric = 0;
                        if (parseNumber(item, numeric)) items.push_back(std::make_shared<VariableValue>(numeric));
                        else items.push_back(std::make_shared<VariableValue>(item));
                    }
                }
            }
            variables[tokens[1]] = std::move(items);
            types[tokens[1]] = "list";
            continue;
        }

        if (cmd == "push" || cmd == "get" || cmd == "remove") {
            if (tokens.size() != (cmd == "get" ? 4u : 3u) || !variables.count(tokens[1]) || types[tokens[1]] != "list") {
                reportError(i + 1, cmd == "push" ? "use push LIST VALUE" : (cmd == "get" ? "use get LIST INDEX VARIABLE" : "use remove LIST INDEX"));
                continue;
            }
            auto* items = std::get_if<VariableList>(&variables[tokens[1]].value);
            if (!items) { reportError(i + 1, "variable is not a list", tokens[1]); continue; }
            if (cmd == "push") {
                if (variables.count(tokens[2])) items->push_back(std::make_shared<VariableValue>(variables[tokens[2]]));
                else if (lower(tokens[2]) == "true" || lower(tokens[2]) == "false")
                    items->push_back(std::make_shared<VariableValue>(lower(tokens[2]) == "true"));
                else {
                    double numeric = 0;
                    if (parseNumber(valueFor(tokens[2], variables), numeric)) items->push_back(std::make_shared<VariableValue>(numeric));
                    else items->push_back(std::make_shared<VariableValue>(valueFor(tokens[2], variables)));
                }
            }
            else {
                double parsedIndex = 0;
                if (!parseNumber(valueFor(tokens[2], variables), parsedIndex) || parsedIndex < 0 ||
                    parsedIndex >= static_cast<double>(items->size())) { reportError(i + 1, "list index is out of range", tokens[2]); continue; }
                const std::size_t index = static_cast<std::size_t>(parsedIndex);
                if (cmd == "get") {
                    if (!validName(tokens[3])) { reportError(i + 1, "get needs a valid destination variable", tokens[3]); continue; }
                    if (!(*items)[index]) { reportError(i + 1, "list item is invalid", tokens[2]); continue; }
                    variables[tokens[3]] = *(*items)[index];
                    if (std::holds_alternative<double>((*items)[index]->value)) types[tokens[3]] = "num";
                    else if (std::holds_alternative<bool>((*items)[index]->value)) types[tokens[3]] = "toggle";
                    else if (std::holds_alternative<VariableList>((*items)[index]->value) ||
                             std::holds_alternative<std::vector<std::string>>((*items)[index]->value)) types[tokens[3]] = "list";
                    else if (std::holds_alternative<std::shared_ptr<DictionaryData>>((*items)[index]->value)) types[tokens[3]] = "dictionary";
                    else types[tokens[3]] = "word";
                } else items->erase(items->begin() + static_cast<std::ptrdiff_t>(index));
            }
            continue;
        }

        if (cmd == "pop" || cmd == "insert" || cmd == "index_of" || cmd == "split" || cmd == "join" || cmd == "trim" || cmd == "substring") {
            const std::size_t expected = cmd == "pop" || cmd == "trim" ? 3 :
                (cmd == "insert" || cmd == "index_of" || cmd == "split" ? 4 :
                 (cmd == "substring" ? 5 : 4));
            if (tokens.size() != expected || (cmd != "insert" && !validName(tokens.back()))) {
                reportError(i + 1, "invalid arguments for " + cmd, cmd);
                continue;
            }

            if (cmd == "pop" || cmd == "insert" || cmd == "join") {
                if (!variables.count(tokens[1]) || types[tokens[1]] != "list") {
                    reportError(i + 1, cmd + " needs a list variable", tokens[1]);
                    continue;
                }
                auto* items = std::get_if<VariableList>(&variables[tokens[1]].value);
                if (!items) { reportError(i + 1, "variable is not a list", tokens[1]); continue; }
                if (cmd == "pop") {
                    if (items->empty()) { reportError(i + 1, "cannot pop an empty list", tokens[1]); continue; }
                    if (!items->back()) { reportError(i + 1, "list item is invalid", tokens[1]); continue; }
                    variables[tokens[2]] = *items->back();
                    if (std::holds_alternative<double>(items->back()->value)) types[tokens[2]] = "num";
                    else if (std::holds_alternative<bool>(items->back()->value)) types[tokens[2]] = "toggle";
                    else if (std::holds_alternative<VariableList>(items->back()->value)) types[tokens[2]] = "list";
                    else if (std::holds_alternative<std::shared_ptr<DictionaryData>>(items->back()->value)) types[tokens[2]] = "dictionary";
                    else types[tokens[2]] = "word";
                    items->pop_back();
                } else if (cmd == "insert") {
                    double parsedIndex = 0;
                    if (!parseNumber(valueFor(tokens[2], variables), parsedIndex) || parsedIndex < 0 ||
                        parsedIndex > static_cast<double>(items->size()) || std::floor(parsedIndex) != parsedIndex) {
                        reportError(i + 1, "insert index must be a whole number from 0 to list length", tokens[2]);
                        continue;
                    }
                    VariableValue inserted;
                    if (variables.count(tokens[3])) inserted = variables[tokens[3]];
                    else {
                        const std::string raw = valueFor(tokens[3], variables);
                        double numeric = 0;
                        if (lower(raw) == "true" || lower(raw) == "false") inserted = lower(raw) == "true";
                        else if (parseNumber(raw, numeric)) inserted = numeric;
                        else inserted = raw;
                    }
                    items->insert(items->begin() + static_cast<std::ptrdiff_t>(parsedIndex),
                                  std::make_shared<VariableValue>(std::move(inserted)));
                } else {
                    const std::string delimiter = valueFor(tokens[2], variables);
                    std::string joined;
                    for (std::size_t n = 0; n < items->size(); ++n) {
                        if (n) joined += delimiter;
                        if ((*items)[n]) joined += static_cast<std::string>(*(*items)[n]);
                    }
                    variables[tokens[3]] = joined;
                    types[tokens[3]] = "word";
                }
                continue;
            }

            if (cmd == "index_of") {
                const std::string source = valueFor(tokens[1], variables);
                const std::string search = valueFor(tokens[2], variables);
                const auto found = source.find(search);
                variables[tokens[3]] = found == std::string::npos ? -1.0 : static_cast<double>(found);
                types[tokens[3]] = "num";
                continue;
            }

            if (cmd == "split") {
                const std::string source = valueFor(tokens[1], variables);
                const std::string delimiter = valueFor(tokens[2], variables);
                if (delimiter.empty()) { reportError(i + 1, "split delimiter cannot be empty", tokens[2]); continue; }
                VariableList parts;
                std::size_t begin = 0;
                std::size_t found = 0;
                while ((found = source.find(delimiter, begin)) != std::string::npos) {
                    parts.push_back(std::make_shared<VariableValue>(source.substr(begin, found - begin)));
                    begin = found + delimiter.size();
                }
                parts.push_back(std::make_shared<VariableValue>(source.substr(begin)));
                variables[tokens[3]] = std::move(parts);
                types[tokens[3]] = "list";
                continue;
            }

            if (cmd == "trim") {
                variables[tokens[2]] = trim(valueFor(tokens[1], variables));
                types[tokens[2]] = "word";
                continue;
            }

            if (cmd == "substring") {
                const std::string source = valueFor(tokens[1], variables);
                double startValue = 0, lengthValue = 0;
                if (!parseNumber(valueFor(tokens[2], variables), startValue) ||
                    !parseNumber(valueFor(tokens[3], variables), lengthValue) ||
                    startValue < 0 || lengthValue < 0 ||
                    std::floor(startValue) != startValue || std::floor(lengthValue) != lengthValue ||
                    startValue > static_cast<double>(source.size())) {
                    reportError(i + 1, "substring needs non-negative whole-number START and LENGTH values", cmd);
                    continue;
                }
                variables[tokens[4]] = source.substr(static_cast<std::size_t>(startValue), static_cast<std::size_t>(lengthValue));
                types[tokens[4]] = "word";
                continue;
            }
        }

        if (cmd == "upper" || cmd == "lower" || cmd == "length" || cmd == "contains" || cmd == "replace") {
            const std::size_t expected = cmd == "replace" ? 5 : (cmd == "contains" ? 4 : 3);
            if (tokens.size() != expected || !validName(tokens.back())) {
                reportError(i + 1, cmd == "replace" ? "use replace VALUE SEARCH REPLACEMENT RESULT" :
                    (cmd == "contains" ? "use contains VALUE SEARCH RESULT" : "use " + cmd + " VALUE RESULT"));
                continue;
            }
            const std::string source = valueFor(tokens[1], variables);
            std::string result;
            if (cmd == "upper") { result = source; std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); }); }
            else if (cmd == "lower") result = lower(source);
            else if (cmd == "length") {
                const auto variable = variables.find(tokens[1]);
                const auto* listValue = variable == variables.end() ? nullptr : std::get_if<VariableList>(&variable->second.value);
                result = std::to_string(listValue ? listValue->size() : source.size());
            } else if (cmd == "contains") result = source.find(valueFor(tokens[2], variables)) == std::string::npos ? "false" : "true";
            else {
                const std::string search = valueFor(tokens[2], variables);
                const std::string replacement = valueFor(tokens[3], variables);
                if (!search.empty()) {
                    result = source;
                    std::size_t position = 0;
                    while ((position = result.find(search, position)) != std::string::npos) {
                        result.replace(position, search.size(), replacement);
                        position += replacement.size();
                    }
                } else result = source;
            }
            if (cmd == "contains") { variables[tokens.back()] = result == "true"; types[tokens.back()] = "toggle"; }
            else if (cmd == "length" && !(variables.count(tokens[1]) && types[tokens[1]] == "list")) {
                double numeric = 0; parseNumber(result, numeric); variables[tokens.back()] = numeric; types[tokens.back()] = "num";
            } else if (cmd == "length") { double numeric = 0; parseNumber(result, numeric); variables[tokens.back()] = numeric; types[tokens.back()] = "num"; }
            else { variables[tokens.back()] = result; if (!types.count(tokens.back())) types[tokens.back()] = "word"; }
            continue;
        }

        if (cmd == "read_file" || cmd == "write_file" || cmd == "append_file" || cmd == "delete_file" || cmd == "file_exists") {
            const std::size_t minimum = cmd == "write_file" || cmd == "append_file" ? 3 :
                (cmd == "read_file" || cmd == "file_exists" ? 3 : 2);
            if (!filesEnabled) { reportError(i + 1, "enable file commands with add (files)"); continue; }
            if (tokens.size() < minimum) { reportError(i + 1, "missing file command arguments", cmd); continue; }
            bool pathValid = false;
            const std::filesystem::path filePath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid) { reportError(i + 1, "file path must stay inside the TRG project folder", tokens[1]); continue; }
            try {
                if (cmd == "read_file") {
                    if (!validName(tokens[2])) { reportError(i + 1, "read_file needs a destination variable", tokens[2]); continue; }
                    std::ifstream input(filePath, std::ios::binary);
                    if (!input) { reportError(i + 1, "could not read file", tokens[1]); continue; }
                    std::ostringstream content; content << input.rdbuf();
                    variables[tokens[2]] = content.str();
                    types[tokens[2]] = "word";
                } else if (cmd == "file_exists") {
                    if (!validName(tokens[2])) { reportError(i + 1, "file_exists needs a destination variable", tokens[2]); continue; }
                    variables[tokens[2]] = std::filesystem::exists(filePath);
                    types[tokens[2]] = "toggle";
                } else if (cmd == "delete_file") {
                    if (!std::filesystem::remove(filePath)) reportError(i + 1, "file does not exist", tokens[1]);
                } else {
                    std::ofstream output(filePath, std::ios::binary | (cmd == "append_file" ? std::ios::app : std::ios::trunc));
                    if (!output) { reportError(i + 1, "could not write file", tokens[1]); continue; }
                    for (std::size_t n = 2; n < tokens.size(); ++n) { if (n > 2) output << ' '; output << valueFor(tokens[n], variables); }
                    output << '\n';
                    if (!output) reportError(i + 1, "failed while writing file", tokens[1]);
                }
            } catch (const std::filesystem::filesystem_error& error) { reportError(i + 1, error.what(), tokens[1]); }
            continue;
        }

        if (cmd == "play_audio" || cmd == "show_image" || cmd == "play_video") {
            if (tokens.size() != 2) { reportError(i + 1, "use " + cmd + " PROJECT_FILE"); continue; }
            bool pathValid = false;
            const std::filesystem::path mediaPath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(mediaPath)) {
                reportError(i + 1, "media file must exist inside the TRG project folder", tokens[1]);
                continue;
            }
#ifdef __ANDROID__
            trgStudioActionAndroid("media:" + cmd + ":" + mediaPath.u8string());
#elif defined(_WIN32)
            const std::wstring widePath = mediaPath.wstring();
            const HINSTANCE result = ShellExecuteW(mainWindow, L"open", widePath.c_str(), nullptr,
                                                    projectRoot.wstring().c_str(), SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(result) <= 32) reportError(i + 1, "could not open media file", tokens[1]);
#else
            reportError(i + 1, "media playback is not supported on this platform", tokens[1]);
#endif
            continue;
        }

        if (cmd == "web_page") {
            if (!webEnabled) { reportError(i + 1, "enable TRG Web with add (web)"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "use web_page TITLE"); continue; }
            std::string title;
            for (std::size_t n = 1; n < tokens.size(); ++n) { if (n > 1) title += ' '; title += valueFor(tokens[n], variables); }
            const std::string safeTitle = htmlEscape(title);
            webPageHtml = "<!doctype html><html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>" + safeTitle +
                "</title><style>body{margin:0;padding:24px;font:18px system-ui;background:#20202a;color:#f3f0fa}button{padding:12px 20px;border:0;border-radius:8px;background:#7043be;color:white;font-size:16px}</style></head><body><h1>" + safeTitle + "</h1>";
            continue;
        }

        if (cmd == "web_text" || cmd == "web_heading") {
            if (!webEnabled || webPageHtml.empty()) { reportError(i + 1, "start a TRG Web page with add (web) and web_page TITLE"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "web text needs a value"); continue; }
            std::string value;
            for (std::size_t n = 1; n < tokens.size(); ++n) { if (n > 1) value += ' '; value += valueFor(tokens[n], variables); }
            const std::string escaped = htmlEscape(value);
            webPageHtml += cmd == "web_heading" ? "<h2>" + escaped + "</h2>" : "<p>" + escaped + "</p>";
            continue;
        }

        if (cmd == "web_button") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() < 2) { reportError(i + 1, "use web_button LABEL [MESSAGE] after web_page"); continue; }
            const std::string label = htmlEscape(valueFor(tokens[1], variables));
            const std::string message = htmlEscape(tokens.size() > 2 ? valueFor(tokens[2], variables) : valueFor(tokens[1], variables));
            webPageHtml += "<button data-message=\"" + message + "\" onclick=\"alert(this.dataset.message)\">" + label + "</button> ";
            continue;
        }

        if (cmd == "web_color") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() != 2) { reportError(i + 1, "use web_color COLOR after web_page"); continue; }
            unsigned red = 0, green = 0, blue = 0;
            const std::string color = valueFor(tokens[1], variables);
            if (!parseColor(color, red, green, blue)) { reportError(i + 1, "invalid TRG Web background color", color); continue; }
            const std::size_t styleEnd = webPageHtml.find("</style>");
            if (styleEnd != std::string::npos) webPageHtml.insert(styleEnd, "body{background:" + color + ";}");
            continue;
        }

        if (cmd == "web_image") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() != 2) { reportError(i + 1, "use web_image PROJECT_FILE after web_page"); continue; }
            bool pathValid = false;
            const auto imagePath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(imagePath)) { reportError(i + 1, "image must be inside the TRG project folder", tokens[1]); continue; }
            std::ifstream imageFile(imagePath, std::ios::binary);
            std::ostringstream bytes; bytes << imageFile.rdbuf();
            const std::string extension = lower(imagePath.extension().u8string());
            const std::string mime = extension == ".png" ? "image/png" : extension == ".gif" ? "image/gif" :
                extension == ".webp" ? "image/webp" : "image/jpeg";
            webPageHtml += "<img style=\"max-width:100%;height:auto\" src=\"data:" + mime + ";base64," + base64Encode(bytes.str()) + "\">";
            continue;
        }

        if (cmd == "web_show") {
            if (!webEnabled || webPageHtml.empty()) { reportError(i + 1, "create a TRG Web page before web_show"); continue; }
            const std::string html = webPageHtml + "</body></html>";
#ifdef __ANDROID__
            trgStudioActionAndroid("webpage:" + html);
#elif defined(_WIN32)
            const std::filesystem::path previewPath = std::filesystem::temp_directory_path() / "trg_web_preview.html";
            { std::ofstream previewFile(previewPath, std::ios::binary); previewFile << html; }
            const HINSTANCE opened = ShellExecuteW(mainWindow, L"open", previewPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(opened) <= 32) reportError(i + 1, "could not open the TRG Web preview");
#else
            appendOutput("TRG Web preview is available in TRG Studio.\r\n");
#endif
            continue;
        }

        if (cmd == "run_wasm") {
            if (!webEnabled) { reportError(i + 1, "enable WebAssembly with add (web)"); continue; }
            if (tokens.size() != 2) { reportError(i + 1, "use run_wasm PROJECT_FILE"); continue; }
            bool pathValid = false;
            const std::filesystem::path wasmPath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(wasmPath) || lower(wasmPath.extension().u8string()) != ".wasm") {
                reportError(i + 1, "run_wasm needs an existing .wasm file inside the TRG project folder", tokens[1]);
                continue;
            }
#ifdef __ANDROID__
            trgStudioActionAndroid("wasm:" + wasmPath.u8string());
#elif defined(_WIN32)
            std::ifstream moduleFile(wasmPath, std::ios::binary);
            std::ostringstream moduleBytes; moduleBytes << moduleFile.rdbuf();
            const std::string encoded = base64Encode(moduleBytes.str());
            const std::string html = "<!doctype html><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><body style=\"background:#20202a;color:#fff;font:16px sans-serif;padding:24px\"><h2>TRG WebAssembly</h2><p>" +
                htmlEscape(wasmPath.filename().u8string()) + "</p><pre id=\"status\">Loading module…</pre><script>try{const b=Uint8Array.from(atob('" + encoded +
                "'),c=>c.charCodeAt(0));WebAssembly.instantiate(b,{env:{}}).then(r=>{window.trgWasm=r.instance;document.getElementById('status').textContent='Module loaded and its start function ran.';}).catch(e=>document.getElementById('status').textContent='Module error: '+e)}catch(e){document.getElementById('status').textContent='Module error: '+e}</script></body>";
            const auto previewPath = std::filesystem::temp_directory_path() / "trg_wasm_preview.html";
            { std::ofstream previewFile(previewPath, std::ios::binary); previewFile << html; }
            const HINSTANCE opened = ShellExecuteW(mainWindow, L"open", previewPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(opened) <= 32) reportError(i + 1, "could not open the WebAssembly preview");
#else
            reportError(i + 1, "WebAssembly preview is not supported on this platform", tokens[1]);
#endif
            continue;
        }

        
        // Beginner-friendly inferred variable declaration: var name = value.
        if (cmd == "let" || cmd == "var") {
            if (tokens.size() < 4 || !validName(tokens[1])) {
                reportError(i + 1, "use var NAME = VALUE", "var");
                continue;
            }
            const std::size_t namePos = positionAfterArgument(line, tokens[1]);
            if (namePos == std::string::npos) {
                reportError(i + 1, "var needs a variable name", tokens[1]);
                continue;
            }
            std::string rawValue = trim(line.substr(namePos));
            if (!rawValue.empty() && rawValue.front() == '=') rawValue = trim(rawValue.substr(1));
            if (rawValue.empty()) {
                reportError(i + 1, "var needs a value", tokens[1]);
                continue;
            }
            const std::string name = tokens[1];
            const std::vector<std::string> valueTokens = tokenize(rawValue);
            if (valueTokens.empty()) { reportError(i + 1, "var needs a value", name); continue; }
            const std::string first = lower(valueTokens[0]);
            double number = 0;
            if (first == "true" || first == "false") {
                variables[name] = first == "true";
                types[name] = "toggle";
            } else if (evaluateExpression(rawValue, variables, number)) {
                variables[name] = number;
                types[name] = "num";
            } else {
                std::string value;
                if (rawValue.front() == '"' || rawValue.front() == 39) {
                    value = valueTokens.size() == 1 ? valueTokens[0] : rawValue;
                    if (value == rawValue && value.size() >= 2) value = value.substr(1, value.size() - 2);
                } else if (valueTokens.size() == 1 && variables.count(valueTokens[0])) {
                    value = valueFor(valueTokens[0], variables);
                } else {
                    value = rawValue;
                }
                variables[name] = value;
                types[name] = "word";
            }
            continue;
        }
        if (cmd == "word" || cmd == "num" || cmd == "toggle" || cmd == "color" || cmd == "code") {
            if (tokens.size() < 2 || !validName(tokens[1])) {
                reportError(i + 1, "invalid declaration", tokens.size() > 1 ? tokens[1] : "");
                continue;
            }
            const std::string name = tokens[1];
            std::string rawValue;
            const std::size_t namePos = positionAfterArgument(line, name);
            if (namePos == std::string::npos) { reportError(i + 1, "invalid declaration", name); continue; }
            rawValue = trim(line.substr(namePos));
            if (!rawValue.empty() && rawValue[0] == '=') rawValue = trim(rawValue.substr(1));
            else if (!rawValue.empty()) { reportError(i + 1, "expected '=' before value", name); continue; }

            std::string value = defaults[cmd];
            if (!rawValue.empty()) {
                const std::vector<std::string> valueTokens = tokenize(rawValue);
                if (valueTokens.size() == 1) value = valueFor(valueTokens[0], variables);
                else value = rawValue;
                if (cmd == "word" && rawValue.front() == '"') value = valueTokens.empty() ? "" : valueTokens[0];
                if (cmd == "code") value = rawValue;
                if (cmd == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(rawValue, variables, numeric)) { reportError(i + 1, "invalid number", rawValue); continue; }
                    value = numberString(numeric);
                } else if (cmd == "toggle") {
                    value = lower(value);
                    if (value != "true" && value != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                } else if (cmd == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "color must be #RRGGBB or 0xRRGGBB", value); continue; }
                }
            }
            if (cmd == "num") { double numeric = 0; parseNumber(value, numeric); variables[name] = numeric; }
            else if (cmd == "toggle") variables[name] = value == "true";
            else variables[name] = value;
            types[name] = cmd;
            continue;
        }

        
        if (cmd == "default") {
            if (tokens.size() < 3 || defaults.count(tokens[1]) == 0) {
                reportError(i + 1, "use default TYPE VALUE");
                continue;
            }
            const std::size_t typePos = positionAfterArgument(line, tokens[1]);
            if (typePos == std::string::npos) { reportError(i + 1, "use default TYPE VALUE"); continue; }
            std::string valueText = trim(line.substr(typePos));
            if (!valueText.empty() && valueText.front() == '=') valueText = trim(valueText.substr(1));
            const std::vector<std::string> valueTokens = tokenize(valueText);
            const std::string value = valueTokens.size() == 1 ? valueFor(valueTokens[0], variables) : valueText;
            if (tokens[1] == "num") {
                double numeric = 0;
                if (!evaluateExpression(valueText, variables, numeric)) { reportError(i + 1, "invalid number", valueText); continue; }
                defaults[tokens[1]] = numberString(numeric);
            } else if (tokens[1] == "toggle") {
                const std::string toggleValue = lower(value);
                if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                defaults[tokens[1]] = toggleValue;
            } else if (tokens[1] == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                defaults[tokens[1]] = value;
            } else {
                defaults[tokens[1]] = value;
            }
            continue;
        }

        
        if (cmd == "group" || cmd == "if" || cmd == "loop" || cmd == "every" || cmd == "for") {
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close, blockIndex)) { reportError(i + 1, "missing block"); continue; }
            if (cmd == "group") {
                const bool namedGroup = tokens.size() >= 2 && validName(tokens[1]);
                const std::string groupName = namedGroup ? tokens[1] : "";
                std::unordered_set<std::string> before;
                for (const auto& item : variables) before.insert(item.first);
                const std::string previousGroup = currentUiGroup;
                Defaults localDefaults = defaults;
                if (namedGroup && uiMode) {
                    bool initiallyVisible = false;
                    for (std::size_t part = 2; part + 1 < tokens.size(); part += 2)
                        if (lower(tokens[part]) == "visible") initiallyVisible = lower(valueFor(tokens[part + 1], variables)) == "true";
                    auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) { return item.name == groupName; });
                    const std::string parentGroup = currentUiGroup == groupName ? "default" : currentUiGroup;
                    if (group == uiGroups.end()) uiGroups.push_back({groupName, initiallyVisible, parentGroup});
                    else { group->visible = initiallyVisible; group->parent = parentGroup; }
                    currentUiGroup = groupName;
                }
                if (!groupName.empty()) { groupStack.push_back(groupName); groupSnapshots.push_back(before); }
                const std::vector<std::string> membership = groupStack;
                Flow inner;
                executeBlock(program, open + 1, close, variables, types, localDefaults, inner, manualMode,
                             namedGroup && uiMode ? true : uiMode, blockIndex);
                if (!groupName.empty()) { groupStack.pop_back(); groupSnapshots.pop_back(); }
                currentUiGroup = previousGroup;
                if (!groupName.empty()) {
                    
                    
                    for (const auto& item : variables) {
                        if (before.count(item.first) || variableGroups.count(item.first)) continue;
                        variableGroups[item.first] = membership;
                    }
                }
                if (inner.stopped) { flow.stopped = true; return; }
                if (inner.returned) { flow.returned = true; flow.returnValue = inner.returnValue; return; }
            } else if (cmd == "for") {
                if (tokens.size() != 6 || lower(tokens[1]) != "each" ||
                    !validName(tokens[2]) || lower(tokens[3]) != "in" ||
                    !variables.count(tokens[4]) || types[tokens[4]] != "list") {
                    reportError(i + 1, "use for each ITEM in LIST ( ... )");
                    i = close;
                    continue;
                }
                const auto* sourceItems = std::get_if<VariableList>(&variables[tokens[4]].value);
                if (!sourceItems) {
                    reportError(i + 1, "for each needs a list of values", tokens[4]);
                    i = close;
                    continue;
                }
                const VariableList items = *sourceItems;
                const std::string itemName = tokens[2];
                const auto previousValue = variables.find(itemName);
                const bool hadPreviousValue = previousValue != variables.end();
                VariableValue savedValue;
                if (hadPreviousValue) savedValue = previousValue->second;
                const auto previousType = types.find(itemName);
                const bool hadPreviousType = previousType != types.end();
                const std::string savedType = hadPreviousType ? previousType->second : "";
                for (const auto& item : items) {
                    if (!item) continue;
                    variables[itemName] = *item;
                    if (std::holds_alternative<double>(item->value)) types[itemName] = "num";
                    else if (std::holds_alternative<bool>(item->value)) types[itemName] = "toggle";
                    else if (std::holds_alternative<VariableList>(item->value)) types[itemName] = "list";
                    else if (std::holds_alternative<std::shared_ptr<DictionaryData>>(item->value)) types[itemName] = "dictionary";
                    else types[itemName] = "word";
                    Flow iteration;
                    executeBlock(program, open + 1, close, variables, types, defaults,
                                 iteration, manualMode, uiMode, blockIndex);
                    if (iteration.stopped) { flow.stopped = true; break; }
                    if (iteration.returned) {
                        flow.returned = true;
                        flow.returnValue = iteration.returnValue;
                        break;
                    }
                    if (iteration.broke) break;
                    // continue intentionally advances to the next item.
                }
                if (hadPreviousValue) variables[itemName] = savedValue;
                else variables.erase(itemName);
                if (hadPreviousType) types[itemName] = savedType;
                else types.erase(itemName);
                if (flow.stopped || flow.returned) return;
            } else if (cmd == "every") {
                long long interval = 0;
                if (tokens.size() < 2 || !parseDurationMilliseconds(valueFor(tokens[1], variables), interval) || interval <= 0) {
                    reportError(i + 1, "use every 1s ( ... ) or every 100ms ( ... )");
                    i = close;
                    continue;
                }
                EveryTask task;
                task.body.assign(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                                 program.begin() + static_cast<std::ptrdiff_t>(close));
                task.blockIndex = std::make_shared<ProgramBlockIndex>(task.body);
                task.interval = std::chrono::milliseconds(interval);
                task.nextRun = std::chrono::steady_clock::now() + task.interval;
                task.manualMode = manualMode;
                task.uiMode = uiMode;
                everyTasks.push_back(task);
                appendOutput("Scheduled repeating code every " + std::to_string(interval) + " ms.\r\n");
            } else if (cmd == "if") {
                const auto conditionFromHeader = [](const std::string& header, const std::string& keyword) {
                    std::string condition = trim(header.substr(keyword.size()));
                    if (!condition.empty() && (condition.back() == '(' || condition.back() == '{')) condition = trim(condition.substr(0, condition.size() - 1));
                    return condition;
                };
                bool branchTaken = false;
                Flow branchFlow;
                const std::string firstHeader = trim(line);
                if (conditionIsTrue(conditionFromHeader(firstHeader, "if"), variables)) {
                    branchTaken = true;
                    executeBlock(program, open + 1, close, variables, types, defaults, branchFlow, manualMode, uiMode, blockIndex);
                }
                std::size_t chainLine = close + 1;
                std::size_t finalChainClose = close;
                while (chainLine < end) {
                    while (chainLine < end && trim(program[chainLine]).empty()) ++chainLine;
                    if (chainLine >= end) break;
                    const std::string chainHeader = trim(program[chainLine]);
                    const std::string loweredHeader = lower(chainHeader);
                    const bool isElseIf = loweredHeader.rfind("else if", 0) == 0 &&
                        (loweredHeader.size() == 7 || std::isspace(static_cast<unsigned char>(loweredHeader[7])) ||
                         loweredHeader[7] == '(');
                    const bool isElse = !isElseIf && (loweredHeader == "else" || loweredHeader.rfind("else(", 0) == 0 ||
                        (loweredHeader.rfind("else", 0) == 0 && loweredHeader.size() > 4 &&
                         std::isspace(static_cast<unsigned char>(loweredHeader[4]))));
                    if (!isElseIf && !isElse) break;
                    std::size_t chainOpen = 0, chainClose = 0;
                    if (!findBlock(program, chainLine, end, chainOpen, chainClose, blockIndex)) break;
                    finalChainClose = chainClose;
                    const bool matches = isElse || conditionIsTrue(conditionFromHeader(chainHeader, "else if"), variables);
                    if (!branchTaken && matches) {
                        branchTaken = true;
                        executeBlock(program, chainOpen + 1, chainClose, variables, types, defaults, branchFlow, manualMode, uiMode, blockIndex);
                    }
                    chainLine = chainClose + 1;
                    if (isElse) break;
                }
                if (branchFlow.stopped) { flow.stopped = true; return; }
                if (branchFlow.returned) { flow.returned = true; flow.returnValue = branchFlow.returnValue; return; }
                if (branchFlow.broke) { flow.broke = true; return; }
                if (branchFlow.continued) { flow.continued = true; return; }
                if (finalChainClose > close) close = finalChainClose;
            } else {
                const std::size_t conditionStart = line.find_first_of(" \t");
                std::string condition = conditionStart == std::string::npos ? "" : trim(line.substr(conditionStart + 1));
                if (!condition.empty() && condition.back() == '(') condition = trim(condition.substr(0, condition.size() - 1));
                int guard = 0;
                while (conditionIsTrue(condition, variables)) {
                    if (++guard > 100000) { reportError(i + 1, "loop limit"); break; }
                    Flow inner;
                    executeBlock(program, open + 1, close, variables, types, defaults, inner, manualMode, uiMode, blockIndex);
                    if (inner.stopped) { flow.stopped = true; return; }
                if (inner.returned) { flow.returned = true; flow.returnValue = inner.returnValue; return; }
                    if (inner.broke) break;
#ifdef _WIN32
                    if (controlsEnabled) Sleep(10);
#elif defined(__ANDROID__)
                    if (controlsEnabled) std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
                }
            }
            i = close;
            continue;
        }

        if (cmd == "return") {
            if (tokens.size() < 2) {
                flow.returnValue = VariableValue();
            } else {
                const std::string raw = trim(line.substr(line.find_first_of(" \t") + 1));
                double numeric = 0;
                if (evaluateExpression(raw, variables, numeric)) {
                    flow.returnValue = numeric;
                } else if (tokens.size() == 2 && variables.count(tokens[1])) {
                    flow.returnValue = variables[tokens[1]];
                } else if (tokens.size() == 2 &&
                           (lower(tokens[1]) == "true" || lower(tokens[1]) == "false")) {
                    flow.returnValue = lower(tokens[1]) == "true";
                } else {
                    std::string textValue;
                    for (std::size_t part = 1; part < tokens.size(); ++part) {
                        if (part > 1) textValue += ' ';
                        textValue += valueFor(tokens[part], variables);
                    }
                    flow.returnValue = textValue;
                }
            }
            flow.returned = true;
            return;
        }
        if (cmd == "break") { flow.broke = true; return; }
        if (cmd == "continue") { flow.continued = true; return; }
        if (cmd == "stop" || cmd == "exit") { flow.stopped = true; return; }

        
        if (cmd == "register" || cmd == "registry" || cmd == "move" || cmd == "add" ||
            cmd == "subtract" || cmd == "multiply" || cmd == "divide") {
            if (!manualMode) { reportError(i + 1, "low-level commands need add (manual)", cmd); continue; }
            if (cmd == "register" || cmd == "registry") {
                const std::string name = tokens.size() > 1 ? unbraced(tokens[1]) : "";
                if (tokens.size() < 2 || tokens.size() > 3 || !validName(name)) {
                    reportError(i + 1, "use register NAME [num|word|toggle|color]");
                    continue;
                }
                const std::string type = tokens.size() == 3 ? tokens[2] : "num";
                if (!defaults.count(type)) { reportError(i + 1, "unknown register type", type); continue; }
                if (type == "num") { double numeric = 0; parseNumber(defaults[type], numeric); variables[name] = numeric; }
                else if (type == "toggle") variables[name] = defaults[type] == "true";
                else variables[name] = defaults[type];
                types[name] = type;
                continue;
            }
            const std::string name = tokens.size() > 1 ? unbraced(tokens[1]) : "";
            if (tokens.size() != 3 || !variables.count(name)) {
                reportError(i + 1, "register instruction needs an existing register and value");
                continue;
            }
            const std::string type = types.count(name) ? types[name] : "num";
            if (cmd == "move") {
                const std::string value = valueFor(tokens[2], variables);
                if (type == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(tokens[2], variables, numeric)) { reportError(i + 1, "invalid number", tokens[2]); continue; }
                    variables[name] = numeric;
                } else if (type == "toggle") {
                    const std::string toggleValue = lower(value);
                    if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                    variables[name] = toggleValue == "true";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                    variables[name] = value;
                } else variables[name] = value;
                continue;
            }
            double left = 0, right = 0;
            if (type != "num" || !parseNumber(static_cast<std::string>(variables[name]), left) ||
                !evaluateExpression(tokens[2], variables, right)) {
                reportError(i + 1, "arithmetic instructions need numeric registers and values");
                continue;
            }
            if (cmd == "add") left += right;
            else if (cmd == "subtract") left -= right;
            else if (cmd == "multiply") left *= right;
            else if (right == 0) { reportError(i + 1, "division by zero"); continue; }
            else left /= right;
            variables[name] = left;
            continue;
        }

        
        if (cmd == "ui") {
            if (!uiMode) { reportError(i + 1, "UI editing commands need add (ui)"); continue; }
            if (tokens.size() < 3) { reportError(i + 1, "use ui create|duplicate|delete|rename|move|resize|color|shape"); continue; }
            const std::string action = lower(tokens[1]);
            const auto uniqueUiName = [&]() {
                std::string candidate;
                do { candidate = "object_" + std::to_string(uiAutoNameCounter++); }
                while (std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == candidate; }) ||
                       std::any_of(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == candidate; }));
                return candidate;
            };
            if (action == "create") {
                if (tokens.size() < 14 || lower(tokens[2]) != "block") {
                    reportError(i + 1, "use ui create block NAME x X y Y w W h H color COLOR [shape SHAPE]");
                    continue;
                }
                UiBlock item;
                item.group = currentUiGroup;
                item.name = valueFor(tokens[3], variables);
                if (lower(item.name) == "auto") item.name = uniqueUiName();
                bool valid = validName(item.name);
                bool hasX = false, hasY = false, hasW = false, hasH = false, hasColor = false;
                for (std::size_t part = 4; valid && part + 1 < tokens.size(); part += 2) {
                    const std::string key = lower(tokens[part]);
                    const std::string value = valueFor(tokens[part + 1], variables);
                    double number = 0;
                    if (key == "x") { valid = parseNumber(value, item.x); hasX = valid; }
                    else if (key == "y") { valid = parseNumber(value, item.y); hasY = valid; }
                    else if (key == "w") { valid = parseNumber(value, item.width) && item.width > 0; hasW = valid; }
                    else if (key == "h") { valid = parseNumber(value, item.height) && item.height > 0; hasH = valid; }
                    else if (key == "z") { valid = parseNumber(value, number); if (valid) item.z = static_cast<int>(number); }
                    else if (key == "color") {
                        unsigned red = 0, green = 0, blue = 0;
                        valid = parseColor(value, red, green, blue);
                        if (valid) { item.color = value; hasColor = true; }
                    } else if (key == "shape") {
                        const std::string shape = lower(value);
                        valid = shape == "square" || shape == "rect" || shape == "circle" ||
                                shape == "triangle" || shape == "pentagon" || shape == "hexagon" || shape == "star";
                        if (valid) item.shape = shape == "rect" ? "square" : shape;
                    } else valid = false;
                }
                const bool exists = std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& other) { return other.name == item.name; });
                if (!valid || !hasX || !hasY || !hasW || !hasH || !hasColor || exists) {
                    reportError(i + 1, "ui create needs a unique name, x/y/w/h, a valid color, and an optional supported shape", item.name);
                    continue;
                }
                uiBlocks.push_back(item);
                variables["last_ui_object"] = item.name;
                types["last_ui_object"] = "word";
                continue;
            }
            if (action == "duplicate" || action == "copy") {
                if (tokens.size() != 4) { reportError(i + 1, "use ui duplicate SOURCE NAME"); continue; }
                const std::string sourceName = valueFor(tokens[2], variables);
                std::string newName = valueFor(tokens[3], variables);
                if (lower(newName) == "auto") newName = uniqueUiName();
                if (!validName(newName)) { reportError(i + 1, "duplicate name must be a valid identifier", newName); continue; }
                const bool nameUsed = std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == newName; }) ||
                                      std::any_of(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == newName; }) ||
                                      std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == newName; }) ||
                                      std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == newName; });
                if (nameUsed) { reportError(i + 1, "ui duplicate name already exists", newName); continue; }
                bool created = false;
                for (const UiBlock& source : uiBlocks) if (source.name == sourceName) {
                    UiBlock copy = source; copy.name = newName; copy.group = currentUiGroup;
                    copy.x = std::clamp(copy.x + 3.0, 0.0, 100.0 - copy.width);
                    copy.y = std::clamp(copy.y + 3.0, 0.0, 100.0 - copy.height);
                    copy.visible = true; uiBlocks.push_back(copy); created = true; break;
                }
                if (!created) for (const UiSprite& source : uiSprites) if (source.name == sourceName) {
                    UiSprite copy = source; copy.name = newName; copy.group = currentUiGroup;
                    copy.x = std::clamp(copy.x + 3.0, 0.0, 100.0 - copy.width);
                    copy.y = std::clamp(copy.y + 3.0, 0.0, 100.0 - copy.height);
                    copy.visible = true; uiSprites.push_back(copy); created = true; break;
                }
                if (!created) for (const UiButton& source : uiButtons) if (source.name == sourceName) {
                    UiButton copy = source; copy.name = newName; copy.group = currentUiGroup;
                    copy.x = std::clamp(copy.x + 3.0, 0.0, 100.0 - copy.width);
                    copy.y = std::clamp(copy.y + 3.0, 0.0, 100.0 - copy.height);
                    copy.visible = true; uiButtons.push_back(copy); created = true; break;
                }
                if (!created) for (const UiText& source : uiTexts) if (source.name == sourceName) {
                    UiText copy = source; copy.name = newName; copy.group = currentUiGroup;
                    copy.x = std::clamp(copy.x + 3.0, 0.0, 100.0 - copy.size);
                    copy.y = std::clamp(copy.y + 3.0, 0.0, 100.0 - copy.size);
                    copy.visible = true; uiTexts.push_back(copy); created = true; break;
                }
                if (!created) reportError(i + 1, "ui duplicate needs an existing UI object", sourceName);
                else {
                    variables["last_ui_object"] = newName;
                    types["last_ui_object"] = "word";
                }
                continue;
            }
            if (action == "delete" && tokens.size() == 3) {
                const std::string name = valueFor(tokens[2], variables);
                const auto oldBlocks = uiBlocks.size(); const auto oldSprites = uiSprites.size();
                const auto oldButtons = uiButtons.size(); const auto oldTexts = uiTexts.size();
                uiBlocks.erase(std::remove_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == name; }), uiBlocks.end());
                uiSprites.erase(std::remove_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == name; }), uiSprites.end());
                uiButtons.erase(std::remove_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == name; }), uiButtons.end());
                uiTexts.erase(std::remove_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == name; }), uiTexts.end());
                if (uiBlocks.size() == oldBlocks && uiSprites.size() == oldSprites && uiButtons.size() == oldButtons && uiTexts.size() == oldTexts)
                    reportError(i + 1, "unknown UI object", name);
                else physicsBodies.erase(name);
                continue;
            }
            if (action == "rename" && tokens.size() == 4) {
                const std::string oldName = valueFor(tokens[2], variables);
                const std::string newName = valueFor(tokens[3], variables);
                if (!validName(newName)) { reportError(i + 1, "new UI name must be a valid identifier", newName); continue; }
                const bool nameUsed = std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == newName && item.name != oldName; }) ||
                                      std::any_of(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == newName && item.name != oldName; }) ||
                                      std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == newName && item.name != oldName; }) ||
                                      std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == newName && item.name != oldName; });
                if (nameUsed) { reportError(i + 1, "UI name already exists", newName); continue; }
                bool renamed = false;
                for (UiBlock& item : uiBlocks) if (item.name == oldName) { item.name = newName; renamed = true; break; }
                if (!renamed) for (UiSprite& item : uiSprites) if (item.name == oldName) { item.name = newName; renamed = true; break; }
                if (!renamed) for (UiButton& item : uiButtons) if (item.name == oldName) { item.name = newName; renamed = true; break; }
                if (!renamed) for (UiText& item : uiTexts) if (item.name == oldName) { item.name = newName; renamed = true; break; }
                if (!renamed) reportError(i + 1, "unknown UI object", oldName);
                else {
                    auto body = physicsBodies.find(oldName);
                    if (body != physicsBodies.end()) { PhysicsBody moved = body->second; physicsBodies.erase(body); moved.name = newName; physicsBodies[newName] = moved; }
                    for (CollisionWatch& watch : collisionWatches) { if (watch.first == oldName) watch.first = newName; if (watch.second == oldName) watch.second = newName; }
                }
                continue;
            }
            const std::string name = tokens.size() > 2 ? valueFor(tokens[2], variables) : "";
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == name; });
            auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == name; });
            if (action == "move" && tokens.size() == 5) {
                double dx = 0, dy = 0;
                if (!evaluateExpression(tokens[3], variables, dx) || !evaluateExpression(tokens[4], variables, dy)) {
                    reportError(i + 1, "use ui move NAME DX DY"); continue;
                }
                if (block != uiBlocks.end()) { block->x += dx; block->y += dy; }
                else if (sprite != uiSprites.end()) { sprite->x += dx; sprite->y += dy; }
                else {
                    auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == name; });
                    auto text = std::find_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == name; });
                    if (button != uiButtons.end()) { button->x += dx; button->y += dy; }
                    else if (text != uiTexts.end()) { text->x += dx; text->y += dy; }
                    else { reportError(i + 1, "unknown movable UI object", name); continue; }
                }
                refreshCollisionWatches(variables, types);
                continue;
            }
            if (action == "resize" && tokens.size() == 5) {
                double dw = 0, dh = 0;
                if (!evaluateExpression(tokens[3], variables, dw) || !evaluateExpression(tokens[4], variables, dh)) {
                    reportError(i + 1, "use ui resize NAME WIDTH_CHANGE HEIGHT_CHANGE"); continue;
                }
                if (block != uiBlocks.end() && block->width + dw > 0 && block->height + dh > 0) { block->width += dw; block->height += dh; }
                else if (sprite != uiSprites.end() && sprite->width + dw > 0 && sprite->height + dh > 0) { sprite->width += dw; sprite->height += dh; }
                else {
                    auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == name; });
                    auto text = std::find_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == name; });
                    if (button != uiButtons.end() && button->width + dw > 0 && button->height + dh > 0) { button->width += dw; button->height += dh; }
                    else if (text != uiTexts.end() && text->size + dw > 0) { text->size += dw; }
                    else { reportError(i + 1, "resize needs a UI object and a positive result", name); continue; }
                }
                refreshCollisionWatches(variables, types);
                continue;
            }
            if (action == "color" && tokens.size() == 4) {
                const std::string color = valueFor(tokens[3], variables);
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(color, red, green, blue)) { reportError(i + 1, "ui color needs #RRGGBB or 0xRRGGBB", color); continue; }
                bool changed = false;
                for (UiBlock& item : uiBlocks) if (item.name == name) { item.color = color; changed = true; }
                for (UiButton& item : uiButtons) if (item.name == name) { item.color = color; changed = true; }
                for (UiText& item : uiTexts) if (item.name == name) { item.color = color; changed = true; }
                if (!changed) reportError(i + 1, "unknown recolorable UI object", name);
                continue;
            }
            if (action == "shape" && tokens.size() == 4) {
                const std::string shape = lower(valueFor(tokens[3], variables));
                if (block == uiBlocks.end() || (shape != "square" && shape != "rect" && shape != "circle" &&
                    shape != "triangle" && shape != "pentagon" && shape != "hexagon" && shape != "star")) {
                    reportError(i + 1, "ui shape needs a block and a supported shape", name); continue;
                }
                block->shape = shape == "rect" ? "square" : shape;
                block->cornerRadius = 0;
                continue;
            }
            reportError(i + 1, "unknown or invalid ui operation", action);
            continue;
        }

        if (cmd == "3d") {
            if (!threeDEnabled) { reportError(i + 1, "3D commands need add (3d)"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "use 3d box|sphere|camera|spawn|plane|texture|move|rotate|scale|color|rename|delete|clear"); continue; }
            const std::string action = lower(tokens[1]);
            const auto unique3DName = [&]() {
                std::string candidate;
                do { candidate = "model_" + std::to_string(threeDAutoNameCounter++); }
                while (std::any_of(ui3DObjects.begin(), ui3DObjects.end(), [&](const Ui3DObject& item) { return item.name == candidate; }));
                return candidate;
            };
            if (action == "box" || action == "sphere" || action == "camera" ||
                action == "spawn" || action == "plane") {
                Ui3DObject object;
                object.group = currentUiGroup;
                object.shape = action;
                object.name = tokens.size() > 2 ? valueFor(tokens[2], variables) : "";
                if (lower(object.name) == "auto") object.name = unique3DName();
                if (action == "camera") object.width = 1.1, object.height = 0.8, object.depth = 1.4;
                else if (action == "spawn") object.width = object.height = object.depth = 1.0;
                else if (action == "plane") object.width = object.depth = 6.0, object.height = 0.15;
                bool valid = validName(object.name);
                bool hasColor = false;
                for (std::size_t part = 3; valid && part + 1 < tokens.size(); part += 2) {
                    const std::string key = lower(tokens[part]);
                    const std::string value = valueFor(tokens[part + 1], variables);
                    double number = 0;
                    if (key == "x") valid = parseNumber(value, object.x);
                    else if (key == "y") valid = parseNumber(value, object.y);
                    else if (key == "z") valid = parseNumber(value, object.z);
                    else if (key == "w") valid = parseNumber(value, object.width) && object.width > 0;
                    else if (key == "h") valid = parseNumber(value, object.height) && object.height > 0;
                    else if (key == "d") valid = parseNumber(value, object.depth) && object.depth > 0;
                    else if (key == "size") {
                        valid = parseNumber(value, number) && number > 0;
                        if (valid) object.width = object.height = object.depth = number;
                    } else if (key == "color") {
                        unsigned red = 0, green = 0, blue = 0;
                        valid = parseColor(value, red, green, blue);
                        if (valid) { object.color = value; hasColor = true; }
                    } else valid = false;
                }
                const bool exists = std::any_of(ui3DObjects.begin(), ui3DObjects.end(), [&](const Ui3DObject& item) { return item.name == object.name; });
                if (!valid || !hasColor || exists) {
                    reportError(i + 1, "use 3d box|sphere|camera|spawn|plane NAME x X y Y z Z ... color COLOR", object.name);
                    continue;
                }
                ui3DObjects.push_back(object);
                variables["last_3d_object"] = object.name;
                types["last_3d_object"] = "word";
                continue;
            }
            if ((action == "duplicate" || action == "copy") && tokens.size() == 4) {
                const std::string sourceName = valueFor(tokens[2], variables);
                std::string newName = valueFor(tokens[3], variables);
                if (lower(newName) == "auto") newName = unique3DName();
                const bool nameUsed = std::any_of(ui3DObjects.begin(), ui3DObjects.end(),
                    [&](const Ui3DObject& item) { return item.name == newName; });
                auto source = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                    [&](const Ui3DObject& item) { return item.name == sourceName; });
                if (source == ui3DObjects.end() || !validName(newName) || nameUsed) {
                    reportError(i + 1, "use 3d duplicate EXISTING_NAME NEW_UNIQUE_NAME", sourceName);
                    continue;
                }
                Ui3DObject copy = *source;
                copy.name = newName;
                copy.group = currentUiGroup;
                copy.x += 1.0;
                copy.z += 1.0;
                copy.visible = true;
                ui3DObjects.push_back(copy);
                variables["last_3d_object"] = newName;
                types["last_3d_object"] = "word";
                continue;
            }
            if (action == "save" && tokens.size() == 3) {
                bool validPath = false;
                const std::filesystem::path scenePath = projectFilePath(valueFor(tokens[2], variables), validPath);
                if (!validPath) { reportError(i + 1, "3d save needs a project-relative .trgscene path"); continue; }
                const std::filesystem::path canonicalRoot = canonicalProjectPath(projectRoot);
                const std::filesystem::path canonicalTarget = canonicalProjectPath(scenePath);
                const std::filesystem::path relativeTarget = canonicalTarget.lexically_relative(canonicalRoot);
                if (relativeTarget.empty() || relativeTarget == ".." || *relativeTarget.begin() == "..") {
                    reportError(i + 1, "3d save cannot write outside the project directory"); continue;
                }
                std::error_code directoryError;
                if (!scenePath.parent_path().empty())
                    std::filesystem::create_directories(scenePath.parent_path(), directoryError);
                if (directoryError) { reportError(i + 1, "could not create the scene directory", directoryError.message()); continue; }
                std::ofstream sceneFile(scenePath, std::ios::binary | std::ios::trunc);
                if (!sceneFile) { reportError(i + 1, "could not write the 3D scene", scenePath.string()); continue; }
                sceneFile << "TRGSCENE 2\n" << ui3DObjects.size() << "\n";
                for (const Ui3DObject& item : ui3DObjects) {
                    sceneFile << std::quoted(item.name) << ' ' << std::quoted(item.group) << ' '
                              << std::quoted(item.shape) << ' ' << std::quoted(item.color) << ' '
                              << std::quoted(item.texture) << ' ' << std::setprecision(17)
                              << item.x << ' ' << item.y << ' ' << item.z << ' '
                              << item.width << ' ' << item.height << ' ' << item.depth << ' '
                              << item.rotateX << ' ' << item.rotateY << ' ' << item.rotateZ << ' '
                              << item.opacity << ' ' << (item.visible ? 1 : 0) << '\n';
                }
                if (!sceneFile) { reportError(i + 1, "failed while writing the 3D scene", scenePath.string()); continue; }
                appendOutput("Saved 3D scene: " + scenePath.string() + "\r\n");
                continue;
            }
            if (action == "load" && tokens.size() == 3) {
                bool validPath = false;
                const std::filesystem::path scenePath = projectExistingFile(valueFor(tokens[2], variables), validPath);
                if (!validPath || !std::filesystem::is_regular_file(scenePath)) {
                    reportError(i + 1, "3d load needs an existing project-relative .trgscene file", tokens[2]); continue;
                }
                std::ifstream sceneFile(scenePath, std::ios::binary);
                std::string magic;
                unsigned version = 0;
                std::size_t objectCount = 0;
                if (!sceneFile || !(sceneFile >> magic >> version >> objectCount) ||
                    magic != "TRGSCENE" || (version != 1 && version != 2) || objectCount > 100000) {
                    reportError(i + 1, "invalid or unsupported 3D scene file", scenePath.string()); continue;
                }
                std::vector<Ui3DObject> loadedObjects;
                loadedObjects.reserve(objectCount);
                std::unordered_set<std::string> loadedNames;
                bool validScene = true;
                for (std::size_t objectIndex = 0; objectIndex < objectCount; ++objectIndex) {
                    Ui3DObject item;
                    int visible = 0;
                    if (!(sceneFile >> std::quoted(item.name) >> std::quoted(item.group)
                          >> std::quoted(item.shape) >> std::quoted(item.color)) ||
                        (version >= 2 && !(sceneFile >> std::quoted(item.texture))) ||
                        !(sceneFile >> item.x >> item.y >> item.z
                          >> item.width >> item.height >> item.depth
                          >> item.rotateX >> item.rotateY >> item.rotateZ
                          >> item.opacity >> visible) ||
                        !validName(item.name) ||
                        (item.shape != "box" && item.shape != "sphere" && item.shape != "camera" &&
                         item.shape != "spawn" && item.shape != "plane") ||
                        (item.texture != "smooth" && item.texture != "grid" && item.texture != "metal" &&
                         item.texture != "wood" && item.texture != "stone") ||
                        item.width <= 0 || item.height <= 0 || item.depth <= 0 ||
                        item.opacity < 0 || item.opacity > 1 ||
                        (visible != 0 && visible != 1) ||
                        !loadedNames.insert(item.name).second) {
                        validScene = false;
                        break;
                    }
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(item.color, red, green, blue)) { validScene = false; break; }
                    item.visible = visible != 0;
                    loadedObjects.push_back(std::move(item));
                }
                std::string trailing;
                if (validScene && (sceneFile >> trailing)) validScene = false;
                if (!validScene || loadedObjects.size() != objectCount) {
                    reportError(i + 1, "3D scene contains invalid or incomplete object data", scenePath.string()); continue;
                }
                ui3DObjects = std::move(loadedObjects);
                if (variables.count("objects") && types["objects"] == "list") {
                    if (auto* names = std::get_if<VariableList>(&variables["objects"].value)) {
                        names->clear();
                        names->reserve(ui3DObjects.size());
                        for (const Ui3DObject& item : ui3DObjects)
                            names->push_back(std::make_shared<VariableValue>(item.name));
                    }
                }
                if (variables.count("objectCount")) {
                    variables["objectCount"] = static_cast<double>(ui3DObjects.size());
                    types["objectCount"] = "num";
                }
                if (variables.count("selectedIndex")) {
                    variables["selectedIndex"] = 0.0;
                    types["selectedIndex"] = "num";
                }
                if (variables.count("selectedName")) {
                    variables["selectedName"] = ui3DObjects.empty() ? std::string() : ui3DObjects.front().name;
                    types["selectedName"] = "word";
                }
                if (!ui3DObjects.empty()) {
                    const Ui3DObject& selected = ui3DObjects.front();
                    variables["currentColor"] = selected.color;
                    variables["posX"] = selected.x;
                    variables["posY"] = selected.y;
                    variables["posZ"] = selected.z;
                    variables["scaleX"] = selected.width / 2.0;
                    variables["scaleY"] = selected.height / 2.0;
                    variables["scaleZ"] = selected.depth / 2.0;
                    types["currentColor"] = "word";
                    types["posX"] = types["posY"] = types["posZ"] = "num";
                    types["scaleX"] = types["scaleY"] = types["scaleZ"] = "num";
                }
                appendOutput("Loaded 3D scene: " + scenePath.string() + "\r\n");
                continue;
            }
            if (action == "clear" && tokens.size() == 2) { ui3DObjects.clear(); continue; }
            const std::string name = tokens.size() > 2 ? valueFor(tokens[2], variables) : "";
            auto object = std::find_if(ui3DObjects.begin(), ui3DObjects.end(), [&](const Ui3DObject& item) { return item.name == name; });
            if (action == "delete" && tokens.size() == 3) {
                if (object == ui3DObjects.end()) reportError(i + 1, "unknown 3D object", name);
                else ui3DObjects.erase(object);
                continue;
            }
            if (action == "rename" && tokens.size() == 4) {
                const std::string newName = valueFor(tokens[3], variables);
                const bool nameUsed = std::any_of(ui3DObjects.begin(), ui3DObjects.end(),
                    [&](const Ui3DObject& item) { return item.name == newName && item.name != name; });
                if (object == ui3DObjects.end() || !validName(newName) || nameUsed) {
                    reportError(i + 1, "use 3d rename EXISTING_NAME NEW_UNIQUE_NAME");
                    continue;
                }
                object->name = newName;
                continue;
            }
            if (object == ui3DObjects.end()) { reportError(i + 1, "unknown 3D object", name); continue; }
            if (action == "move" && tokens.size() == 6) {
                double dx = 0, dy = 0, dz = 0;
                if (!evaluateExpression(tokens[3], variables, dx) ||
                    !evaluateExpression(tokens[4], variables, dy) ||
                    !evaluateExpression(tokens[5], variables, dz)) { reportError(i + 1, "use 3d move NAME DX DY DZ"); continue; }
                object->x += dx; object->y += dy; object->z += dz;
                continue;
            }
            if (action == "rotate" && tokens.size() == 6) {
                double rx = 0, ry = 0, rz = 0;
                if (!evaluateExpression(tokens[3], variables, rx) ||
                    !evaluateExpression(tokens[4], variables, ry) ||
                    !evaluateExpression(tokens[5], variables, rz)) { reportError(i + 1, "use 3d rotate NAME RX RY RZ"); continue; }
                object->rotateX += rx; object->rotateY += ry; object->rotateZ += rz;
                continue;
            }
            if (action == "scale" && (tokens.size() == 4 || tokens.size() == 6)) {
                double scaleX = 1, scaleY = 1, scaleZ = 1;
                if (tokens.size() == 4) {
                    if (!evaluateExpression(tokens[3], variables, scaleX)) {
                        reportError(i + 1, "3d scale needs a numeric factor"); continue;
                    }
                    scaleY = scaleX;
                    scaleZ = scaleX;
                } else if (!evaluateExpression(tokens[3], variables, scaleX) ||
                           !evaluateExpression(tokens[4], variables, scaleY) ||
                           !evaluateExpression(tokens[5], variables, scaleZ)) {
                    reportError(i + 1, "use 3d scale NAME FACTOR or 3d scale NAME X Y Z"); continue;
                }
                if (scaleX <= 0 || scaleX > 10 || scaleY <= 0 || scaleY > 10 ||
                    scaleZ <= 0 || scaleZ > 10) {
                    reportError(i + 1, "3d scale factors must be greater than 0 and at most 10"); continue;
                }
                object->width *= scaleX;
                object->height *= scaleY;
                object->depth *= scaleZ;
                continue;
            }
            if (action == "color" && tokens.size() == 4) {
                const std::string color = valueFor(tokens[3], variables);
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(color, red, green, blue)) { reportError(i + 1, "3d color needs a valid color", color); continue; }
                object->color = color;
                continue;
            }
            if (action == "texture" && tokens.size() == 4) {
                const std::string texture = lower(valueFor(tokens[3], variables));
                static const std::unordered_set<std::string> textures = {"smooth","grid","metal","wood","stone"};
                if (!textures.count(texture)) {
                    reportError(i + 1, "3d texture must be smooth, grid, metal, wood, or stone", texture);
                    continue;
                }
                object->texture = texture;
                continue;
            }
            reportError(i + 1, "unknown or invalid 3D operation", action);
            continue;
        }

        if (cmd == "style") {
            if (!styleEnabled) { reportError(i + 1, "style needs add (style)"); continue; }
            if (tokens.size() >= 4 && lower(tokens[1]) == "apply") {
                const std::string targetName = tokens[2];
                const std::string styleName = lower(tokens[3]);
                const auto styleIt = uiStyles.find(styleName);
                if (styleIt == uiStyles.end()) { reportError(i + 1, "unknown style", styleName); continue; }
                const UiStyle& style = styleIt->second;
                bool targetFound = false;
                for (UiBlock& item : uiBlocks) if (item.name == targetName) {
                    if (style.hasBackground) item.color = style.background;
                    if (style.hasGlow) { item.glowColor = style.glow; item.glowIntensity = style.glowIntensity; }
                    if (style.hasRadius) item.cornerRadius = style.radius;
                    if (style.hasOpacity) item.opacity = style.opacity;
                    targetFound = true; break;
                }
                if (!targetFound) for (UiButton& item : uiButtons) if (item.name == targetName) {
                    if (style.hasBackground) item.color = style.background;
                    if (style.hasText) item.textColor = style.text;
                    if (style.hasRadius) item.cornerRadius = style.radius;
                    if (style.hasOpacity) item.opacity = style.opacity;
                    if (style.hasFont) item.font = style.font;
                    if (style.hasSize) item.textSize = std::clamp(style.size, 0.05, 1.0);
                    if (style.hasBold) item.textBold = style.bold;
                    if (style.hasGlow) reportError(i + 1, "glow styles currently apply to blocks, not buttons", targetName);
                    targetFound = true; break;
                }
                if (!targetFound) for (UiText& item : uiTexts) if (item.name == targetName) {
                    if (style.hasText) item.color = style.text;
                    if (style.hasOpacity) item.opacity = style.opacity;
                    if (style.hasFont) item.font = style.font;
                    if (style.hasSize) item.size = style.size;
                    if (style.hasBold) item.bold = style.bold;
                    if (style.hasAlign) item.align = style.align;
                    if (style.hasBackground || style.hasGlow || style.hasRadius)
                        reportError(i + 1, "background, glow, and radius styles apply to blocks or buttons", targetName);
                    targetFound = true; break;
                }
                if (!targetFound) reportError(i + 1, "unknown UI object for style", targetName);
                continue;
            }
            if (tokens.size() < 3 || lower(tokens[1]) != "name" || !validName(tokens[2])) {
                reportError(i + 1, "use style name NAME ( ... ) or style apply OBJECT NAME");
                continue;
            }
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close, blockIndex)) {
                reportError(i + 1, "style definition needs a ( ... ) block", tokens[2]);
                continue;
            }
            UiStyle style;
            bool validStyle = true;
            for (std::size_t row = open + 1; row < close; ++row) {
                const auto parts = tokenize(trim(program[row]));
                if (parts.empty()) continue;
                const std::string property = lower(parts[0]);
                auto getValue = [&](std::size_t index) -> std::string {
                    return index < parts.size() ? valueFor(parts[index], variables) : std::string();
                };
                double number = 0;
                unsigned red = 0, green = 0, blue = 0;
                if (property == "background" && parts.size() == 2) {
                    style.background = getValue(1);
                    validStyle = parseColor(style.background, red, green, blue);
                    style.hasBackground = validStyle;
                } else if (property == "text" && parts.size() == 2) {
                    style.text = getValue(1);
                    validStyle = parseColor(style.text, red, green, blue);
                    style.hasText = validStyle;
                } else if (property == "glow" && (parts.size() == 2 || parts.size() == 3)) {
                    style.glow = getValue(1);
                    validStyle = parseColor(style.glow, red, green, blue);
                    if (validStyle && parts.size() == 3)
                        validStyle = parseNumber(getValue(2), style.glowIntensity) && style.glowIntensity >= 0 && style.glowIntensity <= 5;
                    style.hasGlow = validStyle;
                } else if (property == "radius" && parts.size() == 2) {
                    validStyle = parseNumber(getValue(1), style.radius) && style.radius >= 0;
                    style.hasRadius = validStyle;
                } else if (property == "opacity" && parts.size() == 2) {
                    validStyle = parseNumber(getValue(1), style.opacity) && style.opacity >= 0 && style.opacity <= 1;
                    style.hasOpacity = validStyle;
                } else if (property == "font" && parts.size() == 2) {
                    style.font = lower(getValue(1));
                    if (style.font == "monospace") style.font = "mono";
                    validStyle = style.font == "sans" || style.font == "serif" || style.font == "mono";
                    style.hasFont = validStyle;
                } else if (property == "size" && parts.size() == 2) {
                    validStyle = parseNumber(getValue(1), style.size) && style.size > 0;
                    style.hasSize = validStyle;
                } else if (property == "bold" && parts.size() == 2) {
                    const std::string flag = lower(getValue(1));
                    validStyle = flag == "true" || flag == "false" || flag == "1" || flag == "0";
                    style.bold = flag == "true" || flag == "1";
                    style.hasBold = validStyle;
                } else if (property == "align" && parts.size() == 2) {
                    style.align = lower(getValue(1));
                    validStyle = style.align == "left" || style.align == "center" || style.align == "right";
                    style.hasAlign = validStyle;
                } else validStyle = false;
                if (!validStyle) {
                    reportError(row + 1, "invalid style property or value", property);
                    break;
                }
            }
            if (validStyle) uiStyles[lower(tokens[2])] = style;
            i = close;
            continue;
        }

        if (cmd == "text") {
            if (!uiMode || tokens.size() < 10) {
                reportError(i + 1, "use text NAME VALUE x X y Y size SIZE color COLOR [font sans|serif|mono] [align left|center|right]");
                continue;
            }
            UiText text;
            text.group = currentUiGroup;
            std::size_t firstProperty = 2;
            const bool named = tokens.size() > 10 && validName(tokens[1]) && lower(tokens[2]) != "x";
            if (named) {
                text.name = tokens[1];
                text.value = valueFor(tokens[2], variables);
                firstProperty = 3;
            } else {
                text.name = "text_" + std::to_string(uiTexts.size());
                text.value = valueFor(tokens[1], variables);
                firstProperty = 2;
            }
            unsigned red = 0, green = 0, blue = 0;
            bool validText = true;
            for (std::size_t part = firstProperty; part + 1 < tokens.size(); part += 2) {
                const std::string key = lower(tokens[part]);
                const std::string value = valueFor(tokens[part + 1], variables);
                if (key == "x") validText = parseNumber(value, text.x);
                else if (key == "y") validText = parseNumber(value, text.y);
                else if (key == "size") validText = parseNumber(value, text.size) && text.size > 0;
                else if (key == "color") { text.color = value; validText = parseColor(value, red, green, blue); }
                else if (key == "font") {
                    text.font = lower(value);
                    if (text.font == "monospace") text.font = "mono";
                    validText = text.font == "sans" || text.font == "serif" || text.font == "mono";
                } else if (key == "align") {
                    text.align = lower(value);
                    validText = text.align == "left" || text.align == "center" || text.align == "right";
                } else if (key == "bold") text.bold = lower(value) == "true" || value == "1";
                else if (key == "opacity") validText = parseNumber(value, text.opacity) && text.opacity >= 0 && text.opacity <= 1;
                else validText = false;
                if (!validText) break;
            }
            if (!validText || text.color.empty() || text.size <= 0) {
                reportError(i + 1, "invalid text position, size, color, font, alignment, or opacity", text.name);
                continue;
            }
            if (std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == text.name; })) {
                reportError(i + 1, "duplicate text name", text.name);
                continue;
            }
            uiTexts.push_back(std::move(text));
            continue;
        }
        if (cmd == "button") {
            if (!uiMode || tokens.size() < 3) {
                reportError(i + 1, "use button NAME LABEL x X y Y w WIDTH h HEIGHT ( ... ) inside add (ui)");
                continue;
            }
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close, blockIndex)) {
                reportError(i + 1, "button needs a code block", tokens[1]);
                continue;
            }
            UiButton button;
            button.name = tokens[1];
            button.label = valueFor(tokens[2], variables);
            button.group = currentUiGroup;
            bool validButton = validName(button.name);
            for (std::size_t a = 3; validButton && a + 1 < tokens.size(); a += 2) {
                const std::string key = lower(tokens[a]);
                double number = 0;
                const std::string property = valueFor(tokens[a + 1], variables);
                if (key == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(property, red, green, blue)) { reportError(i + 1, "button color must be #RRGGBB or 0xRRGGBB", property); validButton = false; }
                    else button.color = property;
                } else if (key == "text_color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(property, red, green, blue)) { reportError(i + 1, "button text_color must be a color", property); validButton = false; }
                    else button.textColor = property;
                } else if (key == "font") {
                    button.font = lower(property);
                    if (button.font == "monospace") button.font = "mono";
                    if (button.font != "sans" && button.font != "serif" && button.font != "mono") validButton = false;
                } else if (key == "bold") {
                    button.textBold = lower(property) == "true" || property == "1";
                } else if (key == "text_size") {
                    if (!parseNumber(property, number) || number <= 0 || number > 1) validButton = false;
                    else button.textSize = number;
                } else if (key == "opacity") {
                    if (!parseNumber(property, number) || number < 0 || number > 1) validButton = false;
                    else button.opacity = number;
                } else if (key == "edges" || key == "shape") {
                    const std::string edge = lower(property);
                    if (edge == "round" || edge == "rounded") button.cornerRadius = 7;
                    else if (edge == "square") button.cornerRadius = 0;
                    else { reportError(i + 1, "button edges must be round or square", property); validButton = false; }
                } else if (key == "radius") {
                    if (!parseNumber(property, number) || number < 0) { reportError(i + 1, "button radius must be zero or more", property); validButton = false; }
                    else button.cornerRadius = number;
                } else if ((key == "x" || key == "y" || key == "w" || key == "h" ||
                            key == "width" || key == "height") && lower(property) == "auto") {
                    // Missing and auto properties use the button's intrinsic defaults.
                } else if (!parseNumber(property, number)) {
                    reportError(i + 1, "invalid button coordinate", tokens[a + 1]);
                    validButton = false;
                } else if (key == "x") button.x = number;
                else if (key == "y") button.y = number;
                else if (key == "w" || key == "width") button.width = number;
                else if (key == "h" || key == "height") button.height = number;
                else if (key == "opacity") button.opacity = number;
                else {
                    reportError(i + 1, "unknown button property", key);
                    validButton = false;
                }
            }
            if (!validButton || button.width <= 0 || button.height <= 0 || button.opacity < 0 || button.opacity > 1) {
                if (validButton && (button.width <= 0 || button.height <= 0)) reportError(i + 1, "button size must be positive", button.name);
                else if (validButton) reportError(i + 1, "button opacity must be between 0 and 1", button.name);
                i = close;
                continue;
            }
            if (std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& existing) { return existing.name == button.name; })) {
                reportError(i + 1, "duplicate button name", button.name);
                i = close;
                continue;
            }
            button.body.assign(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                               program.begin() + static_cast<std::ptrdiff_t>(close));
            button.blockIndex = std::make_shared<ProgramBlockIndex>(button.body);
            uiButtons.push_back(std::move(button));
            i = close;
            continue;
        }
        if (cmd == "block") {
            if (!uiMode) { reportError(i + 1, "block needs add (ui)"); continue; }
            UiBlock block;
            block.group = currentUiGroup;
            for (std::size_t a = 1; a + 1 < tokens.size(); a += 2) {
                const std::string key = lower(tokens[a]);
                const std::string value = valueFor(tokens[a + 1], variables);
                double number = 0;
                if (key == "name") block.name = tokens[a + 1];
                else if (key == "color") block.color = value;
                else if (key == "opacity") {
                    if (!parseNumber(value, number) || number < 0 || number > 1) { reportError(i + 1, "block opacity must be between 0 and 1", value); block.name.clear(); break; }
                    block.opacity = number;
                }
                else if (key == "edges" || key == "shape") {
                    const std::string edge = lower(value);
                    if (key == "shape") {
                        if (edge == "square" || edge == "rect") block.shape = "square";
                        else if (edge == "circle" || edge == "round") block.shape = "circle";
                        else if (edge == "triangle") block.shape = "triangle";
                        else if (edge == "pentagon") block.shape = "pentagon";
                        else if (edge == "hexagon") block.shape = "hexagon";
                        else if (edge == "star") block.shape = "star";
                        else { reportError(i + 1, "block shape must be square, circle, triangle, pentagon, hexagon, or star", value); block.name.clear(); break; }
                        block.cornerRadius = 0;
                    } else {
                        if (edge == "round" || edge == "rounded") block.cornerRadius = 10;
                        else if (edge == "square") block.cornerRadius = 0;
                        else { reportError(i + 1, "block edges must be round or square", value); block.name.clear(); break; }
                    }
                } else if (key == "glow") {
                    // glow COLOR INTENSITY
                    block.glowColor = value;
                    if (a + 3 < tokens.size()) {
                        if (!parseNumber(tokens[a + 3], number)) { reportError(i + 1, "glow needs intensity number", tokens[a + 3]); block.name.clear(); break; }
                        block.glowIntensity = number;
                        a += 2; // skip intensity value
                    } else {
                        block.glowIntensity = 0.5;
                    }
                } else if (key == "radius") {
                    if (!parseNumber(value, number) || number < 0) { reportError(i + 1, "block radius must be zero or more", value); block.name.clear(); break; }
                    block.cornerRadius = number;
                } else if (key == "z" || key == "x" || key == "y" ||
                           key == "h" || key == "w" || key == "height" || key == "width") {
                    if (lower(value) == "auto" && key != "z") continue;
                    if (!parseNumber(value, number)) { reportError(i + 1, "invalid block coordinate", value); block.name.clear(); break; }
                    if (key == "z") block.z = static_cast<int>(number);
                    else if (key == "x") block.x = number;
                    else if (key == "y") block.y = number;
                    else if (key == "h" || key == "height") block.height = number;
                    else block.width = number;
                } else { reportError(i + 1, "unknown block property", key); block.name.clear(); break; }
            }
            unsigned red = 0, green = 0, blue = 0;
            if (block.name.empty() || !parseColor(block.color, red, green, blue)) {
                if (!block.name.empty()) reportError(i + 1, "block color must be a color variable or #RRGGBB value", block.color);
                continue;
            }
            const auto existing = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == block.name; });
            if (existing != uiBlocks.end()) { reportError(i + 1, "duplicate block name", block.name); continue; }
            uiBlocks.push_back(block);
            continue;
        }
        if (cmd == "image" || cmd == "sprite") {
            if (!uiMode || tokens.size() < 3 || !validName(tokens[1]) ||
                (tokens.size() - 3) % 2 != 0) {
                reportError(i + 1, "use image NAME FILE [x X] [y Y] [w WIDTH|auto] [h HEIGHT|auto] [opacity 0..1] inside add (ui)");
                continue;
            }
            UiSprite sprite;
            sprite.name = tokens[1];
            sprite.group = currentUiGroup;
            sprite.path = valueFor(tokens[2], variables);
            bool valid = true;
            for (std::size_t a = 3; a + 1 < tokens.size(); a += 2) {
                const std::string key = lower(tokens[a]);
                const std::string property = valueFor(tokens[a + 1], variables);
                const std::string setting = lower(property);
                double number = 0;
                if (key == "x" || key == "y") {
                    if (setting == "auto") number = 0;
                    else if (!parseNumber(property, number)) valid = false;
                    if (key == "x") sprite.x = number;
                    else sprite.y = number;
                } else if (key == "w" || key == "width") {
                    if (setting == "auto") sprite.widthAuto = true;
                    else if (!parseNumber(property, number) || number <= 0) valid = false;
                    else { sprite.width = number; sprite.widthAuto = false; }
                } else if (key == "h" || key == "height") {
                    if (setting == "auto") sprite.heightAuto = true;
                    else if (!parseNumber(property, number) || number <= 0) valid = false;
                    else { sprite.height = number; sprite.heightAuto = false; }
                } else if (key == "opacity") {
                    if (!parseNumber(property, number) || number < 0 || number > 1) valid = false;
                    else sprite.opacity = number;
                } else {
                    valid = false;
                }
                if (!valid) break;
            }
            bool pathValid = false;
            const auto imagePath = projectFilePath(sprite.path, pathValid);
            if (!valid || !pathValid || !std::filesystem::is_regular_file(imagePath)) {
                reportError(i + 1, "image needs a project file and valid optional x/y/w/h/opacity properties", sprite.path);
                continue;
            }
            sprite.path = imagePath.u8string();
            uiSprites.push_back(std::move(sprite));
            continue;
        }
        if (cmd == "animate") {
            if (!uiMode || tokens.size() != 10) { reportError(i + 1, "use animate NAME x DX y DY fps RATE time SECONDS"); continue; }
            const std::string targetName = valueFor(tokens[1], variables);
            double startX = 0, startY = 0;
            bool targetFound = false;
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == targetName; });
            if (block != uiBlocks.end()) { startX = block->x; startY = block->y; targetFound = true; }
            if (!targetFound) {
                auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == targetName; });
                if (sprite != uiSprites.end()) { startX = sprite->x; startY = sprite->y; targetFound = true; }
            }
            if (!targetFound) {
                auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == targetName; });
                if (button != uiButtons.end()) { startX = button->x; startY = button->y; targetFound = true; }
            }
            if (!targetFound) {
                auto text = std::find_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == targetName; });
                if (text != uiTexts.end()) { startX = text->x; startY = text->y; targetFound = true; }
            }
            if (!targetFound) { reportError(i + 1, "unknown UI object to animate", targetName); continue; }
            double dx = 0, dy = 0, fps = 0, duration = 0;
            if (lower(tokens[2]) != "x" || lower(tokens[4]) != "y" ||
                lower(tokens[6]) != "fps" || lower(tokens[8]) != "time" ||
                !parseNumber(valueFor(tokens[3], variables), dx) || !parseNumber(valueFor(tokens[5], variables), dy) ||
                !parseNumber(valueFor(tokens[7], variables), fps)) {
                reportError(i + 1, "invalid animation");
                continue;
            }
            std::string durationText = valueFor(tokens[9], variables);
            if (!durationText.empty() && (durationText.back() == 's' || durationText.back() == 'S')) durationText.pop_back();
            if (!parseNumber(durationText, duration) || duration <= 0 || fps <= 0) {
                reportError(i + 1, "animation time and fps must be positive");
                continue;
            }
            UiAnimation animation;
            animation.name = targetName;
            animation.startTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
            animation.startX = startX;
            animation.startY = startY;
            for (const UiAnimation& previous : uiAnimations) {
                if (previous.name == targetName) {
                    animation.startTime = std::max(animation.startTime, previous.startTime + previous.duration);
                    animation.startX = previous.startX + previous.dx;
                    animation.startY = previous.startY + previous.dy;
                }
            }
            animation.dx = dx;
            animation.dy = dy;
            animation.duration = duration;
            animation.fps = static_cast<int>(std::min(fps, 1000.0));
            uiAnimations.push_back(animation);
            continue;
        }

        
        const std::size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string name = trim(line.substr(0, eqPos));
            std::string rawValue = trim(line.substr(eqPos + 1));

            // Support both assignment styles:
            //   variable = expression
            //   expression = variable
            // If the right side is an existing variable and the left side is
            // not an existing variable path, treat the right side as the target.
            if ((!validVariablePath(name) || !variables.count(name)) &&
                validVariablePath(rawValue) && variables.count(rawValue)) {
                const std::string expression = name;
                name = rawValue;
                rawValue = expression;
            }

            if (validVariablePath(name) && variables.count(name)) {
                const std::string type = types.count(name) ? types[name] : "word";
                const std::string value = valueFor(rawValue, variables);
                if (type == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(rawValue, variables, numeric)) { reportError(i + 1, "invalid number", rawValue); continue; }
                    variables[name] = numeric;
                } else if (type == "toggle") {
                    if (lower(value) != "true" && lower(value) != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                    variables[name] = lower(value) == "true";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                    variables[name] = value;
                } else {
                    const std::vector<std::string> valueTokens = tokenize(rawValue);
                    if (valueTokens.empty()) variables[name].clear();
                    else if (valueTokens.size() == 1) variables[name] = valueFor(valueTokens[0], variables);
                    else variables[name] = rawValue;
                }
                continue;
            }
        }

        
        reportError(i + 1, "unknown command", cmd);
    }
}

void applyUiAnimationPosition(const std::string& name, double& x, double& y) {
    const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
    for (const UiAnimation& animation : uiAnimations) {
        if (animation.name != name) continue;
        if (elapsed < animation.startTime) break;
        const double progress = std::min(1.0, (elapsed - animation.startTime) / animation.duration);
        x = animation.startX + animation.dx * progress;
        y = animation.startY + animation.dy * progress;
    }
}

bool uiEntityBounds(const std::string& name, double& x, double& y, double& width, double& height) {
    for (const UiBlock& block : uiBlocks) {
        if (block.name != name || !block.visible || !uiGroupIsVisible(block.group)) continue;
        x = block.x; y = block.y; width = block.width; height = block.height;
        applyUiAnimationPosition(name, x, y);
        return true;
    }
    for (const UiSprite& sprite : uiSprites) {
        if (sprite.name == name && sprite.visible && uiGroupIsVisible(sprite.group)) {
            x = sprite.x; y = sprite.y; width = sprite.width; height = sprite.height;
            applyUiAnimationPosition(name, x, y); return true;
        }
    }
    for (const UiButton& button : uiButtons) {
        if (button.name == name && button.visible && uiGroupIsVisible(button.group)) {
            x = button.x; y = button.y; width = button.width; height = button.height;
            applyUiAnimationPosition(name, x, y); return true;
        }
    }
    for (const UiText& text : uiTexts) {
        if (text.name == name && text.visible && uiGroupIsVisible(text.group)) {
            x = text.x; y = text.y; width = text.size; height = text.size;
            applyUiAnimationPosition(name, x, y); return true;
        }
    }
    return false;
}

void refreshCollisionWatches(Vars& variables, Types& currentTypes) {
    for (const CollisionWatch& watch : collisionWatches) {
        double ax = 0, ay = 0, aw = 0, ah = 0, bx = 0, by = 0, bw = 0, bh = 0;
        const bool hasA = uiEntityBounds(watch.first, ax, ay, aw, ah);
        const bool hasB = uiEntityBounds(watch.second, bx, by, bw, bh);
        const bool overlap = hasA && hasB && ax < bx + bw && ax + aw > bx &&
            ay < by + bh && ay + ah > by;
        variables[watch.result] = overlap;
        if (!currentTypes.count(watch.result)) currentTypes[watch.result] = "toggle";
    }
}

bool setUiEntityPosition(const std::string& name, double x, double y) {
    for (UiBlock& item : uiBlocks) if (item.name == name) { item.x = x; item.y = y; return true; }
    for (UiSprite& item : uiSprites) if (item.name == name) { item.x = x; item.y = y; return true; }
    for (UiButton& item : uiButtons) if (item.name == name) { item.x = x; item.y = y; return true; }
    return false;
}

void physicsStep(double seconds, Vars& variables, Types& currentTypes) {
#ifdef __ANDROID__
    if (controlsEnabled) {
        std::lock_guard<std::mutex> lock(androidInputMutex);
        for (const auto& control : androidMobileInputs) variables[control.first] = control.second;
    }
#endif
    const double dt = std::clamp(seconds, 0.0, 0.1);
    if (dt <= 0) return;
    std::unordered_set<std::string> contactPairs;
    std::vector<std::string> names;
    names.reserve(physicsBodies.size());
    for (const auto& entry : physicsBodies) if (entry.second.dynamic) names.push_back(entry.first);
    for (const std::string& name : names) {
        auto found = physicsBodies.find(name);
        if (found == physicsBodies.end()) continue;
        PhysicsBody& body = found->second;
        double x = 0, y = 0, width = 0, height = 0;
        if (!uiEntityBounds(name, x, y, width, height)) continue;
        if (body.dragLaunch && (!body.launched || body.midAirLaunch)) {
            double pointerX = 0, pointerY = 0;
            const std::string pointerDown = valueFor("pointer_down", variables);
            const bool isDown = lower(pointerDown) == "press" || lower(pointerDown) == "true";
            if (isDown && !body.dragLaunching &&
                evaluateExpression("pointer_x", variables, pointerX) &&
                evaluateExpression("pointer_y", variables, pointerY) &&
                pointerX >= x && pointerX <= x + width && pointerY >= y && pointerY <= y + height) {
                body.dragLaunching = true;
                body.dragStartX = pointerX;
                body.dragStartY = pointerY;
            } else if (!isDown && body.dragLaunching) {
                if (evaluateExpression("pointer_x", variables, pointerX) &&
                    evaluateExpression("pointer_y", variables, pointerY)) {
                    double launchX = pointerX - body.dragStartX;
                    double launchY = pointerY - body.dragStartY;
                    const double magnitude = std::hypot(launchX, launchY);
                    if (magnitude > 0.5) {
                        const double speed = std::min(body.launchSpeedLimit, magnitude * 4.0 * body.launchPowerMultiplier);
                        body.vx = launchX / magnitude * speed;
                        body.vy = launchY / magnitude * speed;
                        body.launched = true;
                    }
                }
                body.dragLaunching = false;
            }
        }
        if (body.frozen || (body.dragLaunch && !body.launched)) continue;

        if (body.gravityDirection == "top") body.vy -= body.gravity * dt;
        else if (body.gravityDirection == "bottom") body.vy += body.gravity * dt;
        else if (body.gravityDirection == "left") body.vx -= body.gravity * dt;
        else if (body.gravityDirection == "right") body.vx += body.gravity * dt;
        if (body.drag > 0) {
            const double damping = std::exp(-body.drag * dt);
            body.vx *= damping;
            body.vy *= damping;
        }

        const auto overlapsSolid = [&](double testX, double testY, std::string& hitName) {
            for (const auto& otherEntry : physicsBodies) {
                const PhysicsBody& other = otherEntry.second;
                if (other.name == name || !other.solid || other.trigger) continue;
                double ox = 0, oy = 0, ow = 0, oh = 0;
                if (!uiEntityBounds(other.name, ox, oy, ow, oh)) continue;
                if (testX < ox + ow && testX + width > ox &&
                    testY < oy + oh && testY + height > oy) {
                    hitName = other.name;
                    return true;
                }
            }
            return false;
        };

        const double nextX = x + body.vx * body.speedMultiplier * dt;
        std::string hitX;
        if (setUiEntityPosition(name, nextX, y) && overlapsSolid(nextX, y, hitX)) {
            contactPairs.insert(name + "\n" + hitX);
            setUiEntityPosition(name, x, y);
            body.vx = -body.vx * body.restitution;
            ++body.bounceCount;
            variables["bb_bounces"] = static_cast<double>(body.bounceCount);
        }

        double currentX = 0, currentY = 0, currentW = 0, currentH = 0;
        if (!uiEntityBounds(name, currentX, currentY, currentW, currentH)) continue;
        const double nextY = currentY + body.vy * body.speedMultiplier * dt;
        std::string hitY;
        if (setUiEntityPosition(name, currentX, nextY) && overlapsSolid(currentX, nextY, hitY)) {
            contactPairs.insert(name + "\n" + hitY);
            setUiEntityPosition(name, currentX, currentY);
            body.vy = -body.vy * body.restitution;
            ++body.bounceCount;
            variables["bb_bounces"] = static_cast<double>(body.bounceCount);
        }
    }
    double ballX = 0, ballY = 0, ballW = 0, ballH = 0;
    if (uiEntityBounds("ball", ballX, ballY, ballW, ballH)) {
        for (auto& entry : physicsBodies) {
            PhysicsBody& triggerBody = entry.second;
            if (!triggerBody.trigger) continue;
            double tx = 0, ty = 0, tw = 0, th = 0;
            if (!uiEntityBounds(triggerBody.name, tx, ty, tw, th)) continue;
            if (ballX < tx + tw && ballX + ballW > tx && ballY < ty + th && ballY + ballH > ty) {
                if (triggerBody.triggerKind == "finish-line") {
                    variables["bb_finish_hit"] = true; currentTypes["bb_finish_hit"] = "toggle";
                    if (physicsBodies.count("ball")) physicsBodies["ball"].frozen = true;
                } else if (triggerBody.triggerKind == "death-brick") {
                    auto ballBody = physicsBodies.find("ball");
                    if (ballBody != physicsBodies.end() && !ballBody->second.immortal) {
                        if (ballBody->second.lives > 1) {
                            --ballBody->second.lives;
                            setUiEntityPosition("ball", ballBody->second.spawnX, ballBody->second.spawnY);
                            ballBody->second.vx = ballBody->second.vy = 0;
                            ballBody->second.frozen = false;
                            ballBody->second.launched = false;
                            variables["bb_lives"] = static_cast<double>(ballBody->second.lives);
                            currentTypes["bb_lives"] = "num";
                        } else {
                            variables["bb_death_hit"] = true; currentTypes["bb_death_hit"] = "toggle";
                            ballBody->second.frozen = true;
                        }
                    }
                } else if (triggerBody.triggerKind == "trigger-block" &&
                           (!triggerBody.triggerHasActivated || triggerBody.triggerRepeats)) {
                    auto ballBody = physicsBodies.find("ball");
                    if (ballBody != physicsBodies.end()) {
                        ballBody->second.gravityDirection = triggerBody.gravityDirection;
                        ballBody->second.gravity = triggerBody.gravity;
                        ballBody->second.speedMultiplier = triggerBody.speedMultiplier;
                        ballBody->second.launchPowerMultiplier = triggerBody.launchPowerMultiplier;
                        ballBody->second.drag = triggerBody.drag;
                        ballBody->second.restitution = triggerBody.restitution;
                        ballBody->second.midAirLaunch = triggerBody.midAirLaunch;
                        ballBody->second.lives = triggerBody.lives;
                        triggerBody.triggerHasActivated = true;
                        if (triggerBody.ballSizeMultiplier > 0) {
                            double bx=0, by=0, bw=0, bh=0;
                            if (uiEntityBounds("ball", bx, by, bw, bh)) {
                                const double cx=bx+bw/2, cy=by+bh/2;
                                for (UiBlock& ballBlock : uiBlocks) if (ballBlock.name=="ball") {
                                    ballBlock.width=5*triggerBody.ballSizeMultiplier;
                                    ballBlock.height=5*triggerBody.ballSizeMultiplier;
                                    ballBlock.x=cx-ballBlock.width/2; ballBlock.y=cy-ballBlock.height/2;
                                }
                            }
                        }
                    }
                } else if (triggerBody.triggerKind == "freeze-pad") {
                    variables["bb_freeze_hit"] = true; currentTypes["bb_freeze_hit"] = "toggle";
                    if (physicsBodies.count("ball")) physicsBodies["ball"].frozen = true;
                }
            }
        }
    }
    refreshCollisionWatches(variables, currentTypes);
    for (const CollisionWatch& watch : collisionWatches) {
        if (contactPairs.count(watch.first + "\n" + watch.second) ||
            contactPairs.count(watch.second + "\n" + watch.first)) {
            variables[watch.result] = true;
            if (!currentTypes.count(watch.result)) currentTypes[watch.result] = "toggle";
        }
    }
}

#ifdef _WIN32
std::pair<double, double> animatedPosition(const UiBlock& block, double elapsed) {
    double x = block.x;
    double y = block.y;
    for (const UiAnimation& animation : uiAnimations) {
        if (animation.name != block.name) continue;
        if (elapsed < animation.startTime) break;
        const double progress = std::min(1.0, (elapsed - animation.startTime) / animation.duration);
        x = animation.startX + animation.dx * progress;
        y = animation.startY + animation.dy * progress;
    }
    return {x, y};
}

struct DesktopLayout {
    LONG width;
    LONG height;
    LONG headerHeight;
    LONG panelTop;
    LONG canvasHeight;
    LONG margin;
    LONG outputTop;
    LONG outputBottom;
};

DesktopLayout desktopLayout(HWND window, const RECT& client) {
    const LONG width = std::max<LONG>(0, client.right - client.left);
    const LONG height = std::max<LONG>(0, client.bottom - client.top);
    const LONG margin = scaleForDpi(window, 20);
    if (standaloneAppMode) {
        return {width, height, 0, height, std::max<LONG>(1, height), 0, height, height};
    }
    if (previewMode) {
        const LONG outputHeight = std::min<LONG>(
            std::max<LONG>(scaleForDpi(window, 132), height / 5),
            std::max<LONG>(scaleForDpi(window, 110), height / 3));
        const LONG panelTop = std::max<LONG>(scaleForDpi(window, 220), height - outputHeight);
        const LONG studioHeader = desktop3DEngineActive ? scaleForDpi(window, 82) : 0;
        return {width, height, studioHeader, panelTop,
            std::max<LONG>(1, panelTop - studioHeader),
            margin, panelTop, std::max<LONG>(panelTop, height - margin)};
    }
    const LONG headerHeight = scaleForDpi(window, 84);
    const LONG outputMinimum = scaleForDpi(window, 112);
    const LONG canvasMinimum = scaleForDpi(window, 100);
    const LONG panelMinimum = std::min(height, headerHeight + canvasMinimum);
    const LONG panelMaximum = std::max(panelMinimum, height - outputMinimum);
    const LONG panelTop = std::clamp<LONG>(height * 65 / 100, panelMinimum, panelMaximum);
    return {width, height, headerHeight, panelTop,
        std::max<LONG>(1, panelTop - headerHeight), margin, panelTop,
        std::max<LONG>(panelTop, height - margin)};
}


void setDesktop3DLabel(const std::string& name, const std::string& value) {
    for (UiText& label : uiTexts) if (label.name == name) label.value = value;
}

void syncDesktop3DEditorState() {
    if (!desktop3DEngineActive) return;
    VariableList names;
    names.reserve(ui3DObjects.size());
    for (const Ui3DObject& model : ui3DObjects)
        names.push_back(std::make_shared<VariableValue>(model.name));
    uiVariables["objects"] = std::move(names);
    uiTypes["objects"] = "list";
    uiVariables["objectCount"] = static_cast<double>(ui3DObjects.size());
    uiTypes["objectCount"] = "num";
    auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
        [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
    if (selected == ui3DObjects.end() && !ui3DObjects.empty()) {
        selected = ui3DObjects.begin();
        desktop3DSelectedName = selected->name;
    }
    desktop3DHierarchyScroll = std::clamp(desktop3DHierarchyScroll, 0,
        std::max(0, static_cast<int>(ui3DObjects.size()) - 11));
    const std::size_t index = selected == ui3DObjects.end() ? 0 :
        static_cast<std::size_t>(std::distance(ui3DObjects.begin(), selected));
    uiVariables["selectedIndex"] = static_cast<double>(index);
    uiTypes["selectedIndex"] = "num";
    uiVariables["selectedName"] = selected == ui3DObjects.end() ? std::string() : selected->name;
    uiTypes["selectedName"] = "word";
    setDesktop3DLabel("object_count", std::to_string(ui3DObjects.size()) + " OBJECTS");
    setDesktop3DLabel("hierarchy_selected",
        selected == ui3DObjects.end() ? "No objects in scene" : selected->name);
    setDesktop3DLabel("model_label",
        selected == ui3DObjects.end() ? "Nothing selected" : selected->name);
    setDesktop3DLabel("viewport_status",
        selected == ui3DObjects.end() ? "Empty scene — drag an asset into the viewport" : selected->name);
    if (selected != ui3DObjects.end()) {
        setDesktop3DLabel("current_color_label", "Material: " + selected->color);
        setDesktop3DLabel("texture_label", "Texture: " + selected->texture);
    } else {
        setDesktop3DLabel("current_color_label", "Material: —");
        setDesktop3DLabel("texture_label", "Texture: —");
    }
    if (mainWindow) InvalidateRect(mainWindow, nullptr, FALSE);
}

std::string desktop3DUniqueName(const std::string& base) {
    std::string candidate;
    unsigned index = 1;
    do {
        candidate = base + "_" + std::to_string(index++);
    } while (std::any_of(ui3DObjects.begin(), ui3DObjects.end(),
        [&](const Ui3DObject& model) { return model.name == candidate; }));
    return candidate;
}

void createDesktop3DAsset(int kind, double x, double y, double z) {
    Ui3DObject model;
    model.group = "default";
    model.x = x;
    model.y = y;
    model.z = z;
    switch (kind) {
        case 1:
            model.name = desktop3DUniqueName("Sphere");
            model.shape = "sphere";
            model.width = model.height = model.depth = 1.6;
            model.color = "#4F9CFB";
            break;
        case 2:
            model.name = desktop3DUniqueName("Camera");
            model.shape = "camera";
            model.width = 1.1; model.height = 0.8; model.depth = 1.4;
            model.color = "#B79BEA";
            break;
        case 3:
            model.name = desktop3DUniqueName("Spawn");
            model.shape = "spawn";
            model.width = model.height = model.depth = 1.0;
            model.color = "#E7A85C";
            break;
        case 4:
            model.name = desktop3DUniqueName("Plane");
            model.shape = "plane";
            model.width = model.depth = 6.0;
            model.height = 0.15;
            model.y = -1.0;
            model.color = "#70B68A";
            break;
        default:
            model.name = desktop3DUniqueName("Box");
            model.shape = "box";
            model.width = model.height = model.depth = 2.0;
            model.color = "#8B5CF6";
            break;
    }
    ui3DObjects.push_back(std::move(model));
    desktop3DSelectedName = ui3DObjects.back().name;

    syncDesktop3DEditorState();
}

void updateDesktop3DLayout(HWND window, const DesktopLayout& layout) {
    const LONG width = std::max<LONG>(1, layout.width);
    const LONG canvasBottom = layout.panelTop;
    const LONG margin = scaleForDpi(window, 10);
    const LONG gap = scaleForDpi(window, 7);
    const LONG toolbarHeight = scaleForDpi(window, 42);
    const LONG shelfHeight = std::clamp<LONG>(layout.canvasHeight / 5,
        scaleForDpi(window, 96), scaleForDpi(window, 128));
    const LONG toolbarTop = layout.headerHeight + margin;
    const LONG sideTop = layout.headerHeight + toolbarHeight + gap * 2;
    const LONG shelfTop = std::max<LONG>(sideTop + scaleForDpi(window, 90),
        canvasBottom - shelfHeight);
    const LONG hierarchyWidth = std::clamp<LONG>(width / 5,
        scaleForDpi(window, 168), scaleForDpi(window, 224));
    const LONG inspectorWidth = std::clamp<LONG>(width / 4,
        scaleForDpi(window, 194), scaleForDpi(window, 258));
    desktop3DToolbarRect = {margin, toolbarTop,
        std::max<LONG>(margin + scaleForDpi(window, 520),
            width - margin - scaleForDpi(window, 86)), toolbarTop + toolbarHeight};
    desktop3DHierarchyRect = {margin, sideTop, margin + hierarchyWidth, shelfTop - gap};
    desktop3DInspectorRect = {width - margin - inspectorWidth, sideTop,
        width - margin, shelfTop - gap};
    desktop3DViewportRect = {desktop3DHierarchyRect.right + gap, sideTop,
        desktop3DInspectorRect.left - gap, shelfTop - gap};
    desktop3DAssetShelfRect = {margin, shelfTop, width - margin,
        std::max<LONG>(shelfTop + scaleForDpi(window, 72), canvasBottom - margin)};

    const LONG toolGap = scaleForDpi(window, 5);
    const LONG toolPadding = scaleForDpi(window, 7);
    const LONG toolCount = static_cast<LONG>(desktop3DToolRects.size());
    const LONG toolWidth = std::max<LONG>(scaleForDpi(window, 36),
        (desktop3DToolbarRect.right - desktop3DToolbarRect.left -
            toolPadding * 2 - toolGap * (toolCount - 1)) / toolCount);
    LONG toolX = desktop3DToolbarRect.left + toolPadding;
    for (std::size_t index = 0; index < desktop3DToolRects.size(); ++index) {
        desktop3DToolRects[index] = {toolX, desktop3DToolbarRect.top + scaleForDpi(window, 5),
            std::min<LONG>(desktop3DToolbarRect.right - toolPadding, toolX + toolWidth),
            desktop3DToolbarRect.bottom - scaleForDpi(window, 5)};
        toolX += toolWidth + toolGap;
    }

    const LONG shelfLabelHeight = scaleForDpi(window, 24);
    const LONG cardTop = desktop3DAssetShelfRect.top + shelfLabelHeight + scaleForDpi(window, 4);
    const LONG cardBottom = desktop3DAssetShelfRect.bottom - scaleForDpi(window, 7);
    const LONG assetStart = desktop3DAssetShelfRect.left + scaleForDpi(window, 82);
    const LONG textureWidth = std::clamp<LONG>((width - scaleForDpi(window, 40)) / 14,
        scaleForDpi(window, 48), scaleForDpi(window, 70));
    const LONG textureGap = scaleForDpi(window, 5);
    const LONG textureStart = desktop3DAssetShelfRect.right -
        (textureWidth * 5 + textureGap * 4);
    const LONG assetEnd = textureStart - scaleForDpi(window, 18);
    const LONG assetGap = scaleForDpi(window, 5);
    const LONG assetWidth = std::max<LONG>(scaleForDpi(window, 48),
        (assetEnd - assetStart - assetGap * 4) / 5);
    for (std::size_t index = 0; index < desktop3DAssetRects.size(); ++index) {
        const LONG left = assetStart + static_cast<LONG>(index) * (assetWidth + assetGap);
        desktop3DAssetRects[index] = {left, cardTop,
            std::min<LONG>(assetEnd, left + assetWidth), cardBottom};
        const LONG textureLeft = textureStart +
            static_cast<LONG>(index) * (textureWidth + textureGap);
        desktop3DTextureRects[index] = {textureLeft, cardTop,
            textureLeft + textureWidth, cardBottom};
    }
    desktop3DWorldScale = std::max<double>(8.0,
        std::min<LONG>(desktop3DViewportRect.right - desktop3DViewportRect.left,
                       desktop3DViewportRect.bottom - desktop3DViewportRect.top) *
            0.19 * desktop3DZoom);
}

bool projectDesktop3DPoint(double x, double y, double z, POINT& result) {
    const double dx = x - desktop3DCameraX;
    const double dy = y - desktop3DCameraY;
    const double dz = z - desktop3DCameraZ;
    const double cosYaw = std::cos(desktop3DCameraYaw);
    const double sinYaw = std::sin(desktop3DCameraYaw);
    const double viewX = dx * cosYaw - dz * sinYaw;
    const double forwardZ = dx * sinYaw + dz * cosYaw;
    const double cosPitch = std::cos(desktop3DCameraPitch);
    const double sinPitch = std::sin(desktop3DCameraPitch);
    const double viewY = dy * cosPitch - forwardZ * sinPitch;
    const double depth = forwardZ * cosPitch + dy * sinPitch;
    if (depth <= 0.25) return false;
    const double perspective = 12.0 / depth;
    const double centerX = (desktop3DViewportRect.left + desktop3DViewportRect.right) * 0.5;
    const double centerY = (desktop3DViewportRect.top + desktop3DViewportRect.bottom) * 0.5;
    result.x = static_cast<LONG>(centerX + viewX * desktop3DWorldScale * perspective);
    result.y = static_cast<LONG>(centerY - viewY * desktop3DWorldScale * perspective);
    return true;
}

void drawDesktop3DGrid(HDC dc) {
    if (!desktop3DGridVisible) return;
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(43, 37, 57));
    HGDIOBJ oldPen = SelectObject(dc, gridPen);
    for (int x = -24; x <= 24; ++x) {
        POINT first{}, last{};
        if (projectDesktop3DPoint(x, -1.35, -7, first) &&
            projectDesktop3DPoint(x, -1.35, 34, last)) {
            MoveToEx(dc, first.x, first.y, nullptr);
            LineTo(dc, last.x, last.y);
        }
    }
    for (int z = -7; z <= 34; ++z) {
        POINT first{}, last{};
        if (projectDesktop3DPoint(-24, -1.35, z, first) &&
            projectDesktop3DPoint(24, -1.35, z, last)) {
            MoveToEx(dc, first.x, first.y, nullptr);
            LineTo(dc, last.x, last.y);
        }
    }
    HPEN axisX = CreatePen(PS_SOLID, 2, RGB(173, 77, 93));
    SelectObject(dc, axisX);
    POINT x1{}, x2{};
    if (projectDesktop3DPoint(-24, -1.34, 0, x1) &&
        projectDesktop3DPoint(24, -1.34, 0, x2)) {
        MoveToEx(dc, x1.x, x1.y, nullptr);
        LineTo(dc, x2.x, x2.y);
    }
    HPEN axisZ = CreatePen(PS_SOLID, 2, RGB(82, 128, 194));
    SelectObject(dc, axisZ);
    POINT z1{}, z2{};
    if (projectDesktop3DPoint(0, -1.34, -7, z1) &&
        projectDesktop3DPoint(0, -1.34, 34, z2)) {
        MoveToEx(dc, z1.x, z1.y, nullptr);
        LineTo(dc, z2.x, z2.y);
    }
    SelectObject(dc, oldPen);
    DeleteObject(gridPen);
    DeleteObject(axisX);
    DeleteObject(axisZ);
}

void runDesktop3DCommand(const std::string& command) {
    const std::vector<std::string> lines = {command};
    Flow commandFlow;
    executeBlock(lines, 0, lines.size(), uiVariables, uiTypes, uiDefaults,
        commandFlow, false, true);
    syncDesktop3DEditorState();
}

void drawDesktop3DWorkspaceChrome(HDC dc, HWND window, const DesktopLayout& layout) {
    auto fillPanel = [&](const RECT& rect, COLORREF fill, COLORREF border, int radius = 8) {
        HBRUSH brush = CreateSolidBrush(fill);
        HPEN pen = CreatePen(PS_SOLID, 1, border);
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        HGDIOBJ oldPen = SelectObject(dc, pen);
        RoundRect(dc, rect.left, rect.top, rect.right, rect.bottom,
            scaleForDpi(window, radius), scaleForDpi(window, radius));
        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    };
    auto drawLabel = [&](const std::wstring& value, RECT rect, COLORREF color,
                         int size, bool bold = false, UINT format = DT_LEFT | DT_VCENTER | DT_SINGLELINE) {
        HFONT font = CreateFontW(-scaleForDpi(window, size), 0, 0, 0,
            bold ? FW_BOLD : FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ oldFont = SelectObject(dc, font);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, color);
        DrawTextW(dc, value.c_str(), -1, &rect, format);
        SelectObject(dc, oldFont);
        DeleteObject(font);
    };
    auto drawButton = [&](const RECT& rect, const wchar_t* label, bool active) {
        fillPanel(rect, active ? RGB(91, 61, 145) : RGB(39, 31, 54),
            active ? RGB(165, 127, 236) : RGB(73, 61, 91), 6);
        drawLabel(label, rect, RGB(242, 235, 255), 10, true,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    };

    fillPanel(desktop3DToolbarRect, RGB(29, 22, 42), RGB(65, 51, 88), 8);
    const wchar_t* toolbarLabels[] = {
        L"SELECT", L"MOVE", L"ROTATE", L"SCALE", L"PART", L"SPHERE", L"CAMERA",
        L"SPAWN", L"PLANE", L"PLAY", L"STOP", L"SAVE", L"LOAD"
    };
    for (std::size_t index = 0; index < desktop3DToolRects.size(); ++index)
        drawButton(desktop3DToolRects[index], toolbarLabels[index],
            (index < 4 && desktop3DToolMode == static_cast<int>(index)) ||
            (index == 9 && desktop3DPlayMode));

    fillPanel(desktop3DHierarchyRect, RGB(24, 19, 35), RGB(76, 56, 108), 8);
    fillPanel(desktop3DInspectorRect, RGB(24, 19, 35), RGB(76, 56, 108), 8);
    fillPanel(desktop3DAssetShelfRect, RGB(24, 19, 35), RGB(76, 56, 108), 8);
    HPEN viewportBorder = CreatePen(PS_SOLID, 1, RGB(91, 61, 135));
    HGDIOBJ oldViewportPen = SelectObject(dc, viewportBorder);
    HGDIOBJ oldViewportBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
    RoundRect(dc, desktop3DViewportRect.left, desktop3DViewportRect.top,
        desktop3DViewportRect.right, desktop3DViewportRect.bottom,
        scaleForDpi(window, 7), scaleForDpi(window, 7));
    SelectObject(dc, oldViewportBrush);
    SelectObject(dc, oldViewportPen);
    DeleteObject(viewportBorder);
    const LONG gizmoX = desktop3DViewportRect.right - scaleForDpi(window, 42);
    const LONG gizmoY = desktop3DViewportRect.top + scaleForDpi(window, 64);
    auto drawAxis = [&](int dx, int dy, COLORREF color, const wchar_t* label) {
        HPEN axisPen = CreatePen(PS_SOLID, 2, color);
        HGDIOBJ previousPen = SelectObject(dc, axisPen);
        MoveToEx(dc, gizmoX, gizmoY, nullptr);
        LineTo(dc, gizmoX + scaleForDpi(window, dx), gizmoY + scaleForDpi(window, dy));
        SelectObject(dc, previousPen);
        DeleteObject(axisPen);
        RECT textRect = {gizmoX + scaleForDpi(window, dx) - scaleForDpi(window, 8),
            gizmoY + scaleForDpi(window, dy) - scaleForDpi(window, 10),
            gizmoX + scaleForDpi(window, dx) + scaleForDpi(window, 12),
            gizmoY + scaleForDpi(window, dy) + scaleForDpi(window, 10)};
        drawLabel(label, textRect, color, 9, true, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    };
    drawAxis(0, -25, RGB(91, 220, 128), L"Y");
    drawAxis(28, 9, RGB(240, 82, 105), L"X");
    drawAxis(-25, 16, RGB(84, 137, 255), L"Z");
    RECT hierarchyTitle = {desktop3DHierarchyRect.left + scaleForDpi(window, 12),
        desktop3DHierarchyRect.top + scaleForDpi(window, 8),
        desktop3DHierarchyRect.right - scaleForDpi(window, 8),
        desktop3DHierarchyRect.top + scaleForDpi(window, 32)};
    drawLabel(L"WORLD  /  HIERARCHY", hierarchyTitle, RGB(207, 184, 246), 11, true);
    RECT sceneTitle = hierarchyTitle;
    sceneTitle.top += scaleForDpi(window, 27);
    sceneTitle.bottom += scaleForDpi(window, 27);
    drawLabel(L"Scene", sceneTitle, RGB(143, 130, 166), 10, true);

    const LONG rowTop = desktop3DHierarchyRect.top + scaleForDpi(window, 60);
    const LONG rowHeight = scaleForDpi(window, 25);
    const LONG maxVisible = std::max<LONG>(0,
        (desktop3DHierarchyRect.bottom - rowTop - scaleForDpi(window, 8)) / rowHeight);
    if (ui3DObjects.empty()) {
        RECT empty = {desktop3DHierarchyRect.left + scaleForDpi(window, 15), rowTop,
            desktop3DHierarchyRect.right - scaleForDpi(window, 8), rowTop + rowHeight};
        drawLabel(L"No objects in scene", empty, RGB(133, 121, 151), 10);
    }
    for (LONG visible = 0; visible < maxVisible; ++visible) {
        const std::size_t index = static_cast<std::size_t>(desktop3DHierarchyScroll + visible);
        if (index >= ui3DObjects.size()) break;
        const Ui3DObject& model = ui3DObjects[index];
        RECT row = {desktop3DHierarchyRect.left + scaleForDpi(window, 6),
            rowTop + visible * rowHeight,
            desktop3DHierarchyRect.right - scaleForDpi(window, 6),
            rowTop + (visible + 1) * rowHeight};
        if (model.name == desktop3DSelectedName) {
            HBRUSH selectedBrush = CreateSolidBrush(RGB(55, 42, 78));
            FillRect(dc, &row, selectedBrush);
            DeleteObject(selectedBrush);
            RECT accent = {row.left, row.top, row.left + scaleForDpi(window, 3), row.bottom};
            HBRUSH accentBrush = CreateSolidBrush(RGB(171, 129, 255));
            FillRect(dc, &accent, accentBrush);
            DeleteObject(accentBrush);
        }
        RECT label = row;
        InflateRect(&label, -scaleForDpi(window, 10), 0);
        drawLabel(wideFromUtf8(model.name), label,
            model.name == desktop3DSelectedName ? RGB(248, 242, 255) : RGB(190, 181, 207),
            10, model.name == desktop3DSelectedName);
    }
    RECT hierarchyFoot = {desktop3DHierarchyRect.left + scaleForDpi(window, 12),
        desktop3DHierarchyRect.bottom - scaleForDpi(window, 27),
        desktop3DHierarchyRect.right - scaleForDpi(window, 8),
        desktop3DHierarchyRect.bottom - scaleForDpi(window, 7)};
    drawLabel(L"Scroll to browse objects", hierarchyFoot, RGB(119, 108, 137), 9);

    RECT inspectorTitle = {desktop3DInspectorRect.left + scaleForDpi(window, 12),
        desktop3DInspectorRect.top + scaleForDpi(window, 8),
        desktop3DInspectorRect.right - scaleForDpi(window, 8),
        desktop3DInspectorRect.top + scaleForDpi(window, 32)};
    drawLabel(L"INSPECTOR", inspectorTitle, RGB(207, 184, 246), 11, true);
    auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
        [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
    LONG inspectorY = desktop3DInspectorRect.top + scaleForDpi(window, 48);
    if (selected == ui3DObjects.end()) {
        RECT hint = {desktop3DInspectorRect.left + scaleForDpi(window, 12), inspectorY,
            desktop3DInspectorRect.right - scaleForDpi(window, 8), inspectorY + scaleForDpi(window, 68)};
        drawLabel(L"Select an object to edit its properties.", hint,
            RGB(145, 132, 166), 10, false, DT_LEFT | DT_TOP | DT_WORDBREAK);
    } else {
        const Ui3DObject& model = *selected;
        auto field = [&](const wchar_t* name, const std::string& value) {
            RECT label = {desktop3DInspectorRect.left + scaleForDpi(window, 12), inspectorY,
                desktop3DInspectorRect.right - scaleForDpi(window, 8), inspectorY + scaleForDpi(window, 14)};
            drawLabel(name, label, RGB(157, 137, 188), 8, true);
            RECT valueCard = {desktop3DInspectorRect.left + scaleForDpi(window, 9),
                inspectorY + scaleForDpi(window, 15),
                desktop3DInspectorRect.right - scaleForDpi(window, 9),
                inspectorY + scaleForDpi(window, 33)};
            fillPanel(valueCard, RGB(35, 27, 50), RGB(66, 51, 91), 5);
            RECT valueLabel = valueCard;
            InflateRect(&valueLabel, -scaleForDpi(window, 7), 0);
            drawLabel(wideFromUtf8(value), valueLabel, RGB(235, 226, 250), 9);
            inspectorY += scaleForDpi(window, 36);
        };
        field(L"NAME", model.name + "  [" + model.shape + "]");
        field(L"POSITION  X / Y / Z",
            std::to_string(model.x) + "  /  " + std::to_string(model.y) + "  /  " + std::to_string(model.z));
        field(L"ROTATION  X / Y / Z",
            std::to_string(model.rotateX) + "  /  " + std::to_string(model.rotateY) + "  /  " + std::to_string(model.rotateZ));
        field(L"SIZE  W / H / D",
            std::to_string(model.width) + "  /  " + std::to_string(model.height) + "  /  " + std::to_string(model.depth));
        field(L"MATERIAL / COLOR", model.color);
        field(L"TEXTURE", model.texture);
        field(L"VISIBILITY", model.visible ? "Visible" : "Hidden");
    }

    RECT assetHeader = {desktop3DAssetShelfRect.left + scaleForDpi(window, 12),
        desktop3DAssetShelfRect.top + scaleForDpi(window, 4),
        desktop3DAssetShelfRect.right - scaleForDpi(window, 12),
        desktop3DAssetShelfRect.top + scaleForDpi(window, 25)};
    drawLabel(L"CONTENT BROWSER   /   ASSETS", assetHeader, RGB(207, 184, 246), 10, true);
    const wchar_t* assetNames[] = {L"BOX", L"SPHERE", L"CAMERA", L"SPAWN", L"PLANE"};
    const COLORREF assetColors[] = {
        RGB(139, 92, 246), RGB(78, 153, 232), RGB(183, 155, 234),
        RGB(229, 168, 89), RGB(102, 171, 133)
    };
    for (std::size_t index = 0; index < desktop3DAssetRects.size(); ++index) {
        const RECT& card = desktop3DAssetRects[index];
        fillPanel(card, RGB(36, 29, 51), RGB(72, 59, 93), 6);
        const LONG cx = (card.left + card.right) / 2;
        const LONG cy = card.top + scaleForDpi(window, 15);
        const LONG unit = scaleForDpi(window, 8);
        HBRUSH objectBrush = CreateSolidBrush(assetColors[index]);
        HPEN objectPen = CreatePen(PS_SOLID, 1, assetColors[index]);
        HGDIOBJ oldBrush = SelectObject(dc, objectBrush);
        HGDIOBJ oldPen = SelectObject(dc, objectPen);
        if (index == 1 || index == 3) {
            if (index == 3) SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
            Ellipse(dc, cx - unit, cy - unit, cx + unit, cy + unit);
        } else if (index == 2) {
            SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
            Rectangle(dc, cx - unit - 2, cy - unit / 2, cx + unit - 2, cy + unit / 2);
            MoveToEx(dc, cx + unit - 2, cy - unit / 2, nullptr);
            LineTo(dc, cx + unit + 6, cy - unit);
            LineTo(dc, cx + unit + 6, cy + unit);
            LineTo(dc, cx + unit - 2, cy + unit / 2);
        } else if (index == 4) {
            Rectangle(dc, cx - unit - 4, cy - unit / 2, cx + unit + 4, cy + unit / 2);
        } else {
            Rectangle(dc, cx - unit, cy - unit, cx + unit, cy + unit);
        }
        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(objectBrush);
        DeleteObject(objectPen);
        RECT nameRect = {card.left + scaleForDpi(window, 2),
            card.bottom - scaleForDpi(window, 19),
            card.right - scaleForDpi(window, 2), card.bottom - scaleForDpi(window, 2)};
        drawLabel(assetNames[index], nameRect, RGB(225, 215, 241), 9, true,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }

    const wchar_t* textureNames[] = {L"SMOOTH", L"GRID", L"METAL", L"WOOD", L"STONE"};
    const COLORREF textureColors[] = {
        RGB(130, 100, 177), RGB(70, 100, 137), RGB(130, 139, 153),
        RGB(137, 90, 57), RGB(100, 104, 117)
    };
    for (std::size_t index = 0; index < desktop3DTextureRects.size(); ++index) {
        const RECT& card = desktop3DTextureRects[index];
        fillPanel(card, RGB(35, 29, 49), RGB(70, 59, 87), 5);
        RECT swatch = {card.left + scaleForDpi(window, 5), card.top + scaleForDpi(window, 5),
            card.right - scaleForDpi(window, 5), card.top + scaleForDpi(window, 24)};
        HBRUSH swatchBrush = CreateSolidBrush(textureColors[index]);
        FillRect(dc, &swatch, swatchBrush);
        DeleteObject(swatchBrush);
        if (index > 0) {
            HBRUSH hatch = CreateHatchBrush(index == 1 ? HS_CROSS :
                index == 2 ? HS_DIAGCROSS : index == 3 ? HS_FDIAGONAL : HS_BDIAGONAL,
                RGB(225, 217, 238));
            HGDIOBJ old = SelectObject(dc, hatch);
            const int oldMode = SetBkMode(dc, TRANSPARENT);
            Rectangle(dc, swatch.left, swatch.top, swatch.right, swatch.bottom);
            SetBkMode(dc, oldMode);
            SelectObject(dc, old);
            DeleteObject(hatch);
        }
        RECT label = {card.left + scaleForDpi(window, 1),
            card.bottom - scaleForDpi(window, 19), card.right - scaleForDpi(window, 1),
            card.bottom - scaleForDpi(window, 2)};
        drawLabel(textureNames[index], label, RGB(225, 215, 241), 8, true,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }

    if (desktop3DEngineActive && desktop3DAssetDragging && desktop3DAssetCandidate >= 0) {
        const wchar_t* ghostNames[] = {L"BOX", L"SPHERE", L"CAMERA", L"SPAWN", L"PLANE"};
        HPEN ghostPen = CreatePen(PS_DASH, 2, RGB(190, 150, 255));
        HGDIOBJ oldPen = SelectObject(dc, ghostPen);
        HGDIOBJ oldBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
        Ellipse(dc, desktop3DMouseCurrent.x - scaleForDpi(window, 19),
            desktop3DMouseCurrent.y - scaleForDpi(window, 19),
            desktop3DMouseCurrent.x + scaleForDpi(window, 19),
            desktop3DMouseCurrent.y + scaleForDpi(window, 19));
        RECT ghost = {desktop3DMouseCurrent.x - scaleForDpi(window, 46),
            desktop3DMouseCurrent.y + scaleForDpi(window, 22),
            desktop3DMouseCurrent.x + scaleForDpi(window, 46),
            desktop3DMouseCurrent.y + scaleForDpi(window, 42)};
        drawLabel(ghostNames[std::clamp(desktop3DAssetCandidate, 0, 4)], ghost,
            RGB(225, 212, 255), 9, true, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(ghostPen);
    }

    if (desktop3DContextOpen) {
        const RECT popup = desktop3DContextRect;
        fillPanel(popup, RGB(30, 24, 43), RGB(119, 89, 163), 8);
        RECT title = {popup.left + scaleForDpi(window, 12),
            popup.top + scaleForDpi(window, 7),
            popup.right - scaleForDpi(window, 8),
            popup.top + scaleForDpi(window, 38)};
        drawLabel(wideFromUtf8(desktop3DSelectedName), title,
            RGB(239, 228, 255), 11, true, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        const wchar_t* actions[] = {
            L"Rename object", L"Duplicate object", L"Delete object",
            L"Change texture", L"Focus selected object", L"Edit position..."
        };
        for (int row = 0; row < 6; ++row) {
            RECT item = {popup.left + scaleForDpi(window, 7),
                popup.top + scaleForDpi(window, 43 + row * 29),
                popup.right - scaleForDpi(window, 7),
                popup.top + scaleForDpi(window, 43 + (row + 1) * 29)};
            if (row == 2) {
                HBRUSH danger = CreateSolidBrush(RGB(74, 37, 55));
                FillRect(dc, &item, danger);
                DeleteObject(danger);
            }
            drawLabel(actions[row], item, row == 2 ? RGB(255, 163, 190) : RGB(222, 211, 239),
                10, row == 0, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        }
    }
}

bool buildStandaloneTrgExecutable(const std::filesystem::path& sourcePath,
                                  std::filesystem::path& outputPath,
                                  std::string& errorMessage) {
    std::ifstream source(sourcePath, std::ios::binary);
    if (!source) {
        errorMessage = "Could not open source file: " + sourcePath.u8string();
        return false;
    }
#ifdef _WIN32
    wchar_t executableBuffer[32768] = {};
    const DWORD executableLength = GetModuleFileNameW(nullptr, executableBuffer, 32768);
    if (executableLength == 0 || executableLength >= 32768) {
        errorMessage = "Could not locate the TRG runtime executable.";
        return false;
    }
    const std::filesystem::path executablePath(std::wstring(executableBuffer, executableLength));
#else
    errorMessage = "Standalone .exe export is only available on Windows.";
    return false;
#endif
    std::error_code fileError;
    const std::uint64_t payloadSize = std::filesystem::file_size(sourcePath, fileError);
    if (fileError) {
        errorMessage = "Could not determine source size: " + fileError.message();
        return false;
    }
    outputPath = sourcePath;
    outputPath.replace_extension(".exe");
    std::error_code copyError;
    std::filesystem::copy_file(executablePath, outputPath,
        std::filesystem::copy_options::overwrite_existing, copyError);
    if (copyError) {
        errorMessage = "Could not create " + outputPath.u8string() + ": " + copyError.message();
        return false;
    }
    std::ofstream output(outputPath, std::ios::binary | std::ios::app);
    if (!output) {
        errorMessage = "Could not append the TRG program to " + outputPath.u8string();
        return false;
    }
    output << source.rdbuf();
    for (unsigned byte = 0; byte < 8; ++byte)
        output.put(static_cast<char>((payloadSize >> (byte * 8)) & 0xff));
    output.write("TRGPKG01", 8);
    output.close();
    if (!output) {
        errorMessage = "Failed while finalizing " + outputPath.u8string();
        return false;
    }
    return true;
}

LRESULT CALLBACK mainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        mainWindow = window;
        openButton = CreateWindowExW(0, L"BUTTON", L"Open",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(openButtonId), GetModuleHandleW(nullptr), nullptr);
        saveButton = CreateWindowExW(0, L"BUTTON", L"Save",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(saveButtonId), GetModuleHandleW(nullptr), nullptr);
        runButton = CreateWindowExW(0, L"BUTTON", L"Run",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(runButtonId), GetModuleHandleW(nullptr), nullptr);
        previewButton = CreateWindowExW(0, L"BUTTON", L"Preview",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(previewButtonId), GetModuleHandleW(nullptr), nullptr);
        newButton = CreateWindowExW(0, L"BUTTON", L"New",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(newButtonId), GetModuleHandleW(nullptr), nullptr);
        buildButton = CreateWindowExW(0, L"BUTTON", L"Build EXE",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(buildButtonId), GetModuleHandleW(nullptr), nullptr);
        dictionaryButton = CreateWindowExW(0, L"BUTTON", L"Dictionary",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(dictionaryButtonId), GetModuleHandleW(nullptr), nullptr);
        editor2DButton = CreateWindowExW(0, L"BUTTON", L"2D Editor",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(editor2DButtonId), GetModuleHandleW(nullptr), nullptr);
        editor3DButton = CreateWindowExW(0, L"BUTTON", L"3D Engine",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(editor3DButtonId), GetModuleHandleW(nullptr), nullptr);
        exportButton = CreateWindowExW(0, L"BUTTON", L"Export TRG",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(exportButtonId), GetModuleHandleW(nullptr), nullptr);
        richEditModule = LoadLibraryW(L"Msftedit.dll");
        const wchar_t* editorClass = richEditModule ? MSFTEDIT_CLASS : L"EDIT";
        codeEditor = CreateWindowExW(WS_EX_CLIENTEDGE, editorClass, L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL |
                ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_NOHIDESEL,
            0, 0, 100, 100, window, reinterpret_cast<HMENU>(codeEditorId),
            GetModuleHandleW(nullptr), nullptr);
        SendMessageW(codeEditor, EM_SETLIMITTEXT, 8 * 1024 * 1024, 0);
        previousCodeEditorProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
            codeEditor, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(codeEditorWindowProc)));
        outputControl = CreateWindowExW(WS_EX_CLIENTEDGE, editorClass, L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE |
                ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY | ES_NOHIDESEL,
            0, 0, 100, 100, window, reinterpret_cast<HMENU>(outputControlId),
            GetModuleHandleW(nullptr), nullptr);
        if (outputControl) previousOutputProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(outputControl, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(outputWindowProc)));
        terminalInputControl = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
            0, 0, 100, 28, window, reinterpret_cast<HMENU>(terminalInputControlId),
            GetModuleHandleW(nullptr), nullptr);
        terminalSendButton = CreateWindowExW(0, L"BUTTON", L"Send",
            WS_CHILD | BS_OWNERDRAW | WS_TABSTOP, 0, 0, 72, 28, window,
            reinterpret_cast<HMENU>(terminalSendButtonId), GetModuleHandleW(nullptr), nullptr);
        if (terminalInputControl) previousTerminalInputProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(terminalInputControl, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(terminalInputWindowProc)));
        setOpenButtonFont(window);
        if (outputControl && richEditModule) {
            SendMessageW(outputControl, EM_SETBKGNDCOLOR, 0, RGB(8, 7, 12));
            CHARFORMAT2W outputFormat = {};
            outputFormat.cbSize = sizeof(outputFormat);
            outputFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
            outputFormat.crTextColor = RGB(255, 255, 255);
            outputFormat.yHeight = 180;
            std::wcsncpy(outputFormat.szFaceName, L"Consolas", LF_FACESIZE - 1);
            outputFormat.szFaceName[LF_FACESIZE - 1] = L'\0';
            SendMessageW(outputControl, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&outputFormat));
        }
        if (terminalInputControl && codeEditorFont)
            SendMessageW(terminalInputControl, WM_SETFONT, reinterpret_cast<WPARAM>(codeEditorFont), TRUE);
        if (outputControl && codeEditorFont)
            SendMessageW(outputControl, WM_SETFONT, reinterpret_cast<WPARAM>(codeEditorFont), TRUE);
        if (terminalSendButton && openButtonFont)
            SendMessageW(terminalSendButton, WM_SETFONT, reinterpret_cast<WPARAM>(openButtonFont), TRUE);
        terminalInputBrush = CreateSolidBrush(RGB(12, 9, 19));
        outputControlBrush = CreateSolidBrush(RGB(8, 7, 12));
        if (restoreDesktopStateOnCreate) restoreDesktopAutosave();
        if (codeEditor && richEditModule) {
            SendMessageW(codeEditor, EM_SETBKGNDCOLOR, 0, RGB(19, 13, 31));
            CHARFORMAT2W baseFormat = {};
            baseFormat.cbSize = sizeof(baseFormat);
            baseFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD;
            baseFormat.crTextColor = RGB(239, 232, 250);
            baseFormat.yHeight = 195;
            baseFormat.dwEffects = 0;
            std::wcsncpy(baseFormat.szFaceName, L"Consolas", LF_FACESIZE - 1);
            baseFormat.szFaceName[LF_FACESIZE - 1] = L'\0';
            SendMessageW(codeEditor, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&baseFormat));
        }
        layoutOutputControls(window);
        updateEditorWindowTitle();
        return 0;
    }
    if (message == WM_DRAWITEM) {
        auto* item = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
        if (item && item->CtlType == ODT_BUTTON) {
            COLORREF fill = RGB(43, 31, 63);
            COLORREF textColor = RGB(244, 238, 252);
            switch (item->CtlID) {
                case runButtonId: fill = RGB(103, 69, 168); break;
                case previewButtonId: fill = RGB(76, 49, 123); break;
                case buildButtonId: fill = RGB(64, 45, 94); break;
                case newButtonId: fill = RGB(53, 39, 75); break;
                case dictionaryButtonId: fill = RGB(48, 35, 68); break;
                case editor2DButtonId:
                case editor3DButtonId: fill = RGB(82, 55, 133); break;
                case exportButtonId: fill = RGB(73, 55, 104); break;
                default: break;
            }
            if (item->itemState & ODS_SELECTED) {
                fill = RGB(GetRValue(fill) * 3 / 4, GetGValue(fill) * 3 / 4,
                    GetBValue(fill) * 3 / 4);
            }
            RECT area = item->rcItem;
            HBRUSH brush = CreateSolidBrush(fill);
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(104, 76, 151));
            HGDIOBJ oldBrush = SelectObject(item->hDC, brush);
            HGDIOBJ oldPen = SelectObject(item->hDC, pen);
            RoundRect(item->hDC, area.left + 1, area.top + 1, area.right - 1,
                area.bottom - 1, scaleForDpi(window, 10), scaleForDpi(window, 10));
            SelectObject(item->hDC, oldBrush);
            SelectObject(item->hDC, oldPen);
            DeleteObject(brush);
            DeleteObject(pen);
            SetBkMode(item->hDC, TRANSPARENT);
            SetTextColor(item->hDC, textColor);
            HFONT font = CreateFontW(-scaleForDpi(window, 11), 0, 0, 0, FW_BOLD,
                FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
                L"Segoe UI");
            HGDIOBJ oldFont = SelectObject(item->hDC, font);
            wchar_t label[128] = {};
            GetWindowTextW(item->hwndItem, label, 127);
            InflateRect(&area, -scaleForDpi(window, 3), 0);
            DrawTextW(item->hDC, label, -1, &area,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
            SelectObject(item->hDC, oldFont);
            DeleteObject(font);
            if (item->itemState & ODS_FOCUS) {
                InflateRect(&area, -scaleForDpi(window, 3), -scaleForDpi(window, 3));
                DrawFocusRect(item->hDC, &area);
            }
            return TRUE;
        }
    }
    if ((message == WM_CTLCOLOREDIT || message == WM_CTLCOLORSTATIC) &&
        reinterpret_cast<HWND>(lParam) == terminalInputControl) {
        HDC dc = reinterpret_cast<HDC>(wParam);
        SetTextColor(dc, RGB(241, 235, 250));
        SetBkColor(dc, RGB(12, 9, 19));
        return reinterpret_cast<LRESULT>(terminalInputBrush);
    }
    if ((message == WM_CTLCOLOREDIT || message == WM_CTLCOLORSTATIC) &&
        reinterpret_cast<HWND>(lParam) == outputControl) {
        HDC dc = reinterpret_cast<HDC>(wParam);
        SetTextColor(dc, RGB(255, 255, 255));
        SetBkColor(dc, RGB(8, 7, 12));
        return reinterpret_cast<LRESULT>(outputControlBrush);
    }
    if (message == WM_SIZE) {
        refreshDisplayVariables(uiVariables, uiTypes);
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_GETMINMAXINFO) {
        auto* limits = reinterpret_cast<MINMAXINFO*>(lParam);
        limits->ptMinTrackSize.x = scaleForDpi(window, 820);
        limits->ptMinTrackSize.y = scaleForDpi(window, 420);
        return 0;
    }
    if (message == WM_DPICHANGED) {
        const RECT* suggested = reinterpret_cast<const RECT*>(lParam);
        SetWindowPos(window, nullptr, suggested->left, suggested->top,
                     suggested->right - suggested->left, suggested->bottom - suggested->top,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        setOpenButtonFont(window);
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == codeEditorId && HIWORD(wParam) == EN_CHANGE) {
        if (!loadingEditorText && !highlightingEditor) {
            editorDirty = true;
            updateEditorWindowTitle();
            SetTimer(window, 2, 500, nullptr);
            applyEditorSyntaxHighlighting();
        }
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == openButtonId) {
        std::filesystem::path path;
        if (openSourceDialog(path, window) && confirmDiscardEditorChanges(window)) loadSourceFile(path);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == saveButtonId) {
        saveSourceFile();
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == runButtonId) {
        runEditorSource();
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == editor2DButtonId) {
        launchDesktopVisualEditor("2D Editor");
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == editor3DButtonId) {
        launchDesktopVisualEditor("3D Editor");
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == dictionaryButtonId) {
        showDesktopDictionary(window);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == exportButtonId) {
        std::filesystem::path destination = currentSourcePath.empty()
            ? std::filesystem::path(L"program.trg") : currentSourcePath.filename();
        if (saveSourceDialog(destination, window)) {
            if (lower(destination.extension().u8string()) != ".trg")
                destination.replace_extension(".trg");
            const int length = GetWindowTextLengthW(codeEditor);
            std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
            GetWindowTextW(codeEditor, wideSource.data(), length + 1);
            wideSource.resize(static_cast<std::size_t>(length));
            const std::string source = utf8FromWide(wideSource);
            std::ofstream exported(destination, std::ios::binary | std::ios::trunc);
            if (!exported) appendOutput("ERROR Could not export TRG source.\r\n");
            else {
                exported.write(source.data(), static_cast<std::streamsize>(source.size()));
                if (exported) appendOutput("Exported TRG source: " + destination.u8string() + "\r\n");
                else appendOutput("ERROR Failed while exporting TRG source.\r\n");
            }
        }
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == terminalSendButtonId && terminalInputActive) {
        const int length = GetWindowTextLengthW(terminalInputControl);
        std::wstring answer(static_cast<std::size_t>(length) + 1, L'\0');
        GetWindowTextW(terminalInputControl, answer.data(), length + 1);
        answer.resize(static_cast<std::size_t>(length));
        inputResult = utf8FromWide(answer);
        inputAccepted = true;
        inputFinished = true;
        terminalInputActive = false;
        appendOutput("> " + inputResult + "\r\n");
        SetWindowTextW(terminalInputControl, L"");
        layoutOutputControls(window);
        SetFocus(window);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == buildButtonId) {
        if (!saveSourceFile()) return 0;
        appendOutput("Building standalone Windows EXE with the TRG program embedded...\r\n");
        std::filesystem::path outputPath;
        std::string errorMessage;
        if (!buildStandaloneTrgExecutable(currentSourcePath, outputPath, errorMessage)) {
            appendOutput("ERROR " + errorMessage + "\r\n");
        } else {
            appendOutput("Created " + outputPath.u8string() + " (standalone TRG app).\r\n");
        }
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == previewButtonId) {
        if (desktopVisualEditorActive && previewMode) {
            returnFromDesktopVisualEditor();
            return 0;
        }
        previewMode = !previewMode;
        SetWindowTextW(previewButton, previewMode ? L"Code" : L"Preview");
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == newButtonId) {
        if (confirmDiscardEditorChanges(window)) {
            currentSourcePath.clear();
            editorDirty = false;
            previewMode = false;
            loadingEditorText = true;
            SetWindowTextW(codeEditor, L"");
            loadingEditorText = false;
            saveDesktopAutosave();
            SetWindowTextW(previewButton, L"Preview");
            clearDesktopOutput();
            projectRoot = std::filesystem::current_path();
            uiBlocks.clear(); ui3DObjects.clear(); uiSprites.clear(); uiAnimations.clear();
            everyTasks.clear(); uiButtons.clear(); uiTexts.clear();
            collisionWatches.clear();
            updateEditorWindowTitle();
            layoutOutputControls(window);
            InvalidateRect(window, nullptr, TRUE);
        }
        return 0;
    }
    if (message == WM_KEYDOWN && desktop3DEngineActive && previewMode && !terminalInputActive) {
        const double step = (GetKeyState(VK_SHIFT) & 0x8000) ? 0.9 : 0.32;
        switch (wParam) {
            case 'W': case 'A': case 'S': case 'D':
                return 0; // WASD movement is updated continuously by the camera timer.
            case 'Q': desktop3DCameraY -= step; break;
            case 'E': desktop3DCameraY += step; break;
            case VK_LEFT: desktop3DCameraYaw -= 0.055; break;
            case VK_RIGHT: desktop3DCameraYaw += 0.055; break;
            case VK_UP: desktop3DCameraPitch = std::clamp(desktop3DCameraPitch + 0.045, -1.2, 1.2); break;
            case VK_DOWN: desktop3DCameraPitch = std::clamp(desktop3DCameraPitch - 0.045, -1.2, 1.2); break;
            default: return DefWindowProcW(window, message, wParam, lParam);
        }
        InvalidateRect(window, nullptr, FALSE);
        return 0;
    }
    if (message == WM_TIMER && wParam == 3 && desktop3DEngineActive && previewMode) {
        if (terminalInputActive || GetFocus() != window) return 0;
        const double step = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? 0.23 : 0.075;
        const double yaw = desktop3DCameraYaw;
        bool moved = false;
        if (GetAsyncKeyState('W') & 0x8000) {
            desktop3DCameraX += std::sin(yaw) * step;
            desktop3DCameraZ += std::cos(yaw) * step;
            moved = true;
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            desktop3DCameraX -= std::sin(yaw) * step;
            desktop3DCameraZ -= std::cos(yaw) * step;
            moved = true;
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            desktop3DCameraX -= std::cos(yaw) * step;
            desktop3DCameraZ += std::sin(yaw) * step;
            moved = true;
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            desktop3DCameraX += std::cos(yaw) * step;
            desktop3DCameraZ -= std::sin(yaw) * step;
            moved = true;
        }
        if (moved) InvalidateRect(window, nullptr, FALSE);
        return 0;
    }
    if (message == WM_MOUSEWHEEL && desktop3DEngineActive && previewMode) {
        POINT cursor = {};
        GetCursorPos(&cursor);
        ScreenToClient(window, &cursor);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (PtInRect(&desktop3DHierarchyRect, cursor)) {
            const LONG rowTop = desktop3DHierarchyRect.top + scaleForDpi(window, 60);
            const LONG rowHeight = scaleForDpi(window, 25);
            const int visibleRows = std::max(1, static_cast<int>(
                (desktop3DHierarchyRect.bottom - rowTop - scaleForDpi(window, 8)) / rowHeight));
            const int maximum = std::max(0, static_cast<int>(ui3DObjects.size()) - visibleRows);
            desktop3DHierarchyScroll = std::clamp(desktop3DHierarchyScroll + (delta > 0 ? -3 : 3),
                0, maximum);
        } else if (PtInRect(&desktop3DViewportRect, cursor)) {
            const double factor = delta > 0 ? 1.12 : 0.89;
            desktop3DZoom = std::clamp(desktop3DZoom * factor, 0.35, 3.5);
        }
        InvalidateRect(window, nullptr, FALSE);
        return 0;
    }
    if (message == WM_RBUTTONDOWN && desktop3DEngineActive && previewMode) {
        const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        desktop3DRightClickMoved = false;
        if (!PtInRect(&desktop3DViewportRect, point)) {
            desktop3DDraggingCamera = false;
            return 0;
        }
        SetFocus(window);
        desktop3DDraggingCamera = true;
        desktop3DMouseDown = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        desktop3DMouseCurrent = desktop3DMouseDown;
        desktop3DDragStartYaw = desktop3DCameraYaw;
        desktop3DDragStartPitch = desktop3DCameraPitch;
        SetCapture(window);
        return 0;
    }
    if (message == WM_MOUSEMOVE && desktop3DEngineActive && desktop3DDraggingCamera &&
        (GetKeyState(VK_RBUTTON) & 0x8000)) {
        const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        desktop3DMouseCurrent = point;
        const LONG dx = point.x - desktop3DMouseDown.x;
        const LONG dy = point.y - desktop3DMouseDown.y;
        if (std::abs(dx) + std::abs(dy) > 4) desktop3DRightClickMoved = true;
        if (desktop3DRightClickMoved) {
            desktop3DCameraYaw = desktop3DDragStartYaw + dx * 0.0065;
            desktop3DCameraPitch = std::clamp(desktop3DDragStartPitch + dy * 0.0065, -1.2, 1.2);
            InvalidateRect(window, nullptr, FALSE);
        }
        return 0;
    }
    if (message == WM_RBUTTONUP && desktop3DEngineActive && previewMode) {
        if (GetCapture() == window) ReleaseCapture();
        desktop3DDraggingCamera = false;
        const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        if (!desktop3DRightClickMoved) {
            bool selectedObject = false;
            for (auto hit = desktop3DProjectedHits.rbegin(); hit != desktop3DProjectedHits.rend(); ++hit) {
                if (!PtInRect(&hit->second, point)) continue;
                desktop3DSelectedName = hit->first;
                selectedObject = true;
                break;
            }
            if (!selectedObject && PtInRect(&desktop3DHierarchyRect, point)) {
                const LONG rowTop = desktop3DHierarchyRect.top + scaleForDpi(window, 60);
                const LONG rowHeight = scaleForDpi(window, 25);
                const int row = static_cast<int>((point.y - rowTop) / rowHeight) + desktop3DHierarchyScroll;
                if (row >= 0 && static_cast<std::size_t>(row) < ui3DObjects.size()) {
                    desktop3DSelectedName = ui3DObjects[static_cast<std::size_t>(row)].name;
                    selectedObject = true;
                }
            }
            if (selectedObject) {
                const LONG popupWidth = scaleForDpi(window, 224);
                const LONG popupHeight = scaleForDpi(window, 236);
                LONG minX = desktop3DViewportRect.left + 4;
                LONG maxX = desktop3DViewportRect.right - popupWidth - 4;
                LONG minY = desktop3DViewportRect.top + 4;
                RECT client = {};
                GetClientRect(window, &client);
                const DesktopLayout layout = desktopLayout(window, client);
                LONG maxY = layout.panelTop;
                if (PtInRect(&desktop3DHierarchyRect, point)) {
                    minX = desktop3DHierarchyRect.right + scaleForDpi(window, 6);
                    maxX = std::max<LONG>(minX, client.right - popupWidth - scaleForDpi(window, 10));
                }
                maxY = std::max<LONG>(minY, maxY - popupHeight - scaleForDpi(window, 4));
                const LONG x = std::clamp<LONG>(point.x, minX, maxX);
                const LONG y = std::clamp<LONG>(point.y, minY, maxY);
                desktop3DContextRect = {x, y, x + popupWidth, y + popupHeight};
                desktop3DContextPoint = point;
                desktop3DContextOpen = true;
                syncDesktop3DEditorState();
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        return 0;
    }
    if (message == WM_LBUTTONDOWN && desktop3DEngineActive && previewMode) {
        const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        SetFocus(window);
        const LONG tabTop = scaleForDpi(window, 9);
        const LONG tabBottom = scaleForDpi(window, 43);
        const LONG tabStart = scaleForDpi(window, 250);
        const LONG tabStep = scaleForDpi(window, 94);
        if (point.y >= tabTop && point.y <= tabBottom &&
            point.x >= tabStart && point.x < tabStart + tabStep * 5) {
            const int tab = static_cast<int>((point.x - tabStart) / tabStep);
            desktop3DActiveTab = std::clamp(tab, 0, 4);
            if (desktop3DActiveTab == 2)
                appendOutput("Terrain tools selected. Use Plane in the ribbon or Toolbox to create terrain.\\r\\n");
            else if (desktop3DActiveTab == 3) {
                desktop3DGridVisible = !desktop3DGridVisible;
                appendOutput(desktop3DGridVisible ? "Viewport grid enabled.\\r\\n" : "Viewport grid hidden.\\r\\n");
            } else if (desktop3DActiveTab == 4)
                appendOutput("No desktop plugins are installed yet.\\r\\n");
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        desktop3DMouseDown = point;
        desktop3DMouseCurrent = point;
        desktop3DAssetCandidate = -1;
        desktop3DAssetDragging = false;

        if (desktop3DContextOpen) {
            if (PtInRect(&desktop3DContextRect, point)) {
                const int row = (point.y - desktop3DContextRect.top - scaleForDpi(window, 43)) /
                    scaleForDpi(window, 29);
                auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                    [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
                if (selected != ui3DObjects.end() && row >= 0 && row < 6) {
                    if (row == 0) {
                        std::string newName;
                        if (askForInput("Rename object:", newName) && validName(newName)) {
                            const bool used = std::any_of(ui3DObjects.begin(), ui3DObjects.end(),
                                [&](const Ui3DObject& model) { return model.name == newName && model.name != selected->name; });
                            if (!used) {
                                selected->name = newName;
                                desktop3DSelectedName = newName;
                            } else appendOutput("ERROR That object name is already in use.\r\n");
                        }
                    } else if (row == 1) {
                        Ui3DObject copy = *selected;
                        copy.name = desktop3DUniqueName(copy.shape == "sphere" ? "Sphere" :
                            copy.shape == "camera" ? "Camera" : copy.shape == "spawn" ? "Spawn" : "Object");
                        copy.x += 1.0;
                        copy.z += 1.0;
                        ui3DObjects.push_back(copy);
                        desktop3DSelectedName = copy.name;
                    } else if (row == 2) {
                        ui3DObjects.erase(selected);
                        desktop3DSelectedName.clear();
                    } else if (row == 3) {
                        if (selected->texture == "smooth") selected->texture = "grid";
                        else if (selected->texture == "grid") selected->texture = "metal";
                        else if (selected->texture == "metal") selected->texture = "wood";
                        else if (selected->texture == "wood") selected->texture = "stone";
                        else selected->texture = "smooth";
                    } else if (row == 4) {
                        desktop3DCameraX = selected->x;
                        desktop3DCameraY = selected->y + 2.0;
                        desktop3DCameraZ = selected->z - 8.0;
                        desktop3DCameraYaw = 0.0;
                        desktop3DCameraPitch = 0.15;
                    } else if (row == 5) {
                        std::string value;
                        double number = 0;
                        if (askForInput("Set X position:", value) && parseNumber(value, number)) selected->x = number;
                        if (askForInput("Set Y position:", value) && parseNumber(value, number)) selected->y = number;
                        if (askForInput("Set Z position:", value) && parseNumber(value, number)) selected->z = number;
                    }
                }
                desktop3DContextOpen = false;
                syncDesktop3DEditorState();
                InvalidateRect(window, nullptr, FALSE);
                return 0;
            }
            desktop3DContextOpen = false;
        }

        for (std::size_t index = 0; index < desktop3DToolRects.size(); ++index) {
            if (!PtInRect(&desktop3DToolRects[index], point)) continue;
            if (index < 4) {
                desktop3DToolMode = static_cast<int>(index);
                desktop3DPlayMode = false;
            } else if (index < 9) {
                const double worldX = desktop3DCameraX + std::sin(desktop3DCameraYaw) * 12.0;
                const double worldZ = desktop3DCameraZ + std::cos(desktop3DCameraYaw) * 12.0;
                createDesktop3DAsset(static_cast<int>(index) - 4, worldX, 0.0, worldZ);
            } else if (index == 9) {
                desktop3DPlayMode = true;
                appendOutput("Play preview enabled. Press Stop to return to editing.\\r\\n");
            } else if (index == 10) {
                desktop3DPlayMode = false;
                appendOutput("Play preview stopped.\\r\\n");
            } else if (index == 11) runDesktop3DCommand("3d save \"scene.trgscene\"");
            else if (index == 12) runDesktop3DCommand("3d load \"scene.trgscene\"");
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }

        for (std::size_t index = 0; index < desktop3DTextureRects.size(); ++index) {
            if (!PtInRect(&desktop3DTextureRects[index], point)) continue;
            auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
            if (selected != ui3DObjects.end()) {
                const char* textures[] = {"smooth", "grid", "metal", "wood", "stone"};
                selected->texture = textures[index];
                syncDesktop3DEditorState();
            } else {
                appendOutput("Select a model before applying a material.\r\n");
            }
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }

        for (std::size_t index = 0; index < desktop3DAssetRects.size(); ++index) {
            if (!PtInRect(&desktop3DAssetRects[index], point)) continue;
            desktop3DAssetCandidate = static_cast<int>(index);
            SetCapture(window);
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }

        if (PtInRect(&desktop3DHierarchyRect, point)) {
            const LONG rowTop = desktop3DHierarchyRect.top + scaleForDpi(window, 60);
            const LONG rowHeight = scaleForDpi(window, 25);
            const int row = static_cast<int>((point.y - rowTop) / rowHeight) + desktop3DHierarchyScroll;
            if (row >= 0 && static_cast<std::size_t>(row) < ui3DObjects.size()) {
                desktop3DSelectedName = ui3DObjects[static_cast<std::size_t>(row)].name;
                syncDesktop3DEditorState();
            }
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        if (PtInRect(&desktop3DInspectorRect, point)) {
            auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
            if (selected == ui3DObjects.end()) return 0;
            const LONG rowTop = desktop3DInspectorRect.top + scaleForDpi(window, 48);
            const LONG rowHeight = scaleForDpi(window, 36);
            const int row = static_cast<int>((point.y - rowTop) / rowHeight);
            std::string value;
            double number = 0.0;
            auto askNumber = [&](const char* prompt, double& target) {
                if (!askForInput(prompt, value)) return;
                if (parseNumber(value, number)) target = number;
                else appendOutput("ERROR Enter a valid number.\\r\\n");
            };
            auto askVector = [&](const char* title, double& x, double& y, double& z) {
                std::string prompt = std::string(title) + " X:";
                askNumber(prompt.c_str(), x);
                prompt = std::string(title) + " Y:";
                askNumber(prompt.c_str(), y);
                prompt = std::string(title) + " Z:";
                askNumber(prompt.c_str(), z);
            };
            if (row == 0) {
                if (askForInput("Rename object:", value) && validName(value)) {
                    const bool used = std::any_of(ui3DObjects.begin(), ui3DObjects.end(),
                        [&](const Ui3DObject& model) { return model.name == value && model.name != selected->name; });
                    if (!used) { selected->name = value; desktop3DSelectedName = value; }
                    else appendOutput("ERROR That object name is already in use.\\r\\n");
                }
            } else if (row == 1) askVector("Position", selected->x, selected->y, selected->z);
            else if (row == 2) askVector("Rotation", selected->rotateX, selected->rotateY, selected->rotateZ);
            else if (row == 3) {
                double width = selected->width, height = selected->height, depth = selected->depth;
                askVector("Size", width, height, depth);
                if (width > 0 && height > 0 && depth > 0) {
                    selected->width = width; selected->height = height; selected->depth = depth;
                } else appendOutput("ERROR Object dimensions must be positive.\\r\\n");
            } else if (row == 4) {
                if (askForInput("Material color (#RRGGBB):", value)) {
                    unsigned red = 0, green = 0, blue = 0;
                    if (parseColor(value, red, green, blue)) selected->color = value;
                    else appendOutput("ERROR Use a valid #RRGGBB color.\\r\\n");
                }
            } else if (row == 5) {
                if (askForInput("Texture (smooth, grid, metal, wood, stone):", value)) {
                    value = lower(value);
                    if (value == "smooth" || value == "grid" || value == "metal" ||
                        value == "wood" || value == "stone") selected->texture = value;
                    else appendOutput("ERROR Unsupported texture.\\r\\n");
                }
            } else if (row == 6) selected->visible = !selected->visible;
            syncDesktop3DEditorState();
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        if (PtInRect(&desktop3DAssetShelfRect, point) ||
            PtInRect(&desktop3DToolbarRect, point))
            return 0;

        if (PtInRect(&desktop3DViewportRect, point)) {
            bool hitObject = false;
            for (auto hit = desktop3DProjectedHits.rbegin(); hit != desktop3DProjectedHits.rend(); ++hit) {
                if (!PtInRect(&hit->second, point)) continue;
                desktop3DSelectedName = hit->first;
                auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                    [&](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
                if (selected != ui3DObjects.end() && desktop3DToolMode != 0) {
                    desktop3DDraggingObject = true;
                    desktop3DDragStartX = selected->x;
                    desktop3DDragStartY = selected->y;
                    desktop3DDragStartZ = selected->z;
                    desktop3DDragStartWidth = selected->width;
                    desktop3DDragStartHeight = selected->height;
                    desktop3DDragStartDepth = selected->depth;
                    desktop3DDragStartRotX = selected->rotateX;
                    desktop3DDragStartRotY = selected->rotateY;
                    SetCapture(window);
                }
                hitObject = true;
                break;
            }
            if (!hitObject) desktop3DSelectedName.clear();
            syncDesktop3DEditorState();
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        return 0;
    }
    if (message == WM_MOUSEMOVE && desktop3DEngineActive && previewMode &&
        (GetKeyState(VK_LBUTTON) & 0x8000)) {
        const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
        desktop3DMouseCurrent = point;
        if (desktop3DAssetCandidate >= 0) {
            const LONG dx = point.x - desktop3DMouseDown.x;
            const LONG dy = point.y - desktop3DMouseDown.y;
            if (std::abs(dx) + std::abs(dy) > 7) desktop3DAssetDragging = true;
        }
        if (desktop3DDraggingObject) {
            auto selected = std::find_if(ui3DObjects.begin(), ui3DObjects.end(),
                [](const Ui3DObject& model) { return model.name == desktop3DSelectedName; });
            if (selected != ui3DObjects.end()) {
                const double scale = std::max(8.0, desktop3DWorldScale);
                const double dx = point.x - desktop3DMouseDown.x;
                const double dy = point.y - desktop3DMouseDown.y;
                if (desktop3DToolMode == 1) {
                    const double horizontal = dx / scale;
                    selected->x = desktop3DDragStartX + horizontal * std::cos(desktop3DCameraYaw);
                    selected->z = desktop3DDragStartZ - horizontal * std::sin(desktop3DCameraYaw);
                    selected->y = desktop3DDragStartY - dy / scale;
                } else if (desktop3DToolMode == 2) {
                    selected->rotateY = desktop3DDragStartRotY + dx * 0.65;
                    selected->rotateX = desktop3DDragStartRotX + dy * 0.65;
                } else if (desktop3DToolMode == 3) {
                    const double factor = std::clamp(std::exp((dx - dy) * 0.004), 0.08, 12.0);
                    selected->width = std::max(0.05, desktop3DDragStartWidth * factor);
                    selected->height = std::max(0.05, desktop3DDragStartHeight * factor);
                    selected->depth = std::max(0.05, desktop3DDragStartDepth * factor);
                }
                syncDesktop3DEditorState();
            }
        }
        InvalidateRect(window, nullptr, FALSE);
    }
    if (message == WM_LBUTTONUP && desktop3DEngineActive && previewMode) {
        if (desktop3DAssetCandidate >= 0) {
            const POINT point = {static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))};
            double worldX = desktop3DCameraX + std::sin(desktop3DCameraYaw) * 12.0;
            double worldZ = desktop3DCameraZ + std::cos(desktop3DCameraYaw) * 12.0;
            double worldY = 0.0;
            if (desktop3DAssetDragging && PtInRect(&desktop3DViewportRect, point)) {
                const double scale = std::max(8.0, desktop3DWorldScale);
                const double centerX = (desktop3DViewportRect.left + desktop3DViewportRect.right) * 0.5;
                const double centerY = (desktop3DViewportRect.top + desktop3DViewportRect.bottom) * 0.5;
                const double screenX = (point.x - centerX) / scale;
                const double screenY = (centerY - point.y) / scale;
                worldX += std::cos(desktop3DCameraYaw) * screenX;
                worldZ -= std::sin(desktop3DCameraYaw) * screenX;
                worldY = std::max(-1.0, desktop3DCameraY + screenY);
            }
            if (!desktop3DAssetDragging || PtInRect(&desktop3DViewportRect, point))
                createDesktop3DAsset(desktop3DAssetCandidate, worldX, worldY, worldZ);
            desktop3DAssetCandidate = -1;
            desktop3DAssetDragging = false;
            desktop3DDraggingObject = false;
            if (GetCapture() == window) ReleaseCapture();
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        if (desktop3DDraggingObject) {
            desktop3DDraggingObject = false;
            if (GetCapture() == window) ReleaseCapture();
            syncDesktop3DEditorState();
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        desktop3DAssetCandidate = -1;
        desktop3DAssetDragging = false;
    }
    if (message == WM_MOUSEMOVE || message == WM_LBUTTONDOWN || message == WM_LBUTTONUP) {
        RECT client = {};
        GetClientRect(window, &client);
        const DesktopLayout layout = desktopLayout(window, client);
        const double canvasWidth = static_cast<double>(std::max<LONG>(1, layout.width));
        const double canvasHeight = static_cast<double>(std::max<LONG>(1, layout.canvasHeight));
        const double mouseX = static_cast<double>(static_cast<short>(LOWORD(lParam)));
        const double mouseY = static_cast<double>(static_cast<short>(HIWORD(lParam)));
        uiVariables["pointer_x"] = mouseX * 100.0 / canvasWidth + viewportX;
        uiVariables["pointer_y"] = (mouseY - layout.headerHeight) * 100.0 / canvasHeight + viewportY;
        uiVariables["pointer_down"] = (message == WM_LBUTTONDOWN ||
            (message == WM_MOUSEMOVE && (GetKeyState(VK_LBUTTON) & 0x8000))) ? "press" : "released";
        uiTypes["pointer_x"] = "num";
        uiTypes["pointer_y"] = "num";
        uiTypes["pointer_down"] = "word";
        if (message == WM_LBUTTONDOWN) SetCapture(window);
        if (message == WM_LBUTTONUP && GetCapture() == window) ReleaseCapture();
    }
    if (message == WM_LBUTTONUP && !uiButtons.empty()) {
        RECT client = {};
        GetClientRect(window, &client);
        const DesktopLayout layout = desktopLayout(window, client);
        const double canvasWidth = static_cast<double>(std::max<LONG>(1, layout.width));
        const double canvasHeight = static_cast<double>(layout.canvasHeight);
        const LONG mouseX = static_cast<short>(LOWORD(lParam));
        const LONG mouseY = static_cast<short>(HIWORD(lParam));
        for (auto button = uiButtons.rbegin(); button != uiButtons.rend(); ++button) {
            if (!button->visible || !uiGroupIsVisible(button->group)) continue;
            const double buttonX = button->x - viewportX;
            const double buttonY = button->y - viewportY;
            RECT area = {
                static_cast<LONG>(buttonX * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + buttonY * canvasHeight / 100.0),
                static_cast<LONG>((buttonX + button->width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight +
                    (buttonY + button->height) * canvasHeight / 100.0)
            };
            if (PtInRect(&area, POINT{mouseX, mouseY})) {
                clearDesktopOutput();
                appendOutput("--- " + button->label + " ---\r\n");
                conditionIsTrue("false", uiVariables);
                Flow flow;
                executeBlock(button->body, 0, button->body.size(), uiVariables, uiTypes,
                             uiDefaults, flow, false, true, button->blockIndex.get());
                if (desktop3DEngineActive) {
                    if (button->name == "reset_view") {
                        desktop3DCameraX = 0.0;
                        desktop3DCameraY = 0.0;
                        desktop3DCameraZ = -12.0;
                        desktop3DCameraYaw = 0.0;
                        desktop3DCameraPitch = 0.18;
                        desktop3DZoom = 1.0;
                    }
                    desktop3DSelectedName = static_cast<std::string>(uiVariables["selectedName"]);
                    syncDesktop3DEditorState();
                }
                refreshCollisionWatches(uiVariables, uiTypes);
                if (flow.stopped) everyTasks.clear();
                if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(window, 1, 16, nullptr);
                InvalidateRect(window, nullptr, TRUE);
                return 0;
            }
        }
    }
    if (message == WM_TIMER && wParam == 2) {
        KillTimer(window, 2);
        saveDesktopAutosave();
        return 0;
    }
    if (message == WM_TIMER) {
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
        const auto now = std::chrono::steady_clock::now();
        for (std::size_t index = 0; index < everyTasks.size();) {
            if (now < everyTasks[index].nextRun) { ++index; continue; }
            const EveryTask task = everyTasks[index];
            everyTasks[index].nextRun = now + task.interval;
            refreshCollisionWatches(uiVariables, uiTypes);
            Flow taskFlow;
            executeBlock(task.body, 0, task.body.size(), uiVariables, uiTypes,
                         uiDefaults, taskFlow, task.manualMode, task.uiMode, task.blockIndex.get());
            if (taskFlow.stopped) { everyTasks.clear(); break; }
            if (taskFlow.broke) everyTasks.erase(everyTasks.begin() + static_cast<std::ptrdiff_t>(index));
            else ++index;
        }
        const bool animationsFinished = std::all_of(uiAnimations.begin(), uiAnimations.end(),
            [&](const UiAnimation& animation) { return elapsed >= animation.startTime + animation.duration; });
        InvalidateRect(window, nullptr, FALSE);
        if (animationsFinished && everyTasks.empty()) KillTimer(window, 1);
        return 0;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(window, &paint);
        RECT client;
        GetClientRect(window, &client);
        if (standaloneTerminalMode) {
            HBRUSH terminalBackground = CreateSolidBrush(RGB(12, 9, 20));
            FillRect(dc, &client, terminalBackground);
            DeleteObject(terminalBackground);
            RECT terminalHeader = {0, 0, client.right, scaleForDpi(window, 66)};
            HBRUSH terminalHeaderBrush = CreateSolidBrush(RGB(35, 22, 58));
            FillRect(dc, &terminalHeader, terminalHeaderBrush);
            DeleteObject(terminalHeaderBrush);
            RECT accent = {0, 0, client.right, scaleForDpi(window, 4)};
            HBRUSH accentBrush = CreateSolidBrush(RGB(169, 125, 255));
            FillRect(dc, &accent, accentBrush);
            DeleteObject(accentBrush);
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            HFONT terminalTitleFont = CreateFontW(-scaleForDpi(window, 21), 0, 0, 0,
                FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
                L"Segoe UI");
            HGDIOBJ oldTerminalFont = SelectObject(dc, terminalTitleFont);
            RECT titleRect = {scaleForDpi(window, 18), scaleForDpi(window, 10),
                client.right - scaleForDpi(window, 18), scaleForDpi(window, 42)};
            DrawTextW(dc, L"TRG TERMINAL", -1, &titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SelectObject(dc, oldTerminalFont);
            DeleteObject(terminalTitleFont);
            SetTextColor(dc, RGB(198, 169, 242));
            HFONT subtitleFont = CreateFontW(-scaleForDpi(window, 10), 0, 0, 0,
                FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
                L"Segoe UI");
            HGDIOBJ oldSubtitleFont = SelectObject(dc, subtitleFont);
            RECT subtitleRect = {scaleForDpi(window, 20), scaleForDpi(window, 39),
                client.right - scaleForDpi(window, 18), scaleForDpi(window, 60)};
            DrawTextW(dc, L"LOCAL PROGRAM OUTPUT", -1, &subtitleRect,
                DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SelectObject(dc, oldSubtitleFont);
            DeleteObject(subtitleFont);
            EndPaint(window, &paint);
            return 0;
        }
        HBRUSH background = CreateSolidBrush(RGB(17, 12, 27));
        FillRect(dc, &client, background);
        DeleteObject(background);
        const DesktopLayout layout = desktopLayout(window, client);
        if (desktop3DEngineActive && previewMode) updateDesktop3DLayout(window, layout);
        RECT header = {0, 0, layout.width, layout.headerHeight};
        HBRUSH headerBrush = CreateSolidBrush(RGB(24, 17, 38));
        FillRect(dc, &header, headerBrush);
        DeleteObject(headerBrush);
        if (layout.headerHeight > 0) {
            RECT accentBar = {0, 0, layout.width, scaleForDpi(window, 4)};
            HBRUSH accentBrush = CreateSolidBrush(RGB(155, 105, 255));
            FillRect(dc, &accentBar, accentBrush);
            DeleteObject(accentBrush);
        }
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(255, 255, 255));
        HFONT titleFont = CreateFontW(-scaleForDpi(window, desktop3DEngineActive && previewMode ? 23 : 32), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousFont = SelectObject(dc, titleFont);
        RECT title = {
            scaleForDpi(window, 24), scaleForDpi(window, 7),
            scaleForDpi(window, 230), layout.headerHeight - scaleForDpi(window, 28)
        };
        DrawTextW(dc, desktop3DEngineActive && previewMode ? L"TRG ENGINE" : L"TRG",
            -1, &title, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, previousFont);
        DeleteObject(titleFont);
        if (desktop3DEngineActive && previewMode) {
            const wchar_t* tabs[] = {L"HOME", L"MODEL", L"TERRAIN", L"VIEW", L"PLUGINS"};
            LONG tabX = scaleForDpi(window, 250);
            for (int index = 0; index < 5; ++index) {
                RECT tab = {tabX, scaleForDpi(window, 9),
                    tabX + scaleForDpi(window, index == 0 ? 82 : 88),
                    scaleForDpi(window, 43)};
                if (index == desktop3DActiveTab) {
                    HBRUSH activeTab = CreateSolidBrush(RGB(75, 39, 121));
                    HPEN activePen = CreatePen(PS_SOLID, 1, RGB(142, 83, 224));
                    HGDIOBJ oldB = SelectObject(dc, activeTab);
                    HGDIOBJ oldP = SelectObject(dc, activePen);
                    RoundRect(dc, tab.left, tab.top, tab.right, tab.bottom,
                        scaleForDpi(window, 6), scaleForDpi(window, 6));
                    SelectObject(dc, oldB); SelectObject(dc, oldP);
                    DeleteObject(activeTab); DeleteObject(activePen);
                }
                SetBkMode(dc, TRANSPARENT);
                SetTextColor(dc, index == 0 ? RGB(255, 247, 255) : RGB(190, 166, 222));
                HFONT tabFont = CreateFontW(-scaleForDpi(window, 11), 0, 0, 0,
                    FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
                HGDIOBJ oldTabFont = SelectObject(dc, tabFont);
                DrawTextW(dc, tabs[index], -1, &tab, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(dc, oldTabFont);
                DeleteObject(tabFont);
                tabX += scaleForDpi(window, 94);
            }
            RECT subtitle = {scaleForDpi(window, 24), scaleForDpi(window, 47),
                scaleForDpi(window, 500), scaleForDpi(window, 72)};
            SetTextColor(dc, RGB(132, 111, 163));
            HFONT subtitleFont = CreateFontW(-scaleForDpi(window, 9), 0, 0, 0,
                FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HGDIOBJ oldSubtitleFont = SelectObject(dc, subtitleFont);
            DrawTextW(dc, L"DESKTOP 3D STUDIO  /  PROJECT WORKSPACE", -1, &subtitle,
                DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SelectObject(dc, oldSubtitleFont);
            DeleteObject(subtitleFont);
        }
        const LONG panelTop = layout.panelTop;
        RECT canvas = {0, layout.headerHeight, layout.width, panelTop};
        HBRUSH canvasBrush = CreateSolidBrush(RGB(15, 10, 25));
        FillRect(dc, &canvas, canvasBrush);
        DeleteObject(canvasBrush);
        HFONT bodyFont = CreateFontW(-scaleForDpi(window, 13), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousBodyFont = bodyFont ? SelectObject(dc, bodyFont) : nullptr;
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
       
        for (auto it = particles.begin(); it != particles.end();) {
            it->x += it->vx;
            it->y += it->vy;
            it->life -= 0.016;
            if (it->life <= 0) it = particles.erase(it);
            else ++it;
        }
        std::vector<const UiBlock*> ordered;
        for (const UiBlock& block : uiBlocks) ordered.push_back(&block);
        std::stable_sort(ordered.begin(), ordered.end(), [](const UiBlock* a, const UiBlock* b) { return a->z < b->z; });
        const double canvasWidth = static_cast<double>(std::max<LONG>(1, layout.width));
        const double canvasHeight = static_cast<double>(layout.canvasHeight);
        for (const UiBlock* block : ordered) {
            if (!block->visible || !uiGroupIsVisible(block->group)) continue;
            unsigned red = 0, green = 0, blue = 0;
            if (!parseColor(block->color, red, green, blue)) continue;
            const auto position = animatedPosition(*block, elapsed);
            
            double bx = position.first - viewportX;
            double by = position.second - viewportY;
            RECT area = {
                static_cast<LONG>(bx * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + by * canvasHeight / 100.0),
                static_cast<LONG>((bx + block->width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + (by + block->height) * canvasHeight / 100.0)
            };
            if (block->glowIntensity > 0 && !block->glowColor.empty()) {
                unsigned gr = 0, gg = 0, gb = 0;
                if (parseColor(block->glowColor, gr, gg, gb)) {
                    int glowPad = static_cast<int>(block->glowIntensity * 12.0);
                    RECT glowArea = { area.left - glowPad, area.top - glowPad, area.right + glowPad, area.bottom + glowPad };
                    HBRUSH glowBrush = CreateSolidBrush(RGB(gr, gg, gb));
                    HPEN glowPen = CreatePen(PS_SOLID, 1, RGB(gr, gg, gb));
                    HGDIOBJ oldPen2 = SelectObject(dc, glowPen);
                    HGDIOBJ oldBrush2 = SelectObject(dc, glowBrush);
                    if (block->shape == "circle") {
                        const LONG diameter = std::min(glowArea.right - glowArea.left,
                                                        glowArea.bottom - glowArea.top);
                        const LONG cx = (glowArea.left + glowArea.right) / 2;
                        const LONG cy = (glowArea.top + glowArea.bottom) / 2;
                        Ellipse(dc, cx - diameter / 2, cy - diameter / 2,
                            cx + diameter / 2, cy + diameter / 2);
                    } else if (block->shape == "square" || block->shape == "rect") {
                        RoundRect(dc, glowArea.left, glowArea.top, glowArea.right, glowArea.bottom, glowPad * 2, glowPad * 2);
                    } else {
                        Rectangle(dc, glowArea.left, glowArea.top, glowArea.right, glowArea.bottom);
                    }
                    SelectObject(dc, oldBrush2); SelectObject(dc, oldPen2);
                    DeleteObject(glowPen); DeleteObject(glowBrush);
                }
            }
            HBRUSH brush = CreateSolidBrush(RGB(red, green, blue));
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
            HGDIOBJ oldPen = SelectObject(dc, pen);
            HGDIOBJ oldBrush = SelectObject(dc, brush);
            const int centerX = (area.left + area.right) / 2;
            const int centerY = (area.top + area.bottom) / 2;
            const int radius = std::max(1, static_cast<int>(std::min(block->width, block->height) * std::min(canvasWidth, canvasHeight) / 200.0));
            if (block->shape == "circle") {
                const LONG diameter = std::min(area.right - area.left, area.bottom - area.top);
                const LONG circleX = (area.left + area.right) / 2;
                const LONG circleY = (area.top + area.bottom) / 2;
                Ellipse(dc, circleX - diameter / 2, circleY - diameter / 2,
                    circleX + diameter / 2, circleY + diameter / 2);
            } else if (block->shape == "triangle") {
                POINT tri[3] = { {centerX, area.top}, {area.left, area.bottom}, {area.right, area.bottom} };
                Polygon(dc, tri, 3);
            } else if (block->shape == "pentagon") {
                POINT pent[5];
                for (int k = 0; k < 5; ++k) {
                    double angle = -90.0 + k * 72.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    pent[k].x = centerX + static_cast<LONG>(radius * cos(rad));
                    pent[k].y = centerY + static_cast<LONG>(radius * sin(rad));
                }
                Polygon(dc, pent, 5);
            } else if (block->shape == "hexagon") {
                POINT hex[6];
                for (int k = 0; k < 6; ++k) {
                    double angle = -90.0 + k * 60.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    hex[k].x = centerX + static_cast<LONG>(radius * cos(rad));
                    hex[k].y = centerY + static_cast<LONG>(radius * sin(rad));
                }
                Polygon(dc, hex, 6);
            } else if (block->shape == "star") {
                POINT star[10];
                for (int k = 0; k < 10; ++k) {
                    double angle = -90.0 + k * 36.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    double r = (k % 2 == 0) ? radius : radius * 0.4;
                    star[k].x = centerX + static_cast<LONG>(r * cos(rad));
                    star[k].y = centerY + static_cast<LONG>(r * sin(rad));
                }
                Polygon(dc, star, 10);
            } else {
                if (block->cornerRadius > 0) {
                    const int rad = std::max(1, static_cast<int>(block->cornerRadius * std::min(canvasWidth, canvasHeight) / 100.0));
                    RoundRect(dc, area.left, area.top, area.right, area.bottom, rad * 2, rad * 2);
                } else {
                    Rectangle(dc, area.left, area.top, area.right, area.bottom);
                }
            }
            SelectObject(dc, oldBrush); SelectObject(dc, oldPen);
            DeleteObject(pen); DeleteObject(brush);
            
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            const std::wstring name = wideFromUtf8(block->name);
            DrawTextW(dc, name.c_str(), -1, &area, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        
        if (gdiplusToken != 0) {
            Gdiplus::Graphics graphics(dc);
            for (const Particle& p : particles) {
                unsigned pr = 0, pg = 0, pb = 0;
                if (!parseColor(p.color, pr, pg, pb)) continue;
                Gdiplus::Color gcolor(static_cast<BYTE>(255 * (p.life / p.maxLife)), pr, pg, pb);
                Gdiplus::SolidBrush brush(gcolor);
                double px = p.x - viewportX;
                double py = p.y - viewportY;
                Gdiplus::RectF rect(
                    static_cast<Gdiplus::REAL>(px * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(layout.headerHeight + py * canvasHeight / 100.0),
                    static_cast<Gdiplus::REAL>(p.size * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(p.size * canvasHeight / 100.0));
                graphics.FillEllipse(&brush, rect);
            }
        }
        if (gdiplusToken != 0) {
            Gdiplus::Graphics graphics(dc);
            graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
            for (const UiSprite& sprite : uiSprites) {
                if (!sprite.visible || !uiGroupIsVisible(sprite.group)) continue;
                auto& image = desktopSpriteCache[sprite.path];
                if (!image) {
                    const std::wstring path = std::filesystem::u8path(sprite.path).wstring();
                    image = std::make_unique<Gdiplus::Image>(path.c_str());
                }
                if (image->GetLastStatus() != Gdiplus::Ok) continue;
                const double naturalWidth = std::max<UINT>(1, image->GetWidth());
                const double naturalHeight = std::max<UINT>(1, image->GetHeight());
                const double aspect = naturalWidth / naturalHeight;
                double drawWidth = sprite.widthAuto ? naturalWidth : sprite.width * canvasWidth / 100.0;
                double drawHeight = sprite.heightAuto ? naturalHeight : sprite.height * canvasHeight / 100.0;
                if (sprite.widthAuto && !sprite.heightAuto) {
                    drawWidth = drawHeight * aspect;
                } else if (!sprite.widthAuto && sprite.heightAuto) {
                    drawHeight = drawWidth / aspect;
                } else if (sprite.widthAuto && sprite.heightAuto) {
                    const double fit = std::min({1.0,
                        static_cast<double>(canvasWidth) / naturalWidth,
                        static_cast<double>(canvasHeight) / naturalHeight});
                    drawWidth *= fit;
                    drawHeight *= fit;
                }
                if (drawWidth <= 0 || drawHeight <= 0) continue;
                const Gdiplus::RectF destination(
                    static_cast<Gdiplus::REAL>(sprite.x * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(layout.headerHeight + sprite.y * canvasHeight / 100.0),
                    static_cast<Gdiplus::REAL>(drawWidth),
                    static_cast<Gdiplus::REAL>(drawHeight));
                graphics.DrawImage(image.get(), destination);
            }
        }
        const int sceneClipSave = SaveDC(dc);
        const LONG viewportLeft = desktop3DEngineActive && previewMode
            ? desktop3DViewportRect.left : static_cast<LONG>(canvasWidth * 0.19);
        const LONG viewportRight = desktop3DEngineActive && previewMode
            ? desktop3DViewportRect.right : static_cast<LONG>(canvasWidth * 0.80);
        const LONG viewportTop = desktop3DEngineActive && previewMode
            ? desktop3DViewportRect.top : layout.headerHeight + static_cast<LONG>(canvasHeight * 0.13);
        const LONG viewportBottom = desktop3DEngineActive && previewMode
            ? desktop3DViewportRect.bottom : layout.headerHeight + static_cast<LONG>(canvasHeight * 0.78);
        if (desktop3DEngineActive && previewMode) {
            desktop3DProjectedHits.clear();
            IntersectClipRect(dc, viewportLeft, viewportTop, viewportRight, viewportBottom);
            drawDesktop3DGrid(dc);
        }
        const std::array<std::array<int, 4>, 6> modelFaces = {{
            {{0, 1, 2, 3}}, {{4, 5, 6, 7}}, {{0, 3, 7, 4}},
            {{1, 2, 6, 5}}, {{0, 1, 5, 4}}, {{3, 2, 6, 7}}
        }};
        const std::array<double, 6> faceLight = {{0.58, 1.0, 0.68, 0.78, 0.52, 0.90}};
        for (const Ui3DObject& model : ui3DObjects) {
            if (!model.visible || !uiGroupIsVisible(model.group)) continue;
            unsigned red = 0, green = 0, blue = 0;
            if (!parseColor(model.color, red, green, blue)) continue;
            const double rx = model.rotateX * 3.141592653589793 / 180.0;
            const double ry = model.rotateY * 3.141592653589793 / 180.0;
            const double rz = model.rotateZ * 3.141592653589793 / 180.0;
            const double cosineX = std::cos(rx), sineX = std::sin(rx);
            const double cosineY = std::cos(ry), sineY = std::sin(ry);
            const double cosineZ = std::cos(rz), sineZ = std::sin(rz);
            const double halfW = model.width * 0.5, halfH = model.height * 0.5, halfD = model.depth * 0.5;
            const std::array<std::array<double, 3>, 8> local = {{
                {{-halfW,-halfH,-halfD}}, {{halfW,-halfH,-halfD}},
                {{halfW,halfH,-halfD}}, {{-halfW,halfH,-halfD}},
                {{-halfW,-halfH,halfD}}, {{halfW,-halfH,halfD}},
                {{halfW,halfH,halfD}}, {{-halfW,halfH,halfD}}
            }};
            std::array<std::array<double, 3>, 8> rotated{};
            std::array<POINT, 8> projected{};
            const double viewWidth = std::max<LONG>(1, viewportRight - viewportLeft);
            const double viewHeight = std::max<LONG>(1, viewportBottom - viewportTop);
            const double modelScale = std::min(viewWidth, viewHeight) * 0.19 * desktop3DZoom;
            desktop3DWorldScale = modelScale;
            const double centerX = (viewportLeft + viewportRight) * 0.5;
            const double centerY = (viewportTop + viewportBottom) * 0.5;
            for (std::size_t vertex = 0; vertex < local.size(); ++vertex) {
                const double x1 = local[vertex][0];
                const double y1 = local[vertex][1] * cosineX - local[vertex][2] * sineX;
                const double z1 = local[vertex][1] * sineX + local[vertex][2] * cosineX;
                const double x2 = x1 * cosineY + z1 * sineY;
                const double y2 = y1;
                const double z2 = -x1 * sineY + z1 * cosineY;
                const double x3 = x2 * cosineZ - y2 * sineZ + model.x;
                const double y3 = x2 * sineZ + y2 * cosineZ + model.y;
                const double z3 = z2 + model.z;
                rotated[vertex] = {{x3, y3, z3}};
                const double dx = x3 - desktop3DCameraX;
                const double dy = y3 - desktop3DCameraY;
                const double dz = z3 - desktop3DCameraZ;
                const double cosYaw = std::cos(desktop3DCameraYaw);
                const double sinYaw = std::sin(desktop3DCameraYaw);
                const double viewX = dx * cosYaw - dz * sinYaw;
                const double forwardZ = dx * sinYaw + dz * cosYaw;
                const double cosPitch = std::cos(desktop3DCameraPitch);
                const double sinPitch = std::sin(desktop3DCameraPitch);
                const double viewY = dy * cosPitch - forwardZ * sinPitch;
                const double depth = forwardZ * cosPitch + dy * sinPitch;
                const double perspective = depth > 0.25 ? 12.0 / depth : 0.0;
                projected[vertex].x = static_cast<LONG>(centerX + viewX * modelScale * perspective);
                projected[vertex].y = static_cast<LONG>(centerY - viewY * modelScale * perspective);
            }
            LONG minX = projected[0].x, maxX = projected[0].x;
            LONG minY = projected[0].y, maxY = projected[0].y;
            for (const POINT& point : projected) {
                minX = std::min(minX, point.x); maxX = std::max(maxX, point.x);
                minY = std::min(minY, point.y); maxY = std::max(maxY, point.y);
            }
            RECT objectHit = {minX - 8, minY - 8, maxX + 8, maxY + 8};
            RECT clippedHit = {};
            if (IntersectRect(&clippedHit, &objectHit, &desktop3DViewportRect))
                desktop3DProjectedHits.emplace_back(model.name, clippedHit);
            const LONG shapeCenterX = (minX + maxX) / 2;
            const LONG shapeCenterY = (minY + maxY) / 2;
            if (model.shape == "sphere" || model.shape == "spawn") {
                const LONG diameter = std::max<LONG>(10, std::min(maxX - minX, maxY - minY));
                RECT sphereArea = {shapeCenterX - diameter / 2, shapeCenterY - diameter / 2,
                    shapeCenterX + diameter / 2, shapeCenterY + diameter / 2};
                HBRUSH sphereBrush = CreateSolidBrush(RGB(red, green, blue));
                HPEN spherePen = CreatePen(PS_SOLID, 2, RGB(47, 34, 70));
                HGDIOBJ oldBrush = SelectObject(dc, sphereBrush), oldPen = SelectObject(dc, spherePen);
                if (model.shape == "spawn") {
                    SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
                    Ellipse(dc, sphereArea.left - 5, sphereArea.top - 5,
                        sphereArea.right + 5, sphereArea.bottom + 5);
                    MoveToEx(dc, shapeCenterX, sphereArea.top - 12, nullptr);
                    LineTo(dc, shapeCenterX, sphereArea.bottom + 12);
                    SelectObject(dc, sphereBrush);
                    Ellipse(dc, sphereArea.left, sphereArea.top, sphereArea.right, sphereArea.bottom);
                } else {
                    Ellipse(dc, sphereArea.left, sphereArea.top, sphereArea.right, sphereArea.bottom);
                    HPEN detailPen = CreatePen(PS_SOLID, 1, RGB(220, 206, 255));
                    HGDIOBJ previousDetailPen = SelectObject(dc, detailPen);
                    Arc(dc, sphereArea.left + diameter / 5, sphereArea.top + diameter / 5,
                        sphereArea.right - diameter / 5, sphereArea.bottom - diameter / 5,
                        sphereArea.left, shapeCenterY, sphereArea.right, shapeCenterY);
                    if (model.texture != "smooth") {
                        int sphereHatch = model.texture == "grid" ? HS_CROSS :
                            model.texture == "wood" ? HS_FDIAGONAL :
                            model.texture == "stone" ? HS_BDIAGONAL : HS_DIAGCROSS;
                        HBRUSH textureBrush = CreateHatchBrush(sphereHatch,
                            RGB(static_cast<BYTE>(std::min(255u, red + 34u)),
                                static_cast<BYTE>(std::min(255u, green + 34u)),
                                static_cast<BYTE>(std::min(255u, blue + 34u))));
                        HGDIOBJ previousTextureBrush = SelectObject(dc, textureBrush);
                        const int previousMode = SetBkMode(dc, TRANSPARENT);
                        Ellipse(dc, sphereArea.left, sphereArea.top, sphereArea.right, sphereArea.bottom);
                        SetBkMode(dc, previousMode);
                        SelectObject(dc, previousTextureBrush);
                        DeleteObject(textureBrush);
                    }
                    SelectObject(dc, previousDetailPen);
                    DeleteObject(detailPen);
                }
                if (desktop3DEngineActive && model.name == desktop3DSelectedName) {
                    HPEN selectedPen = CreatePen(PS_SOLID, 2, RGB(196, 164, 255));
                    HGDIOBJ oldSelectedPen = SelectObject(dc, selectedPen);
                    HGDIOBJ oldSelectedBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
                    Ellipse(dc, sphereArea.left - 5, sphereArea.top - 5,
                        sphereArea.right + 5, sphereArea.bottom + 5);
                    SelectObject(dc, oldSelectedBrush);
                    SelectObject(dc, oldSelectedPen);
                    DeleteObject(selectedPen);
                }
                SelectObject(dc, oldBrush); SelectObject(dc, oldPen);
                DeleteObject(sphereBrush); DeleteObject(spherePen);
                continue;
            }
            if (model.shape == "camera") {
                HPEN cameraPen = CreatePen(PS_SOLID, 2, RGB(red, green, blue));
                HGDIOBJ oldCameraPen = SelectObject(dc, cameraPen);
                SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
                Rectangle(dc, shapeCenterX - 14, shapeCenterY - 9, shapeCenterX + 12, shapeCenterY + 9);
                POINT lens[3] = {{shapeCenterX + 12, shapeCenterY - 5},
                    {shapeCenterX + 24, shapeCenterY - 12}, {shapeCenterX + 24, shapeCenterY + 12}};
                Polyline(dc, lens, 3);
                MoveToEx(dc, shapeCenterX + 24, shapeCenterY + 12, nullptr);
                LineTo(dc, shapeCenterX + 12, shapeCenterY + 5);
                if (desktop3DEngineActive && model.name == desktop3DSelectedName) {
                    HPEN selectionPen = CreatePen(PS_SOLID, 2, RGB(196, 164, 255));
                    HGDIOBJ oldSelectionPen = SelectObject(dc, selectionPen);
                    HGDIOBJ oldSelectionBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
                    Rectangle(dc, shapeCenterX - 19, shapeCenterY - 14,
                        shapeCenterX + 29, shapeCenterY + 14);
                    SelectObject(dc, oldSelectionBrush);
                    SelectObject(dc, oldSelectionPen);
                    DeleteObject(selectionPen);
                }
                SelectObject(dc, oldCameraPen);
                DeleteObject(cameraPen);
                continue;
            }
            std::array<int, 6> faceOrder = {{0, 1, 2, 3, 4, 5}};
            std::stable_sort(faceOrder.begin(), faceOrder.end(), [&](int first, int second) {
                double firstDepth = 0, secondDepth = 0;
                for (int vertex : modelFaces[first]) firstDepth += rotated[vertex][2];
                for (int vertex : modelFaces[second]) secondDepth += rotated[vertex][2];
                return firstDepth < secondDepth;
            });
            for (int faceIndex : faceOrder) {
                POINT polygon[4];
                for (int corner = 0; corner < 4; ++corner) polygon[corner] = projected[modelFaces[faceIndex][corner]];
                const double shade = faceLight[faceIndex] * std::clamp(model.opacity, 0.0, 1.0);
                const COLORREF faceColor = RGB(static_cast<BYTE>(red * shade),
                    static_cast<BYTE>(green * shade), static_cast<BYTE>(blue * shade));
                HBRUSH faceBrush = CreateSolidBrush(faceColor);
                HPEN facePen = CreatePen(PS_SOLID, 1, RGB(38, 27, 57));
                HGDIOBJ oldBrush = SelectObject(dc, faceBrush), oldPen = SelectObject(dc, facePen);
                Polygon(dc, polygon, 4);
                if (model.texture != "smooth") {
                    int hatch = HS_DIAGCROSS;
                    if (model.texture == "grid") hatch = HS_CROSS;
                    else if (model.texture == "wood") hatch = HS_FDIAGONAL;
                    else if (model.texture == "stone") hatch = HS_BDIAGONAL;
                    HBRUSH textureBrush = CreateHatchBrush(hatch,
                        RGB(static_cast<BYTE>(std::min(255u, red + 28u)),
                            static_cast<BYTE>(std::min(255u, green + 28u)),
                            static_cast<BYTE>(std::min(255u, blue + 28u))));
                    HGDIOBJ oldTextureBrush = SelectObject(dc, textureBrush);
                    const int previousBackgroundMode = SetBkMode(dc, TRANSPARENT);
                    Polygon(dc, polygon, 4);
                    SetBkMode(dc, previousBackgroundMode);
                    SelectObject(dc, oldTextureBrush);
                    DeleteObject(textureBrush);
                }
                if (desktop3DEngineActive && model.name == desktop3DSelectedName) {
                    HPEN selectionPen = CreatePen(PS_SOLID, 2, RGB(196, 164, 255));
                    HGDIOBJ oldSelectionPen = SelectObject(dc, selectionPen);
                    HGDIOBJ oldHollowBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
                    Polygon(dc, polygon, 4);
                    SelectObject(dc, oldHollowBrush);
                    SelectObject(dc, oldSelectionPen);
                    DeleteObject(selectionPen);
                }
                SelectObject(dc, oldBrush); SelectObject(dc, oldPen); DeleteObject(faceBrush); DeleteObject(facePen);
            }
        }
        if (sceneClipSave) RestoreDC(dc, sceneClipSave);
        for (const UiButton& button : uiButtons) {
            if (!button.visible || !uiGroupIsVisible(button.group)) continue;
            const double buttonX = button.x - viewportX;
            const double buttonY = button.y - viewportY;
            RECT area = {
                static_cast<LONG>(buttonX * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + buttonY * canvasHeight / 100.0),
                static_cast<LONG>((buttonX + button.width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight +
                    (buttonY + button.height) * canvasHeight / 100.0)
            };
            unsigned red = 0, green = 0, blue = 0;
            parseColor(button.color, red, green, blue);
            const double opacity = std::clamp(button.opacity, 0.0, 1.0);
            const COLORREF blended = RGB(
                static_cast<BYTE>(red * opacity + 38 * (1.0 - opacity)),
                static_cast<BYTE>(green * opacity + 36 * (1.0 - opacity)),
                static_cast<BYTE>(blue * opacity + 44 * (1.0 - opacity)));
            HBRUSH buttonBrush = CreateSolidBrush(blended);
            if (button.cornerRadius > 0) {
                HPEN pen = CreatePen(PS_SOLID, 1, blended);
                HGDIOBJ oldPen = SelectObject(dc, pen);
                HGDIOBJ oldBrush = SelectObject(dc, buttonBrush);
                const int radius = std::max(1, static_cast<int>(button.cornerRadius * systemDpi() / 96.0));
                RoundRect(dc, area.left, area.top, area.right, area.bottom, radius * 2, radius * 2);
                SelectObject(dc, oldBrush); SelectObject(dc, oldPen); DeleteObject(pen);
            } else FillRect(dc, &area, buttonBrush);
            DeleteObject(buttonBrush);
            if (button.cornerRadius <= 0) FrameRect(dc, &area, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            const std::wstring label = wideFromUtf8(button.label);
            DrawTextW(dc, label.c_str(), -1, &area, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        }
        for (const UiText& text : uiTexts) {
            if (!text.visible || !uiGroupIsVisible(text.group)) continue;
            unsigned red = 0, green = 0, blue = 0;
            if (!parseColor(text.color, red, green, blue)) continue;
            const int fontHeight = std::max(1, static_cast<int>(
                text.size * canvasHeight / 100.0));
            HFONT font = CreateFontW(-fontHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HGDIOBJ previousFont = SelectObject(dc, font);
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(red, green, blue));
            const std::wstring value = wideFromUtf8(text.value);
            const LONG x = static_cast<LONG>(text.x * canvasWidth / 100.0);
            const LONG baseline = static_cast<LONG>(
                layout.headerHeight + text.y * canvasHeight / 100.0);
            RECT area = {x, baseline - fontHeight, client.right, baseline + fontHeight / 4};
            DrawTextW(dc, value.c_str(), -1, &area, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
            SelectObject(dc, previousFont);
            DeleteObject(font);
        }
        if (desktop3DEngineActive && previewMode)
            drawDesktop3DWorkspaceChrome(dc, window, layout);

        RECT outputHeader = {layout.margin, layout.panelTop,
            std::max<LONG>(layout.margin, layout.width - layout.margin),
            layout.panelTop + scaleForDpi(window, 25)};
        HBRUSH outputHeaderBrush = CreateSolidBrush(RGB(34, 25, 48));
        FillRect(dc, &outputHeader, outputHeaderBrush);
        DeleteObject(outputHeaderBrush);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(190, 160, 255));
        HFONT terminalHeaderFont = CreateFontW(-scaleForDpi(window, 11), 0, 0, 0, FW_BOLD,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousTerminalFont = SelectObject(dc, terminalHeaderFont);
        RECT terminalCaption = outputHeader;
        InflateRect(&terminalCaption, -scaleForDpi(window, 9), 0);
        DrawTextW(dc, L"OUTPUT  /  TERMINAL", -1, &terminalCaption,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, previousTerminalFont);
        DeleteObject(terminalHeaderFont);
        if (bodyFont) {
            SelectObject(dc, previousBodyFont);
            DeleteObject(bodyFont);
        }
        EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_CLOSE) {
        KillTimer(window, 2);
        saveDesktopAutosave();
        DestroyWindow(window);
        return 0;
    }
    if (message == WM_DESTROY) {
        mainWindow = nullptr;
        codeEditor = nullptr;
        openButton = saveButton = runButton = previewButton = newButton = buildButton = dictionaryButton = nullptr;
        editor2DButton = editor3DButton = nullptr;
        exportButton = outputControl = terminalInputControl = terminalSendButton = nullptr;
        previousOutputProc = nullptr;
        dictionaryWindow = dictionaryEdit = nullptr;
        if (terminalInputBrush) DeleteObject(terminalInputBrush);
        if (outputControlBrush) DeleteObject(outputControlBrush);
        terminalInputBrush = nullptr;
        outputControlBrush = nullptr;
        desktopSpriteCache.clear();
        if (gdiplusToken != 0) {
            Gdiplus::GdiplusShutdown(gdiplusToken);
            gdiplusToken = 0;
        }
        if (openButtonFont) {
            DeleteObject(openButtonFont);
            openButtonFont = nullptr;
        }
        if (codeEditorFont) {
            DeleteObject(codeEditorFont);
            codeEditorFont = nullptr;
        }
        if (richEditModule) { FreeLibrary(richEditModule); richEditModule = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

LRESULT CALLBACK inputWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        const auto scale = [window](int value) { return scaleForDpi(window, value); };
        CreateWindowExW(0, L"STATIC", inputPrompt.c_str(), WS_CHILD | WS_VISIBLE,
            scale(16), scale(16), scale(400), scale(36),
            window, nullptr, GetModuleHandleW(nullptr), nullptr);
        inputControl = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
            scale(16), scale(58), scale(400), scale(28),
            window, reinterpret_cast<HMENU>(inputEditId),
            GetModuleHandleW(nullptr), nullptr);
        CreateWindowExW(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            scale(330), scale(100), scale(86), scale(30),
            window, reinterpret_cast<HMENU>(inputButtonId),
            GetModuleHandleW(nullptr), nullptr);
        SetFocus(inputControl);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == inputButtonId) {
        const int length = GetWindowTextLengthW(inputControl);
        std::wstring answer(static_cast<std::size_t>(length) + 1, L'\0');
        GetWindowTextW(inputControl, &answer[0], length + 1);
        answer.resize(static_cast<std::size_t>(length));
        inputResult = utf8FromWide(answer);
        inputAccepted = true;
        inputFinished = true;
        DestroyWindow(window);
        return 0;
    }
    if (message == WM_CLOSE) {
        inputAccepted = false;
        inputFinished = true;
        DestroyWindow(window);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

bool askForInput(const std::string& prompt, std::string& answer) {
#ifdef TRG_TEST_CONSOLE
    appendOutput(prompt + " ");
    return static_cast<bool>(std::getline(std::cin, answer));
#else
    if (!mainWindow || !terminalInputControl || !terminalSendButton) {
        appendOutput(prompt + "\r\n[Input unavailable: desktop terminal is not active.]\r\n");
        return false;
    }
    inputFinished = false;
    inputAccepted = false;
    inputResult.clear();
    terminalInputActive = true;
    appendOutput(prompt + "\r\n");
    SetWindowTextW(terminalInputControl, L"");
    layoutOutputControls(mainWindow);
    ShowWindow(terminalInputControl, SW_SHOW);
    ShowWindow(terminalSendButton, SW_SHOW);
    SetFocus(terminalInputControl);
    MSG message;
    while (!inputFinished) {
        const BOOL result = GetMessageW(&message, nullptr, 0, 0);
        if (result <= 0) {
            inputFinished = true;
            inputAccepted = false;
            break;
        }
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    terminalInputActive = false;
    layoutOutputControls(mainWindow);
    if (mainWindow) SetFocus(mainWindow);
    if (inputAccepted) answer = inputResult;
    return inputAccepted;
#endif
}

bool runUi(bool showImmediately = true, bool restoreEditorState = true) {
    restoreDesktopStateOnCreate = restoreEditorState;
    enablePerMonitorDpiAwareness();
    Gdiplus::GdiplusStartupInput gdiplusInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, nullptr);
    const wchar_t* className = L"TRGMainWindow";
    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = mainWindowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.hIcon = LoadIconW(windowClass.hInstance, MAKEINTRESOURCEW(IDI_TRG));
    if (!RegisterClassW(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return false;
    const int dpi = systemDpi();
    const DWORD windowStyle = WS_OVERLAPPEDWINDOW |
        (showImmediately ? WS_VISIBLE : 0);
    mainWindow = CreateWindowExW(0, className, L"TRG", windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, MulDiv(1000, dpi, 96), MulDiv(720, dpi, 96),
        nullptr, nullptr, windowClass.hInstance, nullptr);
    if (!mainWindow) return false;
    uiStart = std::chrono::steady_clock::now();
    int fps = 60;
    for (const UiAnimation& animation : uiAnimations) fps = std::max(fps, animation.fps);
    if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(mainWindow, 1, static_cast<UINT>(std::max(1, 1000 / fps)), nullptr);
    if (showImmediately) {
        ShowWindow(mainWindow, SW_SHOWNORMAL);
        SetWindowPos(mainWindow, HWND_TOP, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        BringWindowToTop(mainWindow);
        SetForegroundWindow(mainWindow);
        UpdateWindow(mainWindow);
        if (!standaloneTerminalMode)
            appendOutput("TRG is ready. Output and errors appear here.\r\n");
    } else {
        layoutOutputControls(mainWindow);
        refreshDisplayVariables(uiVariables, uiTypes);
    }
    return true;
}

void runMessageLoop() {
    MSG message;
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

#else
bool askForInput(const std::string& prompt, std::string& answer) {
#ifdef __ANDROID__
    bool accepted = false;
    answer = trgAskAndroid(prompt, accepted);
    return accepted;
#else
    appendOutput(prompt + " ");
    return static_cast<bool>(std::getline(std::cin, answer));
#endif
}
#endif

std::vector<std::string> readProgram(std::istream& source) {
    std::vector<std::string> rawLines;
    std::string line;
    bool firstLine = true;
    while (std::getline(source, line)) {
        if (firstLine && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF)
            line.erase(0, 3);
        firstLine = false;
        rawLines.push_back(stripComment(line));
    }

    // Preserve source exactly as written. TRG never rewrites command names.
    std::vector<std::string> program;
    program.reserve(rawLines.size());
    for (std::string& cleanLine : rawLines) program.push_back(std::move(cleanLine));
    return program;
}
void executeProgram(const std::vector<std::string>& program) {
    uiStart = std::chrono::steady_clock::now();
#ifdef __ANDROID__
    androidStopRequested.store(false);
#endif
    uiVariables.clear();
    uiTypes.clear();
    uiDefaults = standardDefaults();
    refreshDisplayVariables(uiVariables, uiTypes);
    keyboardControls.clear();
    controllerControls.clear();
    controlsEnabled = false;
    filesEnabled = false;
    jsonEnabled = false;
    databaseEnabled = false;
    trgDatabaseTables.clear();
    trgDatabasePath.clear();
    webEnabled = false;
    collisionEnabled = false;
    physicsEnabled = false;
    physicsBodies.clear();
    collisionWatches.clear();
    webPageHtml.clear();
    Flow flow;
    errorCount = 0;
    trgCallStack.clear();
    uiBlocks.clear();
    ui3DObjects.clear();
    uiAutoNameCounter = 1;
    threeDAutoNameCounter = 1;
    uiSprites.clear();
    uiGroups.clear();
    currentUiGroup = "default";
#ifdef _WIN32
    desktopSpriteCache.clear();
#endif
    uiAnimations.clear();
    everyTasks.clear();
    uiButtons.clear();
    uiTexts.clear();
    userStructs.clear();
    variableGroups.clear();
    groupStack.clear();
    groupSnapshots.clear();
    executedFileKeys.clear();
    activeRunFiles.clear();

    
    
    
#ifdef TRG_TEST_CONSOLE
    // Regression tests must execute only their selected script. Project-wide
    // discovery would merge every neighbouring test file into each test run.
    uiProgram = program;
#else
    if (preparedLaunchProgramReady) {
        uiProgram = std::move(preparedLaunchProgram);
        preparedLaunchProgram.clear();
        preparedLaunchProgramReady = false;
    } else {
        uiProgram = mergePrograms(program, currentSourcePath, true, true);
    }
#endif
    LibraryState libraries;
    scanLibraries(uiProgram, libraries);
    filesEnabled = libraries.files;
    jsonEnabled = libraries.json;
    databaseEnabled = libraries.database;
    webEnabled = libraries.web;
    collisionEnabled = libraries.collision;
    physicsEnabled = libraries.physics;
    networkEnabled = libraries.network;
    aiEnabled = libraries.ai;
    mathEnabled = libraries.math;
    binaryEnabled = libraries.binary;
    regexEnabled = libraries.regex;
    compressionEnabled = libraries.compression;
    powershellEnabled = libraries.powershell;
    raspberryEnabled = libraries.raspberry;
    accessibilityEnabled = libraries.accessibility;
    timeEnabled = libraries.time;
    convertEnabled = libraries.convert;
    statsEnabled = libraries.stats;
    unitsEnabled = libraries.units;
    styleEnabled = libraries.style;
    uiStyles.clear();
    threeDEnabled = libraries.threeD;
    aiModels.clear();
    if (libraries.controls) initializeControls(uiVariables, uiTypes);
    registerFunctions(uiProgram);
    executeBlock(uiProgram, 0, uiProgram.size(), uiVariables, uiTypes, uiDefaults, flow,
                 libraries.manual, libraries.ui);
    if (flow.stopped) everyTasks.clear();
    if (!everyTasks.empty()) appendOutput("Program started with repeating tasks.\r\n");
    else if (errorCount == 0) appendOutput("Program finished.\r\n");
    else appendOutput("Program finished with " + std::to_string(errorCount) + " error(s).\r\n");
#ifdef _WIN32
    if (mainWindow) {
        if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(mainWindow, 1, 16, nullptr);
        InvalidateRect(mainWindow, nullptr, TRUE);
    }
#elif !defined(__ANDROID__)
    if (!uiBlocks.empty()) appendOutput("TRG UI is currently supported on Windows only.\n");
#endif
}

#ifdef _WIN32

void applyEditorSyntaxHighlighting() {
    if (!codeEditor || !richEditModule || highlightingEditor) return;
    const int length = GetWindowTextLengthW(codeEditor);
    if (length <= 0) return;
    std::wstring source(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(codeEditor, source.data(), length + 1);
    source.resize(static_cast<std::size_t>(length));

    // RichEdit formatting for every token can freeze the editor on large files.
    // Large documents remain immediately readable with the normal editor colour.
    if (length > 48000) {
        CHARFORMAT2W plain = {};
        plain.cbSize = sizeof(plain);
        plain.dwMask = CFM_COLOR | CFM_BOLD;
        plain.crTextColor = RGB(239, 232, 250);
        plain.dwEffects = 0;
        SendMessageW(codeEditor, EM_SETSEL, 0, length);
        SendMessageW(codeEditor, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&plain));
        SendMessageW(codeEditor, EM_SETSEL, 0, 0);
        return;
    }

    highlightingEditor = true;
    SendMessageW(codeEditor, WM_SETREDRAW, FALSE, 0);
    DWORD selectionStart = 0, selectionEnd = 0;
    SendMessageW(codeEditor, EM_GETSEL, reinterpret_cast<WPARAM>(&selectionStart),
                 reinterpret_cast<LPARAM>(&selectionEnd));
    CHARFORMAT2W base = {};
    base.cbSize = sizeof(base);
    base.dwMask = CFM_COLOR | CFM_BOLD;
    base.crTextColor = RGB(220, 220, 220);
    base.dwEffects = CFE_BOLD;
    SendMessageW(codeEditor, EM_SETSEL, 0, length);
    SendMessageW(codeEditor, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&base));
    static const std::unordered_set<std::wstring> keywords = {
        L"var",L"let",L"num",L"word",L"toggle",L"color",L"list",L"record",L"struct",L"class",L"new",
        L"if",L"else",L"loop",L"every",L"wait",L"break",L"stop",L"exit",L"continue",L"return",
        L"function",L"define",L"call",L"for",L"each",L"group",L"hide",L"show",
        L"say",L"print",L"ask",L"random",L"add",L"create",L"run",L"json",L"database",L"table",
        L"query",L"sql",L"read_file",L"write_file",L"append_file",L"file_exists",L"delete_file",
        L"push",L"insert",L"get",L"set",L"remove",L"pop",L"join",L"split",L"upper",L"lower",
        L"trim",L"ttrim",L"length",L"contains",L"index_of",L"replace",L"substring",L"find",
        L"find_position",L"replace_text",L"count_text",L"starts_with",L"ends_with",
        L"dictionary",L"dict_set",L"dict_get",L"dict_has",L"dict_remove",L"dict_keys",L"dict_count",
        L"binary",L"bit",L"checksum",L"regex",L"compress",L"http_get",L"http_post",L"run_wasm",
        L"math",L"stats",L"units",L"convert",L"time",L"physics",L"collides",L"watch_collision",
        L"particle",L"scroll",L"set_text",L"set_color",L"ai",L"text",L"button",L"block",
        L"rectangle",L"circle",L"image",L"sprite",L"animate",L"ui",L"3d",L"box",L"sphere",
        L"camera",L"spawn",L"plane",L"texture",L"save",L"load",L"move",L"rotate",L"scale",
        L"duplicate",L"rename",L"delete",L"true",L"false",L"none"
    };
    auto paint = [&](std::size_t start, std::size_t count, COLORREF color, bool bold = false) {
        if (!count) return;
        CHARFORMAT2W format = {};
        format.cbSize = sizeof(format);
        format.dwMask = CFM_COLOR | CFM_BOLD;
        format.crTextColor = color;
        format.dwEffects = bold ? CFE_BOLD : 0;
        SendMessageW(codeEditor, EM_SETSEL, static_cast<WPARAM>(start), static_cast<LPARAM>(start + count));
        SendMessageW(codeEditor, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&format));
    };
    std::size_t i = 0;
    while (i < source.size()) {
        if (source[i] == L'/' && i + 1 < source.size() && source[i + 1] == L'/') {
            const std::size_t start = i;
            while (i < source.size() && source[i] != L'\n') ++i;
            paint(start, i - start, RGB(143, 124, 171));
        } else if (source[i] == L'"') {
            const std::size_t start = i++;
            bool escaped = false;
            while (i < source.size()) {
                const wchar_t ch = source[i++];
                if (ch == L'"' && !escaped) break;
                if (ch == L'\\' && !escaped) escaped = true;
                else escaped = false;
            }
            paint(start, i - start, RGB(206, 145, 120));
        } else if (std::iswdigit(source[i])) {
            const std::size_t start = i++;
            while (i < source.size() && (std::iswalnum(source[i]) || source[i] == L'.')) ++i;
            paint(start, i - start, RGB(190, 170, 220));
        } else if (std::iswalpha(source[i]) || source[i] == L'_') {
            const std::size_t start = i++;
            while (i < source.size() && (iswalnum(source[i]) || source[i] == L'_')) ++i;
            std::wstring word = source.substr(start, i - start);
            std::transform(word.begin(), word.end(), word.begin(), [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
            if (keywords.count(word)) paint(start, i - start, RGB(168, 139, 255), true);
        } else if (source[i] == L'#') {
            const std::size_t start = i++;
            while (i < source.size() && std::iswxdigit(source[i])) ++i;
            if (i - start == 7 || i - start == 9) paint(start, i - start, RGB(197, 134, 192));
        } else ++i;
    }
    SendMessageW(codeEditor, EM_SETSEL, selectionStart, selectionEnd);
    highlightingEditor = false;
    SendMessageW(codeEditor, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(codeEditor, nullptr, FALSE);
}

bool loadSourceFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        appendOutput("ERROR Could not open \"" + path.u8string() + "\".\r\n");
        return false;
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    std::string source = contents.str();
    if (source.size() >= 3 && static_cast<unsigned char>(source[0]) == 0xEF &&
        static_cast<unsigned char>(source[1]) == 0xBB && static_cast<unsigned char>(source[2]) == 0xBF)
        source.erase(0, 3);
    loadingEditorText = true;
    SetWindowTextW(codeEditor, wideFromUtf8(source).c_str());
    loadingEditorText = false;
    applyEditorSyntaxHighlighting();
    currentSourcePath = std::filesystem::absolute(path);
    projectRoot = currentSourcePath.parent_path();
    editorDirty = false;
    previewMode = false;
    saveDesktopAutosave();
    if (previewButton) SetWindowTextW(previewButton, L"Preview");
    updateEditorWindowTitle();
    layoutOutputControls(mainWindow);
    clearDesktopOutput();
    appendOutput("Opened: " + currentSourcePath.u8string() + "\r\n");
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

std::filesystem::path desktopAutosavePath() {
    static const std::filesystem::path path = [] {
        const DWORD required = GetEnvironmentVariableW(L"LOCALAPPDATA", nullptr, 0);
        if (required > 1) {
            std::wstring root(static_cast<std::size_t>(required), L'\0');
            const DWORD copied = GetEnvironmentVariableW(L"LOCALAPPDATA", root.data(), required);
            if (copied > 0 && copied < required) {
                root.resize(copied);
                return std::filesystem::path(root) / L"TRG Studio" / L"desktop-autosave.trg";
            }
        }
        return std::filesystem::temp_directory_path() / L"TRG Studio" / L"desktop-autosave.trg";
    }();
    return path;
}

void saveDesktopAutosave() {
    if (!codeEditor) return;
    try {
        const std::filesystem::path path = desktopAutosavePath();
        std::filesystem::create_directories(path.parent_path());
        const int length = GetWindowTextLengthW(codeEditor);
        std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
        GetWindowTextW(codeEditor, wideSource.data(), length + 1);
        wideSource.resize(static_cast<std::size_t>(length));
        const std::string source = utf8FromWide(wideSource);
        std::ofstream output(path, std::ios::binary | std::ios::trunc);
        if (!output) return;
        output.write(source.data(), static_cast<std::streamsize>(source.size()));
        if (!output) return;
        std::filesystem::path metadataPath = path;
        metadataPath += ".meta";
        std::ofstream metadata(metadataPath, std::ios::binary | std::ios::trunc);
        if (metadata) metadata << currentSourcePath.u8string() << "\n" << (editorDirty ? "1" : "0");
    } catch (...) {
        // Autosave is best-effort and must never interrupt editing.
    }
}

void restoreDesktopAutosave() {
    if (!codeEditor) return;
    try {
        const std::filesystem::path path = desktopAutosavePath();
        std::ifstream input(path, std::ios::binary);
        if (!input) return;
        std::ostringstream contents;
        contents << input.rdbuf();
        const std::string source = contents.str();
        std::string sourcePath;
        bool wasDirty = true;
        std::filesystem::path metadataPath = path;
        metadataPath += ".meta";
        std::ifstream metadata(metadataPath, std::ios::binary);
        if (metadata) {
            std::getline(metadata, sourcePath);
            std::string dirty;
            std::getline(metadata, dirty);
            wasDirty = dirty != "0";
        }
        loadingEditorText = true;
        SetWindowTextW(codeEditor, wideFromUtf8(source).c_str());
        loadingEditorText = false;
        applyEditorSyntaxHighlighting();
        if (!sourcePath.empty()) {
            currentSourcePath = std::filesystem::u8path(sourcePath);
            projectRoot = currentSourcePath.parent_path();
        }
        editorDirty = wasDirty;
        updateEditorWindowTitle();
    } catch (...) {
        loadingEditorText = false;
    }
}

bool saveSourceFile() {
    std::filesystem::path path = currentSourcePath;
    if (path.empty() && !saveSourceDialog(path, mainWindow)) return false;
    const int length = GetWindowTextLengthW(codeEditor);
    std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(codeEditor, wideSource.data(), length + 1);
    wideSource.resize(static_cast<std::size_t>(length));
    const std::string source = utf8FromWide(wideSource);
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        appendOutput("ERROR Could not save \"" + path.u8string() + "\".\r\n");
        return false;
    }
    output.write(source.data(), static_cast<std::streamsize>(source.size()));
    if (!output) {
        appendOutput("ERROR Failed while writing \"" + path.u8string() + "\".\r\n");
        return false;
    }
    currentSourcePath = std::filesystem::absolute(path);
    projectRoot = currentSourcePath.parent_path();
    editorDirty = false;
    updateEditorWindowTitle();
    saveDesktopAutosave();
    appendOutput("Saved: " + currentSourcePath.u8string() + "\r\n");
    return true;
}

bool launchDesktopVisualEditor(const std::string& fileName) {
    if (!codeEditor) return false;
    const char* embeddedSource = nullptr;
    if (fileName == "2DEditor.trg" || fileName == "2D Editor")
        embeddedSource = trg_builtin::editor2D;
    else if (fileName == "3DEditor.trg" || fileName == "3D Editor")
        embeddedSource = trg_builtin::desktopEditor3D;

    if (!embeddedSource) {
        appendOutput("ERROR Unknown built-in visual editor.\r\n");
        return false;
    }

    if (!desktopVisualEditorActive) {
        desktopPreviousPath = currentSourcePath;
        desktopPreviousDirty = editorDirty;
    }
    desktopVisualEditorActive = true;
    desktop3DEngineActive = fileName == "3D Editor";
    desktop3DContextOpen = false;
    desktop3DDraggingObject = false;
    desktop3DDraggingCamera = false;
    desktop3DAssetCandidate = -1;
    desktop3DAssetDragging = false;
    if (desktop3DEngineActive) {
        desktop3DSelectedName.clear();
        desktop3DPlayMode = false;
        desktop3DCameraX = 0.0;
        desktop3DCameraY = 0.0;
        desktop3DCameraZ = -12.0;
        desktop3DCameraYaw = 0.0;
        desktop3DCameraPitch = 0.18;
        desktop3DZoom = 1.0;
        desktop3DToolMode = 1;
        desktop3DGridVisible = true;
        desktop3DHierarchyScroll = 0;
    }
    currentSourcePath.clear();
    projectRoot = std::filesystem::current_path();
    clearDesktopOutput();
    appendOutput("Opening built-in " + fileName + " workspace...\r\n\r\n");
    std::istringstream builtInStream(embeddedSource);
    const std::vector<std::string> builtInProgram = readProgram(builtInStream);
    executeProgram(builtInProgram);
    if (desktop3DEngineActive) syncDesktop3DEditorState();
    currentSourcePath = desktopPreviousPath;
    projectRoot = currentSourcePath.empty() ? std::filesystem::current_path() : currentSourcePath.parent_path();
    previewMode = true;
    if (desktop3DEngineActive && mainWindow) SetTimer(mainWindow, 3, 16, nullptr);
    if (previewButton) SetWindowTextW(previewButton, L"Code");
    layoutOutputControls(mainWindow);
    if (mainWindow) SetFocus(mainWindow);
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

bool returnFromDesktopVisualEditor() {
    if (!desktopVisualEditorActive || !codeEditor) return false;
    currentSourcePath = desktopPreviousPath;
    projectRoot = currentSourcePath.empty() ? std::filesystem::current_path() : currentSourcePath.parent_path();
    editorDirty = desktopPreviousDirty;
    desktopVisualEditorActive = false;
    desktop3DEngineActive = false;
    KillTimer(mainWindow, 3);
    desktop3DContextOpen = false;
    desktop3DDraggingObject = false;
    desktop3DDraggingCamera = false;
    desktop3DAssetCandidate = -1;
    desktop3DAssetDragging = false;
    previewMode = false;
    SetWindowTextW(previewButton, L"Preview");
    updateEditorWindowTitle();
    layoutOutputControls(mainWindow);
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

bool runEditorSource() {
    if (!codeEditor) return false;
    const int length = GetWindowTextLengthW(codeEditor);
    std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(codeEditor, wideSource.data(), length + 1);
    wideSource.resize(static_cast<std::size_t>(length));
    const std::string source = utf8FromWide(wideSource);
    if (!currentSourcePath.empty()) projectRoot = currentSourcePath.parent_path();
    else projectRoot = std::filesystem::current_path();
    clearDesktopOutput();
    appendOutput("Running: " + (currentSourcePath.empty() ? std::string("Untitled.trg") : currentSourcePath.u8string()) + "\r\n\r\n");
    std::istringstream input(source);
    executeProgram(readProgram(input));
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

bool runSourceFile(const std::filesystem::path& path) {
    return loadSourceFile(path) && runEditorSource();
}
#endif




#ifndef TRG_ANDROID_BRIDGE
int runApplication(int argc, char** argv) {
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    const bool editMode = argc > 2 && lower(argv[1]) == "edit";
    bool launchingProgram = argc > 1 && lower(argv[1]) != "compile" && !editMode;
    if (argc == 1) {
        wchar_t ownPath[32768] = {};
        const DWORD ownLength = GetModuleFileNameW(nullptr, ownPath, 32768);
        if (ownLength > 0 && ownLength < 32768) {
            std::ifstream ownExecutable(std::filesystem::path(std::wstring(ownPath, ownLength)), std::ios::binary);
            if (ownExecutable) {
                ownExecutable.seekg(0, std::ios::end);
                const std::streamoff ownSize = ownExecutable.tellg();
                if (ownSize >= 16) {
                    ownExecutable.seekg(ownSize - 8);
                    char marker[8] = {};
                    ownExecutable.read(marker, 8);
                    launchingProgram = ownExecutable && std::string(marker, 8) == "TRGPKG01";
                }
            }
        }
    }
    // Give standalone launches a visible top-level window immediately.
    // Start in terminal layout while libraries are scanned; the detected app
    // mode replaces this layout once the program has been loaded.
    if (launchingProgram) {
        standaloneAppMode = false;
        standaloneTerminalMode = true;
        previewMode = true;
    }
    if (!runUi(true, !launchingProgram)) {
        MessageBoxW(nullptr, L"Could not create the TRG window.", L"TRG error", MB_OK | MB_ICONERROR);
        return 1;
    }
#endif

#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    if (editMode) {
        if (argc != 3) {
            MessageBoxW(nullptr, L"Usage: TRG.exe edit <file.trg>", L"TRG Editor", MB_OK | MB_ICONERROR);
            return 2;
        }
        const std::filesystem::path editPath = std::filesystem::u8path(argv[2]);
        if (!loadSourceFile(editPath)) {
            MessageBoxW(nullptr, L"TRG could not open this file in the editor.", L"TRG Editor", MB_OK | MB_ICONERROR);
            return 1;
        }
        ShowWindow(mainWindow, SW_SHOW);
        UpdateWindow(mainWindow);
        SetFocus(codeEditor);
        runMessageLoop();
        return 0;
    }

#endif

    if (argc > 1 && lower(argv[1]) == "compile") {
        if (argc != 3) {
            appendOutput("ERROR Usage: TRG compile <file.trg>\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 2;
        }
        std::filesystem::path sourcePath = std::filesystem::u8path(argv[2]);
        if (!std::filesystem::exists(sourcePath) && sourcePath.extension().empty()) sourcePath += ".trg";
        std::ifstream source(sourcePath, std::ios::binary);
        if (!source) {
            appendOutput("ERROR Could not open \"" + sourcePath.u8string() + "\".\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 1;
        }
#ifdef _WIN32
        wchar_t executableBuffer[32768];
        const DWORD executableLength = GetModuleFileNameW(nullptr, executableBuffer, 32768);
        if (executableLength == 0 || executableLength >= 32768) {
            appendOutput("ERROR Could not locate the TRG executable.\r\n");
#if !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 1;
        }
        const std::filesystem::path executablePath(std::wstring(executableBuffer, executableLength));
#else
        const std::filesystem::path executablePath = std::filesystem::absolute(argv[0]);
#endif
        std::filesystem::path outputPath = sourcePath;
        outputPath.replace_extension(".exe");
        std::error_code error;
        const std::uint64_t payloadSize = std::filesystem::file_size(sourcePath, error);
        if (error) { appendOutput("ERROR Could not determine source size: " + error.message() + "\r\n"); return 1; }
        std::filesystem::copy_file(executablePath, outputPath, std::filesystem::copy_options::overwrite_existing, error);
        if (error) { appendOutput("ERROR Could not create " + outputPath.u8string() + ": " + error.message() + "\r\n"); return 1; }
        std::ofstream output(outputPath, std::ios::binary | std::ios::app);
        if (!output) { appendOutput("ERROR Could not write " + outputPath.u8string() + "\r\n"); return 1; }
        output << source.rdbuf();
        for (unsigned byte = 0; byte < 8; ++byte) output.put(static_cast<char>((payloadSize >> (byte * 8)) & 0xff));
        output.write("TRGPKG01", 8);
        output.close();
        if (!output) { appendOutput("ERROR Could not finish writing " + outputPath.u8string() + "\r\n"); return 1; }
        appendOutput("Created " + outputPath.u8string() + " (standalone TRG app).\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
        runMessageLoop();
#endif
        return 0;
    }

    std::filesystem::path scriptPath;
    std::string embeddedSource;
    bool hasEmbeddedSource = false;
    if (argc > 1) {
        scriptPath = argv[1];
    } else {
#ifdef _WIN32
        wchar_t executableBuffer[32768];
        const DWORD executableLength = GetModuleFileNameW(nullptr, executableBuffer, 32768);
        if (executableLength > 0 && executableLength < 32768) scriptPath = std::filesystem::path(std::wstring(executableBuffer, executableLength));
#else
        scriptPath = std::filesystem::absolute(argv[0]);
#endif
        std::ifstream executable(scriptPath, std::ios::binary);
        if (executable) {
            executable.seekg(0, std::ios::end);
            const std::streamoff totalSize = executable.tellg();
            if (totalSize >= 16) {
                executable.seekg(totalSize - 8);
                char marker[8] = {};
                executable.read(marker, 8);
                if (executable && std::string(marker, 8) == "TRGPKG01") {
                    executable.seekg(totalSize - 16);
                    std::uint64_t payloadSize = 0;
                    for (unsigned byte = 0; byte < 8; ++byte) {
                        const int value = executable.get();
                        if (value == EOF) { payloadSize = 0; break; }
                        payloadSize |= static_cast<std::uint64_t>(static_cast<unsigned char>(value)) << (byte * 8);
                    }
                    if (payloadSize > static_cast<std::uint64_t>(totalSize - 16)) {
                        appendOutput("ERROR Invalid embedded TRG program.\r\n");
#if !defined(TRG_TEST_CONSOLE)
                        runMessageLoop();
#endif
                        return 1;
                    }
                    executable.seekg(totalSize - 16 - static_cast<std::streamoff>(payloadSize));
                    embeddedSource.resize(static_cast<std::size_t>(payloadSize));
                    if (payloadSize > 0) executable.read(&embeddedSource[0], static_cast<std::streamsize>(payloadSize));
                    if (!executable) {
                        appendOutput("ERROR Could not read embedded TRG program.\r\n");
#if !defined(TRG_TEST_CONSOLE)
                        runMessageLoop();
#endif
                        return 1;
                    }
                    hasEmbeddedSource = true;
                }
            }
        }
    }

    std::vector<std::string> program;
    if (hasEmbeddedSource) {
        std::istringstream input(embeddedSource);
        program = readProgram(input);
        if (!scriptPath.empty()) {
            currentSourcePath = std::filesystem::absolute(scriptPath);
            projectRoot = currentSourcePath.parent_path();
        }
    } else if (!scriptPath.empty() && argc > 1) {
        std::ifstream file(scriptPath);
        if (!file.is_open()) {
            appendOutput("ERROR Could not open \"" + scriptPath.u8string() + "\".\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            standaloneTerminalMode = true;
            previewMode = true;
            SetWindowTextW(mainWindow, L"TRG Terminal - Launch error");
            layoutOutputControls(mainWindow);
            ShowWindow(mainWindow, SW_SHOW);
            UpdateWindow(mainWindow);
            runMessageLoop();
#endif
            return 1;
        }
#ifdef _WIN32
        SetWindowTextW(mainWindow, (L"TRG - " + scriptPath.filename().wstring()).c_str());
#endif
        program = readProgram(file);
        currentSourcePath = std::filesystem::absolute(scriptPath);
        projectRoot = currentSourcePath.parent_path();
    } else if (argc == 1) {
#ifdef _WIN32
        if (mainWindow) runMessageLoop();
        return 0;
#else
        std::cout << "TRG Interpreter. Type 'exit' or 'stop' to quit.\n";
        std::string line;
        Vars variables;
        Types types;
        Defaults defaults = standardDefaults();
        Flow flow;
        while (std::getline(std::cin, line)) {
            line = stripComment(line);
            if (trim(line).empty()) continue;
            std::vector<std::string> interactiveProgram = {line};
            registerFunctions(interactiveProgram);
            executeBlock(interactiveProgram, 0, interactiveProgram.size(), variables, types, defaults, flow);
            if (flow.stopped) break;
        }
        return 0;
#endif
    }

#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    LibraryState launchLibraries;
    const std::vector<std::string> launchProgram = mergePrograms(program, currentSourcePath, true, true);
    scanLibraries(launchProgram, launchLibraries);
    preparedLaunchProgram = launchProgram;
    preparedLaunchProgramReady = true;
#if defined(TRG_FORCE_UI_APP)
    standaloneAppMode = true;
    standaloneTerminalMode = false;
#elif defined(TRG_FORCE_CONSOLE_APP)
    standaloneAppMode = false;
    standaloneTerminalMode = true;
#else
    standaloneAppMode = launchLibraries.ui;
    standaloneTerminalMode = !launchLibraries.ui;
#endif
    previewMode = true;
    if (mainWindow) {
        const std::wstring appName = scriptPath.empty() ? L"Untitled" : scriptPath.filename().wstring();
        SetWindowTextW(mainWindow, ((standaloneAppMode ? L"TRG App - " : L"TRG Terminal - ") + appName).c_str());
        layoutOutputControls(mainWindow);
    }
    // Display the selected app window before executing user code. Programs that
    // wait for input or run continuously must not keep their window hidden.
    if (launchingProgram && mainWindow) {
        ShowWindow(mainWindow, SW_SHOWNORMAL);
        SetForegroundWindow(mainWindow);
        BringWindowToTop(mainWindow);
        UpdateWindow(mainWindow);
    }
#endif
    executeProgram(program);
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    runMessageLoop();
#endif
    return 0;
}


#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argumentCount = 0;
    LPWSTR* wideArguments = CommandLineToArgvW(GetCommandLineW(), &argumentCount);
    if (!wideArguments) {
        MessageBoxW(nullptr, L"Could not read the TRG command line.", L"TRG error", MB_OK | MB_ICONERROR);
        return 1;
    }
    std::vector<std::string> arguments;
    std::vector<char*> narrowArguments;
    arguments.reserve(static_cast<std::size_t>(argumentCount));
    narrowArguments.reserve(static_cast<std::size_t>(argumentCount));
    for (int i = 0; i < argumentCount; ++i) arguments.push_back(utf8FromWide(wideArguments[i]));
    LocalFree(wideArguments);
    for (std::string& argument : arguments) narrowArguments.push_back(&argument[0]);
    return runApplication(argumentCount, narrowArguments.data());
}
#endif
#if !defined(_WIN32) || defined(TRG_TEST_CONSOLE)
int main(int argc, char** argv) {
    return runApplication(argc, argv);
}
#endif
#endif
