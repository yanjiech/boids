//
//  Updatable.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__Updatable__
#define __Boids__Updatable__

namespace boids {

    class Updatable {
    public:
        virtual void updateFrame( float delta ) = 0;
    };
    
}

#endif /* defined(__Boids__Updatable__) */
