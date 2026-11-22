// Function return values, parameter passing, and local-scope regression test.
define add_bonus amount (
    num private_value = 100
    return amount + 5
)

add_bonus 3 -> result
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
