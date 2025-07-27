/**********************************************************************************
// Player (Arquivo de Cabe�alho)
// 
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Define a classe jogador
//
**********************************************************************************/

#ifndef _GungeonCore_PLAYER_H_
#define _GungeonCore_PLAYER_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representa��o de vetores
#include "Particles.h"                  // sistema de part�culas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"

// ---------------------------------------------------------------------------------
enum class PlayerState
{
    IDLE_DOWN,
    IDLE_DOWN_RIGHT,
    IDLE_DOWN_LEFT,
    IDLE_UP,
    IDLE_UP_RIGHT,
    IDLE_UP_LEFT,
    RUNNING_DOWN,
    RUNNING_DOWN_RIGHT,
    RUNNING_DOWN_LEFT,
    RUNNING_UP,
    RUNNING_UP_RIGHT,
    RUNNING_UP_LEFT,
};

enum class VerticalFacing { UP, DOWN };

class Player : public Object
{
private:
    TileSet * sprite;                    // sprite do player
    Animation* animation;

    Timer walkingTimer;
	float const walkingTime = 0.3f;    // tempo de anima��o de caminhada

    Timer invicibilityTimer;
	float const invicibilityTime = 0.3f; // tempo de invencibilidade ap�s receber dano
	
    float jumpStrength;              // for�a do pulo
	float previousY;                // posi��o Y anterior
    
    Controller * gamepad;               // leitura do controle
    bool gamepadOn;                     // controle est� ligado



    //float const attackCooldownDurationShotgun = 0.5f;
    //Timer timerShotgun;                          // controla tempo dos disparos
    //int const fullBulletShotgun = 3;
    //float const timeToReloadShotgun = 1.65f;
    //Timer timerToReloadShotgun;


    //float const attackCooldownDurationMagnum = 0.3f;
    //Timer timerMagnum;                          // controla tempo dos disparos
    //int const fullBulletMagnum = 6;
    //float const timeToReloadMagnum = 1.5f;
    //Timer timerToReloadMagnum;
    //float firingAngle;                    // dire��o dos disparos
    //float armAngle = 0.0f;

    //llong start;                          // marca��o de inicio do disparo
    //bool axisCtrl;                        // habilita leitura de disparos
    //bool keysCtrl;                        // habilita leitura de disparos pelas setas
    //bool keysPressed;                     // qualquer seta de disparo pressionada

public:
    int itemEquiped;
    Object* inventory[10];
    
    Object* vazio;
    //boolean reloadShotgun;
    //boolean reloadMagnum;

    // ITENS
    //int quantBulletsMagnum;
    //int quantBulletsShotgun;
   /* boolean hasMagnum;
    boolean hasShotgun;
    boolean hasKey1;
    boolean hasKey2;*/

    PlayerState state = PlayerState::IDLE_DOWN;
    VerticalFacing lastVFacing = VerticalFacing::DOWN;

	Vector speed;				// velocidade do jogador
    
    float sinkAmount;                // quantidade de pixeis que o player afunda na plataforma
    int const MAX_HEALTH = 100;
	int health = MAX_HEALTH;                       // vida do jogador
    int initialX, initialY;

    bool takeDamage = true;

    Player();                           // construtor
    ~Player();                          // destrutor

    void Reset();
    void OnCollision(Object* obj);     // resolu��o da colis�o
    //void PivotCollision(Object * obj);  // revolve colis�o com piv�

    bool AxisTimed(int axisX, int axisY, float time);
    bool KeysTimed(bool pressed, float time);

    void Update();                      // atualiza��o
    void Draw();                        // desenho
	void TakeDamage(int damage);         // recebe dano
    void TakeDamage(bool take);         // recebe dano?
}; 
// ---------------------------------------------------------------------------------

#endif