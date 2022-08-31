#ifndef STUDENT_SYSTEM_BASE_H
#define STUDENT_SYSTEM_BASE_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define new(type) (type*)malloc(sizeof(type))
#define Max(x, y) ((x)>(y)?(x):(y))
#define Min(x, y) ((x)>(y)?(y):(x))
#define elif else if
typedef char String[64];

//字母转换成数字
int trans(char c);

//数字还原成字母
char detrans(int x);

//判断字符串是否为空
char *isEmpty(char *s);

//判断是否全是数字
int isAllDigit(char *s);

//判断是否为"unchanged"
int isUnchanged(char *s);

//判断是否为"null"
int isNull(char *s);

#endif //STUDENT_SYSTEM_BASE_H
