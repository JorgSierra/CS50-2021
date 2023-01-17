# Import get_string from cs50 library
from cs50 import get_int
# Prompt user for input
while True:
    height = get_int("Height: ")
    # Get out of the infinite loop if Height is between 1 and 8 inclusive
    if height in range(1, 9):
        break
# How many floors
for i in range(1, height + 1):
    # How many spaces first
    for j in range(0, height - i):
        print(" ", end="")
    # How many hashes    
    for j in range(0, i):
        print("#", end="")
    print("  ", end="")
    # Last hash row
    for j in range(0, i):
        print("#", end="")
    print()