#include "RandomMovementVillain.h"
#include "GungeonCore.h"

RandomMovementVillain::RandomMovementVillain(): magnitude(1, 3), angle(0, 359), secs(1.0f, 4.0f)
{
    sprite = new TileSet("Resources/bat_sprite_sheet.png",64,64,4,4);
    animation = new Animation(sprite, 0.1f, true);

    uint Seq1[4] = { 0, 1, 2, 3 };


    animation->Add(FLYING, Seq1, 4);
    animation->Select(FLYING);

    MoveTo(150, 760);
    ScaleTo(0.3f);
    NewDirection();
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

    if (timer.Elapsed(delay))
        NewDirection();

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
    if (x > window->Width())
    {
        MoveTo(window->Width(), y);
        speed.RotateTo(180);
    }
    if (y > window->Height())
    {
        MoveTo(x, window->Height());
        speed.RotateTo(90);
    }

}

void RandomMovementVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}
