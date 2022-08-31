#ifndef STUDENT_SYSTEM_LESSONBASE_H
#define STUDENT_SYSTEM_LESSONBASE_H


#include <string.h>
#include <stdio.h>
#include "base.h"

typedef struct LessonBaseData {
    int val;
    String num;//课程编号
    String url;//课程网址
    String type;//课程性质
} LessonBaseData;
typedef struct LessonBase {
    int val;  //记录学分，0表示不存在该课程。
    LessonBaseData *data;
    struct LessonBase *next[39];
} LessonBase;
extern LessonBase *lbs;

//新建节点
LessonBase *LessonBaseNew();

//插入新课程，若存在，则替换
void LessonBaseInsert(LessonBase *cur, char *lesson, LessonBaseData data);

//找到该课程信息，若不存在，val=0
LessonBaseData LessonBaseFind(LessonBase *cur, char *lesson);

/*数据格式：
第一行：n个课程
每一行：名称、学分、编号、网址、性质，如果为空用"null"占位。*/
LessonBase *LessonBaseLoad(const char *filename);

//析构函数
void LessonBaseDel(LessonBase *rt);

#endif //STUDENT_SYSTEM_LESSONBASE_H
