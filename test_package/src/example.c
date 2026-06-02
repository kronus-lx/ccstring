#include <ccstring/ccstring.h>

#include <stdio.h>

int main(void)
{
    ccstring_t* s = ccstring_new("hello", 5);
    if (!s) {
        return 1;
    }

    const char* text = ccstring_get(s);
    if (text == NULL || text[0] != 'h') {
        ccstring_destroy(&s);
        return 2;
    }

    printf("%s\n", text);
    ccstring_destroy(&s);
    return 0;
}
