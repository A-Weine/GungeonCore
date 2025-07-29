/**********************************************************************************
// WallHit (Código Fonte)
//
// Criação:     03 Ago 2019
// Atualização: 05 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Efeito de explosão usando sistema de partículas
//
**********************************************************************************/

#include "GungeonCore.h"
#include "WallHit.h"
#include "Random.h"

// ---------------------------------------------------------------------------------

WallHit::WallHit(float pX, float pY, float angle)
{
   
    // configura emissor de partículas
    Generator explosion;
    explosion.imgFile = "Resources/Spark.png";    // arquivo de imagem
    explosion.angle = angle-180;                     // direção da explosão
    explosion.spread = 50.0f;                   // espalhamento em graus
    explosion.lifetime = 0.3f;                     // tempo de vida em segundos
    explosion.frequency = 0.000f;                   // tempo entre geração de novas partículas
    explosion.percentToDim = 0.6f;                  // desaparece após 60% da vida
    explosion.minSpeed = 20.0f;                    // velocidade mínima das partículas
    explosion.maxSpeed = 80.0f;                   // velocidade máxima das partículas
    explosion.color.r = 0.5f;
    explosion.color.g = 0.5f;
    explosion.color.b = 0.5f;
    explosion.color.a = 1.0f;                     // transparência da partícula

    // cria sistema de partículas
    sparks = new Particles(explosion);

    // gera 25 partículas na posição de contato
    sparks->Generate(pX, pY, 6);
    type = WALLHIT;
}

// ---------------------------------------------------------------------------------

WallHit::~WallHit()
{
    delete sparks;
}

// -------------------------------------------------------------------------------

void WallHit::Update()
{

    // atualiza posição de cada partícula
    sparks->Update(gameTime, true);

    // remove da cena quando todas as partículas estão inativas
    if (sparks->Inactive())
        GungeonCore::level->GetScene()->Delete();
}

// -------------------------------------------------------------------------------