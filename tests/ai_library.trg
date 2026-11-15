// Test the built-in beginner AI library with a small OR classifier.
add (ai)

ai create gate 2
ai teach gate 0;0 0
ai teach gate 0;1 1
ai teach gate 1;0 1
ai teach gate 1;1 1
ai train gate 3000 0.1

ai classify gate 0;0 off
ai classify gate 1;0 on
say "AI_OR_OFF=" off
say "AI_OR_ON=" on
say "TRG_AI_TESTS_PASSED"
