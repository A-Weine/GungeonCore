// ---------------------------------------------------------------------------------

#ifndef _GungeonCore_JAR_SHRAP_H_
#define _GungeonCore_JAR_SHRAP_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Particles.h"
#include "Jar.h"

// ---------------------------------------------------------------------------------

class JarShrap : public Object
{
private:
    Particles* shrapGen;            // sistema de partículas

public:
    JarShrap(float Px, float Py);        // construtor
    ~JarShrap();                         // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho
};

// ---------------------------------------------------------------------------------


inline void JarShrap::Draw()
{
    shrapGen->Draw(Layer::MIDDLE, 0.0f);
}

// ---------------------------------------------------------------------------------


#endif