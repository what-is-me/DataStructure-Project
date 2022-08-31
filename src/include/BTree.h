#ifndef STUDENT_SYSTEM_BTREE_H
#define STUDENT_SYSTEM_BTREE_H

#include "base.h"
#include "Active.h"
#include "Lesson.h"

#define M 4//阶数是2*M阶
#define TRUE 1
#define FALSE 0
typedef struct {
    String name;            //学生姓名
    int year;               //入学年份
    int birth_year;         //出生日期
    char gender[8];         //性别
    String profession;      //专业
    Lesson *lessons;        //选修哪些课，课程成绩
    Active *activities;     //参加了哪些活动
} Record;

//新建空记录
Record RecordCreate();

typedef struct pair {
    int key;
    Record rcd;
} pair;
typedef struct BTNode {
    pair val[2 * M - 1];
    struct BTNode *child[2 * M];
    struct BTNode *p;
    int keyNum;
    int isLeaf;
} BTNode;
typedef struct BTree {
    BTNode *root;
} BTree;
typedef struct Iterator {
    int i;
    int tag;
    BTNode *data;
} Iterator;

//在[f,l)中找大于等于key的值的地址
pair *lower_bound(pair *f, pair *l, int key);

//新建B树节点
BTNode *BTNodeNew(BTNode *p);

//[ptr,ptr+length)右移shift长度
void shiftBy(void *ptr, int length, int shift, int sz);

//分裂x的第pos个孩子
void split(BTNode *x, int pos);

//在根不满的情况下插入{key,val}
int insertEmpty(BTNode *x, int key, Record val);

//合并x节点第i个key的左右孩子
void merge(BTNode *x, int i);

//在tr中的x子树里删除key
void erase(BTree *tr, BTNode *x, int key);

//新建b-树
BTree *BTreeNew();

//在tr插入{key,val}
int Insert(BTree *tr, int key, Record val);

//在tr里删除key
void Erase(BTree *tr, int key);

//在tr里找到key
Iterator Find(BTree *tr, int key);

//更新姓名等基本信息，删除此类基本信息也包含于本函数
__attribute__((unused)) int StudentUpdate(BTree *tr, int key, Record rcd);

//打印单条学生记录的所有信息,如果simple为1则只打印基本信息
void RecordPrint(Iterator it, FILE *file, int simple);

//中序遍历B-树，输出所有学生信息
int btree_dfs_out_stu(BTNode *rt, FILE *file, int have_print_before, int simple);

//封装上面的函数
void BTreeOutputStu(BTree *tr, FILE *file, int simple);

//下一个函数的辅助函数
void btree_dfs_ul(BTNode *rt, const char *filename);

//将B-树写入文件保存
void BTreeUnload(BTree *tr, const char *filename);

//从文件载入B-树
BTree *BTreeLoad(const char *filename);

//递归删除
void BTNodeDel(BTNode *cur);

//简单封装上面的函数
void BTreeDel(BTree *tr);

//查询某活动参与人数
int BTree_dfs_activities(BTNode *rt, char *name);

//封装上面的函数
int BTreeQueryActivity(BTree *tr, char *name);

typedef struct LessonQueryData {
    double max;
    double min;
    struct TMP {
        double sum;
        int num;
        int passed;
        int good;
    } tmp;
    double ave;
    double passed;
    double good;
} LessonQueryData;

//统计最高分、最低分、人数、总分,优秀人数、及格人数
LessonQueryData BTree_dfs_lessons(BTNode *rt, char *name);

//封装,统计最高分、最低分、平均分、优秀率、及格率
LessonQueryData BTreeQueryLessons(BTree *tr, char *name);

#endif //STUDENT_SYSTEM_BTREE_H
