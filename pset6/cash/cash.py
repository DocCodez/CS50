from cs50 import get_float

# Ask the user for the amount of change owed.

while True:
    change = get_float("Change owed: ")
    if change >= 0:
        break

cents = round(change * 100)
minCoins = 0

# Consider Quarters.
if cents >= 25:
    if cents % 25 == 0:
        minCoins = minCoins + (cents // 25)
        cents = cents - ((cents // 25) * 25)
    else:
        minCoins = minCoins + (cents // 25)
        cents = cents - ((cents // 25) * 25)

# Consider Dimes.
if cents >= 10:
    if cents % 10 == 0:
        minCoins = minCoins + (cents // 10)
        cents = cents - ((cents // 10) * 10)
    else:
        minCoins = minCoins + (cents // 10)
        cents = cents - ((cents // 10) * 10)

# Consider Nickels.
if cents >= 5:
    if cents % 5 == 0:
        minCoins = minCoins + (cents // 5)
        cents = cents - ((cents // 5) * 5)
    else:
        minCoins = minCoins + (cents // 5)
        cents = cents - ((cents // 5) * 5)

# Consider Pennies.
if cents >= 1:
    if cents % 1 == 0:
        minCoins = minCoins + (cents // 1)
        cents = cents - ((cents // 1) * 1)
    else:
        minCoins = minCoins + (cents // 1)
        cents = cents - ((cents // 1) * 1)

print(minCoins)