#ifndef _GungeonCore_Hand_H_
#define _GungeonCore_Hand_H_

// ---------------------------------------------------------------------------------

#include "Object.h"        
#include "Types.h"
#include "Player.h"
#include "Sprite.h"
#include "TileSet.h"
#include "Animation.h"
#include "Gun.h"
// ---------------------------------------------------------------------------------

class Hand : public Object
{

public:
    TileSet* sprite;
    Animation* animations;
    Player* player;
    Gun* gun;

    float handAngle;
    Hand(Player* players, TileSet* tileSet, Animation* animation, Gun* guns);            // Constructor
    ~Hand();           // Destructor

    void Update();      // Object update
    void Draw();        // Object draw
};

// ---------------------------------------------------------------------------------

#endif