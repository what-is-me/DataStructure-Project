#ifndef STUDENT_SYSTEM_ACTIVE_H
#define STUDENT_SYSTEM_ACTIVE_H

#include "base.h"

typedef struct Active {
    String name;
    int val;
    struct Active *next;
} Active;

//新建Active节点
Active *ActiveNew();

//找到某个名字的活动
Active *ActiveFind(Active *cur, String name);

//插入新活动，没有就插入，有就修改,返回新链表
Active *ActiveInsert(Active *rt, String name, int val);

//析构整个链表
void ActiveDel(Active *rt);

//班主任删除学生参与活动情况
Active *ActiveErase(Active *rt, char *name);

//遍历学生参加的活动信息，并统计总综测分
void ActiveTraversal(Active *rt, FILE *file);

//保存到文件中
void ActiveUnload(Active *rt, char *filename);

//从文件读出
Active *ActiveLoad(FILE *file);

#endif //STUDENT_SYSTEM_ACTIVE_H
