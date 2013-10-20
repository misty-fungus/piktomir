//
//  user.cpp
//  Piktomir FTP access
//
//  Created by Anton Dedkov on 02.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "FTPuser.h"
#include <stdio.h>
#include "GameParams.h"

const char* const ftpUsersFile = "ftp://n9.infomir.ru:2121/users.txt";
const char* const ftpPath = "ftp://n9.infomir.ru";
const char* const ftpLogin = "piktomir";
const char* const ftpPass = "mexmat2013";
const int ftpPort = 2121;

using namespace std;

FTPuser::FTPuser (const FTPuser & user)
{
    user_ = user.user_;
    passHash_ = user.passHash_;
    
    writeablePath_ = user.writeablePath_;
    
    downloadChecked = user.downloadChecked;
    uploadChecked = user.uploadChecked;
}

FTPuser::FTPuser (std::string _writeablePath_, std::string login, std::string pass)
{
    writeablePath_ = _writeablePath_;
    user_ = login;
    passHash_ = pass;
}

FTPuser::FTPuser (std::string _writeablePath_, std::string login)
{
    writeablePath_ = _writeablePath_;
    user_ = login;
    passHash_ = "e678d62957a6112ba1cc88d47196324e";
}

FTPuser::FTPuser (std::string _writeablePath_)
{
    writeablePath_ = _writeablePath_;
    user_ = "default";
    passHash_ = "e678d62957a6112ba1cc88d47196324e";
}

bool FTPuser::checkFTP ()
{
    uploadChecked = false;
    downloadChecked = false;
    if (user_ == "default" && passHash_ == "e678d62957a6112ba1cc88d47196324e") {
        uploadChecked = true;
        downloadChecked = true;
        return true;
    } if (user_ == "default" && passHash_ == "d41d8cd98f00b204e9800998ecf8427e") {
        uploadChecked = false;
        downloadChecked = true;
        return true;
    }
    
    downloadFileFTP(ftpUsersFile, ftpLogin, ftpPass, (writeablePath_+"users.txt").c_str());
    FILE * users = fopen((writeablePath_+std::string("users.txt")).c_str(), "r");
    
    if (passHash_ != "d41d8cd98f00b204e9800998ecf8427e" && passHash_ != "e678d62957a6112ba1cc88d47196324e")
    {
        while(true)
        {
            char login[100] = "";
            char pass[100] = "";
            if(fscanf(users, "\"%[^\"]\" = \"%[^\"]\"; ",login, pass)==EOF)break;
            
            if (user_ == login && passHash_ == pass)
            {
                uploadChecked = true;
                downloadChecked = true;
                return true;
            }
        
        }
    }
    else
    {
        while(true)
        {
            char login[100] = "";
            char pass[100] = "";
            
            if(fscanf(users, "\"%[^\"]\" = \"%[^\"]\"; ",login, pass) == EOF)
                break;
            
            if (user_ == login)
            {
                uploadChecked = false;
                downloadChecked = true;
                return true;
            }
      //ebb5866fe7d4aa21d544a717150f308f
        }
    }
    
    return false;
}

int FTPuser::downloadVerMap()
{
    if (!downloadChecked)
    {
        return -1;
    }
    
    string mapVerPath = writeablePath_ + "ver.txt";
    char ftpVerFile[100] = "";
    
    
    sprintf(ftpVerFile, "ftp://n9.infomir.ru:2121/%s/ver.txt", user_.c_str());
    
    
    if(downloadFileFTP(ftpVerFile, ftpLogin, ftpPass, mapVerPath.c_str()) != CURLE_OK)
    {
        CCLOGERROR("Error downloading map version from ftp, using local copy.");
        return -1;
    }
    
    int version = -1;
    FILE *file = fopen(mapVerPath.c_str(), "rt");
    
    fscanf(file,"%d",&version);
    
    fclose(file);
    
    return version;
}

bool FTPuser::downloadMap()
{
    if (!downloadChecked)
    {
        return false;
    }
    
    string mapPath = writeablePath_ + "Maps.zip";
    char ftpMapFile[100] = "";
    
    sprintf(ftpMapFile, "ftp://n9.infomir.ru:2121/%s/Maps.zip", user_.c_str());
    
    if(downloadFileFTP(ftpMapFile, ftpLogin, ftpPass, mapPath.c_str()) != CURLE_OK)
    {
        CCLOG("Error downloading map version from ftp, using local copy.");
        return false;
    }
    
    return true;
}

bool FTPuser::uploadMap (std::string mapVerPath, std::string mapPath)
{
    if (!uploadChecked)
    {
        return false;
    }
    
    char ftpVerFile[100] = "";
    sprintf(ftpVerFile, "ftp://n9.infomir.ru:2121/%s/ver.txt", user_.c_str());
    
    char ftpMapFile[100] = "";
    sprintf(ftpMapFile, "ftp://n9.infomir.ru:2121/%s/Maps.zip", user_.c_str());
    
    if(!uploadFileFTP(ftpVerFile, ftpLogin, ftpPass, mapVerPath.c_str()))
    {
        CCLOG("Error uploading version file from local, to ftp.");
        return false;
    }
    
    if(!uploadFileFTP(ftpMapFile, ftpLogin, ftpPass, mapPath.c_str()))
    {
        CCLOG("Error uploading map version from local, to ftp.");
        return false;
    }
    
    return true;
}
