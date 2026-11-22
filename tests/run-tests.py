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
    ("for_each.trg", "TRG_FOR_EACH_PASSED"),
    ("collections_strings.trg", "TRG_FEATURE_TESTS_PASSED"),
    ("advanced_libraries.trg", "TRG_ADVANCED_LIBRARIES_PASSED"),
    ("math_library.trg", "TRG_MATH_TESTS_PASSED"),
    ("plain_english.trg", "TRG_PLAIN_ENGLISH_TESTS_PASSED"),
    ("3d_scene_roundtrip.trg", "TRG_3D_SCENE_ROUNDTRIP_PASSED"),
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
    for filename, marker in TESTS:
        print(f"\n=== {filename} ===", flush=True)
        result = run([str(BINARY), str(ROOT / "tests" / filename)],
                     stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output = result.stdout
        print(output, end="" if output.endswith("\n") else "\n")
        if result.returncode or marker not in output or "ERROR " in output:
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
