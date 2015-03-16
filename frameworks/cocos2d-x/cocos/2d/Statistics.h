//
//  Statistics.h
//  cocos2d_libs
//
//  Created by Yanjie Chen on 12/9/14.
//
//

#ifndef cocos2d_libs_Statistics_h
#define cocos2d_libs_Statistics_h

#include <map>
#include <string>
#include <stdio.h>

class Statistics {
public:
    ~Statistics() {}
    
    static Statistics* getInstance() {
        if( _instance == nullptr ) {
            _instance = new Statistics();
        }
        return _instance;
    }
    
    void start() {
        gettimeofday( &_lastTime, nullptr );
    }
    
    void end( const std::string& name ) {
        struct timeval now;
        gettimeofday( &now, nullptr );
        long long sum = ( now.tv_sec - _lastTime.tv_sec ) * 1e6 + now.tv_usec - _lastTime.tv_usec;
        auto itr = _data.find( name );
        if( itr != _data.end() ) {
            itr->second += sum;
        }
        else {
            _data.insert( std::make_pair( name, sum ) );
        }
    }
    
    void clear( const std::string& name ) {
        _data.erase( name );
    }
    
    void clearAll() {
        _data.clear();
    }
    
    void print( const std::string& name ) {
        auto itr = _data.find( name );
        if( itr != _data.end() ) {
            long long time = itr->second;
            printf( "%s takes %lf seconds\n", name.c_str(), time / 1.e6 );
        }
    }
    
    void printAll() {
        for( auto itr = _data.begin(); itr != _data.end(); ++itr ) {
            printf( "%s takes %lf seconds\n", itr->first.c_str(), itr->second / 1.e6 );
        }
    }
    
private:
    Statistics() {}
    
    static Statistics* _instance;
    
    struct timeval _lastTime;
    std::map<std::string, long long> _data;
};

Statistics* Statistics::_instance = nullptr;

#endif
