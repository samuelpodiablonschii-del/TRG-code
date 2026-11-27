// Regression test for cached block-boundary lookup across nested blocks.
num value = 0

loop value < 60 (
    if value < 20 (
        increase value
    )
    else if value < 40 (
        increase value
    )
    else (
        increase value
    )
)

if value = 60 (
    say "TRG_BLOCK_INDEX_PASSED"
)
else (
    say "ERROR cached block lookup changed nested control flow"
)
