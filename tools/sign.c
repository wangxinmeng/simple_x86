#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    struct stat st;
    if (argc != 4) {
        fprintf(stderr, "Usage: <input filename> <output filename> <size, mbr or number>\n");
        return -1;
    }
    if (stat(argv[1], &st) != 0) {
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return -1;
    }
    printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);
    
    int blocksize;
    int mbr_flag = 0;
    if (0 == strcmp(argv[3], "mbr"))
    {
        blocksize = 512;
        mbr_flag = 1;
    }
    else
    {
        blocksize = atoi(argv[3]);
    }
    if (blocksize <= 0 )
    {
        fprintf(stderr, "blocksize %d err!!\n", blocksize);
        return -1;
    }
    
    if (st.st_size > blocksize) {
        fprintf(stderr, "%lld >> blocksize %d!!\n", (long long)st.st_size, blocksize);
        return -1;
    }
    
    char *buf = malloc(blocksize);
    if (NULL == buf)
    {
        fprintf(stderr, "malloc buf failed!\n");
        return -1;
    }
    memset(buf, 0, blocksize);
    FILE *ifp = fopen(argv[1], "rb");
    int ori_size = fread(buf, 1, st.st_size, ifp);
    if (ori_size != st.st_size) {
        fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], ori_size);
        return -1;
    }
    fclose(ifp);
    if (1 == mbr_flag)
    {
        buf[510] = 0x55;
        buf[511] = 0xAA;
    }
    FILE *ofp = fopen(argv[2], "wb+");
    int size = fwrite(buf, 1, blocksize, ofp);
    if (size != blocksize) {
        fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
        return -1;
    }
    fclose(ofp);
    free(buf);
    printf("build %d bytes block: '%s' success!\n", blocksize, argv[2]);
    return 0;
}

