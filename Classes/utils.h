//
//  utils.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 05.12.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__utils__
#define __pictomir_cocos2d_x__utils__

#include <string>
#include <zlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "cocos2d.h"

#define CURL_STATICLIB

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
#include <time.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

void base64_decode(const char* encoded_string, char *out_str, size_t size , int *out_size);
void decompress(const char *data,char **out,int size, int expectedSize);
void decodeBinaryLayerData(const char *in, int *out, int size);
void trim(std::string &s);

bool folderExists(std::string path);

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
int downloadFile(const char* ftpFile,const char *username, const char *password, const char * file);

int unzipFile(const char *filename, std::string path);

void resizeSprite(cocos2d::CCSprite *sprite,float width, float height);

/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */

time_t getTimeMs();

struct FtpFile
{
    const char *filename;
    FILE *stream;
};
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
std::string to_string (int a);
std::string to_string (bool a);
#endif
#endif /* defined(__pictomir_cocos2d_x__base64__) */
