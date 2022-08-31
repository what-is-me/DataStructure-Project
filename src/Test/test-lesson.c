#include "../include/Lesson.h"
int main(){
    lbs= LessonBaseLoad("test/lesson/sample-lessons.txt");
    FILE* f =fopen("test/lesson/sample-in.txt","r");
    Lesson* ls= LessonLoad(f);
    LessonInsert(ls,"data-structure",(LessonData){.grade=90,.address="",.teacher="Cao"});
    LessonErase(ls,"english-3");
    LessonTraversal(ls,stdout);
    LessonUnload(ls,"test/lesson/sample-out.txt");
    LessonDel(ls);
    LessonBaseDel(lbs);
    return 0;
}