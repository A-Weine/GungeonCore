

#include "JarShrap.h"
#include "Jar.h"
#include "Random.h"
#include "GungeonCore.h"
#include "Random.h"

JarShrap::JarShrap(Jar* jar) {

    float pX = jar->X();
    float pY = jar->Y();

    
    // configura gerador de partículas
    Generator shrapMaker;
    shrapMaker.imgFile = "Resources/tijelaPedaco.png";   // arquivo de imagem
    shrapMaker.angle = 0;                      // direção das estrelas
    shrapMaker.spread = 360.0f;                  // espalhamento em graus
    shrapMaker.lifetime = 5.0f;                  // tempo de vida (em segundos)
    shrapMaker.frequency = 0.000f;               // tempo entre geração de novas partículas
    shrapMaker.percentToDim = 0.6f;              // desaparece após 60% da vida
    shrapMaker.minSpeed = 30.0f;                 // velocidade mínima das partículas
    shrapMaker.maxSpeed = 60.0f;                // velocidade máxima das partículas
    shrapMaker.color.r = 0.5;
    shrapMaker.color.g = 0.5;           // cor da partícula entre 0 e 1
    shrapMaker.color.b = 0.5;           // cor da partícula entre 0 e 1
    shrapMaker.color.a = 1.0f;                   // transparência da partícula

    // cria sistema de partículas
    shrapGen = new Particles(shrapMaker);

    Random<int> * quant = new Random(3,6);

    shrapGen->Generate(pX, pY, quant->Rand());
    type = JARSHRAP;
}

JarShrap::~JarShrap()
{
    delete shrapGen;
}


void JarShrap::Update()
{
    // atualiza posição de cada partícula
    shrapGen->Update(gameTime, true);

    // remove da cena quando todas as partículas estão inativas
    if (shrapGen->Inactive())
        GungeonCore::level->GetScene()->Delete();
}