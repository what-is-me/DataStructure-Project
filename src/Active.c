#include "Active.h"

Active *ActiveNew() {
    Active *ret = new(Active);
    *ret = (Active) {
            .next=NULL,
            .val=0
    };
    return ret;
}

Active *ActiveFind(Active *cur, char name[64]) {
    for (; cur; cur = cur->next) {
        if (strcmp(name, cur->name) == 0)return cur;
    }
    return NULL;
}

Active *ActiveInsert(Active *rt, char *name, int val) {
    Active *it = ActiveFind(rt, name);
    if (it) {
        it->val = val;
        return rt;
    } else {
        it = ActiveNew();
        *it = (Active) {
                .val=val,
                .next=rt
        };
        strcpy(it->name, name);
        return it;
    }
}

void ActiveDel(Active *rt) {
    if (rt == NULL)return;
    if (rt->next)ActiveDel(rt->next);
    free(rt);
}

Active *ActiveErase(Active *rt, char *name) {
    if (rt == 0)return 0;
    if (strcmp(rt->name, name) == 0) {
        Active *ret = rt->next;
        free(rt);
        return ret;
    }
    rt->next = ActiveErase(rt->next, name);
    return rt;
}

void ActiveTraversal(Active *rt, FILE *file) {
    int sum = 0;
    fprintf(file, "  \"Activities\": [");
    for (; rt; rt = rt->next) {
        if (strcmp(rt->name, "null") == 0 || rt->name[0] == 0)continue;
        sum += rt->val;
        fprintf(file, " \"%s\"", rt->name);
        if (rt->next)fprintf(file, ",");
    }
    fprintf(file, " ],\n");
    fprintf(file, "  \"sum-of-score\": %d\n", sum);
}

void ActiveUnload(Active *rt, char *filename) {
    FILE *file = fopen(filename, "a+");
    for (; rt; rt = rt->next) {
        fprintf(file, "%s %d\n", rt->name, rt->val);
    }
    fprintf(file, "null 0\n");
    fclose(file);
}

Active *ActiveLoad(FILE *file) {
    String name;
    int val;
    Active *ret = NULL;
    while (~fscanf(file, "%s%d", name, &val)) {
        if (strcmp(name, "null") == 0)break;
        ret = ActiveInsert(ret, name, val);
    }
    return ret;
}
