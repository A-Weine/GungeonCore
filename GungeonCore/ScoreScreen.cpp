#include "ScoreScreen.h"
#include "GungeonCore.h"
#include "Level1.h"
#include "Home.h"
#include "Engine.h"
#include <sstream>
#include <iomanip>

void ScoreScreen::Init()
{
    // Set the same size as Home screen
    Size(1024, 576);
    
    zoom = 1.0f;
    viewLeft = 0.0f;
    viewTop = 0.0f;
    viewRight = window->Width();
    viewBottom = window->Height();
    
    // Create scene for score screen objects
    scene = new Scene();
    
    // Create the background (same as Home)
    backg = new Background("Resources/background_t.png", 4.1f);
    
    
    // Create fonts for displaying text
    //font = new Font("Resources/font.png");
    //font->Spacing("Resources/font.dat");

    font = new Font("Resources/terminal24.png");
    //font->Spacing("Resources/consolas12.dat");

    // Initialize viewport variables
    sinkAmount = 0.0f;

    //GungeonCore::audio->Play(LOOP_THEME, true);
    GungeonCore::audio->Play(VICTORY_SONG, true);
}

void ScoreScreen::Update()
{
    // Update scene
    scene->Update();

    // Direct key shortcuts
    if (window->KeyPress(VK_SPACE))
           GungeonCore::NextLevel<Home>();

}

void ScoreScreen::Draw()
{
    // --- CÂMERA ESTÁTICA COM ZOOM PARA O MENU ---

    // 1. O "alvo" da câmera é sempre o centro do nosso mundo de 512x288.
    float cameraX = this->CenterX(); // Será 256
    float cameraY = this->CenterY(); // Será 144

    // 2. Construir a matriz de transformação (lógica idêntica à do Level1)
    float winWidth = window->Width();
    float winHeight = window->Height();

    float viewWidth = winWidth / zoom;   // 1024 / 2 = 512
    float viewHeight = winHeight / zoom; // 576 / 2 = 288

    // Calcula as bordas da visão. Como cameraX = viewWidth/2, viewLeft será 0.
    // O resultado é que a câmera verá exatamente o mundo de 0 a 512.
    this->viewLeft = cameraX - (viewWidth / 2.0f);
    this->viewRight = cameraX + (viewWidth / 2.0f);
    this->viewTop = cameraY - (viewHeight / 2.0f);
    this->viewBottom = cameraY + (viewHeight / 2.0f);

    // Cria a matriz final.
    XMMATRIX finalMatrix = XMMatrixOrthographicOffCenterLH(
        this->viewLeft, this->viewRight, this->viewBottom, this->viewTop, 0.0f, 1.0f);

    // 3. Envia a matriz para o Renderer.
    Engine::renderer->SetTransform(finalMatrix);

    // --- FIM DA CÂMERA ---


    // --- DESENHO DOS ELEMENTOS ---
    // Todos os elementos agora serão desenhados no mundo de 512x288 e ampliados pela GPU.

    // O background precisa de um ajuste para não usar parallax
    if (backg)
        backg->Draw(); // Usando a versão de Draw sem parâmetros
    
	Color white(1, 1, 1, 1.0f);

    // Convert completion time to a formatted string (minutes:seconds.milliseconds)
	float completionTime = GungeonCore::completionTime->Elapsed();
    int minutes = (int)completionTime / 60;
    int seconds = (int)completionTime % 60;
    int milliseconds = (int)((completionTime - (int)completionTime) * 100);
    
    std::stringstream timeText;
    timeText << "YOUR TIME: ";
    timeText << minutes << ":";
    timeText << std::setfill('0') << std::setw(2) << seconds << ".";
    timeText << std::setfill('0') << std::setw(2) << milliseconds;
    // Draw title
    font->Draw(window->CenterX() - 280.0f, window->CenterY() - 100.0f, timeText.str(), white, Layer::FRONT, 0.2f);
    font->Draw(window->CenterX() - 220.0f, window->CenterY() + 100.0f, "PRESS SPACE TO", white, Layer::FRONT, 0.2f);
    font->Draw(window->CenterX() - 120.0f, window->CenterY() + 130.0f, "CONTINUE", white, Layer::FRONT, 0.2f);
}

void ScoreScreen::Finalize()
{
    GungeonCore::audio->Stop(VICTORY_SONG);
    delete backg;
    delete font;
    delete titleFont;
    delete scene;
}