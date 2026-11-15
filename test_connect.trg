// Test: add connect and add global
add global (
    num globalScore = 42
    word globalName = "Shared"
)

add connect (test_json.trg)

say "globalScore:" globalScore
say "globalName:" globalName