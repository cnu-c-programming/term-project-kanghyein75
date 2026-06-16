#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "student.h"

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

ShellResult handle_find(char* args, Student** head) {
    if (args == NULL || *args == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
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

#ifdef ADMIN_MODE
Command commands[] = {
    {"save",    NULL, "save",                  "Save students to CSV"},
    {"reload",  NULL, "reload",                "Reload students from CSV"},
    {"add",     NULL, "add <id> <name> <score>","Add a student"},
    {"delete",  NULL, "delete <id>",           "Delete a student"},
    {"update",  NULL, "update <id> <score>",   "Update student score"},
    {"find",    handle_find, "find <id>",       "Find student"},
    {"list",    handle_list, "list",           "List students"},
    {"stats",   NULL, "stats",                 "Show statistics"},
    {"help",    NULL, "help",                  "Show help"},
    {"clear",   NULL, "clear",                 "Clear screen"},
    {"exit",    NULL, "exit",                  "Exit shell"}
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
    {"exit",    NULL, "exit",                  "Exit shell"}
};
#endif

int num = sizeof(commands) / sizeof(commands[0]);
