#ifndef _GungeonCore_HOME_H_
#define _GungeonCore_HOME_H_

#include "Game.h"
#include "Scene.h"
#include "Background.h"
#include "Mouse.h"
#include "MenuItem.h"
#include <vector>
#include "Sprite.h"
#include "Controller.h"

class Home : public Game
{
private:
    Background* backg = nullptr;
    Scene* scene = nullptr;       // O menu também pode ter uma cena para gerenciar seus objetos
    Mouse* mouse = nullptr;
    Sprite* gameName;
    std::vector<MenuItem*> items; // Um vetor para guardar os itens do menu
    int selectedOption = 0;

    // ADICIONE ESTAS LINHAS: Posição atual da câmera
    float cameraCurrentX;
    float cameraCurrentY;

public:
    float zoom = 2.0f;
    float viewLeft;
    float viewRight;
    float viewTop;
    float viewBottom;
    float sinkAmount;
    Controller* gamepad;
    bool gamepadOn;
    // A classe Home não precisa de NENHUM membro estático.

    void Init() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

    // A Home não gerencia outros níveis, então não precisa de GetViewport, GetScene, etc.
    // Vamos adicionar as funções virtuais apenas para cumprir o "contrato", mas elas podem ser simples.
    Scene* GetScene() const override;
    float GetZoom() const override;
    float GetViewLeft() const override;
    float GetViewTop() const override;
    float GetSinkAmount() const override;
};

inline Scene* Home::GetScene() const { return scene; }
inline float Home::GetZoom() const { return zoom; }
inline float Home::GetViewLeft() const { return viewLeft; }
inline float Home::GetViewTop() const { return viewTop; }
inline float Home::GetSinkAmount() const { return sinkAmount; }

#endif