/********************************************************************
 * 2017 -
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
#include "block/LocalBlockWriter.h"

namespace Gopherwood {
namespace Internal {

LocalBlockWriter::LocalBlockWriter(int fd) : mLocalSpaceFD(fd) {
    mOffset = 0;
}

int LocalBlockWriter::seek(int64_t offset) {
    int res = lseek(mLocalSpaceFD, offset, SEEK_SET);
    return res;
}

int LocalBlockWriter::writeLocal(const char* buffer, int64_t length) {
    int res = write(mLocalSpaceFD, buffer, length);
    return res;
}

LocalBlockWriter::~LocalBlockWriter() {

}

}
}
