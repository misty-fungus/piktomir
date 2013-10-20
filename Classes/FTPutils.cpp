//
//  user.cpp
//  Piktomir FTP access
//
//  Created by Anton Dedkov on 02.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "FTPutils.h"
#include <errno.h>

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FTPFile *out=(struct FTPFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

int downloadFileFTP(const char* ftpFile,const char *username, const char *password, const char * file)
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct FTPFile ftpfile={
        file,
        NULL
    };
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    curl = curl_easy_init();
    if(curl) {
        char username_password[255];
        sprintf(username_password,"%s:%s",username, password);
        
        curl_easy_setopt(curl, CURLOPT_URL,ftpFile);
        curl_easy_setopt( curl, CURLOPT_USERPWD, username_password );
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
        
        /* Switch on full protocol/debug output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
        res = curl_easy_perform(curl);
        
        /* always cleanup */
        curl_easy_cleanup(curl);
        
        if(CURLE_OK != res) {
            /* we failed */
            std::cout << "curl told us " << res << std::endl;
        }
    }
    
    
    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */
    
    curl_global_cleanup();
    
    return res;
}

bool uploadFileFTP(const char* ftpFile,const char *username, const char *password, const char * file)
{
    CURL *curl;
    CURLcode res;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;
    struct curl_slist *headerlist = NULL;
    
    if (stat(file, &file_info)) {
        printf("Couldnt open '%s': %sn", file, strerror(errno));
        return false;
    }
    
    fsize = (curl_off_t) file_info.st_size;
    printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.n", fsize);
    hd_src = fopen(file, "rb");
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        char username_password[255];
        sprintf(username_password,"%s:%s",username, password);
        
        curl_easy_setopt( curl, CURLOPT_USERPWD, username_password );
        
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, ftpFile);
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t) fsize);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headerlist);
        
        curl_easy_cleanup(curl);
    }
    fclose(hd_src);
    
    curl_global_cleanup();
    return true;
    }

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    size_t retcode = fread(ptr, size, nmemb, (FILE *) stream);
    nread = (curl_off_t) retcode;
    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from filen", nread);
    return retcode;
}

