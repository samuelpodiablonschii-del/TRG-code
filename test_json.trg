// Test: JSON library features
add (json)

// Group-scoped variables for JSON
group MyGroup {
    num score = 100
    word player = "Hero"
    toggle ready = true
}

// Create a JSON object in compressed mode (no spaces)
json MyGroup compressed MyJsonResult
say "Compressed:" MyJsonResult

// Create a JSON object in clean mode (one per line, longest names first)
json MyGroup clean MyJsonClean
say "Clean:" MyJsonClean

// Write compressed JSON to a file
create test_output.json (./) MyJsonResult
say "Written test_output.json"

// Read it back
json read (./test_output.json)
say "Read back score:" score
say "Read back player:" player
say "Read back ready:" ready