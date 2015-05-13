//
//  JumpText.h
//  Boids
//
//  Created by Yanjie Chen on 3/16/15.
//
//

#ifndef __Boids__JumpText__
#define __Boids__JumpText__

#include "UnitNodeComponent.h"

class JumpText : public UnitNodeComponent {
public:
    JumpText();
    virtual ~JumpText();
    
    static JumpText* create( const std::string& text, const std::string& type, bool is_critical, int unit_camp, const std::string& name );
    virtual bool init( const std::string& text, const std::string& type, bool is_critical, int unit_camp, const std::string& name );
    
    void start( bool is_critical );
    void stop();
};

#endif /* defined(__Boids__JumpText__) */
