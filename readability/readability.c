// Readability
// Created by Adiel Hernandez 4/11/20

#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// This function counts the number of letters in a given text.
int countLetters(string text)
{
    int numLetters = 0;

    // Loop through the text checking for letters and incrementing counter when found.
    for (int i = 0; i < strlen(text) - 1; i++)
    {
        if (isalpha(text[i]))
        {
            numLetters++;
        }
    } // end for

    return numLetters;

} // end countLetters()

// This function counts the number of words in a given text.
int countWords(string text)
{
    int numWords = 0;

    // Loop through the text checking for letters and incrementing counter when found.
    for (int i = 0; i < strlen(text) - 1; i++)
    {
        if (text[i] == ' ')
        {
            numWords++;
        }
    } // end for

    return numWords + 1;

} // end countWords()

// This function counts the number sentences in a given text.
int countSentences(string text)
{
    int numSentences = 0;

    // Loop through the text checking for letters and incrementing counter when found.
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            numSentences++;
        }
    } // end for

    return numSentences;

} // end countWords()

int main(void)
{
    // index = (0.0588 * L) - (0.296 * S) - 15.8
    // Where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.

    // Start by asking the user for some text.
    string text = get_string("Text: ");
    //printf("%s\n", text);

    //Calculate the number of letters in some text.
    //printf("%i letter(s)\n", countLetters(text));

    //Calculate the number of words and sentences in some text.
    //printf("%i word(s)\n", countWords(text));
    //printf("%i sentence(s)\n", countSentences(text));

    //Calculate grade level.
    float L = (100 / (float)(countWords(text))) * countLetters(text);
    float S = (100 / (float)(countWords(text))) * countSentences(text);
    int index = round((0.0588 * L) - (0.296 * S) - 15.8);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}