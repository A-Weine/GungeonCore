#include "RandomMovementVillain.h"
#include "GungeonCore.h"

RandomMovementVillain::RandomMovementVillain(float x, float y, Player* p) : magnitude(1, 3), angle(0, 359), secs(1.0f, 4.0f)
{
    sprite = new TileSet("Resources/bullet_bat_sprite_sheet.png", 33, 20, 5, 6);
    animation = new Animation(sprite, 0.1f, true);

    player = p;

    uint SeqFlying[6] = { 0, 1, 2, 3, 4 ,5 };
    uint SeqChargin[4] = { 7 , 8 , 9, 10 };
    uint SeqAttacking[8] = { 11 , 12 , 13, 14, 15, 16, 17, 18 };
    BBox(new Circle(25));


    animation->Add(FLYING, SeqFlying, 6);
    animation->Add(CHARGING, SeqChargin, 4);
    animation->Add(ATTACKING, SeqAttacking, 8);
    animation->Select(FLYING);

    MoveTo(x, y);
    ScaleTo(0.3f);
    NewDirection();

    // Se joga no jogador quando chega próximo
    distance = distanceToSee;
    preparingToAttack = 1.2;
    isAttacking = false;

    type = RANDOMMOVEMENTVILLAIN;
}

RandomMovementVillain::~RandomMovementVillain()
{
    delete sprite;
    delete animation;
}

void RandomMovementVillain::NewDirection()
{
    // nova direção aleatória
    speed.ScaleTo(magnitude.Rand());
    speed.RotateTo(angle.Rand());

    // tempo aleatório
    delay = secs.Rand();

    // inicia medição do tempo
    timer.Start();
}

void RandomMovementVillain::Update()
{
    animation->NextFrame();

    // deslocamento padrão
    float delta = 50 * gameTime;

    if (isAttacking)
    {

        bool isPreparing = (speed.Magnitude() == 0.0f);

        if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) > distance)
        {
            isAttacking = false;
            distance = distanceToSee;
            NewDirection();
            animation->Select(FLYING);
        }
        else {
        if (isPreparing && timerToAttack.Elapsed(preparingToAttack))
        {
            Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * delta);

            speed.Add(target);
            animation->Select(ATTACKING);
        }
        }
    }
    else
    {
        if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
        {
            isAttacking = true;
            timerToAttack.Start();
            speed.ScaleTo(0);
            animation->Select(CHARGING);
            distance = distanceToSee * visionMultiplierWhenAttacking;
        }
        else
        {
            if (timer.Elapsed(delay))
                NewDirection();
        }
    }

    // limita a magnitude da velocidade para impedir 
    // seu crescimento indefinido na soma vetorial
    if (speed.Magnitude() > 8)
        speed.ScaleTo(8.0f);

    Translate(speed.XComponent() * delta, -speed.YComponent() * delta);

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

void RandomMovementVillain::OnCollision(Object* obj)
{

   /* if (obj->Type() == FIRE) {
        health--;

        GungeonCore::level->GetScene()->Delete(obj, MOVING);

        if (health == 0) {
            GungeonCore::level->GetScene()->Delete(this, MOVING);
        }
    }*/


}

void RandomMovementVillain::TakeDamage(int damage) {
    health -= damage;

    if (health <= 0)
    {
        // Optionally play a sound or animation here
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}


void RandomMovementVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}
