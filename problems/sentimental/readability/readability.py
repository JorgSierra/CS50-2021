# Import get string
from cs50 import get_string

# Get text from user
text = get_string("Text: ")

# Letters, words, sentences
# words starts in 1 because there is a "missing" ' ' (space) after the last word
data = [0, 1, 0]

# Go trough the text counting letters words and sentences
for i in range(0, len(text)):
    if text[i].isalpha():
        data[0] += 1
    elif text[i].isspace():
        data[1] += 1
    elif (text[i] == '.' or text[i] == '!' or text[i] == '?'):
        data[2] += 1

# Evaluate L and S
L = (data[0] * 100.0) / data[1]  # Letters * 100 / words
S = (data[2] * 100.0) / data[1]  # Sentences * 100 / words

# Evaluate Coleman-Liau index
index = 0.0588 * L - 0.296 * S - 15.8
index = round(index, 0)

# Print the grade of the text
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))