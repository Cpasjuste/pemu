//
// Created by cpasjuste on 25/09/18.
//

#include <libgen.h>
#include <string.h>
#include <sys/types.h>

pid_t waitpid(pid_t pid, int *status, int options) {
    return 0;
}

void __archive_check_child(int in, int out) {

}

int __archive_create_child(const char *cmd, int *child_stdin, int *child_stdout, pid_t *out_child) {
    return 0;
}

char *dirname(char *path) {
    char *p;
    if (path == NULL || *path == '\0')
        return ".";
    p = path + strlen(path) - 1;
    while (*p == '/') {
        if (p == path)
            return path;
        *p-- = '\0';
    }
    while (p >= path && *p != '/')
        p--;
    return
            p < path ? "." :
            p == path ? "/" :
            (*p = '\0', path);
}

char *basename(char *path) {
    char *p;
    if (path == NULL || *path == '\0')
        return ".";
    p = path + strlen(path) - 1;
    while (*p == '/') {
        if (p == path)
            return path;
        *p-- = '\0';
    }
    while (p >= path && *p != '/')
        p--;
    return p + 1;
}
