#include "../include/datastructure.h"

int main() {
    lbs = LessonBaseLoad("test/btree/lessons.txt");
    BTree *tr = BTreeLoad("test/btree/students.txt");

    FILE *json = fopen("test/btree/btree.json", "w");
    BTreeOutputStu(tr, json, 0);
    fclose(json);

    BTreeOutputStu(tr, stdout, 1);

    fprintf(stdout,"num of joined activity-1: %d\n", BTreeQueryActivity(tr, "activity-1"));
    LessonQueryData ret = BTreeQueryLessons(tr, "functions-of-complex-variables-integral-transformations");
    fprintf(stdout,"%f %f %f %f%% %f%%\n", ret.min, ret.max, ret.ave, ret.passed, ret.good);

    LessonBaseDel(lbs);
    BTreeDel(tr);
}
