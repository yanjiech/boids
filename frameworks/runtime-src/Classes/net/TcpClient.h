//
//  TcpClient.h
//  UdpTest
//
//  Created by Yanjie Chen on 1/28/15.
//  Copyright (c) 2015 Shanghai Yuyu Network Technology Co. Ltd. All rights reserved.
//

#ifndef __UdpTest__TcpClient__
#define __UdpTest__TcpClient__

#if defined( WIN32 )
#include "windows.h"
#else
#define INVALID_SOCKET -1

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/fcntl.h>
#endif

#include <deque>
#include <string>
#include "pvp.pb.h"

#define MAX_MESSAGE_SIZE 2048

#pragma pack( 4 )
class BoidsTcpMsg{
public:
    BoidsTcpMsg() {
        size = 0;
        p = 0;
    }
    
    void reset() {
        size = 0;
        p = 0;
    }
    
    int size;
    int p;
    unsigned char data[MAX_MESSAGE_SIZE];
};
#pragma pack()

enum TcpClientState {
    TcpDisconnected = 0,
    TcpConnected = 1
};

class TcpClient {
public:
    TcpClient();
    ~TcpClient();
    
    bool init( const std::string& host_ip, unsigned short host_port );
    void destroy();
    
    void connect();
    void disconnect();
    bool send( const unsigned char* data, int size );
    int receive( unsigned char* data, int size );
    
    bool isConnected() { return _state == TcpClientState::TcpConnected; }
    
private:
    BoidsTcpMsg* popMessage( std::deque<BoidsTcpMsg*>* dequeue );
    
#if defined( WIN32 )
    SOCKET _sock;
#else
    int _sock;
#endif
    
    TcpClientState _state;
    std::string _host_ip;
    unsigned short _host_port;
    
    std::deque<BoidsTcpMsg*> _receive_queue;
    std::deque<BoidsTcpMsg*> _reuse_queue;
    int _total_received_length;
};

#endif /* defined(__UdpTest__TcpClient__) */
