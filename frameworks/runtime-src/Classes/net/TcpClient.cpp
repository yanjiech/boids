//
//  TcpClient.cpp
//  UdpTest
//
//  Created by Yanjie Chen on 1/28/15.
//  Copyright (c) 2015 Shanghai Yuyu Network Technology Co. Ltd. All rights reserved.
//

#include "TcpClient.h"

TcpClient::TcpClient() {
    
}

TcpClient::~TcpClient() {
    
}

bool TcpClient::init( const std::string& host_ip, unsigned short host_port  ) {
    _total_received_length = 0;
    _host_ip = host_ip;
    _host_port = host_port;
    
#if defined( WIN32 )
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) {
        return false;
    }
#endif
    
    _sock = ::socket( AF_INET, SOCK_STREAM, 0 );
    if( _sock == INVALID_SOCKET ) {
        return false;
    }
    
#if defined( WIN32 )
    u_long ul = 1;
    ioctlsocket( _sock, FIONBIO, &ul );
#else
    int old = fcntl( _sock, F_GETFL, 0 );
    if( fcntl( _sock, F_SETFL, old | O_NONBLOCK) < 0 ) {
        printf( "set nonblock failed" );
        return false;
    }
#endif
    
    _state = TcpClientState::TcpDisconnected;
    
    return true;
}

void TcpClient::destroy() {
#if defined( WIN32 )
    closesocket( _sock );
    WSACleanup();
#else
    close( _sock );
#endif
    _state = TcpClientState::TcpDisconnected;
    for( auto itr = _receive_queue.begin(); itr != _receive_queue.end(); ++itr ) {
        delete *itr;
    }
    _receive_queue.clear();
    for( auto itr = _reuse_queue.begin(); itr != _reuse_queue.end(); ++itr ) {
        delete *itr;
    }
    _reuse_queue.clear();
}

void TcpClient::connect() {
    if( _state == TcpClientState::TcpDisconnected ) {
        struct sockaddr_in addr;
        memset( &addr, 0, sizeof( addr ) );
        addr.sin_family = AF_INET;
        addr.sin_port = htons( _host_port );
        addr.sin_addr.s_addr = inet_addr( _host_ip.c_str() );
        if( ::connect( _sock, (const struct sockaddr *)&addr, sizeof( addr ) ) != 0 ) {
    #if defined( WIN32 )
            int err = WSAGetLastError();
            if( err == WSAEISCONN ) {
                _state = TcpClientState::TcpConnected;
            }
            else if( err != WSAEWOULDBLOCK && err != WSAEINPROGRESS && err != WSAEALREADY ) {
                this->disconnect();
//                printf( "connect to server failed!" );
            }
    #else
            int err = errno;
            if( err == EISCONN ) {
                _state = TcpClientState::TcpConnected;
            }
            else if( err != EWOULDBLOCK && err != EINPROGRESS && err != EALREADY ) {
                this->disconnect();
//                printf( "connect to server failed! error: %d\n", errno );
            }
    #endif
        }
        else {
            _state = TcpClientState::TcpConnected;
        }
    }
    else {
        printf( "already connected!" );
    }
}

void TcpClient::disconnect() {
#if defined( WIN32 )
    ::closesocket( _sock );
#else
    ::close( _sock );
#endif
    _state = TcpClientState::TcpDisconnected;
    for( auto itr = _receive_queue.begin(); itr != _receive_queue.end(); ++itr ) {
        BoidsTcpMsg* msg = *itr;
        msg->reset();
        _reuse_queue.push_back( msg );
    }
    _receive_queue.clear();
}

bool TcpClient::send( const unsigned char* data, int size ) {
    if( this->isConnected() ) {
        return ::send( _sock, (const char *)data, size, 0 ) == size;
    }
    return false;
}

int TcpClient::receive( unsigned char* data, int size ) {
    if( _state == TcpClientState::TcpConnected ) {
        BoidsTcpMsg* msg = this->popMessage( &_reuse_queue );
        if( msg == nullptr ) {
            msg = new BoidsTcpMsg();
        }
        int recv_bytes = (int)::recv( _sock, (char*)msg->data, MAX_MESSAGE_SIZE, 0 );
        if( recv_bytes < 0 ) {
#if defined( WIN32 )
            int err = WSAGetLastError();
            if( err != WSAEWOULDBLOCK && err != WSAEINPROGRESS && err != WSAEALREADY ) {
                printf( "socket failed!" );
                this->disconnect();
                return 0;
            }
#else
            int err = errno;
            if( err != EWOULDBLOCK && err != EINPROGRESS && err != EALREADY ) {
                printf( "socket failed!" );
                this->disconnect();
                return 0;
            }
#endif
            msg->reset();
            _reuse_queue.push_back( msg );
        }
        else if( recv_bytes == 0 ) {
            _state = TcpClientState::TcpDisconnected;
            msg->reset();
            _reuse_queue.push_back( msg );
            return 0;
        }
        else {
            msg->size = recv_bytes;
            _receive_queue.push_back( msg );
            _total_received_length += recv_bytes;
        }
        
        if( _total_received_length >= size ) {
            int total_copy_size = 0;
            while( total_copy_size < size ) {
                msg = this->popMessage( &_receive_queue );
                if( msg == nullptr ) {
                    return 0;
                }
                int copy_size = std::min<int>( size - total_copy_size, msg->size - msg->p );
                memcpy( (void*)data, &msg->data[msg->p], copy_size );
                msg->p += copy_size;
                total_copy_size += copy_size;
                if( msg->p >= msg->size ) {
                    msg->reset();
                    _reuse_queue.push_back( msg );
                }
                else {
                    _receive_queue.push_back( msg );
                }
            }
            
            _total_received_length -= total_copy_size;
            return total_copy_size;
        }
    }
    else if( _state == TcpClientState::TcpDisconnected ) {
        this->connect();
    }
    
    return 0;
}

BoidsTcpMsg* TcpClient::popMessage( std::deque<BoidsTcpMsg*>* dequeue ) {
    if( dequeue != nullptr && !dequeue->empty() ) {
        BoidsTcpMsg* ret = dequeue->front();
        dequeue->pop_front();
        return ret;
    }
    return nullptr;
}