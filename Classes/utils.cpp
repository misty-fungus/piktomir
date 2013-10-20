
#include "utils.h"
#include <iostream>
#include "support/zip_support/unzip.h"
#include <sys/stat.h>
#include <time.h>
#include "GameParams.h"

USING_NS_CC;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

void base64_decode(const char* encoded_string, char *out_str, size_t size , int *out_size)
{
    int in_len = size;
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    //std::string ret;
    int out_ind = 0;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                out_str[out_ind++] = char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) out_str[out_ind++] = char_array_3[j];
    }
    *out_size = out_ind;
}

void decodeBinaryLayerData(const char *in, int *out, int size)
{
    char decodedData[512];
    memset(decodedData,0,512);
    int decodedSize;
    //base64_decode(in, decodedData, strlen(in), &size);
    base64_decode(in, decodedData, strlen(in), &decodedSize);
    char *uncompressedData = new char[size];
    decompress(decodedData, &uncompressedData, decodedSize, size);
    
    
    for (int i = 0,j = 0; i < size - 3; i += 4, ++j) {
        const unsigned int gid = uncompressedData[i] |
        uncompressedData[i + 1] << 8 |
        uncompressedData[i + 2] << 16 |
        uncompressedData[i + 3] << 24;
        
        out[ j ] = gid;
    }
    
    delete uncompressedData;
}

void trim(std::string &s)
{
    size_t pos  = s.find('\n');
    
    while(pos != std::string::npos) {
        s.erase(s.begin() + pos);
        pos = s.find('\n');
    }
    
    pos = s.find(' ');
    
    while(pos != std::string::npos) {
        s.erase(s.begin() + pos);
        pos = s.find(' ');
    }
    
    pos = s.find('\t');
    
    while(pos != std::string::npos) {
        s.erase(s.begin() + pos);
        pos = s.find('\t');
    }
}


void decompress(const char *data,char **out,int size, int expectedSize)
{
    z_stream strm;
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *) data;
    strm.avail_in = size;
    strm.next_out = (Bytef *) *out;
    strm.avail_out = expectedSize;
    
    int ret = inflateInit2(&strm, 15 + 32);
    
    if (ret != Z_OK) {
        return;
    }
    
    do {
        ret = inflate(&strm, Z_SYNC_FLUSH);
        
        switch (ret) {
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                return;
        }
        
        if (ret != Z_STREAM_END) {
            int oldSize = expectedSize;
            realloc(*out, expectedSize * 2);
            
            strm.next_out = (Bytef *)(*out + oldSize);
            strm.avail_out = oldSize;
        }
    }
    while (ret != Z_STREAM_END);
    
    if (strm.avail_in != 0) {
        return;
    }
    
    // const int outLength = expectedSize * 2 - strm.avail_out;
    inflateEnd(&strm);
    
    //   out.resize(outLength);
    return;
}

time_t getTimeMs()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    /* Windows */
    FILETIME ft;
    LARGE_INTEGER li;
    
    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    
    unsigned __int64 ret = li.QuadPart;
    ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */
    
    return ret;
#else
    /* Linux */
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    time_t ret = tv.tv_usec;
    /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
    ret /= 1000;
    
    /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
    ret += (tv.tv_sec * 1000);
    
    return ret;
#endif
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

int downloadFile(const char* ftpFile,const char *username, const char *password, const char * file)
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct FtpFile ftpfile={
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
            std::cout << "curl told us" << res << std::endl;
        }
    }
    
    
    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */ 
    
    curl_global_cleanup();
    
    return res;
}

int unzipFile(const char * zipFilename, std::string path)
{
    // Open the zip file
    unzFile zipfile = unzOpen( zipFilename );
    if ( zipfile == NULL )
    {
        printf( "%s: not found\n", zipFilename );
        return -1;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if ( unzGetGlobalInfo( zipfile, &global_info ) != UNZ_OK )
    {
        printf( "could not read file global info\n" );
        unzClose( zipfile );
        return -1;
    }
    
    // Buffer to hold data read from the zip file.
    char read_buffer[ 10000 ];
    
    // Loop to extract all files
    uLong i;
    for ( i = 0; i < global_info.number_entry; ++i )
    {
        // Get info about current file.
        unz_file_info file_info;
        char filename[ 512 ];
        if ( unzGetCurrentFileInfo(
                                   zipfile,
                                   &file_info,
                                   filename,
                                   512,
                                   NULL, 0, NULL, 0 ) != UNZ_OK )
        {
            printf( "could not read file info\n" );
            unzClose( zipfile );
            return -1;
        }
        
        // Check if this entry is a directory or file.
        const size_t filename_length = strlen( filename );
        if ( filename[ filename_length-1 ] == '/' )
        {
            // Entry is a directory, so create it.
            printf( "unzip dir:%s\n", (path + filename).c_str() );
            platform_mkdir( (path + filename).c_str());
        }
        else
        {
            // Entry is a file, so extract it.
            printf( "unzip file:%s\n", (path + filename).c_str() );
            if ( unzOpenCurrentFile( zipfile ) != UNZ_OK )
            {
                printf( "could not open file\n" );
                unzClose( zipfile );
                return -1;
            }
            
            // Open a file to write out the data.
            FILE *out = fopen( (path + filename).c_str(), "wb" );
            if ( out == NULL )
            {
                printf( "could not open destination file\n" );
                unzCloseCurrentFile( zipfile );
                unzClose( zipfile );
                return -1;
            }
            
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile( zipfile, read_buffer, 10000 );
                if ( error < 0 )
                {
                    printf( "error %d\n", error );
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return -1;
                }
                
                // Write data to file.
                if ( error > 0 )
                {
                    fwrite( read_buffer, error, 1, out ); // You should check return of fwrite...
                }
            } while ( error > 0 );
            
            fclose( out );
        }
        
        unzCloseCurrentFile( zipfile );
        
        // Go the the next entry listed in the zip file.
        if ( ( i+1 ) < global_info.number_entry )
        {
            if ( unzGoToNextFile( zipfile ) != UNZ_OK )
            {
                printf( "cound not read next file\n" );
                unzClose( zipfile );
                return -1;
            }
        }
    }
    
    unzClose( zipfile );
    
    return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void resizeSprite(CCSprite *sprite,float width, float height)
{
    sprite->setScaleX(width / sprite->getContentSize().width);
    sprite->setScaleY(height / sprite->getContentSize().height);
}

bool folderExists(std::string path)
{
    struct stat myStat;
    
    return stat(path.c_str(), &myStat) == 0 && ((myStat.st_mode) & S_IFMT) == S_IFDIR;
}
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
std::string to_string (int a)
{
    std::stringstream ss;
    ss << a;
    return ss.str();
}

std::string to_string (bool a)
{
    if (a) return "1";
    else return  "0";
}
#endif