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

list source_list = 1, 2
define pass_list value (
    return value
)
pass_list source_list -> returned_list
get returned_list 0 first_item
if first_item = 1 (
    say "TRG_TYPED_LIST_ARGUMENT_PASSED"
)
else (
    say "ERROR function lost typed list argument"
)

dictionary source_dictionary
dict_set source_dictionary score 10
define pass_dictionary value (
    return value
)
pass_dictionary source_dictionary -> returned_dictionary
dict_get returned_dictionary score dictionary_score
if dictionary_score = 10 (
    say "TRG_TYPED_DICTIONARY_ARGUMENT_PASSED"
)
else (
    say "ERROR function lost typed dictionary argument"
)
