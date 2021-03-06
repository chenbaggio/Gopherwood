#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "client/gopherwood.h"

static gopherwoodFS fs;
char workDir[] = "/tmp/gopherwood";


void testReadWrite(){
    printf("===========Test Concurrent Read Write ===========\n");
    char input[] = "aaaaaaaaaabbbbbbbbbbcccccccccc";
    char* buffer = (char*)malloc(100);

    gwFile file = gwOpenFile(fs, "/test1", GW_CREAT|GW_RDWR);
    gwWrite(fs, file, input, sizeof(input));
    gwFlush(fs, file);

    gwSeek(fs, file, 10, SEEK_SET);
    int len = gwRead(fs, file, buffer, 20);
    buffer[len] = '\0';
    printf("Read From Gopherwood the first time %s \n", buffer);
    buffer[0] = '\0';

    gwFile file1 = gwOpenFile(fs, "/test1", GW_RDONLY);

    gwSeek(fs, file1, 10, SEEK_SET);
    len = gwRead(fs, file1, buffer, 20);
    buffer[len] = '\0';
    printf("Read From Gopherwood the second time %s \n", buffer);

    gwCloseFile(fs, file1);
    gwCloseFile(fs, file);

    gwDeleteFile(fs, "/test1");
    free(buffer);
};

void testSeekExceedEof(){
    printf("===========Test Seek Exceed Eof ===========\n");
    char input[] = "aaaaaaaaaabbbbbbbbbb";
    char* buffer = (char*)malloc(100);

    gwFile file = gwOpenFile(fs, "/test1", GW_CREAT|GW_RDWR);
    gwSeek(fs, file, 10, SEEK_SET);
    gwWrite(fs, file, input, 20);

    gwSeek(fs, file, 10, SEEK_SET);
    int len = gwRead(fs, file, buffer, 20);
    buffer[len] = '\0';
    printf("Read From Gopherwood the first time %s \n", buffer);
    buffer[0] = '\0';

    gwCloseFile(fs, file);
    gwDeleteFile(fs, "/test1");

    free(buffer);
}

void testWriteExceedQuota(){
    printf("===========Test Seq Write Exceed Quota===========\n");
    char input[] = "0123456789";
    char* buffer = (char*)malloc(200);

    gwFile file = gwOpenFile(fs, "/test1", GW_CREAT|GW_RDWR);
    for (int i=0; i<5; i++) {
        gwWrite(fs, file, input, 10);
    }
    gwCloseFile(fs, file);

    gwFile file1 = gwOpenFile(fs, "/test1", GW_RDONLY);
    int ind = 0;
    for (int pos = 0; pos <30; pos+=2){
        gwSeek(fs, file1, pos, SEEK_SET);
        gwRead(fs, file1, buffer+ind, 1);
        ind+=1;
    }
    buffer[ind] = '\0';
    printf("Read From file %s \n", buffer);
    gwCloseFile(fs, file1);

    gwDeleteFile(fs, "/test1");
    free(buffer);
}

int main(int argc, char *argv[])
{
    gwFormatContext(workDir);

    GWContextConfig config;
    config.blockSize = 10;
    config.numBlocks = 20;
    config.numPreDefinedConcurrency =10;

    fs =  gwCreateContext(workDir, &config);

    testReadWrite();
    testSeekExceedEof();
    testWriteExceedQuota();

    gwDestroyContext(fs);

    return 0;
}