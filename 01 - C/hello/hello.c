#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string users_name = get_string("What's your name? ");
    printf("hello, %s\n", users_name);
}