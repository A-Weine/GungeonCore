#include <algorithm>
#include <cmath>

#include "RandomMovementVillain.h"
#include "GungeonCore.h"
#include "Shadow.h"

int RandomMovementVillain::counter = 9;

RandomMovementVillain::RandomMovementVillain(float x, float y, Player* p) : magnitude(1, 3), angle(0, 359), secs(1.0f, 4.0f)
{
    sprite = new TileSet("Resources/bullet_bat_sprite_sheet.png", 33, 20, 5, 19);
    animation = new Animation(sprite, 0.125f, true);

    player = p;

    uint SeqFlying[6] = { 0, 1, 2, 3, 4 ,5 };
    uint SeqChargin[4] = { 6, 7 , 8 , 9 };
    uint SeqAttacking[8] = { 10, 11 , 12 , 13, 14, 15, 16, 17 };
    uint SeqDying[4] = { 18 };
    BBox(new Circle(30));


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
    preparingToAttack = 1.2f;
    isAttacking = false;

    batShadow = new Shadow(this, GungeonCore::level->GetScene(), 18);
    GungeonCore::level->GetScene()->Add(batShadow, STATIC);


    type = RANDOMMOVEMENTVILLAIN;

    number = counter++;
    GungeonCore::audio->Play(number, true);
}

RandomMovementVillain::~RandomMovementVillain()
{
    GungeonCore::audio->Stop(number);

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
    float distance = Point::Distance(Point(x, y), Point(player->X(), player->Y()));
    float maxDistance = (float) sqrt(pow(2560, 2) + pow(864, 2));

    float factor = distance / maxDistance;
    //float volume = 1 - (pow(factor, 2));
    float volume = GungeonCore::maximumBatVolume - factor;

    if (volume < 0) volume = 0;
    if (volume > GungeonCore::maximumBatVolume) volume = GungeonCore::maximumBatVolume;

    GungeonCore::audio->Volume(number, volume);

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

   if (obj->Type() == PLATFORM) {
       NewDirection();

       if (currentState == RandomMovementVillainState::ATTACKING) {
           currentState = RandomMovementVillainState::FLYING;
       }


       Circle * batCircle = static_cast<Circle*>(this->BBox());
       Rect* platformRect = static_cast<Rect*>(obj->BBox());

       if (!batCircle || !platformRect)
           return;

       // --- 2. Encontrar o Ponto Mais Próximo no Quadrado ---

       // Para encontrar o ponto no retângulo mais próximo ao centro do círculo,
       // nós "prendemos" (clamp) as coordenadas do centro do círculo aos limites do retângulo.
       float closestX = (std::max)(platformRect->Left(), (std::min)(batCircle->CenterX(), platformRect->Right()));
       float closestY = (std::max)(platformRect->Top(), (std::min)(batCircle->CenterY(), platformRect->Bottom()));

       // --- 3. Calcular Distância e Detectar Colisão ---

       // Vetor do ponto mais próximo para o centro do círculo
       float diffX = batCircle->CenterX() - closestX;
       float diffY = batCircle->CenterY() - closestY;

       // Distância ao quadrado entre o centro do círculo e o ponto mais próximo
       float distanceSquared = (diffX * diffX) + (diffY * diffY);

       // Se a distância ao quadrado for menor que o raio ao quadrado, há colisão.
       if (distanceSquared < (batCircle->Radius() * batCircle->Radius()))
       {
           // --- 4. Resolver a Colisão ---

           // Calcula a distância real e a sobreposição (overlap)
           float distance = sqrt(distanceSquared);
           float overlap = batCircle->Radius() - distance;

           // Calcula o vetor normalizado de "empurrão" (a direção da colisão)
           float pushX = 0.0f;
           float pushY = 0.0f;

           // Evita divisão por zero se o centro do círculo estiver dentro do retângulo
           if (distance > 0)
           {
               pushX = diffX / distance;
               pushY = diffY / distance;
           }
           else
           {
               // O centro do círculo está exatamente sobre o "ponto mais próximo".
               // Isso pode acontecer se o centro estiver dentro do quadrado.
               // Precisamos de uma lógica de fallback para decidir para onde empurrar.
               // Uma abordagem é empurrar para fora pelo menor caminho.
               // (Esta parte é mais complexa, uma solução simples é empurrar para cima)
               pushX = 0;
               pushY = -1;
               // Para uma solução completa, você calcularia o overlap com as 4 bordas
               // e escolheria o menor, mas a lógica acima já cobre 99% dos casos.
           }

           // Move o jogador para fora da colisão na direção correta
           // 'x' e 'y' são as posições atuais do jogador antes do MoveTo
           float newX = x + (pushX * overlap);
           float newY = y + (pushY * overlap);


           MoveTo(newX, newY);

           float speedX = speed.XComponent();
           float speedY = speed.YComponent();

           // 2. Calcula o "produto escalar" (dot product) entre a velocidade e o vetor de "empurrão" (normal da colisão).
           // Isso nos diz o quanto da velocidade está indo "contra" a parede.
           float dotProduct = (speedX * pushX) + (speedY * pushY);

           // 3. Subtrai a parte da velocidade que está "entrando" na parede.
           // O resultado é um novo vetor de velocidade que é paralelo à superfície de colisão.
           float newSpeedX = speedX - dotProduct * pushX;
           float newSpeedY = speedY - dotProduct * pushY;

           // 4. Define a nova velocidade de "deslize" para o jogador
           speed.SetFromCartesian(newSpeedX, newSpeedY);

           // Opcional: Ajustar a velocidade
           //speed.ScaleTo(0.0f);
       }

   }
}

void RandomMovementVillain::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0 && currentState != RandomMovementVillainState::DYING && currentState != RandomMovementVillainState::EXPLODING)
    {
        GungeonCore::level->GetScene()->Delete(batShadow, STATIC);
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
