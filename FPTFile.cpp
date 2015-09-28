//
//  FPTFile.cpp
//  DQ7CN
//
//  Created by Aidoo on 9/26/15.
//  Copyright © 2015 Aidoo. All rights reserved.
//

#include "FPTFile.hpp"

#include <sys/stat.h>
#include "CodeHelper.hpp"

void FPTFile::ReadFile(const char *path){
    
    struct stat fileInfo;
    if(stat(path, &fileInfo) == -1)
        return;
    
    FILE *fileHandle = fopen(path, "r");
    if(!fileHandle)
        return;
    this->fileLen = fileInfo.st_size;
    this->fileBuf = std::shared_ptr<unsigned char>(new unsigned char [fileInfo.st_size],
                                                   SHARED_ARRAY_DELETOR(unsigned char));
    fread(this->fileBuf.get(), 1, fileInfo.st_size, fileHandle);
    fclose(fileHandle);
    
    const unsigned long indexStart = 0x10;
    
    int controlNum = 0;
    memcpy(&controlNum, this->fileBuf.get() + 0x8, 0x4);
    unsigned long indexEnd = 32 * controlNum + 16;
    
    for(int idx = indexStart;idx < (int)indexEnd ; idx += 0x20){
        unsigned char * fileIndexBuf = this->fileBuf.get() + idx;
        
        TXTEntity *entity =  new TXTEntity;
        memset(entity, 0, sizeof(TXTEntity));
        
        memcpy((void *)entity->name, fileIndexBuf, 0x10);
        memcpy((void *)&entity->macro, fileIndexBuf + 0x10, 0x4);
        memcpy((void *)&entity->location, fileIndexBuf + 0x14, 0x4);
        memcpy((void *)&entity->length, fileIndexBuf + 0x18, 0x4);

        this->entities.push_back(std::shared_ptr<TXTEntity>(entity,SHARED_DELETOR(TXTEntity)));
    }
    
    this->indexStart = indexStart;
    this->dataStart = indexEnd;
}



std::shared_ptr<unsigned char> FPTFile::ReadString(TXTEntity *entity){
    
    unsigned long bufLen = entity->length;
    unsigned long location = entity->location;
    
    std::shared_ptr<unsigned char> result(new unsigned char [bufLen + 1], SHARED_ARRAY_DELETOR(unsigned char));
    memset(result.get(), 0x0, bufLen + 1);
    memcpy(result.get(),
           this->fileBuf.get() + this->dataStart + 0x40 + location,
           bufLen);
    
    return result;
}
