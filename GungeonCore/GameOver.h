#ifndef _GungeonCore_GAME_OVER_H_
#define _GungeonCore_GAME_OVER_H_

#include "Game.h"
#include "Scene.h"
#include "Background.h"
#include "Mouse.h"
#include "MenuItem.h"
#include "Sprite.h"
#include "Controller.h"
#include <vector>

class GameOver : public Game
{
private:
    Background* backg = nullptr;
    Scene* scene = nullptr;       // O menu também pode ter uma cena para gerenciar seus objetos
    Mouse* mouse = nullptr;
    std::vector<MenuItem*> items; // Um vetor para guardar os itens do menu
    int selectedOption = 0;

    // ADICIONE ESTAS LINHAS: Posição atual da câmera
    float cameraCurrentX;
    float cameraCurrentY;
    Sprite * gameOver;

public:
    float zoom = 2.0f;
    float viewLeft;
    float viewRight;
    float viewTop;
    float viewBottom;
    float sinkAmount;
    Controller* gamepad;
    bool gamepadOn;

    void Init() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

    Scene* GetScene() const override;
    float GetZoom() const override;
    float GetViewLeft() const override;
    float GetViewTop() const override;
    float GetSinkAmount() const override;
};

inline Scene* GameOver::GetScene() const { return scene; }
inline float GameOver::GetZoom() const { return zoom; }
inline float GameOver::GetViewLeft() const { return viewLeft; }
inline float GameOver::GetViewTop() const { return viewTop; }
inline float GameOver::GetSinkAmount() const { return sinkAmount; }

#endif