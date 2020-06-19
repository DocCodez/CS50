from cs50 import get_string

# Returns the number of letters in text.


def countLetters(text):
    num = 0
    for c in text:
        if c.isalpha():
            num += 1
    return num

# Returns the number of words in text.


def countWords(text):
    return len(text.split())

# Returns the number of sentences in text.


def countSentences(text):
    noQuestion = text.replace("?", ".")
    allPeriods = noQuestion.replace("!", ".")
    return len(allPeriods.split(".")) - 1


# Get string from user.
text = get_string("Text: ")

# Calculate values.
L = (100 / countWords(text)) * countLetters(text)
S = (100 / countWords(text)) * countSentences(text)
# print(countSentences(text))
# print(countWords(text))
# print(countLetters(text))
# print(f"L: {L}, S: {S}")
index = round((0.0588 * L) - (0.296 * S) - 15.8)

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")