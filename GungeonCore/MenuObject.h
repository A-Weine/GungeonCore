#ifndef _GungeonCore_MenuObject_H_
#define _GungeonCore_MenuObject_H_

// ---------------------------------------------------------------------------------

#include "Object.h"        
#include "Types.h"
#include "Sprite.h"

// ---------------------------------------------------------------------------------

class MenuObject : public Object
{
public:
    Sprite* sprite;
    float sizer;
    MenuObject(string fileName, float px, float py, float size);            // Constructor
    ~MenuObject();           // Destructor

    void Update();      // Object update
    void Draw();        // Object draw
};

// ---------------------------------------------------------------------------------

#endif