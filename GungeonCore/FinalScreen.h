#pragma once
#pragma once

#include "Game.h"
#include "Background.h"
#include "Scene.h"
#include "Mouse.h"
#include "Font.h"
#include "MenuItem.h"
#include <vector>

class FinalScreen : public Game
{
private:
    Background* backg = nullptr;
    Scene* scene = nullptr;
    Font* font = nullptr;
    Font* titleFont = nullptr;

    float zoom = 2.0f;
    float viewLeft;
    float viewRight;
    float viewTop;
    float viewBottom;
    float sinkAmount = 0.0f;

public:
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

inline Scene* FinalScreen::GetScene() const { return scene; }
inline float FinalScreen::GetZoom() const { return zoom; }
inline float FinalScreen::GetViewLeft() const { return viewLeft; }
inline float FinalScreen::GetViewTop() const { return viewTop; }
inline float FinalScreen::GetSinkAmount() const { return sinkAmount; }