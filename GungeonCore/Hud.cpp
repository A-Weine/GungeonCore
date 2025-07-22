/**********************************************************************************
// Hud (C�digo Fonte)
//
// Cria��o:     02 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Heads Up Display
//
**********************************************************************************/

#include "Hud.h"
#include "GungeonCore.h"
#include "Engine.h"

// ------------------------------------------------------------------------------

// inicializa contadores
uint Hud::missiles = 0;
uint Hud::oranges = 0;
uint Hud::magentas = 0;
uint Hud::blues = 0;
uint Hud::greens = 0;
uint Hud::particles = 0;

// ------------------------------------------------------------------------------

Hud::Hud()
{
    // cria fonte para exibi��o de texto
    font = new Font("Resources/chineserocks30.png");

    // carrega sprites
    //infoBox = new Sprite("Resources/InfoBox.png");

    // inicializa contador de frames e tempo transcorrido
    frameCount = 0;
    totalTime = 0.0f;
    fps = 0;
}

// ------------------------------------------------------------------------------

Hud::~Hud()
{
    delete font;
    delete bold;
    //delete infoBox;
}

// -------------------------------------------------------------------------------

void Hud::Update()
{
    // tempo acumulado dos frames
    totalTime += gameTime;

    // incrementa contador de frames
    frameCount++;

    // a cada 1000ms (1 segundo) atualiza indicador de FPS
    if (totalTime >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        totalTime -= 1.0f;
    }
}

// -------------------------------------------------------------------------------

void Hud::Draw(float viewLeft, float viewTop, float cameraZoom)
{
    // =======================================================================
    // --- PAR�METROS DE AJUSTE DO HUD ---
    // Se o espa�amento ainda estiver ruim, aumente o valor de lineSpacing!
    // =======================================================================
    float baseBoxScale = 0.17f;
    float baseFontScale = 0.22f;
    float mainMargin = 25.0f;
    float textMargin = 15.0f;
    float lineSpacing = 30.0f; // <--- AUMENTE ESTE VALOR SE AS LINHAS FICAREM MUITO JUNTAS

    // =======================================================================
    // --- O C�DIGO ABAIXO USA OS PAR�METROS ACIMA ---
    // =======================================================================

    // --- C�LCULOS DE ESCALA ---
    //float infoBoxScale = baseBoxScale / cameraZoom;
    float fontScale = baseFontScale / cameraZoom;

    //// --- L�GICA DE POSICIONAMENTO ---
    //float boxWidth = infoBox->Width();
    //float boxHeight = infoBox->Height();
    //float boxDrawnWidth = boxWidth * infoBoxScale;
    //float boxDrawnHeight = boxHeight * infoBoxScale;

    float margin_z = mainMargin / cameraZoom;

    //float anchorX = viewLeft + margin_z + (boxDrawnWidth / 2.0f);
    //float anchorY = viewTop + margin_z + (boxDrawnHeight / 2.0f);

    //// --- DESENHO DOS ELEMENTOS ---
    //infoBox->Draw(anchorX, anchorY, Layer::FRONT, infoBoxScale, 0.0f, {255, 255, 255, 0.1});

    Color textColor{ 1.0f, 1.0f, 1.0f, 1.0f };

    float textMargin_z = textMargin / cameraZoom;
    float lineSpacing_z = lineSpacing / cameraZoom;

    float textStartX = viewLeft + margin_z + textMargin_z;
    float textStartY = viewTop + margin_z + textMargin_z;

    // --- NOVA L�GICA DE DESENHO DE TEXTO ---
    float currentY = textStartY; // Vari�vel para a posi��o Y da linha atual

    // Linha 2: Vida
    text.str("");
    text << "Vida: " << GungeonCore::player->health;
    font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);

    // Avan�a para a pr�xima linha
    currentY += lineSpacing_z;

    // Linha 3: Balas ou Recarregando
    if (GungeonCore::player->reloadMagnum || GungeonCore::player->reloadShotgun) {
        text.str("");
        text << "...";
        font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);
    }
    else {
        // ... (l�gica para pegar qtdBullets) ...
        int qtdBullets;
        if (GungeonCore::player->weaponEquiped == 2) {
            if (GungeonCore::player->hasShotgun) {
                qtdBullets = GungeonCore::player->quantBulletsShotgun;
            }
        }
        else if (GungeonCore::player->weaponEquiped == 1) {
            if (GungeonCore::player->hasMagnum) {
            qtdBullets = GungeonCore::player->quantBulletsMagnum;
        }}
        else {
            qtdBullets = 0;
        }
        text.str("");
        text << "Balas: " << qtdBullets;
        font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);
    }

    // Avan�a para a pr�xima linha
    currentY += lineSpacing_z;

    // Linha 4: Chaves
    if (GungeonCore::player->hasKey1 || GungeonCore::player->hasKey2) {
        if (GungeonCore::player->hasKey1 && GungeonCore::player->hasKey2) {
            text.str("");
            text << "Chaves: 2";
            font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);
        }
        else {
            text.str("");
            text << "Chaves: 1";
            font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);
        }
    }
    else {
        text.str("");
        text << "Chaves: 0";
        font->Draw(textStartX, currentY, text.str(), textColor, Layer::FRONT, fontScale);
    }
}

void Hud::Draw() {
}
// -------------------------------------------------------------------------------
