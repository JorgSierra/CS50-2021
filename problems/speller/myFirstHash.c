// Not used hash fuction because it requires many operations it becomes inefficient

unsigned int hash(const char *word)
{
    unsigned int code = 0;
    int index = 0;
    // Determine the how many levels does the word apply to
    for (int i = 0; (word[i] != '\0' && index < levels); i++)
    {
        index++;
    }
    for (int i = 0; i < index; i++)
    {
        code += pow(letters, i) * (word[index - (i + 1)] - 'a');
    }
    return code % N;
}