# Imports
import sys
import csv
# Load argv into a variable
argv = sys.argv
# Check for correct comand-line usage
if len(argv) != 3:
    sys.exit("Usage: python dna.py database.csv dnaSequence.txt")
# Create list for headers containing de STR (Short Tandem Repeats)
STR = []
# Read CSV and save the first line on STR
with open(argv[1], "r") as datafile:
    reader = csv.reader(datafile)
    STR = next(reader)
    STR.remove('name')
    STRlen = len(STR)
# Create list of dict for the data base
database = []
# Read CSV into database
with open(argv[1], "r") as datafile:
    reader = csv.DictReader(datafile)
    for row in reader:
        database.append(row)
# Get subject sequence form file
with open(argv[2], "r") as subjectfile:
    sequence = subjectfile.readline()
    # Remove end line
    sequence = sequence[:-1]
# Get sequence lenght
sequencelen = len(sequence)
# Dictionarie for the sequence profile
profile = {}
# For each STR get the number of subsequent repetitions in the sequence
for currentSTR in STR:
    count = 0
    MAX = 0
    lastFound = 0
    consecutive = False
    currentSTRlen = len(currentSTR)
    # Go trough sequence searching for coincidences with sequence slices
    for i in range(0, sequencelen - currentSTRlen + 1):
        # If a coinsidence have been found and the current sequence contains part of the
        # matched last sequence do not process
        if consecutive and (i < lastFound + currentSTRlen):
            continue
        # Get current slice
        currentSlice = sequence[i: i + currentSTRlen]
        # Does the current slice matches the current STR
        if currentSlice == currentSTR:
            lastFound = i
            consecutive = True
            count += 1
        else:
            consecutive = False
            if count > MAX:
                MAX = count
            count = 0
    profile[currentSTR] = MAX
# Is the profile of the sequense in the database
for person in database:
    for currentslice in STR:
        if profile[currentslice] != int(person[currentslice]):
            break
        elif currentslice == STR[STRlen - 1]:
            sys.exit(person["name"])
print("No match")
