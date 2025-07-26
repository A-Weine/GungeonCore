/**********************************************************************************
// Mouse (Arquivo de Cabeçalho)
//
// Criação:     14 Mai 2012
// Atualização: 22 Set 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Gerencia mouse na tela
//
**********************************************************************************/

#ifndef _GungeonCore_MOUSE_H_
#define _GungeonCore_MOUSE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"        
#include "Types.h"
#include "Game.h"

// ---------------------------------------------------------------------------------

class Mouse : public Object
{
public:
    __cdecl Mouse();            // Constructor
    __cdecl ~Mouse();           // Destructor

    bool __cdecl Clicked();     // Mouse click
    void __cdecl Update();      // Object update
    void __cdecl Draw();        // Object draw
};

// ---------------------------------------------------------------------------------

#endif