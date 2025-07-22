/**********************************************************************************
// Player (Arquivo de Cabeçalho)
// 
// Criação:     10 Out 2012
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Define a classe jogador
//
**********************************************************************************/

#ifndef _GungeonCore_PLAYER_H_
#define _GungeonCore_PLAYER_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"

// ---------------------------------------------------------------------------------
enum PLAYERSTATE
{
    IDLE_DOWN,
    IDLE_UP,
    RUNNING_DOWN,
    RUNNING_RIGHT,
    RUNNING_LEFT
};

class Player : public Object
{
private:
    TileSet * sprite;                    // sprite do player
    Animation* animation;

    Timer walkingTimer;
	float const walkingTime = 0.3f;    // tempo de animação de caminhada

    Timer invicibilityTimer;
	float const invicibilityTime = 0.3f; // tempo de invencibilidade após receber dano
	
    float jumpStrength;              // força do pulo
	float previousY;                // posição Y anterior
    
    //Controller * gamepad;               // leitura do controle
    //bool gamepadOn;                     // controle está ligado



    float const attackCooldownDurationShotgun = 0.5f;
    Timer timerShotgun;                          // controla tempo dos disparos
    int const fullBulletShotgun = 3;
    float const timeToReloadShotgun = 1.65f;
    Timer timerToReloadShotgun;


    float const attackCooldownDurationMagnum = 0.3f;
    Timer timerMagnum;                          // controla tempo dos disparos
    int const fullBulletMagnum = 6;
    float const timeToReloadMagnum = 1.5f;
    Timer timerToReloadMagnum;

    float armAngle = 0.0f;

    llong start;                          // marcação de inicio do disparo
    bool axisCtrl;                        // habilita leitura de disparos
    bool keysCtrl;                        // habilita leitura de disparos pelas setas
    bool keysPressed;                     // qualquer seta de disparo pressionada
    float firingAngle;                    // direção dos disparos

public:
    int weaponEquiped;
    boolean reloadShotgun;
    boolean reloadMagnum;

    // ITENS
    int quantBulletsMagnum;
    int quantBulletsShotgun;
    boolean hasMagnum;
    boolean hasShotgun;
    boolean hasKey1;
    boolean hasKey2;


    static Image * fire;             // Imagem da bala que vai ser atirada pelo player
    uint state = IDLE_DOWN;
    
	Vector speed;				// velocidade do jogador
    
    float sinkAmount;                // quantidade de pixeis que o player afunda na plataforma
    int const MAX_HEALTH = 100;
	int health = MAX_HEALTH;                       // vida do jogador
    int initialX, initialY;

    bool takeDamage = true;

    Player();                           // construtor
    ~Player();                          // destrutor

    void Reset();
    void OnCollision(Object* obj);     // resolução da colisão
    //void PivotCollision(Object * obj);  // revolve colisão com pivô

    bool AxisTimed(int axisX, int axisY, float time);
    bool KeysTimed(bool pressed, float time);

    void Update();                      // atualização
    void Draw();                        // desenho
	void TakeDamage(int damage);         // recebe dano
    void TakeDamage(bool take);         // recebe dano?
}; 
// ---------------------------------------------------------------------------------

#endif