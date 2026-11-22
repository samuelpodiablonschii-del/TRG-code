#!/usr/bin/env python3
"""Build TRG's console test runtime and execute the regression suite."""
from pathlib import Path
import os
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BUILD_DIR = ROOT / ".test-build"
BINARY = BUILD_DIR / ("trg-tests.exe" if os.name == "nt" else "trg-tests")
TESTS = [
    ("beginner_syntax.trg", "TRG_BEGINNER_TESTS_PASSED"),
    ("else_if.trg", "TRG_ELSE_IF_TEST_PASSED"),
    ("function_return_scope.trg", "TRG_FUNCTION_RETURN_SCOPE_PASSED"),
    ("function_nested_return.trg", "TRG_NESTED_RETURN_PASSED"),
    ("for_each.trg", "TRG_FOR_EACH_PASSED"),
    ("dictionary.trg", "TRG_DICTIONARY_PASSED"),
    ("collections_strings.trg", "TRG_FEATURE_TESTS_PASSED"),
    ("advanced_libraries.trg", "TRG_ADVANCED_LIBRARIES_PASSED"),
    ("math_library.trg", "TRG_MATH_TESTS_PASSED"),
    ("plain_english.trg", "TRG_PLAIN_ENGLISH_TESTS_PASSED"),
    ("3d_scene_roundtrip.trg", "TRG_3D_SCENE_ROUNDTRIP_PASSED"),
    ("stack_trace.trg", ["at inner_task (called from line", "at outer_task (called from line"], True),
]

def run(command, **kwargs):
    print("+", " ".join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=ROOT, text=True, **kwargs)

def main():
    BUILD_DIR.mkdir(exist_ok=True)
    compiler = os.environ.get("CXX", "g++")
    built = run([compiler, "-std=c++17", "-O2", "-DTRG_TEST_CONSOLE",
                 str(ROOT / "TRG.cpp"), "-o", str(BINARY)])
    if built.returncode:
        print("\nBUILD FAILED")
        return built.returncode
    failures = []
    for test in TESTS:
        filename, markers = test[0], test[1]
        expect_error = test[2] if len(test) > 2 else False
        if isinstance(markers, str):
            markers = [markers]
        print(f"\n=== {filename} ===", flush=True)
        result = run([str(BINARY), str(ROOT / "tests" / filename)],
                     stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output = result.stdout
        print(output, end="" if output.endswith("\n") else "\n")
        missing_markers = [marker for marker in markers if marker not in output]
        has_error = "ERROR " in output
        if result.returncode or missing_markers or (has_error and not expect_error) or (expect_error and not has_error):
            failures.append(filename)
            print(f"FAIL: {filename}")
        else:
            print(f"PASS: {filename}")
    if failures:
        print("\nFAILED TESTS: " + ", ".join(failures))
        return 1
    print(f"\nALL {len(TESTS)} TRG TESTS PASSED")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
