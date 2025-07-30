#ifndef _GungeonCore_BOSS_VILLAIN_H_
#define _GungeonCore_BOSS_VILLAIN_H_

#include "Object.h"
#include "Sprite.h"
#include "Controller.h"
#include "Timer.h"
#include "TileSet.h"
#include "Animation.h"
#include "GungeonCore.h"
#include "Platform.h"

enum BossState
{
    IDLE,
    SUMMONING,
    THROWING,
    DASHING,
};

class Boss : public Object
{
private:
    TileSet* sprite;
    Animation* animation;

public:
    static Player*& player;
    uint state = IDLE;

    boolean dead;

    int health = 500;

    int initialX, initialY;
    float previousY = 0.0f;

    Shadow* villainShadow;

    // Add these member variables to the Boss class:

    // --- Boss State Management ---
    enum BossAttackState { CHASE };
    BossAttackState attackState;

    // --- Dash Variables ---

    Boss();
    ~Boss();

    void Reset();
    void OnCollision(Object* obj);
    void Update();
    void Draw();

    void TakeDamage(int damage);
};

#endif