// Regression tests for TRG's binary, regex, and compression libraries.
add (binary)
add (regex)
add (compression)

binary to_bin 42 binary_value
binary from_bin 101010 decoded_number
binary to_hex "TRG" hex_value
binary from_hex "545247" decoded_hex
binary base64_encode "Hello TRG" base64_value
binary base64_decode "SGVsbG8gVFJH" decoded_base64

bit and 12 10 and_value
bit xor 12 10 xor_value
bit shift_left 3 2 shifted_value

checksum crc32 "123456789" crc_value
regex match "TRG language" "^TRG" regex_match
regex search "version 4.2" "[0-9]+\.[0-9]+" regex_position
regex replace "TRG language" "language" "runtime" replaced_text

compress encode "aaabbbcccc" packed
compress decode packed restored

say "binary=" binary_value
say "decoded_number=" decoded_number
say "hex=" hex_value
say "decoded_hex=" decoded_hex
say "base64=" base64_value
say "decoded_base64=" decoded_base64
say "and=" and_value
say "xor=" xor_value
say "shift=" shifted_value
say "crc32=" crc_value
say "regex=" regex_match
say "regex_position=" regex_position
say "replaced=" replaced_text
say "restored=" restored
say "TRG_ADVANCED_LIBRARIES_PASSED"
