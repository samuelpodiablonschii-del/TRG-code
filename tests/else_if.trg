// Regression test for ordered else-if branch selection.
num value = 2
num branch = 0

if value = 1 (
    branch = 1
)
else if value = 2 (
    branch = 2
)
else if value = 3 (
    branch = 3
)
else (
    branch = 4
)

if branch = 2 (
    say "TRG_ELSE_IF_TEST_PASSED"
)
else (
    say "ERROR else-if selected the wrong branch"
)
