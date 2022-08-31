#include "Lesson.h"

Lesson *LessonNew() {
    Lesson *ret = new(Lesson);
    memset(ret->next, 0, sizeof(ret->next));
    ret->tag = 0;
    ret->data = 0;
    return ret;
}

int LessonInsert(Lesson *cur, char *lesson, LessonData data) {
    if (LessonBaseFind(lbs, lesson).val) {
        if (cur == NULL) {
            fprintf(stderr, "Trie of Lesson Base Root is Empty");
            return 0;
        }
        for (char *s = lesson; *s; s++) {
            if (cur->next[trans(*s)] == 0)cur->next[trans(*s)] = LessonNew();
            cur = cur->next[trans(*s)];
        }
        cur->tag = 1;
        if (cur->data) {
            data.grade = cur->data->grade;
            *cur->data = data;
            return 1;
        }
        cur->data = new(LessonData);
        *cur->data = data;
        return 1;
    }
    return 0;
}


Lesson *lesson_find(Lesson *cur, char *lesson) {
    for (char *s = lesson; *s; s++) {
        if (cur->next[trans(*s)] == 0)return NULL;
        cur = cur->next[trans(*s)];
    }
    return cur->tag == 0 ? NULL : cur;
}

void LessonErase(Lesson *cur, char *lesson) {
    cur = lesson_find(cur, lesson);
    if (cur == NULL)return;
    cur->tag = 0;
    if (cur->data)free(cur->data);
    cur->data = 0;
}

void LessonDel(Lesson *rt) {
    if (rt == NULL)return;
    for (int i = 0; i < 39; ++i)LessonDel(rt->next[i]);
    free(rt);
}

LessonData LessonFind(Lesson *cur, char *lesson) {
    cur = lesson_find(cur, lesson);
    if (cur == NULL)return (LessonData) {.grade=-2};
    return *cur->data;
}

LessonResult lesson_dfs(Lesson *cur, LessonBase *curr, char *buf, int pos, FILE *file, int *have_value_before) {
    LessonResult ret = (LessonResult) {
            .num=0,
            .sum=0,
            .max=0,
            .min=100
    };
    if (cur->tag) {
        if (*have_value_before)fprintf(file, ",\n    {\n");
        else fprintf(file, "    {\n");
        fprintf(file, "      \"name\": \"%s\",\n", buf);
        fprintf(file, "      \"num\": \"%s\",\n", isEmpty(curr->data->num));
        if (cur->data->grade >= 0)fprintf(file, "      \"grade\": %.1lf,\n", cur->data->grade);
        else fprintf(file, "      \"grade\": null,\n");
        fprintf(file, "      \"credit\": %d,\n", curr->data->val);
        fprintf(file, "      \"teacher\": \"%s\",\n", isEmpty(cur->data->teacher));
        fprintf(file, "      \"address\": \"%s\",\n", isEmpty(cur->data->address));
        fprintf(file, "      \"url\": \"%s\",\n", isEmpty(curr->data->url));
        fprintf(file, "      \"type\": \"%s\"\n", isEmpty(curr->data->type));
        fprintf(file, "    }");
        if (cur->data->grade >= 0)
            ret = (LessonResult) {
                    .num=curr->val,
                    .sum=cur->data->grade * curr->val,
                    .min=cur->data->grade,
                    .max=cur->data->grade
            };
        *have_value_before = 1;
    }
    for (int i = 0; i < 39; i++) {
        if (cur->next[i]) {
            buf[pos] = detrans(i);
            buf[pos + 1] = 0;
            LessonResult res = lesson_dfs(cur->next[i], curr->next[i], buf, pos + 1, file, have_value_before);
            ret = (LessonResult) {
                    .num=ret.num + res.num,
                    .sum=ret.sum + res.sum,
                    .min=Min(ret.min, res.min),
                    .max=Max(ret.max, res.max)
            };
        }
    }
    return ret;
}

void LessonTraversal(Lesson *cur, FILE *file) {
    static String s;
    fprintf(file, "  \"Lessons\": [\n");
    int have_value_before=0;
    LessonResult res = lesson_dfs(cur, lbs, s, 0, file, &have_value_before);
    fprintf(file, "\n  ],\n");
    if (res.num)
        fprintf(file, "  \"lesson-ave\": %.1lf,\n  \"lesson-min\": %.1lf,\n  \"lesson-max\": %.1lf,\n",
                res.sum / res.num, res.min, res.max);
}

void lesson_dfsul(Lesson *cur, char *buf, int pos, FILE *file) {
    if (cur->tag) {
        fprintf(file, "%s %.1lf %s %s\n", buf, cur->data->grade, isEmpty(cur->data->teacher),
                isEmpty(cur->data->address));
    }
    for (int i = 0; i < 39; i++) {
        if (cur->next[i]) {
            buf[pos] = detrans(i);
            buf[pos + 1] = 0;
            lesson_dfsul(cur->next[i], buf, pos + 1, file);
        }
    }
}

void LessonUnload(Lesson *cur, char *filename) {
    FILE *file = fopen(filename, "a+");
    static String s;
    lesson_dfsul(cur, s, 0, file);
    fprintf(file, "null 0 null null\n");
    fclose(file);
}

Lesson *LessonLoad(FILE *file) {
    Lesson *rt = LessonNew();
    String name;
    LessonData data;
    while (~fscanf(file, "%s%lf%s%s", name, &data.grade, data.teacher, data.address)) {
        if (strcmp(name, "null") == 0)break;
        LessonInsert(rt, name, data);
    }
    return rt;
}

int TeacherUpdate(Lesson *cur, char *lesson, double grade) {
    cur = lesson_find(cur, lesson);
    if (cur == NULL)return 0;
    cur->data->grade = grade;
    return 1;
}

