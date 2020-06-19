// Week 2 Assignment 2 Caesar.c
// Created by Adiel Hernandez 4/11/20.

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check for an input key at the command line.
    if (argc == 2)
    {
        bool isNum = true;
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                isNum = false;
            } // end if
        } // end for

        if (isNum == true)
        {
            //printf("Success\n");
            int key = atoi(argv[1]);
            //printf("%i\n", key);
            string plainText = get_string("plaintext: ");

            //Shift the text by the key. Ci = (Pi + k) % 26
            string cipherText = plainText;
            for (int i = 0; i < strlen(plainText); i++)
            {
                if (isalpha(plainText[i]))
                {
                    //Deal with uppercase first.
                    if (isupper(plainText[i]))
                    {
                        int asciiDigit = (((plainText[i] - 65) + key) % 26) + 65;
                        cipherText[i] = asciiDigit;
                    }
                    // Deal with lowercase second.
                    else
                    {
                        int asciiDigit = (((plainText[i] - 97) + key) % 26) + 97;
                        cipherText[i] = asciiDigit;
                    }
                }
            }

            printf("ciphertext: %s\n", cipherText);
            return 0;
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }

    } // end if
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    } // end else


}