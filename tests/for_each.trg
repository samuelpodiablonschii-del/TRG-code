// Regression test for for each list iteration.
list numbers = 1, 2, 3
num total = 0
for each item in numbers (
    increase total item
)
if total = 6 (
    say "TRG_FOR_EACH_PASSED"
)
else (
    say "ERROR for each did not visit every list item"
)
