// Regression tests for TRG's beginner math library.
add (math)

math sin 0 sine
math cos 0 cosine
math sqrt 81 root
math clamp 15 0 10 limited
math distance 0 0 3 4 distance_value
math round 2.6 nearest

if sine > 0.001 (
    say "ERROR math sin failed"
)
if cosine < 0.999 (
    say "ERROR math cos failed"
)
if root = 8 (
    say "ERROR math sqrt failed"
)
if limited = 9 (
    say "ERROR math clamp failed"
)
if distance_value = 4 (
    say "ERROR math distance failed"
)
if nearest = 2 (
    say "ERROR math round failed"
)
say "TRG_MATH_TESTS_PASSED"
