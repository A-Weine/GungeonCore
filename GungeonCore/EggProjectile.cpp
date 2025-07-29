#include "EggProjectile.h"
#include "GungeonCore.h"
#include "Engine.h"
#include "Fire.h"
#include "RunAwayVillain.h"

EggProjectile::EggProjectile(Object * shooter, Player* playerTarget, Vector direction) : Projectile(shooter)
{
    type = ENEMYFIRE;
    layer = CollisionLayer::ENEMY;
    target = playerTarget;
    state = EggState::ARCING;

    tileset = new TileSet("Resources/egg_bullet.png", 12, 15, 1, 1);
    anim = new Animation(tileset, 0.125f, false);

    uint arcingSeq[1] = { 0 };
    anim->Add(0, arcingSeq, 1);
    anim->Select(0);

    speed = direction;
    angle = speed.Angle();

    RotateTo(angle);

    RunAwayVillain* shooterR = static_cast<RunAwayVillain*>(shooter);

    float xEgg = shooterR->currentState == RunAwayVillainState::SHOOTINGRIGHT ? (shooterR->X() + 5.0f) : (shooterR->X() - 5.0f);

    BBox(new Circle(8.0f));
    MoveTo(xEgg, shooterR->Y() - 5);

    lifetimeTimer.Start();
}

EggProjectile::~EggProjectile()
{
}

void EggProjectile::Update()
{
    if (state == EggState::ARCING)
    {
        speed.Add(Vector(270.0f, 30.0f * gameTime));

        if (speed.Magnitude() > 4.0f) {
            speed.ScaleTo(4.0f);
        }

        RotateTo(-speed.Angle()-90.0f);

        Translate(speed.XComponent() * 20 * gameTime, -speed.YComponent() * 20 * gameTime);

        if (lifetimeTimer.Elapsed(0.55f))
        {
            Explode();
        }
    }
}

void EggProjectile::Explode() 
{
    speed.ScaleTo(0.0f);

    uint numProjectiles = 5;
    float angleToPlayer = Line::Angle(Point(x, y), Point(GungeonCore::player->X(), GungeonCore::player->Y()));

    for (int i = 0; i < numProjectiles; i++) 
    {
        float spread = -20.0f + (12.0f * i);
        float finalAngle = angleToPlayer + spread;

        GungeonCore::level->GetScene()->Add(new Fire(this, finalAngle, ENEMYFIRE), MOVING);
    }

    GungeonCore::level->GetScene()->Delete(this, MOVING);
}

void EggProjectile::OnCollision(Object* obj)
{
    if (state == EggState::ARCING && (obj->Type() == PLATFORM))
    {
        Explode();
    }

    if (obj->Type() == PLAYER) {
        Player* player = static_cast<Player*>(obj);
        player->TakeDamage(15);

        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}