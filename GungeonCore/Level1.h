/**********************************************************************************
// Hud (Arquivo de Cabeçalho)
//
// Criação:     02 Ago 2019
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Heads Up Display
//
**********************************************************************************/

#ifndef _GungeonCore_LEVEL1_H_
#define _GungeonCore_LEVEL1_H_

// --------------------------------------------------------------------------------

#include "Game.h"
#include "Scene.h"
#include "Player.h"
#include "Background.h"
#include "Sprite.h"
#include "DroppedItem.h"
#include "Tilemap.h"

// --------------------------------------------------------------------------------

class Level1 : public Game
{
private:
    Background* backg = nullptr;
    Tilemap* tilemap;

    // ADICIONE ESTAS LINHAS: Posição atual da câmera
    float cameraCurrentX;
    float cameraCurrentY;

public:
    static Scene* scene;            // cena do nível
	float viewLeft, viewRight, viewTop, viewBottom;
    float sinkAmount = 6.0f;

    static bool zoomingIn;
    static float zoom;

    void Init();                    // inicialização do nível
    void Update();                  // atualiza lógica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finalização do nível

    Scene* GetScene() const override;
    float GetZoom() const override;
    float GetViewLeft() const override;
    float GetViewTop() const override;
    float GetSinkAmount() const override;
};

inline Scene* Level1::GetScene() const { return scene; }
inline float Level1::GetZoom() const { return zoom; }
inline float Level1::GetViewLeft() const { return viewLeft; }
inline float Level1::GetViewTop() const { return viewTop; }
inline float Level1::GetSinkAmount() const { return sinkAmount; }

// ------------------------------------------------------------------------------

#endif

