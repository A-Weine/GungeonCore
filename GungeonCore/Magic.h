#ifndef _MORTALCELLSREDEMPTION_MAGIC_H_
#define _MORTALCELLSREDEMPTION_MAGIC_H_

#include "Animation.h"
#include "Timer.h"

// Enum para os estados da magia
enum MagicState { SUMMONING, MOVING, EXPLODING };

class Magic : public Object
{
private:
    Animation* anim;          // Objeto de animação para a magia
    TileSet* tileset;         // O spritesheet completo da magia
    MagicState state;         // O estado atual da magia
    Timer stateTimer;         // Um timer para controlar a duração dos estados
    Player* target;           // Um ponteiro para o alvo (o jogador)

    // Constantes de balanceamento
    const float movingDuration = 3.0f;  // Duração da perseguição em segundos
    const float movingSpeed = 250.0f; // Velocidade de perseguição

public:
    // Construtor agora só precisa da posição inicial e do alvo
    Magic(float x, float y, Player* playerTarget);
    ~Magic();

    void Update() override;
    void Draw() override;
    void OnCollision(Object* obj) override;

private:
    // Funções de Handle para cada estado
    void HandleSummoning();
    void HandleMoving();
    void HandleExploding();
};

#endif