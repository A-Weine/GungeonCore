/**********************************************************************************
// Background (Código Fonte)
// 
// Criação:     07 Dez 2011
// Atualização: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Representa um pano de fundo maior que a janela
//
**********************************************************************************/


#include "Background.h"
#include "Engine.h"

// -------------------------------------------------------------------------------

Background::Background(string filename, float sizes)
{
    // carrega imagem
    image = new Image(filename);

    // --- NOVA LÓGICA DE SETUP ---
    // O sprite agora representa a IMAGEM INTEIRA do background,
    // não apenas o que está na tela.
    spriteData.width = image->Width();
    spriteData.height = image->Height();

    // Usaremos a textura inteira, sempre.
    spriteData.texCoord.x = 0.0f;
    spriteData.texCoord.y = 0.0f;
    spriteData.texSize.x = 1.0f;
    spriteData.texSize.y = 1.0f;

    // As outras propriedades permanecem as mesmas
    spriteData.depth = Layer::BACK;
    spriteData.scale = sizes;
    spriteData.rotation = 0.0f;
    spriteData.color = Color(1, 1, 1, 1);
    spriteData.texture = image->View();

    // A posição (x,y) será definida a cada quadro no Draw()
}

// -------------------------------------------------------------------------------

Background::~Background()
{
    delete image;
}

// -------------------------------------------------------------------------------

void Background::Draw(float cameraX, float cameraY)
{
    // --- DEBUG PASSO B ---
    //std::string received_pos = "B) BG Draw Recebeu: X=" + std::to_string(cameraX) + ", Y=" + std::to_string(cameraY) + "\n";
    //OutputDebugStringA(received_pos.c_str());
    // --- FIM DEBUG ---

    // Defina a velocidade do seu background.
    // 0.0 = parado (infinitamente longe)
    // 1.0 = move-se junto com a câmera
    float parallaxFactorX = 0.7f;
    float parallaxFactorY = 0.7f;

    // A posição do centro do background "persegue" a câmera
    // com a velocidade definida pelo fator de parallax.
    // Isso posiciona o background no mundo.
    spriteData.x = cameraX * parallaxFactorX ;
    spriteData.y = (cameraY * parallaxFactorY);

    // Adiciona o sprite na lista de desenho.
    // A câmera da GPU fará o resto do trabalho.
    Engine::renderer->Draw(spriteData);
}

// Esta é a nova função para fundos estáticos (como o menu)
void Background::Draw()
{
    // Simplesmente posiciona o sprite no centro da JANELA
    spriteData.x = Engine::window->CenterX();
    spriteData.y = Engine::window->CenterY();
    Engine::renderer->Draw(spriteData);
}
// -------------------------------------------------------------------------------

