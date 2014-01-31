//
//  Maybe.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef ultraray_Maybe_h
#define ultraray_Maybe_h

template <class T>
class Maybe {
    T value;
    bool empty;
public:
    Maybe(): empty(true) {}
    explicit Maybe(T value): value(value), empty(false) {}
    
    T fromJust() const {
        if (isJust()) {
            return value;
        } else {
            throw "can't return Nothing";
        }
    }
    
    bool isJust         () const noexcept {return !empty;}
    bool isNothing      () const noexcept {return empty;}
};

#endif