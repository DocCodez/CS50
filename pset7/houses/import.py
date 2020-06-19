# TODO This program while be used to learn how to
# use python in conjunction with SQL databases.

#from cs50 import SQL
from sys import argv
from sys import exit
import csv
import sqlite3

# Make sure there is at least one argument passed to the program.
if len(argv) != 2:
    print("Usage: python import.py *.csv")
    exit(1)

# Create the sqlite3 database connection.
db = sqlite3.connect("students.db")
cursor = db.cursor()

# Open the CSV file.
csvFile = open(argv[1], "r")
reader = csv.reader(csvFile)

# Save the headers in a list.
headers = next(reader)

# Iterate through the CSV file.
for row in reader:
    nameList = row[0].split()
    house = row[1]
    birth = int(row[2])

    # If they have a middle name.
    if len(nameList) == 3:
        entities = [nameList[0], nameList[1], nameList[2], house, birth]
        cursor.execute("INSERT INTO students (first, middle, last, house,birth) VALUES (?, ?, ?, ?, ?)", entities)
        db.commit()
    else:
        entities = [nameList[0], None, nameList[1], house, birth]
        cursor.execute("INSERT INTO students (first, middle, last, house,birth) VALUES (?, ?, ?, ?, ?)", entities)
        db.commit()


csvFile.close()
