#include "GameOver.h"
#include "Engine.h"
#include "Level1.h" // Precisa saber sobre o Level1 para poder iniciar
#include "GungeonCore.h" // Precisa para chamar o NextLevel
#include <sstream>

void GameOver::Init()
{
    // O mundo do menu tem o tamanho da janela
    Size(1024, 576);

    zoom = 2.0f; // Menu não tem zoom
    viewLeft = 0.0f;
    viewTop = 0.0f;
    viewRight = window->Width();
    viewBottom = window->Height();

    scene = new Scene();
    backg = new Background("Resources/backgroundLevel2.png"); // Um fundo para o menu
    gameOver = new Sprite("Resources/gameover.png");

    mouse = new Mouse();

    // Cria os itens de menu e os adiciona à cena
    // Os IDs (101, 102) são para identificação na colisão
    items.push_back(new MenuItem(game->CenterX(), game->CenterY() + 25, PLAY, "Resources/TryAgain.png"));
    items.push_back(new MenuItem(game->CenterX(), game->CenterY() + 60, QUIT, "Resources/ExitGame.png"));

    for (auto item : items)
        scene->Add(item, STATIC);

    scene->Add(mouse, MOVING);

    // Seleciona a primeira opção por padrão
    items[selectedOption]->Select();

    //GungeonCore::audio->Play(LOOP_THEME, true);
    GungeonCore::audio->Play(GAME_OVER_SONG, true);

    gamepad = new Controller();
    gamepadOn = gamepad->XboxInitialize();
}

void GameOver::Update()
{
    //std::stringstream text;
    //text << "Mouse: " << mouse->X() << " " << mouse->Y() << "\n";
    //OutputDebugString(text.str().c_str());

    // Atualiza a posição do mouse e os objetos na cena (os itens de menu)
    scene->Update();
    // Lógica para navegar no menu com o teclado
    if (window->KeyPress(VK_DOWN) && selectedOption < items.size() - 1)
    {
        items[selectedOption]->UnSelect();
        selectedOption++;
        items[selectedOption]->Select();
    }
    if (window->KeyPress(VK_UP) && selectedOption > 0)
    {
        items[selectedOption]->UnSelect();
        selectedOption--;
        items[selectedOption]->Select();
    }

    // Lógica para clicar com o mouse ou teclado
    bool itemClicked = false;
    int clickedItem = -1;

    // Checa colisão do mouse com os itens
    for (int i = 0; i < items.size(); ++i)
    {
        if (scene->Collision(mouse, items[i]))
        {
            if (selectedOption != i)
            {
                //std::stringstream text;
                //text << "selectedOption: " << selectedOption << "\n";
                //OutputDebugString(text.str().c_str());

                items[selectedOption]->UnSelect();
                selectedOption = i;
                items[selectedOption]->Select();
            }
            if (mouse->Clicked())
            {
                itemClicked = true;
                clickedItem = i;
            }
            break;
        }
    }

    // Checa "Enter" no teclado
    if (window->KeyPress(VK_RETURN))
    {
        itemClicked = true;
        clickedItem = selectedOption;
    }

    if (gamepadOn && gamepad->XboxUpdateState()) {
        if (gamepad->XboxButton(ButtonA)) {
            itemClicked = true;
            clickedItem = 0;
        }
    }

    // Ação ao clicar
    if (itemClicked)
    {
        if (clickedItem == 0) // Opção "Jogar"
        {
            // Usa a função estática do Gerente Geral para mudar para o Level1
            GungeonCore::NextLevel<Level1>();
        }
        else if (clickedItem == 1) // Opção "Sair"
        {
            window->Close();
        }
    }
}

void GameOver::Draw()
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

    gameOver->Draw(game->CenterX(), game->CenterY() - 80, Layer::FRONT);

    scene->Draw(); // Desenha os botões e o mouse
}

void GameOver::Finalize()
{
    GungeonCore::audio->Stop(GAME_OVER_SONG);
    delete scene;
    delete backg;
    delete gameOver;
    delete gamepad;
}