#ifndef _PROGJOGOS_PROJECTILE_H_
#define _PROGJOGOS_PROJECTILE_H_

#include "Object.h"
#include "Animation.h"
#include "TileSet.h"
#include "Timer.h"

// Classe base abstrata para todos os projéteis do jogo.
class Projectile : public Object
{
protected:
    Animation* anim;
    TileSet* tileset;
    Sprite* sprite;
    Vector speed;
    Object* shooter;

public:
    Projectile(Object* owner);

    static Image* bullet;

    virtual ~Projectile();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void OnCollision(Object* obj) override;
};

#endif