#ifndef _GungeonCore_BOSS_VILLAIN_H_
#define _GungeonCore_BOSS_VILLAIN_H_

#include "Object.h"
#include "Sprite.h"
#include "Particles.h"
#include "Controller.h"
#include "Timer.h"
#include "TileSet.h"
#include "Animation.h"
#include "GungeonCore.h"
#include "Platform.h"
#include "ShortRangeVillain.h"
//#include "Magic.h"

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
    TileSet* spriteIdle;
    TileSet* spriteAction;
    Animation* animationIdle;
    Animation* animationAction;
    Timer attackCooldownTimer;
    float attackCooldownDuration = 0.7f;
    Image* bossMagic;

public:
    static Player*& player;
    uint state = IDLE;

    boolean dead;

    int health = 500;

    float sinkAmount;
    int initialX, initialY;
    float previousY = 0.0f;

    Vector speedY; // <-- Add this for gravity

    Shadow* villainShadow;

    // Add these member variables to the Boss class:

    // --- Boss State Management ---
    enum BossAttackState { BOSS_LONGRANGE, BOSS_SHORTRANGE };
    BossAttackState attackState;
    Timer stateSwitchTimer;
    const float stateSwitchInterval = 10.0f; // Switch every 10s

    // --- Dash Variables ---
    bool isDashing;
    bool dashHitPlayer;
    float dashCooldown;
    const float dashCooldownDuration = 3.0f;
    const float dashSpeed = 500.0f;
    float dashDirX, dashDirY;

    Boss();
    ~Boss();

    void Reset();
    void OnCollision(Object* obj);
    void Update();
    void Draw();

    void TakeDamage(int damage);
};

#endif