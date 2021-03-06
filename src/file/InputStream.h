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
#ifndef _GOPHERWOOD_FILE_INPUTSTREAM_H_
#define _GOPHERWOOD_FILE_INPUTSTREAM_H_

#include "platform.h"

#include "block/BlockInputStream.h"
#include "common/Memory.h"
#include "core/ActiveStatus.h"
#include "oss/oss.h"

namespace Gopherwood {
namespace Internal {

class InputStream {
public:
    InputStream(int fd, shared_ptr<ActiveStatus> status);

    void read(char *buffer, int64_t length);

    void close();

    ~InputStream();

private:
    void updateBlockStream();

    int mLocalSpaceFD;
    shared_ptr<ActiveStatus> mStatus;
    shared_ptr<BlockInputStream> mBlockInputStream;
    int64_t mPos;
};

}
}

#endif //_GOPHERWOOD_FILE_INPUTSTREAM_H_
