#ifndef _EGG_PROJECTILE_H_
#define _EGG_PROJECTILE_H_

#include "Projectile.h"
#include "Player.h"
#include "Vector.h"

// Estados do projétil
enum class EggState { ARCING, EXPLODING };

class EggProjectile : public Projectile
{
private:
    EggState state;
    Timer lifetimeTimer;
    Player* target;
    float angle;

public:
    EggProjectile(Object* shooter, Player* playerTarget, Vector direction);
    ~EggProjectile();

    void Explode();
    void Update() override;
    void OnCollision(Object* obj) override;
};

#endif