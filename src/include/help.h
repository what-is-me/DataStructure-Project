#ifndef STUDENT_SYSTEM_HELP_H
#define STUDENT_SYSTEM_HELP_H

#include <stdio.h>
//帮助信息
void help() {
    fprintf(
            stdout, "Students Message System Help\n"
                    "ls *                              list all students information shortly\n"
                    "ls <student_id> [-a]              list information of student with id student_id shortly\n"
                    "                                  or in detail if with parameter \"-a\"\n"
                    "query lesson <lesson_name>        query the information and the minimum grade,\n"
                    "                                  the maximum grade, the average grade, pass rate,\n"
                    "                                  and excellent rate of <lesson_name>\n"
                    "query act <activity_name>         query the number of students participating in the activity\n"
                    "login student <student_id>        if there exists a student with id <student_id>, just log in;\n"
                    "                                  otherwise, sign up and log in.\n"
                    "login instructor                  the instructor log in\n"
                    "login teacher                     the teacher log in\n"
                    "help                              print this help\n"
                    "save                              save the data\n"
                    "quit                              quit the system\n"
                    "\n"
                    "    the students can do followings:\n"
                    "update                            the student who logged in update his basic information\n"
                    "add <lesson_name>                 the student add lesson or update the lesson of his own\n"
                    "del <lesson_name>                 the student del the lesson of his own, if not exists,do nothing\n"
                    "delete                            delete oneself"
                    "\n"
                    "    the instructor can do followings:\n"
                    "update <student_id>               the instructor update the student's grade\n"
                    "\n"
                    "    the teacher can do followings:\n"
                    "add <student_id>                  the teacher add an activity with value val to the student\n"
                    "    <activity_name> <val>         if the activity exists, just update it\n"
                    "del <student_id>                  the teacher del the activity the student participated in,\n"
                    "    <activity_name>               if not exists, do nothing\n"
                    "\n"
                    "    in this system, we use \"null\" when we want to del the message of an item,\n"
                    "and \"unchanged\" when we don't want to change the item\n\n"
    );
    fflush(stdout);
}

#endif //STUDENT_SYSTEM_HELP_H
