// Function return values and local-scope regression test.
define get_answer (
    num private_value = 100
    return 8
)

get_answer -> result
say "returned=" result
say "TRG_FUNCTION_RETURN_SCOPE_PASSED"
