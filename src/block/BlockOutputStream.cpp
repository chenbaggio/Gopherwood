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
#include "block/BlockOutputStream.h"
#include "common/Exception.h"
#include "common/ExceptionInternal.h"
#include "common/Configuration.h"
#include "common/Logger.h"
#include "file/FileSystem.h"

namespace Gopherwood {
namespace Internal {

BlockOutputStream::BlockOutputStream(int fd) : mLocalSpaceFD(fd) {
    mLocalWriter = shared_ptr<LocalBlockWriter>(new LocalBlockWriter(fd));
    mOssWorker = shared_ptr<OssBlockWorker>(new OssBlockWorker(FileSystem::OSS_CONTEXT, fd));
    mBucketSize = Configuration::LOCAL_BUCKET_SIZE;
    mBlockInfo.reset();
    mCached = false;
}

void BlockOutputStream::setBlockInfo(BlockInfo info) {
    LOG(INFO, "[BlockOutputStream]     |"
              "Set BlockInfo bucketId=%d, new blockOffset=%ld, %s",
        info.bucketId, info.offset, info.isLocal ? "local" : "remote");
    mBlockInfo = info;
}

int64_t BlockOutputStream::remaining() {
    return mBucketSize - mBlockInfo.offset;
}

int64_t BlockOutputStream::write(const char *buffer, int64_t length) {
    int64_t written = -1;

    if (mBlockInfo.isLocal) {
        mLocalWriter->seek(getLocalSpaceOffset());
        LOG(INFO, "[BlockOutputStream]     |"
                  "Write to local space, bucketId=%d, offset=%ld, length=%ld",
            mBlockInfo.bucketId, mBlockInfo.offset, length);
        written = mLocalWriter->writeLocal(buffer, length);
    } else {
        /* Write to OSS */
    }

    mCached = true;

    mBlockInfo.offset += written;
    assert(mBlockInfo.offset <= mBucketSize);

    return written;
}

void BlockOutputStream::flush() {
    if (!mCached){
        return;
    }

    if (mBlockInfo.isLocal) {
        mLocalWriter->flush();
    } else {
        /* TODO: Remote flush, currently we won't write to OSS directly */
    }
}

int64_t BlockOutputStream::getLocalSpaceOffset() {
    return mBlockInfo.bucketId * mBucketSize + mBlockInfo.offset;
}

BlockOutputStream::~BlockOutputStream() {

}

}
}
