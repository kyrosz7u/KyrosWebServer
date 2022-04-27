//
// Created by 樱吹雪 on 2022/4/26.
//

#ifndef KYROSWEBSERVER_FILESYSTEM_H
#define KYROSWEBSERVER_FILESYSTEM_H

#include "StringPiece.h"
#include "sys/mman.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

/*封装mmap函数*/
namespace base {
class FileSystem: public copyable {
    class FileAddr: public noncopyable{
    public:
        FileAddr(int fd ,size_t len){
            mFileLength = len;
            mAddr = (char *) mmap(nullptr, mFileLength,
                                  PROT_READ, MAP_PRIVATE, fd, 0);
        }
        char *Addr(){ return mAddr; }
        ~FileAddr(){
            if(mAddr){
                munmap(mAddr, mFileLength);
                mAddr = nullptr;
            }
        }
    private:
        char *mAddr;
        size_t mFileLength;
    };

public:
    typedef std::shared_ptr<FileAddr> FilePtr;
    enum FILE_STATE{
        IS_FILE,
        IS_DIR,
        NO_FILE,
        CANNOT_ACCESS,
    };

    FileSystem(){ mFileLength = 0; }

    FILE_STATE Open(const StringPiece &path);
    char* getAddr(){
        if (mFile != nullptr) {
            return mFile->Addr();
        }else{
            return nullptr;
        }
    }
    size_t getLength(){ return mFileLength; }

private:
    StringPiece mPath;
    size_t mFileLength;
    struct stat mFileStat;
    FilePtr mFile;
};
}

#endif //KYROSWEBSERVER_FILESYSTEM_H
