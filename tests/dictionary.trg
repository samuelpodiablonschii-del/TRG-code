// Dictionary regression test.
dictionary player
dict_set player name "Sam"
dict_set player score 42
dict_get player score score_value
dict_has player name has_name
dict_count player entry_count
if score_value = 42 (
    if has_name = true (
        if entry_count = 2 (
            say "TRG_DICTIONARY_PASSED"
        )
    )
)
