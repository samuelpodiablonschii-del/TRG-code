// Nested function return propagation regression test.
define choose_value (
    if 1 = 1 (
        return 8
    )
    return 0
)
choose_value -> answer
if answer = 8 (
    say "TRG_NESTED_RETURN_PASSED"
)
else (
    say "ERROR nested function return failed"
)
