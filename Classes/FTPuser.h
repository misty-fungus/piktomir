//
//  user.h
//  Piktomir FTP access
//
//  Created by Anton Dedkov on 02.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __Piktomir_FTP_access__user__
#define __Piktomir_FTP_access__user__

#include <string>
#include "FTPutils.h"

class FTPuser {
private:
    std::string user_;
    std::string passHash_;
    
    std::string writeablePath_;
    
    bool downloadChecked;
    bool uploadChecked;
    
public:
    FTPuser (std::string _writeablePath_);
    FTPuser (std::string _writeablePath_, std::string login);
    FTPuser (std::string _writeablePath_, std::string login, std::string pass);
    
    FTPuser (const FTPuser & user);
    
    bool checkFTP ();
    
    bool uploadMap (std::string mapVerPath, std::string mapPath);
    bool downloadMap ();
    int downloadVerMap ();
    
};

#endif /* defined(__Piktomir_FTP_access__user__) */
