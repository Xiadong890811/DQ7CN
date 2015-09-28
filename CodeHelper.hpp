//
//  CodeHelper.hpp
//  DQ7CN
//
//  Created by Aidoo on 9/26/15.
//  Copyright © 2015 Aidoo. All rights reserved.
//

#ifndef CodeHelper_h
#define CodeHelper_h


#define SHARED_ARRAY_DELETOR(_T_) ([]( _T_ *p ) { delete[] p; })
#define SHARED_DELETOR(_T_) ([]( _T_ *p ) { delete p; })

#endif /* CodeHelper_h */
