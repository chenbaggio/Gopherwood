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
#ifndef GOPHERWOOD_BLOCK_LOCALBLOCKWRITER_H
#define GOPHERWOOD_BLOCK_LOCALBLOCKWRITER_H

#include "platform.h"

#include "common/Memory.h"

namespace Gopherwood {
namespace Internal {

class LocalBlockWriter {
public:
    LocalBlockWriter(int fd);

    int seek(int64_t offset);

    int writeLocal(const char *buffer, int64_t length);

    void flush();

    inline int64_t getCurOffset() {
        return mOffset;
    };

    ~LocalBlockWriter();

private:
    int mLocalSpaceFD;
    int64_t mOffset;            //offset of the local space file
};

}
}
#endif //GOPHERWOOD_BLOCK_LOCALBLOCKWRITER_H
