#include "datastructure.h"
#include "help.h"

const char *lessons_path = "data/lessons.txt";
const char *students_path = "data/students.txt";
BTree *tr;
int identity = -1;  //表明登录身份
int key = 0;
Iterator stu;
String opt, tmp;
char buf[1024];

const char *ReadFile(char *path) {
    FILE *pfile;
    static char data[100000];
    pfile = fopen(path, "rb");
    if (pfile == NULL) {
        return NULL;
    }
    fseek(pfile, 0, SEEK_END);
    int length = ftell(pfile);
    rewind(pfile);
    length = fread(data, 1, length, pfile);
    data[length] = '\0';
    fclose(pfile);
    return data;
}
void SAVE() {
    BTreeUnload(tr, students_path);
    FILE *file = fopen("data/students-table.html", "w");
    fprintf(file, "%s", ReadFile("data/students-table-front.html"));
    BTreeOutputStu(tr, file, 0);
    fprintf(file, "\n");
    fprintf(file, "%s", ReadFile("data/students-table-back.html"));
    fclose(file);
    system("start data/students-table.html");
}

void EXIT() {
    SAVE();
    LessonBaseDel(lbs);
    BTreeDel(tr);
}

void ERROR() {
    fprintf(stderr, "Illegal Input\n");
}

void DENY() {
    fprintf(stderr, "Permission Denied\n");
}

void cpy_s(char *s, char *t) {
    if (isNull(t)) s[0] = 0;
    elif (!isUnchanged(t)) {
        strcpy(s, t);
    }
}

void cpy_d(int *s, char *t, int default_null) {
    if (isNull(t)) s[0] = default_null;
    elif (!isUnchanged(t)) {
        sscanf(t, "%d", s);
    }
}

int main() {
    lbs = LessonBaseLoad(lessons_path);
    tr = BTreeLoad(students_path);

    // help();
    while (TRUE) {
        fflush(stderr);
        fflush(stdout);
        fflush(stdin);

        if (identity == -1) {
            printf(
                "------------------------------------------------------------------------\n"
                "Student System\n"
                "1. list all students' basic information\n"
                "   (Warning: this may be very slow when the number of students is very large)\n"
                "2. list message of a student in detail\n"
                "3. query information of a lesson\n"
                "4. query the number of the students participating in the activity\n"
                "5. login student(if not exists, it will create one)\n"
                "6. login teacher(who manage the activities)\n"
                "7. login instructor(who manage students' grade)\n"
                "8. save\n"
                "9. clear the window and save\n"
                "other. save and quit\n"
                "------------------------------------------------------------------------\n");
            int o;
            scanf("%d", &o);
            if (o == 1 || o == 2) {
                gets(buf);
                tmp[0] = 0;
                if (o == 1) {
                    BTreeOutputStu(tr, stdout, 1);
                } else {
                    int id;
                    puts("input student id:");
                    scanf("%d", &id);
                    RecordPrint(Find(tr, id), stdout, 0);
                    puts("");
                }
            }
            elif (o == 3 || o == 4) {
                if (o == 3) {
                    puts("input the lesson name:");
                    scanf("%s", opt);
                    LessonQueryData ans = BTreeQueryLessons(tr, opt);
                    if (ans.tmp.num > 0)
                        printf(
                            "{\n"
                            "  \"max\": %.1lf,\n"
                            "  \"min\": %.1lf,\n"
                            "  \"ave\": %.1lf,\n"
                            "  \"pass-rate\": \"%.1lf%%\",\n"
                            "  \"good-rate\": \"%.1lf%%\"\n"
                            "}\n",
                            ans.max, ans.min, ans.ave, ans.passed, ans.good);
                    else {
                        puts("no student chose the lesson");
                    }
                } else {
                    puts("input the activity name:");
                    scanf("%s", opt);
                    printf("%d\n", BTreeQueryActivity(tr, opt));
                }
            }
            elif (o == 5 || o == 6 || o == 7) {
                if (o == 6) identity = 2;
                elif (o == 7) identity = 1;
                else {
                    puts("input student id:");
                    scanf("%s", opt);
                    if (isAllDigit(opt)) {
                        identity = 0;
                        sscanf(opt, "%d", &key);
                        stu = Find(tr, key);
                        if (stu.tag == 0) {
                            Insert(tr, key, RecordCreate());
                        }
                        stu = Find(tr, key);
                    } else {
                        ERROR();
                        continue;
                    }
                }
            }
            elif (o == 8) {
                SAVE();
            }
            elif (o == 9) {
                SAVE();
                system("cls");
            }
            else {
                break;
            }
        }  //主菜单

        elif (identity == 0) {
            int o;
            while (1) {
                fflush(stderr);
                fflush(stdout);
                fflush(stdin);
                printf(
                    "------------------------------------------------------------------------\n"
                    "Student System - Student %d\n"
                    "1. add or update an lesson(if the lesson not exists in your lessons, it will insert, otherwise, it will update)\n"
                    "2. delete an lesson\n"
                    "3. update basic information\n"
                    "4. delete yourself\n"
                    "other. log out\n"
                    "------------------------------------------------------------------------\n",
                    key);
                scanf("%d", &o);
                if (o == 1) {
                    puts("input lesson name");
                    scanf("%s", opt);
                    LessonData data = LessonFind(stu.data->val[stu.i].rcd.lessons, opt);
                    if (data.grade < -1.0) data.teacher[0] = 0, data.address[0] = 0, data.grade = -1;
                    printf("type teacher and address, type 'null' as there is no message\n");
                    scanf("%s", tmp);
                    if (isNull(tmp))
                        data.teacher[0] = 0;
                    else if (!isUnchanged(tmp))
                        sscanf(tmp, "%s", data.teacher);
                    scanf("%s", tmp);
                    if (isNull(tmp))
                        data.address[0] = 0;
                    else if (!isUnchanged(tmp))
                        sscanf(tmp, "%s", data.address);
                    if (!LessonInsert(stu.data->val[stu.i].rcd.lessons, opt, data)) {
                        fprintf(stderr, "Can not find the lesson '%s'.\n", opt);
                    }
                }
                elif (o == 2) {
                    scanf("%s", tmp);
                    LessonErase(stu.data->val[stu.i].rcd.lessons, tmp);
                }
                elif (o == 3) {
                    Record *rcd = &stu.data->val[stu.i].rcd;
                    puts("please input your basic information in the order of:");
                    puts("if you don't want to change, type 'unchanged'");
                    puts("if you want to del message of an item, type 'null'");
                    printf("name:");
                    scanf("%s", tmp);
                    cpy_s(rcd->name, tmp);
                    printf("profession:");
                    scanf("%s", tmp);
                    cpy_s(rcd->profession, tmp);
                    printf("gender:");
                    scanf("%s", tmp);
                    cpy_s(rcd->gender, tmp);
                    printf("birth year:");
                    scanf("%s", tmp);
                    cpy_d(&rcd->birth_year, tmp, 0);
                    printf("year come to school:");
                    scanf("%s", tmp);
                    cpy_d(&rcd->year, tmp, 0);
                    RecordPrint(stu, stdout, 0);
                    puts("");
                }
                elif (o == 4) {
                    Erase(tr, key);
                    identity = -1;
                    printf("Student %d has been deleted\n", key);
                    break;
                }
                else {
                    identity = -1;
                    break;
                }
            }
        }  //学生界面

        elif (identity == 2) {
            int o;
            while (1) {
                printf(
                    "------------------------------------------------------------------------\n"
                    "Student System - Teacher\n"
                    "1. add or update an activity to a student\n"
                    "2. delete an activity from a student\n"
                    "other. log out\n"
                    "------------------------------------------------------------------------\n");
                scanf("%d", &o);
                if (o == 1) {
                    int id, val;
                    puts("input student id, activity name and score:");
                    scanf("%s%s%d", opt, tmp, &val);
                    if (isAllDigit(opt)) {
                        sscanf(opt, "%d", &id);
                    } else {
                        ERROR();
                        continue;
                    }
                    Iterator it = Find(tr, id);
                    if (it.tag) {
                        it.data->val[it.i].rcd.activities = ActiveInsert(it.data->val[it.i].rcd.activities, tmp, val);
                    } else {
                        fprintf(stderr, "Student Not Find\n");
                    }
                }
                elif (o == 2) {
                    puts("input a student id and the activity you want to delete");
                    int id;
                    scanf("%s%s", opt, tmp);
                    if (isAllDigit(opt)) {
                        sscanf(opt, "%d", &id);
                    } else {
                        ERROR();
                        continue;
                    }
                    Iterator it = Find(tr, id);
                    if (it.tag) {
                        it.data->val[it.i].rcd.activities = ActiveErase(it.data->val[it.i].rcd.activities, tmp);
                    } else {
                        fprintf(stderr, "Student Not Find\n");
                    }
                }
                else {
                    identity = -1;
                    break;
                }
            }
        }
        elif (identity == 1) {
            int o;
            while (1) {
                printf(
                    "------------------------------------------------------------------------\n"
                    "Student System - instructor\n"
                    "1. update grade of a student\n"
                    "other. log out\n"
                    "------------------------------------------------------------------------\n");
                scanf("%d", &o);
                if (o == 1) {
                    int id;
                    double grade;
                    puts("the student's id:");
                    scanf("%d", &id);
                    Iterator it = Find(tr, id);
                    if (it.tag == 0) {
                        fprintf(stderr, "Student Not Find\n");
                        continue;
                    }
                    puts("type in lesson name and grade:(input null to show the end)");
                    while (~scanf("%s", opt)) {
                        if (isNull(opt)) break;
                        scanf("%s", tmp);
                        if (isNull(tmp))
                            grade = -1;
                        else {
                            sscanf(tmp, "%lf", &grade);
                            if (grade < 0 || grade > 100) {
                                fprintf(stderr, "Illegal grade (should be between 0 to 100)\n");
                                continue;
                            }
                        }
                        if (!TeacherUpdate(it.data->val[it.i].rcd.lessons, opt, grade)) {
                            fprintf(stderr, "Lesson Not Find\n");
                            continue;
                        }
                    }
                } else {
                    identity = -1;
                    break;
                }
            }
        }
    }
    EXIT();
}
