//
// Created by cpasjuste on 25/09/18.
//

#include <libgen.h>
#include <string.h>

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
