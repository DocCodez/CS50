from cs50 import get_int

# Prompt user for an integer between 1 and 8 inclusive.
while True:
    num = get_int("Height: ")
    if num >= 1 and num <= 8:
        break

# Print out the desired pyramid.
star = 1
for i in range(num):
    print(" " * (num - star), end="")
    print("#" * star, end="")
    star += 1
    print()

