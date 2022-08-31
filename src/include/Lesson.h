#ifndef STUDENT_SYSTEM_LESSON_H
#define STUDENT_SYSTEM_LESSON_H

#include "LessonBase.h"

typedef struct LessonData {
    double grade;  //记录成绩
    String teacher;//老师名字
    String address;//上课地址
} LessonData;
typedef struct Lesson {
    int tag;
    LessonData *data;
    struct Lesson *next[39];
} Lesson;
typedef struct LessonResult {
    int num;//总学分
    double sum;//总分
    double min;
    double max;
} LessonResult;

//构造函数
Lesson *LessonNew();

//选择某门课程，如果不在LessonBase中则失败，如果已经选过则更新或者把基本信息置空
int LessonInsert(Lesson *cur, char *lesson, LessonData data);

//查找子函数
Lesson *lesson_find(Lesson *cur, char *lesson);

//删除某门课程
void LessonErase(Lesson *cur, char *lesson);

//析构整个Trie
void LessonDel(Lesson *rt);

//找到某门课的信息,grade=-2表示不存在
LessonData LessonFind(Lesson *cur, char *lesson);

//辅导员添加、更改、删除某学生某课程成绩信息
//添加和更改一样，删除只需把某门课分数置-1
int TeacherUpdate(Lesson *cur, char *lesson, double grade);

//输出课程信息
void LessonTraversal(Lesson *cur, FILE *file);

//将trie树写入文件
void LessonUnload(Lesson *cur, char *filename);

//将trie从文件读出
Lesson *LessonLoad(FILE *file);

#endif //STUDENT_SYSTEM_LESSON_H
