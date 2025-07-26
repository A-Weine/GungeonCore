#ifndef _GungeonCore_EMPTY_H_
#define _GungeonCore_EMPTY_H_

// ---------------------------------------------------------------------------------

#include "Object.h"        
#include "Types.h"

// ---------------------------------------------------------------------------------

class Empty : public Object
{
public:
    Empty();            // Constructor
    ~Empty();           // Destructor

    void Update();      // Object update
    void Draw();        // Object draw
};

// ---------------------------------------------------------------------------------

#endif