//
//  UdpClient.h
//  Boids
//
//  Created by Yanjie Chen on 12/23/14.
//
//

#ifndef __Boids__UdpClient__
#define __Boids__UdpClient__

#if defined( WIN32 )
#include "winsock2.h"
#include "windows.h"
//#define INVALID_SOCKET 0

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

#define PROTO_VERSION 1
#define PROTO_ID 0x1
#define SYNC_PROTO_ID 0x2
#define HEADER_SIZE 12
#define MAX_MESSAGE_SIZE 2048

#define SEQ_NO_BITS 16

#define DEFAULT_RESEND_TRIGGER_TICKS 5

#pragma pack( 4 )
typedef struct {
    unsigned short size;
    unsigned short proto_id;
    unsigned short seq_no;
    unsigned short ack_no;
    unsigned int ack_bits;
    unsigned char data[MAX_MESSAGE_SIZE];
}BoidsMsg;
#pragma pack()

class EndPoint {
public:
    EndPoint() : ip( "" ), port( 0 ) {}
    EndPoint( const std::string& ip_, unsigned short port_ ) : ip( ip_ ), port( port_ ) {}
    
public:
    std::string ip;
    unsigned short port;
};

enum ClientState {
    Disconnected = 0,
    Connected = 1,
    Sync = 2
};

class UdpClient {
public:
    UdpClient();
    ~UdpClient();
    
    bool init( const std::string& host_ip, unsigned short host_port, unsigned short local_port );
    void destroy();
    
    void connect();
    
    bool send( const unsigned char* data, size_t size );
    bool sendBoidsMsg( BoidsMsg* msg );
    void resendBoidsMsgs();
    int receive( unsigned char* data, size_t size );
    
    bool isConnected() { return _state == ClientState::Connected; }
    
private:
    BoidsMsg* getSentMsg( unsigned short seq_no );
    void dropPacket( unsigned short seq_no );
    int diffBetweenSequenceNumbers( unsigned short seq_one, unsigned seq_two );
    unsigned short nextSeqNo( unsigned short seq_no );
    unsigned short prevSeqNo( unsigned short seq_no, unsigned short bias );
    
    std::string _host_ip;
    unsigned short _host_port;
    
    int _sock;
    
    int _resend_tick;
    int _resend_trigger_ticks;
    
    unsigned short _ack_no;
    unsigned int _ack_bits;
    
    unsigned short _seq_no;
    unsigned int _remote_ack_bits;
    
    std::deque<BoidsMsg*> _sent_queue;
    std::deque<BoidsMsg*> _reuse_queue;
    
    ClientState _state;
};

#endif /* defined(__Boids__UdpClient__) */
