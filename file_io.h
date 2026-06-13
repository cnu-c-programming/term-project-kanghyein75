#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

void loadStudents(Student **head, const char *filename);
void saveStudents(Student *head, const char *filename);

#endif
