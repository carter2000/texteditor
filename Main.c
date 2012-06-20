#if 1
#include "editor.h"

int main()
{
    editor_loop();
    return 0;
}
#else
#include <stdio.h>

#include "hstring.h"

int main()
{
    HString str1;
    HString str2;
    HString str3;
    HString str4;
    hstr_init(&str1);
    hstr_init(&str2);
    hstr_init(&str3);
    hstr_init(&str4);

    hstr_assign(&str1, "abc");
    hstr_copy(&str1, &str2);
    printf("str1: ");
    hstr_print(&str1);
    printf("\nstr2: ");
    hstr_print(&str2);
    printf("\nstr3: ");
    hstr_print(&str3);
    printf("\n\n");

    printf("str1 empty ? %d\n", hstr_empty(&str1));
    printf("str2 empty ? %d\n", hstr_empty(&str2));
    printf("str3 empty ? %d\n", hstr_empty(&str3));
    printf("\n");

    hstr_assign(&str3, "de");
    printf("str1 compare str2 ? %d\n", hstr_compare(&str1, &str2));
    printf("str1 compare str3 ? %d\n", hstr_compare(&str1, &str3));
    printf("str2 compare str3 ? %d\n", hstr_compare(&str2, &str3));
    printf("\n");

    printf("str1 length: %d\n", hstr_length(&str1));
    printf("str2 length: %d\n", hstr_length(&str2));
    printf("str3 length: %d\n", hstr_length(&str3));
    printf("\n");

    hstr_clear(&str1);
    printf("str1:");
    hstr_print(&str1);
    printf("\n\n");

    str4 = hstr_concat(&str2, &str3);
    hstr_print(&str4);
    printf("\n\n");

    hstr_clear(&str4);
    hstr_assign(&str1, "abcdefghijk");
    str4 = hstr_sub(&str1, 2, 5);
    printf("str1 sub 2 5: ");
    hstr_print(&str4);
    printf("\n\n");

    printf("find str2 in str1: %d\n", hstr_find(&str1, &str2));
    printf("find str3 in str1: %d\n", hstr_find(&str1, &str3));
    printf("find str3 in str2: %d\n", hstr_find(&str2, &str3));
    printf("\n");

    hstr_insert(&str1, &str2, 5);
    printf("str1 insert str2 at pos 5: ");
    hstr_print(&str1);
    printf("\n\n");

    hstr_remove(&str1, 5, hstr_length(&str2));
    printf("str1 removed: ");
    hstr_print(&str1);
    printf("\n\n");

    hstr_assign(&str1, "AabcBabcCabc");
    hstr_replace(&str1, &str2, &str3);
    printf("str1 replaced:");
    hstr_print(&str1);
    printf("\n\n");

    hstr_clear(&str1);
    hstr_clear(&str2);
    hstr_clear(&str3);
    hstr_clear(&str4);

    return 0;
}

#endif
