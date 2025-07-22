#ifndef _GungeonCore_LONG_RANGE_VILLAIN_H_
#define _GungeonCore_LONG_RANGE_VILLAIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Sprite.h"
#include "Vector.h" // Still useful for calculations like distance to player
#include "Particles.h"
#include "Controller.h"
#include "Timer.h"
#include "TileSet.h"
#include "Animation.h"
#include "GungeonCore.h" // Game constants
#include "Platform.h"            // Game platform
#include "ShortRangeVillain.h"

// ---------------------------------------------------------------------------------
const float LONG_ATTACK_RANGE = 240.0f; // Dist�ncia para o vil�o poder atacar
const float LONG_VISION_RANGE = 240.0f; // Long Range Villains still need to "see" to attack

enum LONG_RANGE_VILLAIN_STATE
{
    STOPPED_RIGHT,          // Stopped facing right
    STOPPED_LEFT            // Stopped facing left
};

class LongRangeVillain : public Object
{
private:
    TileSet* sprite;                    // sprite do player
    Animation* animation;
    Timer attackCooldownTimer;
    float attackCooldownDuration = 1.5f; // Ex: 1.5 segundo de cooldown
    bool playerSpotted;
    bool CanSeePlayer(Vector direction, float range);

public:
    static Player*& player;            // refer�ncia para o player
    uint state = STOPPED_RIGHT; // Visual state for animation
    VILLAIN_STATE_MACHINE currentMachineState = IDLE_OR_PATROLLING; // AI state

    int const MAX_HEALTH = 70;
    int health = MAX_HEALTH;                       // vida do jogador

    float sinkAmount;
    Vector speedX;
    Vector speedY;
    int initialX, initialY;

    float previousY = y;

    static bool toAttack;

    // Constructor and Destructor
    Shadow* villainShadow;
    LongRangeVillain(float x, float y, Scene * scene);
    ~LongRangeVillain();

    // Specific AI state handlers for LongRangeVillain
    void HandlePatrolling();
    void HandlePursuing(); // Still needed to transition to ATTACKING
    void HandleAttacking();

    void Reset();
    void OnCollision(Object* obj); // colis�o com outro objeto
    void MachineState();
    void Update();
    void Draw();

    void TakeDamage(int damage);

    // REMOVED: SetHorizontalVelocity and SetVerticalVelocity
    // As the villain doesn't move, these methods are no longer needed
    // or should be simplified to only affect facing direction if used.
    // If Villain provides default implementations that do nothing, you don't need to override them.
    // If they were *pure virtual* in Villain, you'd need to provide empty overrides here.
};
// ---------------------------------------------------------------------------------

#endif