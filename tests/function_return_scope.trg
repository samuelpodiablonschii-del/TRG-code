// Function return values and local-scope regression test.
define add_bonus amount (
    num private_value = 100
    return amount + 5
)

add_bonus 3 -> result
say "returned=" result
say "TRG_FUNCTION_RETURN_SCOPE_PASSED"
