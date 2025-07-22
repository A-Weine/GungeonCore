#ifndef _GungeonCore_SHORT_RANGE_VILLAIN_H_
#define _GungeonCore_SHORT_RANGE_VILLAIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representa��o de vetores
#include "Particles.h"                  // sistema de part�culas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"
#include "GungeonCore.h"      // constantes do jogo
#include "Platform.h"                   // plataforma do jogo
#include "Shadow.h"
#include "Hitbox.h"

// ---------------------------------------------------------------------------------
const float VISION_RANGE = 120.0f;  // Dist�ncia para o vil�o "ver" o jogador
const float ATTACK_RANGE = 30.0f;   // Dist�ncia para o vil�o poder atacar
const float PATROL_SPEED = 30.0f;
const float CHASE_SPEED = 60.0f;

enum VILLAINSTATE
{
    ATTACKING_RIGHT,
    ATTACKING_LEFT,
    VILLAIN_WALKING_RIGHT,
    VILLAIN_WALKING_LEFT,
};

enum VILLAIN_STATE_MACHINE
{
    IDLE_OR_PATROLLING,  // Renomeado de SEARCHING para ser mais gen�rico
    PURSUING,            // Renomeado de SEEK para ser mais descritivo
    ATTACKING,           // Renomeado de DESTROY para ser mais descritivo
    // FLEEING,          // Novo estado: Vil�o fugindo
    // HURT_OR_STUNNED,  // Novo estado: Vil�o atordoado/machucado
    // CASTING_ABILITY,  // Novo estado: Vil�o carregando ou usando uma habilidade
    // FLANKING,         // Poderia ser um subtipo de PURSUING ou um estado separado
    // SEEKING_COVER,    // Poderia ser um subtipo de FLEEING ou um estado separado
};


class ShortRangeVillain : public Object
{
private:
    TileSet* sprite;
    Animation* animationWalking;
    Animation* animationAttacking;
    float jumpStrength;              // for�a do pulo
    float previousY;                // posi��o Y anterior
    float sinkAmount;
    //Controller * gamepad;               // leitura do controle
    //bool gamepadOn;                     // controle est� ligado

    float patrolLeftLimit{};     // Limite esquerdo da patrulha
    float patrolRightLimit{};    // Limite direito da patrulha
    bool walkingRightInPatrol;   // Dire��o de patrulha

    Timer attackCooldownTimer;
    float attackCooldownDuration = 1.0f; // Ex: 0.5 segundo de cooldown

    Timer stunnedTimer;
    float stunnedDuration = 1.5f; // Ex: 1.5 segundos de atordoamento
    bool CanSeePlayer(Vector direction, float range);
    bool playerSpotted;

public:
    Shadow* villainShadow;
    uint state = RUNNING_RIGHT;
    VILLAIN_STATE_MACHINE currentMachineState = IDLE_OR_PATROLLING;
    int initialX, initialY;

    int const MAX_HEALTH = 70;
    int health = MAX_HEALTH;                       // vida do jogador

    Vector speedX;
    Vector speedY;

    ShortRangeVillain(float x, float y, Scene * scene);                           // construtor
    ~ShortRangeVillain();                          // destrutor

    void SetHorizontalVelocity(float velocityX); // Define a velocidade horizontal
    void SetVerticalVelocity(float velocityY);   // Define a velocidade vertical

    void MoveX(Vector&& v);             // movimenta jogador horizontalmente
    void MoveY(Vector&& v);            // movimenta jogador verticalmente

    void HandlePatrolling();
    void HandlePursuing();
    void HandleAttacking();
    void TakeDamage(int damage);
    
    /*void HandleFleeing();
    void HandleHurtOrStunned();
    void HandleCastingAbility();*/


	void Reset();				// reinicializa o vil�o
    //void PivotCollision(Object * obj);  // revolve colis�o com piv�

	void OnCollision(Object* obj); // colis�o com outro objeto
    void MachineState();
    void Update();                      // atualiza��o
	void Draw();                        // desenho do vil�o
};
// ---------------------------------------------------------------------------------

#endif
