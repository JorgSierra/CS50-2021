# Import get_string from cs50 library
from cs50 import get_string
# Prompt user for input
ccn = get_string("Number : ")
# Get cnn lenght
lenght = len(ccn)
# Check if the lenght is valid
if lenght in [13, 15, 16]:
    # Get first two numbers of the ccn
    firstTwo = ccn[0] + ccn[1]
    # Classify provider with the first two numbers
    if int(firstTwo) in [34, 37]:
        output = "AMEX\n"
    elif int(firstTwo) in range(51, 56):
        output = "MASTERCARD\n"
    elif int(ccn[0]) == 4:
        output = "VISA\n"
    else:
        output = "INVALID\n"
else:
    output = "INVALID\n"
# Luhn's algorithm to check for a valid ccn
if output != "INVALID\n":
    checksum = 0
    for i in range(lenght - 2, -1, -2):
        aux = int(ccn[i]) * 2
        if aux >= 10:
            checksum += int(aux / 10)
            checksum += int(aux % 10)
        else:
            checksum += aux

    for i in range(lenght - 1, -1, -2):
        checksum += int(ccn[i])

    if (checksum % 10) != 0:
        output = "INVALID\n"

print(output, end="")