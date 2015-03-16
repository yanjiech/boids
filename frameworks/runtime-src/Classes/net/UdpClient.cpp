//
//  UdpClient.cpp
//  Boids
//
//  Created by Yanjie Chen on 12/23/14.
//
//

#include "UdpClient.h"
#include "cocos2d.h"
#include <memory>

UdpClient::UdpClient() {
    
}

UdpClient::~UdpClient() {
    
}

bool UdpClient::init( const std::string& host_ip, unsigned short host_port, unsigned short local_port ) {
    _host_ip = host_ip;
    _host_port = host_port;
    
#if defined( WIN32 )
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) {
        return false;
    }
#endif
    
    _sock = ::socket( AF_INET, SOCK_DGRAM, 0 );
    if( _sock == INVALID_SOCKET ) {
        return false;
    }
    
    struct sockaddr_in addr;
    memset( &addr, 0, sizeof( addr ));
    addr.sin_family = AF_INET;
    addr.sin_port = htons( local_port );
    addr.sin_addr.s_addr = htonl( INADDR_ANY );
    if( ::bind( _sock, (const struct sockaddr*)&addr, sizeof( addr ) ) != 0 ) {
        return false;
    }

    _seq_no = 0;
    _remote_ack_bits = 0xffffffff;
    
    _ack_no = 0;
    _ack_bits = 0;
    
    _resend_tick = 0;
    _resend_trigger_ticks = DEFAULT_RESEND_TRIGGER_TICKS;
    
    _state = ClientState::Disconnected;
    
    return true;
}

void UdpClient::destroy() {
    for( auto itr = _sent_queue.begin(); itr != _sent_queue.end(); ++itr ) {
        delete *itr;
    }
    _sent_queue.clear();
    
    for( auto itr = _reuse_queue.begin(); itr != _reuse_queue.end(); ++itr ) {
        delete *itr;
    }
    _reuse_queue.clear();
    
#if defined( WIN32 )
    closesocket( _sock );
    WSACleanup();
#else
    close( _sock );
#endif
}

void UdpClient::connect() {
    if( _state == ClientState::Disconnected ) {
        printf( "connecting...\n" );
        BoidsMsg* msg;
        if( _reuse_queue.empty() ) {
            msg = new BoidsMsg();
        }
        else {
            msg = _reuse_queue.back();
            _reuse_queue.pop_back();
        }
        
        for( auto itr = _sent_queue.begin(); itr != _sent_queue.end(); ++itr ) {
            delete *itr;
        }
        _sent_queue.clear();
        
        _ack_bits = 0;
        _ack_no = 0;
        _remote_ack_bits = 0xffffffff;
        
        unsigned short next_seq_no = this->nextSeqNo( _seq_no );
        msg->size = (unsigned short)( HEADER_SIZE );
        msg->proto_id = SYNC_PROTO_ID;
        msg->seq_no = next_seq_no;
        msg->ack_no = _ack_no;
        msg->ack_bits = _ack_bits;
        this->sendBoidsMsg( msg );

        _seq_no = next_seq_no;
        _remote_ack_bits <<= 1;
        _state = ClientState::Sync;
    }
}

bool UdpClient::send( const unsigned char* data, size_t size ) {
    if( _state == ClientState::Disconnected ) {
        this->connect();
        return true;
    }
    else if( _state == ClientState::Sync ) {
        return false;
    }
    
    if( ( _remote_ack_bits & 0x80000000 ) == 0 )
        return false;
    
    unsigned short next_seq_no = this->nextSeqNo( _seq_no );
    BoidsMsg* msg;
    if( _reuse_queue.empty() ) {
        msg = new BoidsMsg();
    }
    else {
        msg = _reuse_queue.back();
        _reuse_queue.pop_back();
    }
    msg->size = (unsigned short)( size + HEADER_SIZE );
    msg->proto_id = PROTO_ID;
    msg->seq_no = next_seq_no;
    msg->ack_no = _ack_no;
    msg->ack_bits = _ack_bits;
    memcpy( msg->data, (const void*)data, size );
    
    printf( "send seq_no: %d\n", _seq_no + 1 );
    
    if( !this->sendBoidsMsg( msg ) ) {
        _reuse_queue.push_back( msg );
        return false;
    }
    
    _sent_queue.push_back( msg );
    _seq_no = next_seq_no;
    _remote_ack_bits <<= 1;
    
    return true;
}

bool UdpClient::sendBoidsMsg( BoidsMsg* msg ) {
    struct sockaddr_in addr;
    memset( &addr, 0, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( _host_port );
    addr.sin_addr.s_addr = inet_addr( _host_ip.c_str() );
    
    boids::PvpMessage pvp_msg;
    pvp_msg.set_version( PROTO_VERSION );
    pvp_msg.set_proto_id( msg->proto_id );
    pvp_msg.set_ack_no( msg->ack_no );
    pvp_msg.set_ack_bits( msg->ack_bits );
    pvp_msg.set_seq_no( msg->seq_no );
	pvp_msg.set_data((const char*)msg->data, msg->size - HEADER_SIZE);
    
    std::string data;
    pvp_msg.SerializeToString( &data );
    _sent_queue.push_back( msg );
    
    return ::sendto( _sock, data.c_str(), data.size(), 0, (const struct sockaddr*)&addr, sizeof( addr ) ) == data.size();
}

void UdpClient::resendBoidsMsgs() {
    //resend this earliest packet that possibly lost
    _resend_tick++;
    if( _resend_tick > _resend_trigger_ticks ) {
        unsigned int bit_mask = 0x80000000;
        for( int i = 1; i <= 32; i++ ) {
            if( ( _remote_ack_bits & bit_mask ) == 0 ) {
                //packet is thought to be lost, resend it
                unsigned short no =  this->prevSeqNo( _seq_no, 32 - i );
                BoidsMsg* msg = this->getSentMsg( no );
                if( msg ) {
                    msg->ack_no = _ack_no;
                    msg->ack_bits = _ack_bits;
                    if( this->sendBoidsMsg( msg ) ) {
                        printf( "resend packet: %d\n", no );
                    }
                    else {
                        printf( "resend packet failed: %d\n", no );
                    }
                }
                break;
            }
            bit_mask >>= 1;
        }
        _resend_tick = 0;
    }
}

int UdpClient::receive( unsigned char* data, size_t size ) {
    int ret = 0;
    do {
        struct sockaddr_in addr;
#if defined(WIN32)
		int len = sizeof(addr);
#else
		socklen_t len = sizeof(addr);
#endif
        char buffer[1024] = {0};
        int recv_bytes = ::recvfrom( _sock, (char*)buffer, 1024, 0, (struct sockaddr *)&addr, &len );
        if( recv_bytes <= 0 ) {
            if( errno != EAGAIN ) {
                cocos2d::log( "udp receive error:%d\n", errno );
            }
            break;
        }
        std::string ip = std::string( inet_ntoa( addr.sin_addr ) );
        unsigned short port = ntohs( addr.sin_port );
        if( ip != _host_ip || port != _host_port ) {
            break;
        }
        
        boids::PvpMessage pvp_message;
        pvp_message.ParseFromArray( buffer, (int)recv_bytes );
        
        if( pvp_message.proto_id() == SYNC_PROTO_ID ) {
            if( _state == ClientState::Sync ) {
                _state = ClientState::Connected;
                _ack_no = pvp_message.seq_no();
                _ack_bits = 0x1;
                _resend_tick = 0;
                _remote_ack_bits = 0xffffffff;
            }
        }
        else {
        
            //udpate remote_ack_bits
            int diff = this->diffBetweenSequenceNumbers( _seq_no, pvp_message.ack_no() );
            if( diff >= 0 ) {
                for( int i = 0; i < 32 - diff; i++ ) {
                    unsigned int in_local_ack_bit = 1 << ( i + diff );
                    unsigned int in_msg_ack_bit = 1 << i;
                    if( ( _remote_ack_bits & in_local_ack_bit ) == 0 && ( pvp_message.ack_bits() & in_msg_ack_bit ) != 0 ) {
                        _remote_ack_bits |= in_local_ack_bit;
                        this->dropPacket( this->prevSeqNo( pvp_message.ack_no(), i ) );
                    }
                }
            }
            
            int shift = this->diffBetweenSequenceNumbers( pvp_message.seq_no(), _ack_no );
            //update ack info
            if( shift == 0 ) {
                break;  //duplicated packet
            }
            else if( shift < 0 ) { //received ealier packet
                unsigned int bit = 1 << (-shift);
                if( ( _ack_bits & bit ) == 0 ) {
                    _ack_bits |= bit;
                }
                else {
                    break;  //duplicated packet
                }
            }
            else {  //latest packet
                _ack_no = pvp_message.seq_no();
                _ack_bits = ( _ack_bits << shift ) | 0x1;
            }
            
            ret = pvp_message.data().size();

			cocos2d::log("receive seq no: %d", pvp_message.seq_no());

            memcpy( data, pvp_message.data().c_str(), ret );
        }
    }while( 0 );
    this->resendBoidsMsgs();
    
    return ret;
}

BoidsMsg* UdpClient::getSentMsg( unsigned short seq_no ) {
    for( auto itr = _sent_queue.begin(); itr != _sent_queue.end(); ++itr ) {
        BoidsMsg* msg = *itr;
        if( msg->seq_no == seq_no ) {
            return msg;
        }
    }
    return nullptr;
}

void UdpClient::dropPacket( unsigned short seq_no ) {
    for( auto itr = _sent_queue.begin(); itr != _sent_queue.end(); ++itr ) {
        BoidsMsg* msg = *itr;
        if( msg->seq_no == seq_no ) {
            _reuse_queue.push_back( ( msg ) );
            _sent_queue.erase( itr );
            break;
        }
    }
}

int UdpClient::diffBetweenSequenceNumbers( unsigned short seq_one, unsigned seq_two ) {
    int diff = seq_one - seq_two;
    if( diff < -0x8000 ) {
        diff += 0x10000;
    }
    else if( diff > 0x8000 ) {
        diff -= 0x10000;
    }
    return diff;
}

unsigned short UdpClient::nextSeqNo( unsigned short seq_no ) {
    return seq_no == 0xffff ? 1 : seq_no + 1;
}

unsigned short UdpClient::prevSeqNo( unsigned short seq_no, unsigned short bias ) {
    return seq_no > bias ? seq_no - bias : seq_no + 0xffff - bias;
}