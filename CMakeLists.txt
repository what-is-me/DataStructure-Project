cmake_minimum_required(VERSION 3.22)
project(Student_System C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_CURRENT_SOURCE_DIR})
include_directories(src/include)
set(CMAKE_C_STANDARD 23)
#tests
#add_executable(test-lesson Test/test-lesson.c src/LessonBase.c src/Lesson.c src/base.c)
#add_executable(test-active Test/test-active.c src/Active.c)
#add_executable(test-b-tree Test/test-b-tree.c src/BTree.c src/LessonBase.c src/Lesson.c src/base.c src/Active.c)
#main
add_executable(Student_System main.c src/BTree.c src/LessonBase.c src/Lesson.c src/base.c src/Active.c)
