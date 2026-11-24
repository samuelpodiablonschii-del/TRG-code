#include <cstdlib>
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
}
}
