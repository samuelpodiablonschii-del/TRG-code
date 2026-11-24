#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

#define TRG_TEST_CONSOLE 1
#define main trg_console_main
#include "../TRG.cpp"
#undef main

#include <emscripten.h>

EM_JS(char*, trg_web_prompt, (), {
    if (typeof globalThis.prompt !== "function") return 0;
    const value = globalThis.prompt("TRG asks for input");
    if (value === null) return 0;
    const size = lengthBytesUTF8(value) + 1;
    const pointer = _malloc(size);
    if (!pointer) return 0;
    stringToUTF8(value, pointer, size);
    return pointer;
});

EM_JS(void, trg_web_emit_output, (const char* value), {
    const output = UTF8ToString(value);
    if (typeof globalThis.trgCompilerAppendOutput === "function") {
        globalThis.trgCompilerAppendOutput(output);
    }
});

class TrgBrowserInputBuffer final : public std::streambuf {
public:
    int_type underflow() override {
        if (gptr() && gptr() < egptr()) {
            return traits_type::to_int_type(*gptr());
        }
        char* answer = trg_web_prompt();
        if (!answer) return traits_type::eof();
        pending_.assign(answer);
        std::free(answer);
        pending_.push_back('\n');
        setg(pending_.data(), pending_.data(), pending_.data() + pending_.size());
        return traits_type::to_int_type(*gptr());
    }
private:
    std::string pending_;
};

static std::string trgJsonEscape(const std::string& value) {
    std::string result;
    result.reserve(value.size() + 8);
    for (unsigned char ch : value) {
        switch (ch) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (ch < 0x20) {
                    const char hex[] = "0123456789abcdef";
                    result += "\\u00";
                    result += hex[(ch >> 4) & 0x0f];
                    result += hex[ch & 0x0f];
                } else result += static_cast<char>(ch);
        }
    }
    return result;
}

static std::string trgBase64(const std::string& bytes) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    output.reserve(((bytes.size() + 2) / 3) * 4);
    for (std::size_t i = 0; i < bytes.size(); i += 3) {
        const unsigned a = static_cast<unsigned char>(bytes[i]);
        const unsigned b = i + 1 < bytes.size() ? static_cast<unsigned char>(bytes[i + 1]) : 0;
        const unsigned d = i + 2 < bytes.size() ? static_cast<unsigned char>(bytes[i + 2]) : 0;
        const unsigned value = (a << 16) | (b << 8) | d;
        output += alphabet[(value >> 18) & 63];
        output += alphabet[(value >> 12) & 63];
        output += i + 1 < bytes.size() ? alphabet[(value >> 6) & 63] : '=';
        output += i + 2 < bytes.size() ? alphabet[value & 63] : '=';
    }
    return output;
}

static std::string trgSpriteData(const UiSprite& sprite) {
    std::ifstream input(sprite.path, std::ios::binary);
    if (!input) return {};
    std::string bytes((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    if (bytes.empty() || bytes.size() > 2 * 1024 * 1024) return {};
    std::string extension = std::filesystem::path(sprite.path).extension().u8string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    std::string mime = extension == ".jpg" || extension == ".jpeg" ? "image/jpeg" :
                       extension == ".webp" ? "image/webp" :
                       extension == ".gif" ? "image/gif" :
                       extension == ".svg" ? "image/svg+xml" : "image/png";
    return "data:" + mime + ";base64," + trgBase64(bytes);
}

static std::string trgNumber(double value) {
    std::ostringstream stream;
    stream << std::setprecision(10) << value;
    return stream.str();
}

static void trgEmitUiSnapshot() {
    std::ostringstream json;
    json << "{\"blocks\":[";
    bool first = true;
    for (const auto& item : uiBlocks) {
        if (!first) json << ',';
        first = false;
        json << "{\"name\":\"" << trgJsonEscape(item.name)
             << "\",\"group\":\"" << trgJsonEscape(item.group)
             << "\",\"x\":" << trgNumber(item.x)
             << ",\"y\":" << trgNumber(item.y)
             << ",\"width\":" << trgNumber(item.width)
             << ",\"height\":" << trgNumber(item.height)
             << ",\"color\":\"" << trgJsonEscape(item.color)
             << "\",\"shape\":\"" << trgJsonEscape(item.shape)
             << "\",\"opacity\":" << trgNumber(item.opacity)
             << ",\"angle\":" << trgNumber(item.angle)
             << ",\"visible\":" << (item.visible ? "true" : "false") << '}';
    }
    json << "],\"sprites\":[";
    first = true;
    for (const auto& item : uiSprites) {
        if (!first) json << ',';
        first = false;
        json << "{\"name\":\"" << trgJsonEscape(item.name)
             << "\",\"group\":\"" << trgJsonEscape(item.group)
             << "\",\"x\":" << trgNumber(item.x)
             << ",\"y\":" << trgNumber(item.y)
             << ",\"width\":" << trgNumber(item.width)
             << ",\"height\":" << trgNumber(item.height)
             << ",\"opacity\":" << trgNumber(item.opacity)
             << ",\"visible\":" << (item.visible ? "true" : "false")
             << ",\"data\":\"" << trgJsonEscape(trgSpriteData(item)) << "\"}";
    }
    json << "],\"texts\":[";
    first = true;
    for (const auto& item : uiTexts) {
        if (!first) json << ',';
        first = false;
        json << "{\"name\":\"" << trgJsonEscape(item.name)
             << "\",\"group\":\"" << trgJsonEscape(item.group)
             << "\",\"value\":\"" << trgJsonEscape(item.value)
             << "\",\"x\":" << trgNumber(item.x)
             << ",\"y\":" << trgNumber(item.y)
             << ",\"size\":" << trgNumber(item.size)
             << ",\"color\":\"" << trgJsonEscape(item.color)
             << "\",\"font\":\"" << trgJsonEscape(item.font)
             << "\",\"align\":\"" << trgJsonEscape(item.align)
             << "\",\"opacity\":" << trgNumber(item.opacity)
             << ",\"bold\":" << (item.bold ? "true" : "false")
             << ",\"visible\":" << (item.visible ? "true" : "false") << '}';
    }
    json << "],\"buttons\":[";
    first = true;
    for (const auto& item : uiButtons) {
        if (!first) json << ',';
        first = false;
        json << "{\"name\":\"" << trgJsonEscape(item.name)
             << "\",\"group\":\"" << trgJsonEscape(item.group)
             << "\",\"label\":\"" << trgJsonEscape(item.label)
             << "\",\"x\":" << trgNumber(item.x)
             << ",\"y\":" << trgNumber(item.y)
             << ",\"width\":" << trgNumber(item.width)
             << ",\"height\":" << trgNumber(item.height)
             << ",\"color\":\"" << trgJsonEscape(item.color)
             << "\",\"textColor\":\"" << trgJsonEscape(item.textColor)
             << "\",\"opacity\":" << trgNumber(item.opacity)
             << ",\"cornerRadius\":" << trgNumber(item.cornerRadius)
             << ",\"textSize\":" << trgNumber(item.textSize)
             << ",\"textBold\":" << (item.textBold ? "true" : "false")
             << ",\"visible\":" << (item.visible ? "true" : "false") << '}';
    }
    json << "]}";
    const std::string snapshot = json.str();
    const std::string taggedSnapshot = std::string("\x1eTRG_UI:") + snapshot;
    trg_web_emit_output(taggedSnapshot.c_str());
}

extern "C" {
EMSCRIPTEN_KEEPALIVE void trg_web_run(const char* source) {
    std::ostringstream capturedOutput;
    TrgBrowserInputBuffer browserInput;
    std::streambuf* previousOutput = std::cout.rdbuf(capturedOutput.rdbuf());
    std::streambuf* previousInput = std::cin.rdbuf(&browserInput);
    std::cout.clear();
    std::cin.clear();

    try {
        projectRoot = std::filesystem::current_path();
        std::istringstream input(source ? source : "");
        const std::vector<std::string> program = readProgram(input);
        executeProgram(program);
    } catch (const std::exception& error) {
        capturedOutput << "ERROR " << error.what() << "\n";
    } catch (...) {
        capturedOutput << "ERROR Unknown TRG runtime error.\n";
    }

    std::cout.flush();
    std::cin.clear();
    std::cin.rdbuf(previousInput);
    std::cout.clear();
    std::cout.rdbuf(previousOutput);

    const std::string output = capturedOutput.str();
    trg_web_emit_output(output.c_str());
    trgEmitUiSnapshot();
}
}
