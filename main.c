/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add your own header includes here */
#include "student.h"  
#include "file_io.h"  
/* #include "command.h"  */

typedef enum {
    SHELL_OK = 0,
    SHELL_EXIT,
    SHELL_ERR_UNKNOWN_COMMAND,
    SHELL_ERR_INVALID_ARGUMENT,
    SHELL_ERR_MISSING_ARGUMENT,
    SHELL_ERR_FILE_OPEN,
    SHELL_ERR_FILE_WRITE,
    SHELL_ERR_STUDENT_NOT_FOUND,
    SHELL_ERR_DUPLICATE_STUDENT,
    SHELL_ERR_INVALID_SCORE
} ShellResult;

typedef ShellResult (*CommandHandler)(char* args, Student** head);

typedef struct {
    const char* name;
    CommandHandler handler;
    const char* usage;
    const char* description;
} Command;

#ifdef ADMIN_MODE
Command commands[] = {
    {"save",    NULL, "save",                  "Save students to CSV"},
    {"reload",  NULL, "reload",                "Reload students from CSV"},
    {"add",     NULL, "add <id> <name> <score>","Add a student"},
    {"delete",  NULL, "delete <id>",           "Delete a student"},
    {"update",  NULL, "update <id> <score>",   "Update student score"},
    {"find",    NULL, "find <id>",             "Find student"},
    {"list",    NULL, "list",                  "List students"},
    {"stats",   NULL, "stats",                 "Show statistics"},
    {"help",    NULL, "help",                  "Show help"},
    {"clear",   NULL, "clear",                 "Clear screen"},
    {"exit",    NULL, "exit",                  "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload",  NULL, "reload",                "Reload students from CSV"},
    {"find",    NULL, "find <id>",             "Find student"},
    {"list",    NULL, "list",                  "List students"},
    {"stats",   NULL, "stats",                 "Show statistics"},
    {"help",    NULL, "help",                  "Show help"},
    {"clear",   NULL, "clear",                 "Clear screen"},
    {"exit",    NULL, "exit",                  "Exit shell"}
};
#endif

/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(const char *csv_path) {
    char line[500];

#ifdef ADMIN_MODE
    const char *user = "admin> ";
#else
    const char *user = "client> ";
#endif

    while (1) {
        printf("%s", user); 
        fflush(stdout);     

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        char *cmd = strtok(line, " ");
        if (cmd == NULL) continue;
        
        if (strcmp(cmd, "exit") == 0) {
            printf("end\n");
            break;
        }

        int found = 0;
        int num = sizeof(commands) / sizeof(commands[0]);
        for (int i = 0; i < num; i++) {
            if (strcmp(cmd, commands[i].name) == 0) {
                printf("true\n");
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("false\n");
        }
    }
}

/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, const char *csv_path) {
    /* TODO */
    (void)cmd_file;
    (void)csv_path;
}

int main(int argc, char *argv[]) {
    const char *csv_path  = "students.csv"; /* default CSV file */
    const char *cmd_file  = NULL;           /* -f <file> argument */

    /* TODO: Parse command-line arguments.
     *   Supported flags:
     *     -f <file>   run commands from <file> instead of stdin
     *   Remaining positional argument (if any): path to students CSV.
     *
     *   Example parsing skeleton:
     */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            cmd_file = argv[++i];
        } else {
            csv_path = argv[i];
        }
    }

    Student *head = NULL;
    loadStudents(&head, csv_path);
    printStudents(head);

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#else
#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    freeStudents(&head);

    return 0;
}
