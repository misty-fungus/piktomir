#include "NetEngine.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32  && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM !=CC_PLATFORM_LINUX)
pm_server_info::pm_server_info(const char* server_name, const char* map_name, short int clients_needed)
{
    strcpy(server_name_, server_name);
    strcpy(map_name_, map_name);
    clients_needed_ = clients_needed;
    connected_clients_ = 0;
}

pm_server_info::~pm_server_info()
{

}

PM_Server_thread::PM_Server_thread(pm_server_info info)
{
    memcpy(&server_info_, &info, sizeof(info));
}

PM_Server_thread::~PM_Server_thread()
{
    
}

void PM_Server_thread::execute()
{
    void* params = NULL;
    pthread_create(&thread_, NULL, PM_Server_thread::server_function, this);
}

void PM_Server_thread::server_loop()
{
    while(server_info_.connected_clients_ < server_info_.clients_needed_)
    {
        send_server_info(server_info_);
    }
}

void* PM_Server_thread::server_function(void* server)
{
    ((PM_Server_thread*) server)->server_loop();
}

void PM_Server_thread::receive_broadcast()
{
    sockaddr_in from;
    socklen_t len;
    
    int sock;
    sockaddr_in addr;
    char ip[16] = "0.0.0.0";
    int bytes_read;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    if(sock < 0) perror("socket creation failed");
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PIKTOMIR_PORT_BC);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) perror("bind failed");
    
    bytes_read = recvfrom(sock, ip, 16, 0, (sockaddr*)&from, &len);
    
    if (strcmp(ip, "0.0.0.0")) printf("Client found on: %s\n", ip);
    
    close(sock);    
}

void PM_Server_thread::send_server_info(pm_server_info info)
{
    int sock;
    sockaddr_in broadcastAddr;
    int broadcastPermission;
    broadcastPermission = 1;
    
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        perror("socket() failed");
    
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                   sizeof(broadcastPermission)) < 0) perror("setsockopt() failed");
    
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
    broadcastAddr.sin_port = htons(PIKTOMIR_PORT_BC);
    
    if (sendto(sock, (void*)&info, sizeof(pm_server_info), 0, (struct sockaddr *)
               &broadcastAddr, sizeof (broadcastAddr)) != sizeof(pm_server_info))
        perror("sendto() sent a different number of bytes than expected");
    
    close(sock);

}

PM_Client::PM_Client()
{
    //set_ip();
}

PM_Client::~PM_Client()
{
    
    
}

set<pm_server_info, Compare_server_info> PM_Client::find_servers()
{
    
    for(short i = 0; i < 10; i++)
    {
        sockaddr_in from;
        socklen_t len;
        
        pm_server_info tmp;
        
        int sock;
        sockaddr_in addr;
        pm_server_info server_info;
        int bytes_read;
        
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        //fcntl(sock, F_SETFL, O_NONBLOCK);
        if(sock < 0) perror("socket creation failed");
        
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PIKTOMIR_PORT_BC);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) perror("bind failed");
        
        bytes_read = recvfrom(sock, &tmp, sizeof(tmp), 0, (sockaddr*)&from, &len);
        servers.insert(tmp);
        
        printf("%s", tmp.map_name_);
        perror("dsf");
        
        close(sock);    

        
        sleep(0.5);
    }
    return servers;
}
/*void PM_Client::get_server_info()
{
    sockaddr_in from;
    socklen_t len;
    
    int sock;
    sockaddr_in addr;
    pm_server_info info;
    int bytes_read;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) perror("socket creation failed");
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PIKTOMIR_PORT_BC);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) perror("bind failed");
    
    bytes_read = recvfrom(sock, &info, sizeof(info), 0, (sockaddr*)&from, &len);
    
    if(info.clients_needed_ - info.connected_clients_ > 0)
       printf("Server: %s Map name: %s Clients needed: %d Clients connected: %d \n", info.server_name_, info.map_name_,
                                                                                     info.clients_needed_, info.connected_clients_);
    
    close(sock);
}*/

void PM_Client::set_ip()
{
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    
    getifaddrs(&ifAddrStruct);
    
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            if(strcmp(ifa->ifa_name, "en0") == 0) strcpy(ip_, addressBuffer);
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    
    printf("current device ip: %s\n", ip_);
}

void PM_Client::send_broadcast(const char* message)
{
    int sock;
    sockaddr_in broadcastAddr;
    int broadcastPermission;
    broadcastPermission = 1;
    
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        perror("socket() failed");
    
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                   sizeof(broadcastPermission)) < 0) perror("setsockopt() failed");
    
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
    broadcastAddr.sin_port = htons(PIKTOMIR_PORT_BC);
    
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)
               &broadcastAddr, sizeof (broadcastAddr)) != strlen(message))
        perror("sendto() sent a different number of bytes than expected");
    
    close(sock);
}
#endif