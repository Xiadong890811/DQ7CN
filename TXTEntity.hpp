//
//  TXTEntity.hpp
//  DQ7CN
//
//  Created by Aidoo on 9/26/15.
//  Copyright © 2015 Aidoo. All rights reserved.
//

#ifndef TXTEntity_hpp
#define TXTEntity_hpp

class TXTEntity{
    
public:
    char  name[0x10];
    unsigned int location;
    unsigned int length;
    
    int macro;
    
    unsigned char *buf;
};

#endif /* TXTEntity_hpp */
