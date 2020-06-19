from sys import argv
from sys import exit
import csv

# Make sure the the program usage is correct.
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# Read the dna database into a dictionary: DICT where {"Name": [List of DICT where {"STR": numRepeats]}}
dnaData = dict()

databaseFile = open(argv[1], "r")
reader = csv.reader(databaseFile)
headers = next(reader)
for row in reader:
    for i in range(len(row)):
        if i == 0:
            dnaData[row[i]] = [{} for _ in range(len(row) - 1)]
        else:
            dnaData[row[0]][i-1] = {headers[i]: row[i]}

databaseFile.close()


# Read the sequence into a list or string?
dnaSequence = []
sequenceFile = open(argv[2], "r")
dnaSequence = sequenceFile.read()
sequenceFile.close()

# Figure out how many STR repeats there are and store in DICT: {STR: [Repeats at position]}.
sequenceSTRNum = dict()
headerSTR = headers[1:]


# Initialize the Dictionary with STR repeat values.
for i in headerSTR:
    sequenceSTRNum[i] = []

# Loop through the shortTandems in the database and check for greatest number of repeats in sequence.
for shortTandem in headerSTR:

    for pos in range(len(dnaSequence)):
        count = 0
        start = pos
        end = pos + len(shortTandem)
        while dnaSequence.find(shortTandem, start, end) != -1 and end <= len(dnaSequence) - 1:
            start = end
            end = end + len(shortTandem)
            count += 1
        sequenceSTRNum[shortTandem].append(count)

# Create DICT with {STR: MaxRepeatsInSequence}
maxRepeats = []
for shortTandem in headerSTR:
    maxRepeats.append({shortTandem: str(max(sequenceSTRNum[shortTandem]))})

# Loop through the database and check if anyones STR repeats match and if so return their name.
# Create list of repeat nums.

for key in dnaData:
    if maxRepeats == dnaData[key]:
        print(key)
        exit(0)

print("No match")
