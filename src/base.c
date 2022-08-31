#include "base.h"

int trans(char c) {
    if ('0' <= c && c <= '9')return c - '0';
    if ('a' <= c && c <= 'z')return c - 'a' + 10;
    if ('A' <= c && c <= 'Z')return c - 'A' + 10;
    if (c == '(')return 36;
    if (c == ')')return 37;
    if (c == '-')return 38;
    return 38;
}

char detrans(int x) {
    static const char s[40] = "0123456789abcdefghijklmnopqrstuvwxyz()-";
    return s[x];
}

char *isEmpty(char *s) {
    return s[0] ? s : "null";
}

int isAllDigit(char *s) {
    for (; *s; s++) {
        if (!isdigit(*s))return 0;
    }
    return 1;
}

int isUnchanged(char *s) {
    return strcmp(s, "unchanged") == 0;
}

int isNull(char *s) {
    return strcmp(s, "null") == 0;
}
