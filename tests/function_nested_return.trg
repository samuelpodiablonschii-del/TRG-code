// Nested function return propagation regression test.
define classify value (
    if value > 0 (
        return "positive"
    )
    return "not positive"
)
classify 2 -> label
if label = "positive" (
    say "TRG_NESTED_RETURN_PASSED"
)
else (
    say "ERROR nested function return failed"
)
