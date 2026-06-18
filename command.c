#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "student.h"
#include "file_io.h"

extern char global_csv_path[];

ShellResult handle_save(char* args, Student** head) {
    (void)args;

    saveStudents(*head, global_csv_path);

    int count = 0;
    Student *current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    printf("Saved %d students to %s.\n", count, global_csv_path);

    return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head) {
    (void)args;

    freeStudents(head); 
    loadStudents(head, global_csv_path);

    int count = 0;
    Student *current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    printf("Reloaded %d students from %s.\n", count, global_csv_path);

    return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head) {
    if (args == NULL || *args == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char *strId = strtok(args, " ");
    char *name = strtok(NULL, " ");
    char *strScore = strtok(NULL, " ");

    if (strId == NULL || name == NULL || strScore == NULL) {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (strspn(strId, "0123456789") != strlen(strId) || strspn(strScore, "0123456789") != strlen(strScore)) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(strId);
    int score = atoi(strScore);

    if (id <= 0 || score < 0 || score > 100) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student *current = *head;
    while (current != NULL) {
        if (current->id == id) {
            printf("Error: duplicate ID.\n"); 
            return SHELL_ERR_DUPLICATE_STUDENT;
        }
        current = current->next;
    }

    addStudent(head, id, name, score);
    
    printf("Student added.\n");

    return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head) {
    if (args == NULL || *args == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (strspn(args, "0123456789") != strlen(args)) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(args);
    if (id <= 0) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (deleteStudent(head, id)) {
        printf("Student deleted.\n");
        return SHELL_OK;
    } else {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_update(char* args, Student** head) {
    if (args == NULL || *args == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char *strId = strtok(args, " "); 
    char *strScore = strtok(NULL, " ");

    if (strId == NULL || strScore == NULL) {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (strspn(strId, "0123456789") != strlen(strId) || strspn(strScore, "0123456789") != strlen(strScore)) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(strId);
    int score = atoi(strScore);
    
    if (id <= 0 || score < 0 || score > 100) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }
    
    if (updateStudent(*head, id, score)) {
        printf("Student updated.\n");
        return SHELL_OK;
    } else {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_find(char* args, Student** head) {
    if (args == NULL || *args == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (strspn(args, "0123456789") != strlen(args)) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int target = atoi(args);
    if (target <= 0) { 
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }
    
    Student *current = *head;

    while (current != NULL) {
        if (current->id == target) {
            printf("ID: %d\n", current->id);
            printf("Name: %s\n", current->name);
            printf("Score: %d\n", current->score);
            return SHELL_OK;
        }
        current = current->next;
    }

    printf("Error: student not found.\n");
    return SHELL_ERR_STUDENT_NOT_FOUND;
}

ShellResult handle_list(char* args, Student** head) {
    (void)args; 
    if (*head == NULL) {
        printf("No students found.\n");
        return SHELL_OK;
    }

    printf("ID   Name      Score\n");
    Student *current = *head;
    while (current != NULL) {
        printf("%-5d%-10s%d\n", current->id, current->name, current->score);
        current = current->next;
    }
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head) {
    (void)args;

    if (*head == NULL) {
        printf("No student data available.\n");
        return SHELL_OK;
    }

    int count = 0;
    int sum = 0;
    int max = -1;
    int min = 101;

    Student *current = *head;
    while (current != NULL) {
        count++;
        sum += current->score;
        
        if (current->score > max) max = current->score;
        if (current->score < min) min = current->score;
        
        current = current->next;
    }

    double average = (double)sum / count;

    printf("Count: %d\n", count);
    printf("Average: %.1f\n", average);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);

    return SHELL_OK;
}

ShellResult handle_help(char* args, Student** head) {
    (void)args;
    (void)head;

    printf("Commands:\n");
    for (int i = 0; i < num; i++) {
        printf("%-24s%s\n", commands[i].usage, commands[i].description);
    }

    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student** head) {
    (void)args;
    (void)head;

    printf("\033[2J\033[H");
    
    fflush(stdout); 

    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head) {
    (void)args; 
    (void)head; 

    printf("Goodbye.\n"); 
    
    return SHELL_EXIT; 
}


#ifdef ADMIN_MODE
Command commands[] = {
    {"save",    NULL, "save",                  "Save students to CSV"},
    {"reload",  NULL, "reload",                "Reload students from CSV"},
    {"add",     handle_add, "add <id> <name> <score>","Add a student"},
    {"delete",  handle_delete, "delete <id>",           "Delete a student"},
    {"update",  handle_update, "update <id> <score>",   "Update student score"},
    {"find",    handle_find, "find <id>",       "Find student"},
    {"list",    handle_list, "list",           "List students"},
    {"stats",   NULL, "stats",                 "Show statistics"},
    {"help",    NULL, "help",                  "Show help"},
    {"clear",   NULL, "clear",                 "Clear screen"},
    {"exit",    handle_exit, "exit",                  "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload",  NULL, "reload",                "Reload students from CSV"},
    {"find",    handle_find, "find <id>",       "Find student"},
    {"list",    handle_list, "list",           "List students"},
    {"stats",   NULL, "stats",                 "Show statistics"},
    {"help",    NULL, "help",                  "Show help"},
    {"clear",   NULL, "clear",                 "Clear screen"},
    {"exit",    handle_exit, "exit",                  "Exit shell"}
};
#endif

int num = sizeof(commands) / sizeof(commands[0]);
