#include "LessonBase.h"

LessonBase *lbs = 0;

LessonBase *LessonBaseNew() {
    LessonBase *ret = new(LessonBase);
    ret->val = 0;
    ret->data = 0;
    memset(ret->next, 0, sizeof(ret->next));
    return ret;
}

void LessonBaseInsert(LessonBase *cur, char *lesson, LessonBaseData data) {
    if (cur == NULL) {
        fprintf(stderr, "Trie of Lesson Base Root is Empty");
        return;
    }
    for (char *s = lesson; *s; s++) {
        if (cur->next[trans(*s)] == 0)cur->next[trans(*s)] = LessonBaseNew();
        cur = cur->next[trans(*s)];
    }
    cur->val = data.val;
    if (cur->data)free(cur->data);
    cur->data = new(LessonBaseData);
    *cur->data = data;
}

LessonBaseData LessonBaseFind(LessonBase *cur, char *lesson) {
    LessonBaseData ret;
    ret.val = 0;
    for (char *s = lesson; *s; s++) {
        if (cur->next[trans(*s)] == 0)return ret;
        cur = cur->next[trans(*s)];
    }
    return *cur->data;
}

LessonBase *LessonBaseLoad(const char *filename) {
    static String lesson;
    static LessonBaseData data;
    LessonBase *ret = LessonBaseNew();
    FILE *file = fopen(filename, "r");
    int n;
    fscanf(file, "%d", &n);
    while (n--) {
        fscanf(file, "%s", lesson);
        fscanf(file, "%d", &data.val);
        fscanf(file, "%s", data.num);
        fscanf(file, "%s", data.url);
        fscanf(file, "%s", data.type);
        LessonBaseInsert(ret, lesson, data);
    }
    fclose(file);
    return ret;
}

void LessonBaseDel(LessonBase *rt) {
    if (rt == NULL)return;
    for (int i = 0; i < 39; ++i)LessonBaseDel(rt->next[i]);
    free(rt);
}
