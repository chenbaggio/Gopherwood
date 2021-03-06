/********************************************************************
 * 2016 -
 * open source under Apache License Version 2.0
 ********************************************************************/
/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "client/gopherwood.h"
#include "common/DateTime.h"
#include "common/Exception.h"
#include "common/ExceptionInternal.h"
#include "gtest/gtest.h"


char workDir[] = "/tmp/gopherwood";

using namespace Gopherwood;
using namespace Gopherwood::Internal;

class TestActiveStatus: public ::testing::Test {
public:
    TestActiveStatus()
    {
        try {
            GWContextConfig config;
            config.blockSize = 10;
            config.numBlocks = 50;
            config.numPreDefinedConcurrency = 10;

            fs =  gwCreateContext(workDir, &config);
        } catch (...) {

        }
    }

    ~TestActiveStatus() {
        try {
            gwDestroyContext(fs);
        } catch (...) {
        }
    }

protected:
    gopherwoodFS fs;

};

TEST_F(TestActiveStatus, TestFormatWorkDir) {
    gwFormatContext(workDir);
}

TEST_F(TestActiveStatus, TestWriteReadConcurrent) {
    char *buffer = (char *) malloc(100);
    char input[] = "aaaaaaaaaabbbbbbbbbbcccccccccc";
    char fileName[] = "TestWriteReadConcurrent";

    gwFile file = gwOpenFile(fs, fileName, GW_CREAT|GW_RDWR);
    gwWrite(fs, file, input, 1);
    gwFlush(fs, file);

    gwSeek(fs, file, 10, SEEK_SET);
    int len = gwRead(fs, file, buffer, 20);
    buffer[len] = '\0';
    printf("Read From Gopherwood the first time %s \n", buffer);
    buffer[0] = '\0';

    gwFile file1 = gwOpenFile(fs, fileName, GW_RDONLY);

    gwSeek(fs, file1, 10, SEEK_SET);
    len = gwRead(fs, file1, buffer, 20);
    buffer[len] = '\0';
    printf("Read From Gopherwood the second time %s \n", buffer);

    gwCloseFile(fs, file1);
    gwCloseFile(fs, file);
    gwDeleteFile(fs, fileName);

    free(buffer);
}

// if the outputStream's position do not sync with the activeStatus's position. the actual writen size is equal the size(input)*writeCount
TEST_F(TestActiveStatus, TestMutilWrite) {
    EXPECT_TRUE(true);
    int writeCount = 10;

    char *buffer = (char *) malloc(100);
    char input[] = "aaaaaaaaaabbbbbbbbbbcccccccccc";
    char fileName[] = "TestWriteReadConcurrent";

    for (int i = 0; i < writeCount; i++) {
        gwFile gwfile = gwOpenFile(fs, fileName, GW_CREAT | GW_RDWR);

        /*2. seek the end of the file*/
        gwSeek(fs, gwfile, 0, SEEK_END);

        /*5. write data to the gopherwood*/
        gwWrite(fs, gwfile, buffer, sizeof(input));

        /*6. close the file*/
        gwCloseFile(fs, gwfile);
    }
    gwDeleteFile(fs, fileName);

    free(buffer);
}
