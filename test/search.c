#include <stdio.h>
#include <simdstr.h>

int
main (int argc, char **argv)
{
        const char text[106] = "This is some text in which sherlock searches for him self: Sherlock. also ther is some sxxxxxk weird stuff";
        const char* res = simd_strchr(&text, 'i');

        printf ("%s", res);
}