//
// Created by 樱吹雪 on 2022/4/26.
//

#include "FileSystem.h"

#include <memory>

using namespace base;
FileSystem::FILE_STATE FileSystem::Open(const StringPiece &path){
    mPath=path;
    if(stat(mPath.data(),&mFileStat)){
        return NO_FILE;
    }
    if(!(mFileStat.st_mode&S_IROTH)){
        return CANNOT_ACCESS;
    }
    if(S_ISDIR(mFileStat.st_mode)){
        return IS_DIR;
    }
    mFileLength = mFileStat.st_size;

    int fd = open(mPath.data(), O_RDONLY);
    mFile = std::make_shared<FileAddr>(fd, mFileLength);
    close(fd);
    return IS_FILE;
}

