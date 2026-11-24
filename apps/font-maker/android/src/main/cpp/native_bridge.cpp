#include <jni.h>

#include "FontProject.h"
#include "TrueTypeWriter.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace trg::font;

namespace {

std::string fromJava(JNIEnv* env, jstring value) {
    if (!value) return {};
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (!chars) return {};
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}

}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_trg_fontmaker_MainActivity_nativeExportFont(
    JNIEnv* env, jobject, jstring outputPath, jstring familyName, jstring styleName,
    jintArray codepoints, jintArray advances, jintArray offsets, jshortArray xs, jshortArray ys) {

    const jsize glyphCount = env->GetArrayLength(codepoints);
    if (glyphCount <= 0 || env->GetArrayLength(advances) != glyphCount ||
        env->GetArrayLength(offsets) != glyphCount + 1 ||
        env->GetArrayLength(xs) != env->GetArrayLength(ys)) return JNI_FALSE;

    std::vector<jint> cp(static_cast<std::size_t>(glyphCount));
    std::vector<jint> widths(static_cast<std::size_t>(glyphCount));
    std::vector<jint> pointOffsets(static_cast<std::size_t>(glyphCount + 1));
    const jsize pointCount = env->GetArrayLength(xs);
    std::vector<jshort> pointX(static_cast<std::size_t>(pointCount));
    std::vector<jshort> pointY(static_cast<std::size_t>(pointCount));

    env->GetIntArrayRegion(codepoints, 0, glyphCount, cp.data());
    env->GetIntArrayRegion(advances, 0, glyphCount, widths.data());
    env->GetIntArrayRegion(offsets, 0, glyphCount + 1, pointOffsets.data());
    env->GetShortArrayRegion(xs, 0, pointCount, pointX.data());
    env->GetShortArrayRegion(ys, 0, pointCount, pointY.data());

    FontProject project;
    project.familyName = fromJava(env, familyName);
    project.styleName = fromJava(env, styleName);
    for (jsize i = 0; i < glyphCount; ++i) {
        FontGlyph glyph;
        glyph.codepoint = static_cast<std::uint32_t>(std::max<jint>(0, cp[static_cast<std::size_t>(i)]));
        glyph.advanceWidth = static_cast<std::uint16_t>(std::clamp<jint>(widths[static_cast<std::size_t>(i)], 0, 2000));
        const jint begin = pointOffsets[static_cast<std::size_t>(i)];
        const jint end = pointOffsets[static_cast<std::size_t>(i + 1)];
        if (begin < 0 || end < begin || end > pointCount) return JNI_FALSE;
        if (end - begin >= 3) {
            GlyphContour contour;
            for (jint p = begin; p < end; ++p) {
                contour.points.push_back({
                    static_cast<std::int16_t>(pointX[static_cast<std::size_t>(p)]),
                    static_cast<std::int16_t>(pointY[static_cast<std::size_t>(p)]),
                    true
                });
            }
            glyph.contours.push_back(std::move(contour));
        }
        project.glyphs.push_back(std::move(glyph));
    }

    std::string error;
    return exportTrueType(project, fromJava(env, outputPath), &error) ? JNI_TRUE : JNI_FALSE;
}
