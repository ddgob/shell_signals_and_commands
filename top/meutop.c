#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/select.h>  

void get_user(char *pid, char *user, size_t size) {
    char path[256];
    struct stat info;

    snprintf(path, sizeof(path), "/proc/%s", pid);

    if (stat(path, &info) == -1) {
        perror("Error while obtaining process information");
        strncpy(user, "Unknown", size);
        return;
    }

    struct passwd *pw = getpwuid(info.st_uid);

    if (pw != NULL) {
        strncpy(user, pw->pw_name, size);
        user[size - 1] = '\0';
    } else {
        strncpy(user, "Unknown", size);
        user[size - 1] = '\0';
    }

}

void get_process_info(char *pid, char *process_name, char *status, size_t size) {
    char path[256];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    file = fopen(path, "r");

    if (file == NULL) {
        strncpy(process_name, "Unknown", size);
        *status = '?';
        return;
    }

    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *start, *end;

        start = strchr(buffer, '(');
        end = strrchr(buffer, ')');

        if (start && end && end > start) {
            size_t name_len = end - start - 1;
            if (name_len >= size) name_len = size - 1;
            strncpy(process_name, start + 1, name_len);
            process_name[name_len] = '\0';

            char *state_ptr = end + 2; 
            if (*state_ptr != '\0') {
                *status = *state_ptr;
            } else {
                *status = '?';
            }
        } else {
            strncpy(process_name, "Unknown", size);
            *status = '?';
        }
    } else {
        strncpy(process_name, "Unknown", size);
        *status = '?';
    }

    fclose(file);

    
}

void print_table() {
    printf("%-7s| %-20s| %-25s| %-6s|\n", "PID", "User", "PROCNAME", "Estado");
    printf("-------|---------------------|--------------------------|-------|\n");
}

void add_line(char *pid, char *user, char *process_name, char state) {
    printf("%-7s| %-20s| %-25s| %-6c|\n", pid, user, process_name, state);
}

int main() {
    char input[256];
    fd_set readfds;
    struct timeval timeout;

    while (1) {
        system("clear");

        print_table();

        int count = 0;

        DIR *dir = opendir("/proc");
        struct dirent *entry;

        if (dir == NULL) {
            perror("Error while opening /proc");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL && count < 20) {
            if (isdigit(*entry->d_name)) {
                char user[64];
                char process_name[256];
                char state;

                get_user(entry->d_name, user, sizeof(user));
                get_process_info(entry->d_name, process_name, &state, sizeof(process_name));

                add_line(entry->d_name, user, process_name, state);

                count++;
            }
        }

        closedir(dir);

        printf("> ");
        fflush(stdout);

 
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (retval == -1) {
            perror("select()");
            break;
        } else if (retval) {
       
            if (fgets(input, sizeof(input), stdin) != NULL) {
            
                input[strcspn(input, "\n")] = 0;

                if (strlen(input) == 0) {
                
                    continue;
                }

                char pid_str[64];
                char signal_str[64];
                int pid;
                int signal;

                if (sscanf(input, "%s %s", pid_str, signal_str) == 2) {
                    pid = atoi(pid_str);
                    signal = atoi(signal_str);

                    if (pid > 0 && signal > 0) {
                        if (kill(pid, signal) == -1) {
                            perror("Error sending signal");
                            sleep(1); 
                        } else {
                            printf("Signal %d sent to process %d\n", signal, pid);
                            sleep(1); 
                        }
                    } else {
                        printf("Invalid PID or signal number\n");
                        sleep(1); 
                    }
                } else {
                    printf("Invalid input. Please enter 'PID SIGNAL'\n");
                    sleep(1); 
                }
            } else {
                
                break;
            }
        } else {
            
            continue;
        }
    }

    return 0;
}