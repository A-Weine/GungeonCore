#include "RandomMovementVillain.h"
#include "GungeonCore.h"

RandomMovementVillain::RandomMovementVillain(float x, float y, Player* p) : magnitude(1, 3), angle(0, 359), secs(1.0f, 4.0f)
{
    sprite = new TileSet("Resources/bullet_bat_sprite_sheet.png", 33, 20, 5, 19);
    animation = new Animation(sprite, 0.1f, true);

    player = p;

    uint SeqFlying[6] = { 0, 1, 2, 3, 4 ,5 };
    uint SeqChargin[4] = { 6, 7 , 8 , 9 };
    uint SeqAttacking[8] = { 10, 11 , 12 , 13, 14, 15, 16, 17 };
    uint SeqDying[4] = { 18 };
    BBox(new Circle(25));


    animation->Add(static_cast<int>(RandomMovementVillainState::FLYING), SeqFlying, 6);
    animation->Add(static_cast<int>(RandomMovementVillainState::CHARGING), SeqChargin, 4);
    animation->Add(static_cast<int>(RandomMovementVillainState::ATTACKING), SeqAttacking, 8);
    animation->Add(static_cast<int>(RandomMovementVillainState::DYING), SeqDying, 1);
    animation->Select(static_cast<int>(RandomMovementVillainState::FLYING));

    spriteExplosion = new TileSet("Resources/bat_explosion_sprite_sheet.png", 40, 38, 2, 4);
    animationExplosion = new Animation(spriteExplosion, 0.1f, false);

    uint SeqExplosion[4] = { 0, 1, 2, 3 };

    animationExplosion->Add(static_cast<int>(RandomMovementVillainState::EXPLODING), SeqExplosion, 4);
    animationExplosion->Select(static_cast<int>(RandomMovementVillainState::EXPLODING));

    MoveTo(x, y);
    ScaleTo(0.3f);
    NewDirection();

    // Se joga no jogador quando chega pr�ximo
    distance = distanceToSee;
    preparingToAttack = 1.2;
    isAttacking = false;

    type = RANDOMMOVEMENTVILLAIN;
}

RandomMovementVillain::~RandomMovementVillain()
{
    delete sprite;
    delete spriteExplosion;
    delete animation;
    delete animationExplosion;
}

void RandomMovementVillain::NewDirection()
{
    speed.ScaleTo(magnitude.Rand());
    speed.RotateTo(angle.Rand());

    delay = secs.Rand();

    timer.Start();
}

void RandomMovementVillain::Update()
{
    switch (currentState)
    {
    case RandomMovementVillainState::FLYING:
    {
        animation->Select(static_cast<int>(RandomMovementVillainState::FLYING));
        animation->NextFrame();
        RotateTo(0.0f);

        if (timer.Elapsed(delay))
            NewDirection();

        Translate(speed.XComponent() * 50 * gameTime, -speed.YComponent() * 50 * gameTime);

        if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distanceToSee)
        {
            currentState = RandomMovementVillainState::CHARGING;
            speed.ScaleTo(0);
            animation->Select(static_cast<int>(RandomMovementVillainState::CHARGING));
            animation->Restart();
            timerToAttack.Start();
        }
        break;
    }

    case RandomMovementVillainState::CHARGING:
    {
        animation->NextFrame();

        if (timerToAttack.Elapsed(preparingToAttack))
        {
            currentState = RandomMovementVillainState::ATTACKING;
            animation->Select(static_cast<int>(RandomMovementVillainState::ATTACKING));

            Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 8.0f);
            speed = target;
        }
        break;
    }

    case RandomMovementVillainState::ATTACKING:
    {
        animation->NextFrame();

        RotateTo(-speed.Angle());

        Translate(speed.XComponent() * 50 * gameTime, -speed.YComponent() * 50 * gameTime);

        if (x < 0 || x > game->Width() || y < 0 || y > game->Height())
        {
            currentState = RandomMovementVillainState::FLYING;
            NewDirection();
        }
        break;
    }

    case RandomMovementVillainState::DYING:
    {
        animation->NextFrame();

        if (animation->Inactive())
        {
            currentState = RandomMovementVillainState::EXPLODING;
            animationExplosion->Select(static_cast<int>(RandomMovementVillainState::EXPLODING));
            animationExplosion->Restart();
        }
        break;
    }

    case RandomMovementVillainState::EXPLODING:
    {
        animationExplosion->NextFrame();

        if (animationExplosion->Inactive())
        {
            GungeonCore::level->GetScene()->Delete(this, MOVING);
        }
        break;
    }
    }

    if (currentState == RandomMovementVillainState::FLYING)
    {
        if (x < 0)
        {
            MoveTo(0, y);
            speed.RotateTo(0);
        }
        if (y < 0)
        {
            MoveTo(x, 0);
            speed.RotateTo(270);
        }
        if (x > game->Width())
        {
            MoveTo(game->Width(), y);
            speed.RotateTo(180);
        }
        if (y > game->Height())
        {
            MoveTo(x, game->Height());
            speed.RotateTo(90);
        }
    }

}

void RandomMovementVillain::OnCollision(Object* obj)
{

   if (obj->Type() == FIRE) {
        
        GungeonCore::level->GetScene()->Delete(obj, MOVING);

        TakeDamage(20);
    }


}

void RandomMovementVillain::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0 && currentState != RandomMovementVillainState::DYING && currentState != RandomMovementVillainState::EXPLODING)
    {
        currentState = RandomMovementVillainState::DYING;
        speed.ScaleTo(0.0f);
        animation->Select(static_cast<int>(RandomMovementVillainState::DYING));
        animation->Loop(false);
        animation->Restart();
    }
}


void RandomMovementVillain::Draw()
{
    if (currentState == RandomMovementVillainState::EXPLODING)
    {
        animationExplosion->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
    }
    else
    {
        animation->Draw(x, y, Layer::MIDDLE, 1.0f, rotation);
    }
}
