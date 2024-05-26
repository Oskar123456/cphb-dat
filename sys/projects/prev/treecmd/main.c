#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define STB_DS_IMPLEMENTATION
#include "../external/stb/stb_ds.h"

#include "../external/sds/sds.h"
#include "../external/sds/sdsalloc.h"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include "../external/glfw/include/GLFW/glfw3.h"
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "../external/cglm/include/cglm/cglm.h"

#include <dirent.h>

typedef uint32_t uint32;

typedef struct {
    sds key;
    int value;
} sdsintmap;

typedef struct {
    int key;
    sds value;
} intsdsmap;

void
nDepths(int *dArr, int d)
{
    if (dArr == NULL)
        return;
    for (int i = 0; i < d; ++i) {
        if (i == 0)
            continue;
        if (i > 1)
            printf(" ");
        if (dArr[i] == 0)
            printf("   ");
        else
            printf("│  ");
    }
}

int
ownsort(const void *d1, const void *d2)
{
    struct dirent *d1p = (struct dirent *)d1;
    struct dirent *d2p = (struct dirent *)d2;

    return strcasecmp(d1p->d_name, d2p->d_name);
}

void
colorprint(const char *str, int col)
{
    printf("\033[0;%dm", col);
    printf("%s", str);
    printf("\033[0m");
}

void
arrprint(int *dArr, int len)
{
    if (dArr == NULL)
        return;
    printf("[");
    for (int i = 0; i < len; ++i)
        printf("%2d", dArr[i]);
    printf("]");
}

void
treeprint(DIR *dp, int depth, sds relpath, int *dArr, int maxDepth,
    int *nDirs, int *nFiles)
{
    if (depth > maxDepth)
        return;
    struct dirent *direntArr = NULL;
    struct dirent *d = NULL;
    while ((d = readdir(dp)) != NULL)
        arrput(direntArr, *d);
    if (direntArr != NULL)
        qsort(direntArr, arrlen(direntArr), sizeof(struct dirent),
            ownsort);

    for (int i = 0; direntArr != NULL && i < arrlen(direntArr); ++i) {
        struct dirent d = direntArr[i];
        sds dn = d.d_name;

        if (strcmp(dn, ".") == 0 || strcmp(dn, "..") == 0)
            continue;
        /* PRINTING */
        nDepths(dArr, depth);

        if (depth > 1)
            printf(" ");
        int hasNext = (i < arrlen(direntArr) - 1) ? 1 : 0;
        if (hasNext)
            printf("├──");
        else
            printf("└──");

        int col = (d.d_type == DT_DIR) ? 35 : 37;
        col = (d.d_type == DT_LNK) ? 33 : col;
        printf(" ");
        colorprint(dn, col);
        // arrprint(dArr, depth);

        printf("\n");
        /* PRINTING */
        if (d.d_type == DT_DIR) {
            *nDirs = *nDirs + 1;

            dArr[depth] = hasNext;

            sds np = sdsnew(relpath);
            if (sdslen(np) > 0 && np[sdslen(np) - 1] != '/')
                np = sdscat(np, "/");
            np = sdscat(np, d.d_name);
            // printf("(%s) (%ld)\n", np, sdslen(np));
            DIR *ndp = opendir(np);
            if (ndp != NULL)
                treeprint(ndp, depth + 1, np, dArr, maxDepth, nDirs,
                    nFiles);
            sdsfree(np);
        } else
            *nFiles = *nFiles + 1;
    }
    arrfree(direntArr);
}

int
main(int argc, char **argv)
{
    DIR *dp;
    struct dirent *dirp;

    if (argc < 2){
        printf("usage: <dir_name> <max_depth (def. 6)>");
        return EXIT_FAILURE;
    }
    else
        colorprint(argv[1], 35);
    printf("\n");
    if ((dp = opendir(argv[1])) == NULL)
        return EXIT_FAILURE;

    int depth;
    if (argc > 2)
        depth = atoi(argv[2]);
    depth = (depth < 1 || depth > 10) ? 10 : depth;

    int dArr[128] = { 0 };
    int nDirs = 0, nFiles = 0;
    treeprint(dp, 1, sdsnew(argv[1]), dArr, depth, &nDirs, &nFiles);
    printf("\n\n");
    printf("%d directories, %d files", nDirs, nFiles);
    printf("\n\n");
    return EXIT_SUCCESS;
}
