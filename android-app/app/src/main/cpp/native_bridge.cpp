#define TRG_ANDROID_BRIDGE
#include "../../../../../TRG.cpp"

#include <atomic>
#include <iomanip>

JavaVM* trgJavaVm = nullptr;
jobject trgActivity = nullptr;
std::string androidOutput;
std::unordered_map<std::string, std::string> androidMobileInputs;
std::atomic<bool> androidStopRequested{false};
std::mutex androidInputMutex;

namespace {
std::string fromJavaString(JNIEnv* env, jstring value) {
    if (!value) return {};
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (!chars) return {};
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}

jstring toJavaString(JNIEnv* env, const std::string& value) {
    return env->NewStringUTF(value.c_str());
}

std::string jsonString(const std::string& value) {
    std::ostringstream result;
    result << '"';
    for (unsigned char character : value) {
        switch (character) {
            case '"': result << "\\\""; break;
            case '\\': result << "\\\\"; break;
            case '\b': result << "\\b"; break;
            case '\f': result << "\\f"; break;
            case '\n': result << "\\n"; break;
            case '\r': result << "\\r"; break;
            case '\t': result << "\\t"; break;
            default:
                if (character < 0x20) {
                    result << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                           << static_cast<unsigned>(character) << std::dec;
                } else {
                    result << static_cast<char>(character);
                }
        }
    }
    result << '"';
    return result.str();
}

std::string serializeState() {
    std::ostringstream result;
    result << std::setprecision(15);
    result << "{\"output\":" << jsonString(androidOutput)
           << ",\"errors\":" << errorCount
           << ",\"elapsed\":"
           << std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count()
           << ",\"blocks\":[";
    for (std::size_t index = 0; index < uiBlocks.size(); ++index) {
        const UiBlock& block = uiBlocks[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(block.name)
               << ",\"color\":" << jsonString(block.color)
               << ",\"shape\":" << jsonString(block.shape)
               << ",\"z\":" << block.z
               << ",\"x\":" << block.x << ",\"y\":" << block.y
               << ",\"w\":" << block.width << ",\"h\":" << block.height
               << ",\"radius\":" << block.cornerRadius
               << ",\"opacity\":" << block.opacity
               << ",\"visible\":" << (block.visible && uiGroupIsVisible(block.group) ? "true" : "false") << '}';
    }
    result << "],\"sprites\":[";
    for (std::size_t index = 0; index < uiSprites.size(); ++index) {
        const UiSprite& sprite = uiSprites[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(sprite.name)
               << ",\"path\":" << jsonString(sprite.path)
               << ",\"x\":" << sprite.x << ",\"y\":" << sprite.y
               << ",\"w\":" << sprite.width << ",\"h\":" << sprite.height
               << ",\"opacity\":" << sprite.opacity
               << ",\"visible\":" << (sprite.visible && uiGroupIsVisible(sprite.group) ? "true" : "false") << '}';
    }
    result << "],\"models\":[";
    for (std::size_t index = 0; index < ui3DObjects.size(); ++index) {
        const Ui3DObject& model = ui3DObjects[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(model.name)
               << ",\"shape\":" << jsonString(model.shape)
               << ",\"color\":" << jsonString(model.color)
               << ",\"x\":" << model.x << ",\"y\":" << model.y << ",\"z\":" << model.z
               << ",\"w\":" << model.width << ",\"h\":" << model.height << ",\"d\":" << model.depth
               << ",\"rx\":" << model.rotateX << ",\"ry\":" << model.rotateY << ",\"rz\":" << model.rotateZ
               << ",\"opacity\":" << model.opacity
               << ",\"visible\":" << (model.visible && uiGroupIsVisible(model.group) ? "true" : "false") << '}';
    }
    result << "],\"animations\":[";
    for (std::size_t index = 0; index < uiAnimations.size(); ++index) {
        const UiAnimation& animation = uiAnimations[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(animation.name)
               << ",\"x\":" << animation.startX << ",\"y\":" << animation.startY
               << ",\"dx\":" << animation.dx << ",\"dy\":" << animation.dy
               << ",\"start\":" << animation.startTime
               << ",\"duration\":" << animation.duration << '}';
    }
    result << "],\"buttons\":[";
    for (std::size_t index = 0; index < uiButtons.size(); ++index) {
        const UiButton& button = uiButtons[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(button.name)
               << ",\"label\":" << jsonString(button.label)
               << ",\"index\":" << index
               << ",\"x\":" << button.x << ",\"y\":" << button.y
               << ",\"w\":" << button.width << ",\"h\":" << button.height
               << ",\"color\":" << jsonString(button.color)
               << ",\"opacity\":" << button.opacity
               << ",\"radius\":" << button.cornerRadius
               << ",\"font\":" << jsonString(button.font)
               << ",\"text_color\":" << jsonString(button.textColor)
               << ",\"text_size\":" << button.textSize
               << ",\"bold\":" << (button.textBold ? "true" : "false")
               << ",\"visible\":" << (button.visible && uiGroupIsVisible(button.group) ? "true" : "false") << '}';
    }
    result << "],\"texts\":[";
    for (std::size_t index = 0; index < uiTexts.size(); ++index) {
        const UiText& text = uiTexts[index];
        if (index) result << ',';
        result << "{\"name\":" << jsonString(text.name)
               << ",\"value\":" << jsonString(text.value)
               << ",\"color\":" << jsonString(text.color)
               << ",\"x\":" << text.x << ",\"y\":" << text.y
               << ",\"size\":" << text.size
               << ",\"font\":" << jsonString(text.font)
               << ",\"align\":" << jsonString(text.align)
               << ",\"opacity\":" << text.opacity
               << ",\"bold\":" << (text.bold ? "true" : "false")
               << ",\"visible\":" << (text.visible && uiGroupIsVisible(text.group) ? "true" : "false") << '}';
    }
    result << "],\"every\":[";
    for (std::size_t index = 0; index < everyTasks.size(); ++index) {
        if (index) result << ',';
        result << "{\"index\":" << index
               << ",\"ms\":" << everyTasks[index].interval.count() << '}';
    }
    result << "]}";
    return result.str();
}
}

void trgStudioActionAndroid(const std::string& action) {
    if (!trgJavaVm || !trgActivity) return;
    JNIEnv* env = nullptr;
    if (trgJavaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK || !env) return;
    jclass activityClass = env->GetObjectClass(trgActivity);
    if (!activityClass) return;
    jmethodID method = env->GetMethodID(activityClass, "onTrgStudioAction", "(Ljava/lang/String;)V");
    if (method) {
        jstring javaAction = toJavaString(env, action);
        env->CallVoidMethod(trgActivity, method, javaAction);
        env->DeleteLocalRef(javaAction);
    }
    env->DeleteLocalRef(activityClass);
    if (env->ExceptionCheck()) env->ExceptionClear();
}

std::string trgAskAndroid(const std::string& prompt, bool& accepted) {
    accepted = false;
    if (!trgJavaVm || !trgActivity) return {};
    JNIEnv* env = nullptr;
    if (trgJavaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK || !env) return {};
    jclass activityClass = env->GetObjectClass(trgActivity);
    if (!activityClass) return {};
    jmethodID inputMethod = env->GetMethodID(activityClass, "requestTrgInput",
                                             "(Ljava/lang/String;)Ljava/lang/String;");
    if (!inputMethod) {
        env->DeleteLocalRef(activityClass);
        if (env->ExceptionCheck()) env->ExceptionClear();
        return {};
    }
    jstring javaPrompt = toJavaString(env, prompt);
    auto answer = static_cast<jstring>(env->CallObjectMethod(trgActivity, inputMethod, javaPrompt));
    env->DeleteLocalRef(javaPrompt);
    env->DeleteLocalRef(activityClass);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        return {};
    }
    if (!answer) return {};
    accepted = true;
    const std::string result = fromJavaString(env, answer);
    env->DeleteLocalRef(answer);
    return result;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    trgJavaVm = vm;
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL
Java_com_trg_studio_MainActivity_nativeInitialize(JNIEnv* env, jclass, jobject activity) {
    if (trgActivity) env->DeleteGlobalRef(trgActivity);
    trgActivity = env->NewGlobalRef(activity);
    jclass activityClass = env->GetObjectClass(activity);
    jmethodID getFilesDir = activityClass ? env->GetMethodID(activityClass, "getFilesDir", "()Ljava/io/File;") : nullptr;
    jobject directory = getFilesDir ? env->CallObjectMethod(activity, getFilesDir) : nullptr;
    jclass fileClass = directory ? env->GetObjectClass(directory) : nullptr;
    jmethodID getAbsolutePath = fileClass ? env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;") : nullptr;
    auto absolutePath = getAbsolutePath ? static_cast<jstring>(env->CallObjectMethod(directory, getAbsolutePath)) : nullptr;
    if (absolutePath) {
        projectRoot = std::filesystem::u8path(fromJavaString(env, absolutePath));
        env->DeleteLocalRef(absolutePath);
    }
    if (fileClass) env->DeleteLocalRef(fileClass);
    if (directory) env->DeleteLocalRef(directory);
    if (activityClass) env->DeleteLocalRef(activityClass);
    if (env->ExceptionCheck()) env->ExceptionClear();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_trg_studio_MainActivity_nativeRun(JNIEnv* env, jclass, jstring source) {
    androidOutput.clear();
    androidStopRequested.store(false);
    try {
        std::istringstream input(fromJavaString(env, source));
        executeProgram(readProgram(input));
    } catch (const std::exception& error) {
        appendOutput(std::string("ERROR: ") + error.what() + "\n");
    }
    return toJavaString(env, serializeState());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_trg_studio_MainActivity_nativeRunEvery(JNIEnv* env, jclass, jint index) {
    androidOutput.clear();
    if (!androidStopRequested.load() && index >= 0 && static_cast<std::size_t>(index) < everyTasks.size()) {
        EveryTask& scheduled = everyTasks[static_cast<std::size_t>(index)];
        const auto now = std::chrono::steady_clock::now();
        if (now < scheduled.nextRun) return toJavaString(env, serializeState());
        scheduled.nextRun = now + scheduled.interval;
        const EveryTask task = scheduled;
        refreshCollisionWatches(uiVariables, uiTypes);
        Flow flow;
        try {
            executeBlock(task.body, 0, task.body.size(), uiVariables, uiTypes,
                         uiDefaults, flow, task.manualMode, task.uiMode);
        } catch (const std::exception& error) {
            appendOutput(std::string("ERROR: ") + error.what() + "\n");
        }
        if (flow.stopped) everyTasks.clear();
        else if (flow.broke && static_cast<std::size_t>(index) < everyTasks.size())
            everyTasks.erase(everyTasks.begin() + index);
    }
    return toJavaString(env, serializeState());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_trg_studio_MainActivity_nativeClickButton(JNIEnv* env, jclass, jint index) {
    androidOutput.clear();
    if (index < 0 || static_cast<std::size_t>(index) >= uiButtons.size()) {
        appendOutput("ERROR: The selected UI button does not exist.\n");
        return toJavaString(env, serializeState());
    }
    const UiButton& button = uiButtons[static_cast<std::size_t>(index)];
    if (!button.visible || !uiGroupIsVisible(button.group)) return toJavaString(env, serializeState());
    appendOutput("--- " + button.label + " ---\n");
    conditionIsTrue("false", uiVariables);
    Flow flow;
    try {
        executeBlock(button.body, 0, button.body.size(), uiVariables, uiTypes,
                     uiDefaults, flow, false, true);
    } catch (const std::exception& error) {
        appendOutput(std::string("ERROR: ") + error.what() + "\n");
    }
    refreshCollisionWatches(uiVariables, uiTypes);
    if (flow.stopped) everyTasks.clear();
    return toJavaString(env, serializeState());
}

extern "C" JNIEXPORT void JNICALL
Java_com_trg_studio_MainActivity_nativeSetMobileInput(JNIEnv* env, jclass, jstring name,
                                                       jstring state) {
    const std::string control = fromJavaString(env, name);
    const std::string value = fromJavaString(env, state);
    std::lock_guard<std::mutex> lock(androidInputMutex);
    const auto existing = androidMobileInputs.find(control);
    if (existing != androidMobileInputs.end() &&
        (value == "released" || value == "press" || value == "unavailable")) {
        existing->second = value;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_trg_studio_MainActivity_nativeSetPointer(JNIEnv*, jclass, jfloat x, jfloat y, jboolean down) {
    std::lock_guard<std::mutex> lock(androidInputMutex);
    androidMobileInputs["pointer_x"] = std::to_string(std::clamp(static_cast<double>(x), 0.0, 100.0));
    androidMobileInputs["pointer_y"] = std::to_string(std::clamp(static_cast<double>(y), 0.0, 100.0));
    androidMobileInputs["pointer_down"] = down == JNI_TRUE ? "press" : "released";
}

extern "C" JNIEXPORT void JNICALL
Java_com_trg_studio_MainActivity_nativeSetEditorSelection(JNIEnv* env, jclass, jstring nameValue) {
    const std::string name = fromJavaString(env, nameValue);
    if (name.empty()) return;
    uiVariables["selectedName"] = name;
    uiTypes["selectedName"] = "word";
    auto list = uiVariables.find("objects");
    if (list != uiVariables.end()) {
        if (auto* items = std::get_if<std::vector<std::string>>(&list->second.value)) {
            for (std::size_t index = 0; index < items->size(); ++index) {
                if ((*items)[index] == name) {
                    uiVariables["selectedIndex"] = static_cast<double>(index);
                    uiTypes["selectedIndex"] = "num";
                    break;
                }
            }
        }
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_trg_studio_MainActivity_nativeTransformEditorObject(JNIEnv* env, jclass, jstring kindValue,
                                                              jstring nameValue, jfloat dx, jfloat dy, jfloat dz) {
    const std::string kind = fromJavaString(env, kindValue);
    const std::string name = fromJavaString(env, nameValue);
    bool changed = false;
    if (kind == "3d") {
        for (Ui3DObject& item : ui3DObjects) if (item.name == name) {
            item.x += dx; item.y += dy; item.z += dz; changed = true; break;
        }
    } else {
        for (UiBlock& item : uiBlocks) if (item.name == name) {
            item.x += dx; item.y += dy; changed = true; break;
        }
        if (!changed) for (UiSprite& item : uiSprites) if (item.name == name) {
            item.x += dx; item.y += dy; changed = true; break;
        }
        if (!changed) for (UiButton& item : uiButtons) if (item.name == name) {
            item.x += dx; item.y += dy; changed = true; break;
        }
        if (!changed) for (UiText& item : uiTexts) if (item.name == name) {
            item.x += dx; item.y += dy; changed = true; break;
        }
    }
    if (changed) refreshCollisionWatches(uiVariables, uiTypes);
    return toJavaString(env, serializeState());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_trg_studio_MainActivity_nativeScaleEditorObject(JNIEnv* env, jclass, jstring kindValue,
                                                          jstring nameValue, jfloat factor) {
    const std::string kind = fromJavaString(env, kindValue);
    const std::string name = fromJavaString(env, nameValue);
    const double scale = std::clamp(static_cast<double>(factor), 0.1, 10.0);
    if (kind == "3d") {
        for (Ui3DObject& item : ui3DObjects) if (item.name == name) {
            item.width = std::max(0.1, item.width * scale);
            item.height = std::max(0.1, item.height * scale);
            item.depth = std::max(0.1, item.depth * scale);
            break;
        }
    } else {
        for (UiBlock& item : uiBlocks) if (item.name == name) {
            item.width = std::max(0.5, item.width * scale);
            item.height = std::max(0.5, item.height * scale);
            break;
        }
        for (UiSprite& item : uiSprites) if (item.name == name) {
            item.width = std::max(0.5, item.width * scale);
            item.height = std::max(0.5, item.height * scale);
            break;
        }
        for (UiButton& item : uiButtons) if (item.name == name) {
            item.width = std::max(0.5, item.width * scale);
            item.height = std::max(0.5, item.height * scale);
            break;
        }
        for (UiText& item : uiTexts) if (item.name == name) item.size = std::max(0.5, item.size * scale);
    }
    return toJavaString(env, serializeState());
}

extern "C" JNIEXPORT void JNICALL
Java_com_trg_studio_MainActivity_nativeStop(JNIEnv*, jclass) {
    androidStopRequested.store(true);
}
