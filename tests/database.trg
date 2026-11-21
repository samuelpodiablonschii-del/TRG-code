// TRG database library smoke test
add (database)
database open "database-test.trgdb"
table create players name score
record add players name "Alex" score 10
record add players name "Jordan" score 25
record get players 1 name found_name
say "Second player:" found_name
query players score >= 20 winners
say "Matching rows:" query_count
record update players 0 score 15
record delete players 1
table count players remaining
say "Remaining rows:" remaining
database save
