//
//  FPTFile.h
//  DQ7CN
//
//  Created by Aidoo on 9/26/15.
//  Copyright © 2015 Aidoo. All rights reserved.
//

#ifndef FPTFile_h
#define FPTFile_h

#include <string>
#include <vector>
#include <memory>
#include "TXTEntity.hpp"

class FPTFile{
public:
    std::vector<std::shared_ptr<TXTEntity>> entities;

    std::shared_ptr<unsigned char> fileBuf;
    unsigned long fileLen;

    unsigned long indexStart;
    unsigned long dataStart;
    
    
public:
    void ReadFile(const char *path);
    
    std::shared_ptr<unsigned char> ReadString(TXTEntity *entity);
    
    
};

#endif /* FPTFile_h */
