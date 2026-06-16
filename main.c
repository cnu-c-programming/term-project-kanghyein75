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
#include "command.h"  

/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(Student **head, const char *csv_path) {
    char line[500];

#ifdef ADMIN_MODE
    const char *prompt = "admin> ";
#else
    const char *prompt = "client> ";
#endif

    while (1) {
        printf("%s", prompt); 
        fflush(stdout);     

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        char *cmd = strtok(line, " ");
        if (cmd == NULL) continue;
        
        if (strcmp(cmd, "exit") == 0) {
            printf("Goodbye.\n");
            break;
        }

        char *args = strtok(NULL, "");
        if (args != NULL) {
            while (*args == ' ') args++;
        }

        int found = 0;
        for (int i = 0; i < num; i++) {
            if (strcmp(cmd, commands[i].name) == 0) {
                commands[i].handler(args, head);
                found = 1;
                break;
            }
        }

        if (!found) {
#ifdef CLIENT_MODE
            printf("Unknown command or permission denied.\n");
#else
            printf("Unknown command.\n");
#endif
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

    int count = 0;
    Student *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

#ifdef ADMIN_MODE
    printf("[Admin Program]\n");
#elif defined(CLIENT_MODE)
    printf("[Client Program]\n");
#endif
    printf("Loaded %d students from %s.\n", count, csv_path);

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(&head, csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(&head, csv_path);
    }

#else
#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    freeStudents(&head);

    return 0;
}
