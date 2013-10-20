    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID&& CC_TARGET_PLATFORM !=CC_PLATFORM_LINUX)


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <ifaddrs.h>
#include <pthread.h>
#include <set>
#include <vector>

using namespace std;

#ifndef net_lib_h
#define net_lib_h

#define PIKTOMIR_PORT_BC 1487
#define PIKTOMIR_PORT 1486

#define LENGTH 32

class pm_server_info
{
public:
    pm_server_info(const char* server_name = "", const char* map_name = "", short clients_needed = 0);
    ~pm_server_info();
    
    char server_name_[LENGTH];
    char map_name_[LENGTH];
    short  clients_needed_, connected_clients_;
    int world_, map_id;
};

class Compare_server_info
{
public:
    bool operator()(pm_server_info s1, pm_server_info s2)
    {
        if(s1.clients_needed_ < s2.clients_needed_ )
            return true;
        else
            return false;
    }
};

class PM_Server_thread
{
public:
    PM_Server_thread(pm_server_info info);
    ~PM_Server_thread();
    
    void execute();
    
private:
    void receive_broadcast();
    void send_server_info(pm_server_info info);
    void server_loop();
    
    static void *server_function(void* server);
    
    pm_server_info server_info_;
    pthread_t thread_;
    
    int listener_;
    vector<int> client_sockets_;
};

class PM_Client
{
public:
    PM_Client();
    ~PM_Client();
    
    set<pm_server_info, Compare_server_info> find_servers();
    
    set<pm_server_info, Compare_server_info> servers;
    
private:
    void set_ip();
    void send_broadcast(const char* message);
    
    char ip_[16] = "0.0.0.0";
    
};
#endif

#endif