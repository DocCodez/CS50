# This program is used to learn how to fetch data from a database.

from sys import exit
from sys import argv
import sqlite3

# Make sure the user passes atleast one argument into the command line.
houses = ["Slytherin", "Ravenclaw", "Hufflepuff", "Gryffindor"]
if len(argv) != 2 and argv[1] not in houses:
    print("Usage: python roster.py 'House Name'")
    exit(1)

# Establish connection to the database.
try:
    db = sqlite3.connect("students.db")
except Error as e:
    print(e)

cursor = db.cursor()

# Fetch the data the user requested.
print(argv[1])
cursor.execute("SELECT first, middle, last, birth FROM students WHERE house = ?", (argv[1],))
data = cursor.fetchall()

for i in data:
    if i[1] == None:
        print(f"{i[0]} {i[2]}, born in {i[3]}")
    else:
        print(f"{i[0]} {i[1]} {i[2]}, born in {i[3]}")
