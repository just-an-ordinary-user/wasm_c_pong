#ifndef __UTILS__H
#define __UTILS__H

// This part of code was inspired by tsoding (https://github.com/tsoding)

// Function to reverse a string
void reverse_cstr(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
}

// Function to get the length of a string
int cstr_len(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// Function to convert an integer to a string
void int_to_cstr(int num, char *str)
{
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    do
    {
        int digit = num % 10;
        str[i++] = '0' + digit;
        num /= 10;
    } while (num != 0);

    // Add '-' for negative numbers
    if (isNegative)
    {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    reverse_cstr(str, i);
}

#endif // __UTILS__H