#include <jni.h>
#include <android/bitmap.h>

#include "ImageBuffer.h"

#include <exception>

using trg::photo::ImageBuffer;
using trg::photo::Pixel;

namespace {

bool readBitmap(JNIEnv* env, jobject bitmap, ImageBuffer& output) {
    AndroidBitmapInfo info{};
    if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS) return false;
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 || info.width == 0 || info.height == 0) return false;

    void* pixels = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) return false;
    try {
        output = ImageBuffer(static_cast<int>(info.width), static_cast<int>(info.height));
        for (uint32_t y = 0; y < info.height; ++y) {
            const auto* row = static_cast<const uint8_t*>(pixels) + y * info.stride;
            for (uint32_t x = 0; x < info.width; ++x) {
                const auto* p = row + x * 4;
                output.at(static_cast<int>(x), static_cast<int>(y)) = {p[0], p[1], p[2], p[3]};
            }
        }
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);
        return false;
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return true;
}

jobject createBitmap(JNIEnv* env, int width, int height) {
    jclass bitmapClass = env->FindClass("android/graphics/Bitmap");
    jclass configClass = env->FindClass("android/graphics/Bitmap$Config");
    if (!bitmapClass || !configClass) return nullptr;
    jfieldID configField = env->GetStaticFieldID(configClass, "ARGB_8888",
        "Landroid/graphics/Bitmap$Config;");
    if (!configField) return nullptr;
    jobject config = env->GetStaticObjectField(configClass, configField);
    jmethodID createMethod = env->GetStaticMethodID(bitmapClass, "createBitmap",
        "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    if (!createMethod) return nullptr;
    return env->CallStaticObjectMethod(bitmapClass, createMethod, width, height, config);
}

bool writeBitmap(JNIEnv* env, jobject bitmap, const ImageBuffer& source) {
    AndroidBitmapInfo info{};
    if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS) return false;
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 ||
        info.width != static_cast<uint32_t>(source.width()) ||
        info.height != static_cast<uint32_t>(source.height())) return false;

    void* pixels = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) return false;
    for (uint32_t y = 0; y < info.height; ++y) {
        auto* row = static_cast<uint8_t*>(pixels) + y * info.stride;
        for (uint32_t x = 0; x < info.width; ++x) {
            const Pixel p = source.at(static_cast<int>(x), static_cast<int>(y));
            auto* target = row + x * 4;
            target[0] = p.r; target[1] = p.g; target[2] = p.b; target[3] = p.a;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return true;
}

}

extern "C" JNIEXPORT jobject JNICALL
Java_com_trg_photoeditor_MainActivity_nativeTransform(JNIEnv* env, jobject,
                                                       jobject bitmap, jint operation) {
    ImageBuffer source;
    if (!readBitmap(env, bitmap, source)) return nullptr;

    try {
        ImageBuffer result;
        switch (operation) {
            case 0: result = source.expanded2x(); break;
            case 1: result = source.rotatedClockwise(); break;
            case 2: result = source.flippedHorizontal(); break;
            case 3: result = source.flippedVertical(); break;
            case 4: result = source.grayscale(); break;
            case 5: result = source.inverted(); break;
            case 6: result = source.adjusted(15, 0); break;
            case 7: result = source.adjusted(-15, 0); break;
            case 8: result = source.adjusted(0, 15); break;
            case 9: result = source.adjusted(0, -15); break;
            default: return nullptr;
        }
        jobject output = createBitmap(env, result.width(), result.height());
        if (!output || !writeBitmap(env, output, result)) return nullptr;
        return output;
    } catch (const std::exception&) {
        return nullptr;
    }
}
