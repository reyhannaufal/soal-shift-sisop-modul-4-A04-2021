#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/ubuntu";

char rahasia[10] = "AtoZ_";

/*  ----- Encrypt Atbash Cipher Algorithm ----- */
char *atbash(char folder[])
{
    printf("awal: %s\n", folder);
    int i, a;
    char ch, folder2[100];
    for (i = 0; i < strlen(folder); i++)
    {
        if (folder[i] == '/')
        {
            folder2[i] = '/';
            continue;
        }

        if (folder[i] == '.')
            break;

        if (folder[i] == '_')
        {
            folder2[i] = '_';
            continue;
        }
        shift4_s if (folder[i] != '/' && folder[i] != '\0')
        {
            a = folder[i];
            printf("a: %d -- str: %c\n", a, folder[i]);
            if (a > 25)
            {
                a = 91 - (a - 64);
                ch = a;
                folder2[i] = ch;
            }
            folder2[i] = folder[i];
        }
    }
    printf("akhir: %s\n", folder2);
    return folder2;
}

static int
xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath, "%s%s", dirpath, path);
    printf("%s\n", fpath);

    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    printf("Path: %s\n", path);
    char *folder_rahasia = strstr(path, rahasia);
    printf("Folder Rahasia: %s\n", folder_rahasia);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;

    dp = opendir(fpath);

    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (folder_rahasia != NULL)
        {
            printf("folder_name: %s\n", de->d_name);
        }

        res = (filler(buf, de->d_name, &st, 0));

        if (res != 0)
            break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;

        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int main(int argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}