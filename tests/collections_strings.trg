// Regression tests for the expanded TRG list and string commands.
list fruits = "apple", "banana"
insert fruits 1 "pear"
get fruits 1 selected
index_of selected "ea" found_at
pop fruits last_item
join fruits ", " joined
split "red|green|blue" "|" colors
trim "  hello TRG  " clean_text
substring "TRG language" 0 3 prefix
contains clean_text "TRG" has_trg
length colors color_count

say "selected=" selected
say "found_at=" found_at
say "last_item=" last_item
say "joined=" joined
say "color_count=" color_count
say "clean_text=" clean_text
say "prefix=" prefix
say "has_trg=" has_trg
say "TRG_FEATURE_TESTS_PASSED"
