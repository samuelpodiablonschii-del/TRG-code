// Function return values and local-scope regression test.
define get_answer (
    num private_value = 100
    return 8
)

get_answer -> result
if result = 8 (
    if private_value = 100 (
        say "ERROR function-local variable leaked"
    )
    else (
        say "TRG_FUNCTION_RETURN_SCOPE_PASSED"
    )
)
else (
    say "ERROR function return value was incorrect"
)
