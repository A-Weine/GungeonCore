#include "RandomMovementVillain.h"
#include "GungeonCore.h"

RandomMovementVillain::RandomMovementVillain(Player* p) : magnitude(1, 3), angle(0, 359), secs(1.0f, 4.0f)
{
    sprite = new TileSet("Resources/bat_sprite_sheet.png", 64, 64, 4, 4);
    animation = new Animation(sprite, 0.1f, true);

    player = p;

    uint Seq1[4] = { 0, 1, 2, 3 };
    BBox(new Circle(20));


    animation->Add(FLYING, Seq1, 4);
    animation->Select(FLYING);

    MoveTo(300, 760);
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
        }
        else {
        if (isPreparing && timerToAttack.Elapsed(preparingToAttack))
        {
            Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * delta);

            speed.Add(target);

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

void RandomMovementVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}
