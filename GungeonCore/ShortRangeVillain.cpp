#include "ShortRangeVillain.h"
#include "GungeonCore.h"
#include "Scene.h"

const float PATROL_DISTANCE = 120.0f; // Maximum distance from initial position for patrol

ShortRangeVillain::ShortRangeVillain(float x, float y, Scene * scene)
{
    // configura��o do objeto
	layer = CollisionLayer::ENEMY;

    sprite = new TileSet("Resources/short_range_sprite_sheet.png", 74, 63, 6, 30);
    animationWalking = new Animation(sprite, 0.095f, true);
    animationAttacking = new Animation(sprite, 0.095f, false);

	uint Seq1[6] = { 0, 1, 2, 3, 4, 5 };
	uint Seq2[6] = { 6, 7, 8, 9, 10, 11 };
	uint Seq3[9] = { 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	uint Seq4[9] = { 21, 22, 23, 24, 25, 26, 27, 28, 29 };

    animationWalking->Add(VILLAIN_WALKING_RIGHT, Seq1, 6);
    animationWalking->Add(VILLAIN_WALKING_LEFT, Seq2, 6);
    animationAttacking->Add(ATTACKING_RIGHT, Seq3, 9);
    animationAttacking->Add(ATTACKING_LEFT, Seq4, 9);

    BBox(new Rect(-(sprite->TileWidth() / 2.0f) + 24, -(sprite->TileHeight() / 2.0f) + 20, (sprite->TileWidth() / 2.0f) - 24, (sprite->TileHeight() / 2.0f)));
    this->MoveTo(x, y);

    walkingRightInPatrol = true; // Começa patrulhando para a direita
    jumpStrength = 7.5f;
    previousY = y;
    sinkAmount = 6.0f;

    initialX = (int)x;
    initialY = (int)y;

    villainShadow = new Shadow(this, scene);
    scene->Add(villainShadow, STATIC);

    attackCooldownTimer.Reset();
}

ShortRangeVillain::~ShortRangeVillain()
{
    delete sprite;
    delete animationWalking;
    delete animationAttacking;
}

void ShortRangeVillain::SetHorizontalVelocity(float velocityX) {
    // Isso define a magnitude da speedX para o valor absoluto de velocityX
    // e o �ngulo para 0 (direita) ou 180 (esquerda)
    if (velocityX > 0) {
        speedX.RotateTo(0.0f); // Direita
        speedX.ScaleTo(velocityX);
    }
    else if (velocityX < 0) {
        speedX.RotateTo(180.0f); // Esquerda
        speedX.ScaleTo(std::abs(velocityX)); // Magnitude � sempre positiva
    }
    else {
        speedX.ScaleTo(0.0f); // Parado
    }
}

void ShortRangeVillain::SetVerticalVelocity(float velocityY) {
    // Isso define a magnitude da speedY para o valor absoluto de velocityY
    // e o �ngulo para 90 (para cima) ou 270 (para baixo)


    if (velocityY > 0) { // Indo para cima
        speedY.RotateTo(90.0f);
        speedY.ScaleTo(velocityY);
    }
    else if (velocityY < 0) { // Indo para baixo (caindo)
        speedY.RotateTo(270.0f);
        speedY.ScaleTo(std::abs(velocityY)); // Magnitude � sempre positiva
    }
    else {
        speedY.ScaleTo(0.0f); // Parado verticalmente
    }
}

// -------------------------------------------------------------------------------

void ShortRangeVillain::MoveX(Vector&& v)
{
    // soma vetor (v2) ao vetor (v)
    speedX.Add(v);

    if (speedX.Magnitude() > 3.0f)
        speedX.ScaleTo(3.0f);
}

// -------------------------------------------------------------------------------

void ShortRangeVillain::MoveY(Vector&& v)
{
    // soma vetor (v2) ao vetor (v)
    speedY.Add(v);

    if (speedY.Magnitude() > 15.0f)
        speedY.ScaleTo(15.0f);
}
// ---------------------------------------------------------------------------------

void ShortRangeVillain::Reset()
{
    // volta ao estado inicial
    MoveTo((float) initialX, (float) initialY);
    currentMachineState = IDLE_OR_PATROLLING; // Garante que a IA resete o estado
    state = VILLAIN_WALKING_RIGHT;
}

void ShortRangeVillain::Update()
{
    previousY = y;
    float accel = 40.0f * gameTime;
    float gravityForce = 19.6f * gameTime;

    if (state == VILLAIN_WALKING_RIGHT || state == VILLAIN_WALKING_LEFT) {
        animationWalking->NextFrame();
    }
    else {
        animationAttacking->NextFrame();
    }

    if (state == VILLAIN_WALKING_RIGHT)
    {
        animationWalking->Select(VILLAIN_WALKING_RIGHT);
    }
    else if (state == VILLAIN_WALKING_LEFT)
    {
        animationWalking->Select(VILLAIN_WALKING_LEFT);
    }
    else if (state == ATTACKING_RIGHT)
    {
        animationAttacking->Select(ATTACKING_RIGHT);
    }
    else if (state == ATTACKING_LEFT)
    {
        animationAttacking->Select(ATTACKING_LEFT);
    }

    this->MachineState();

    //speedY.Add(Vector(270.0f, gravityForce * gameTime));

    MoveY(Vector(270.0f, gravityForce));

    Translate(speedX.XComponent() * 2.0f * gameTime, -speedY.YComponent() * 50.0f * gameTime);

    //// Restringe a �rea do jogo
    if (x < 0)
        MoveTo(0, y);
    if (y < 0)
        MoveTo(x, 0);
    if (x > game->Width())
        MoveTo(game->Width(), y);
    if (y > game->Height())
        MoveTo(x, game->Height());
}


void ShortRangeVillain::Draw()
{
    if (state == VILLAIN_WALKING_RIGHT || state == VILLAIN_WALKING_LEFT) {
        animationWalking->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
    }
    else {
        animationAttacking->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
    }
}

void ShortRangeVillain::OnCollision(Object* obj)
{
    if (obj->Type() == SOFTPLATFORM)
    {
        // trata geometria como rect
        Rect* villainRect = static_cast<Rect*>(this->BBox());
        Rect* platformRect = static_cast<Rect*>(obj->BBox());

        // verifica validade
        if (!villainRect || !platformRect)
            return;

        // posi��o da base do player no frame anterior
        float previousVillainBottom = previousY + villainRect->bottom;

        // topo da plataforma
        float platformTop = platformRect->Top();

        // verifica se jogador est� caindo
        bool isFalling = speedY.YComponent() < 0;

        // verifica se o jogador estava acima da plataforma no frame anterior
        bool wasAbove = previousVillainBottom <= platformTop + sinkAmount;

        // s� reposiciona o jogador se ele estava em uma posi��o acima da plataforma e est� caindo
        if (isFalling && wasAbove)
        {
            // c�lcula posi��o exata para reposicionamento do jogador
            float correctY = platformTop - villainRect->bottom;
            float finalY = correctY + sinkAmount;

            MoveTo(x, finalY);

            // zera velocidade vertical (para acelera��o da gravidade)
            speedY.ScaleTo(0.0f);

            // pula somente se estiver colidindo com plataforma
            /*if (window->KeyDown('W'))
            {
                speedY.Add(Vector(90.0f, jumpStrength));
            }*/
        }
    }

    if (obj->Type() == SOLIDPLATFORM) {
        // Gets the bounding boxes for the player and the platform
        Rect* playerRect = static_cast<Rect*>(this->BBox());
        Rect* platformRect = static_cast<Rect*>(obj->BBox());

        // Ensure the bounding boxes are valid
        if (!playerRect || !platformRect)
            return;

        // --- 1. Calculate Dimensions and Centers ---

        float playerWidth = playerRect->Right() - playerRect->Left();
        float playerHeight = playerRect->Bottom() - playerRect->Top();
        float platformWidth = platformRect->Right() - platformRect->Left();
        float platformHeight = platformRect->Bottom() - platformRect->Top();

        float playerCenterX = playerRect->Left() + playerWidth / 2.0f;
        float playerCenterY = (playerRect->Top() - sinkAmount) + playerHeight / 2.0f;
        float platformCenterX = platformRect->Left() + platformWidth / 2.0f;
        float platformCenterY = platformRect->Top() + platformHeight / 2.0f;

        // --- 2. Calculate Overlap ---

        float diffX = playerCenterX - platformCenterX;
        float diffY = playerCenterY - platformCenterY;

        float combinedHalfWidths = playerWidth / 2.0f + platformWidth / 2.0f;
        float combinedHalfHeights = playerHeight / 2.0f + platformHeight / 2.0f;

        float overlapX = combinedHalfWidths - abs(diffX);
        float overlapY = combinedHalfHeights - abs(diffY);

        // --- 3. Resolve Collision ---

        if (overlapX > 0 && overlapY > 0)
        {
            if (overlapX < overlapY)
            {
                // HORIZONTAL COLLISION (No sinking)
                if (diffX > 0)
                {
                    MoveTo(x + overlapX, y);
                }
                else
                {
                    MoveTo(x - overlapX, y);
                }
                speedX.ScaleTo(0.0f);
            }
            else
            {
                // VERTICAL COLLISION
                if (diffY > 0)
                {
                    // Hitting from below (No sinking)
                    MoveTo(x, y + overlapY);

                    // Stop vertical movement
                    speedY.ScaleTo(0.0f);
                }
                else
                {
                    // MODIFIED PART: Landing on top (Apply sinking)
                    MoveTo(x, y - overlapY);

                    // Stop vertical movement
                    speedY.ScaleTo(0.0f);

                    // pula somente se estiver colidindo com plataforma
                    /*if (window->KeyDown('W'))
                    {
                        speedY.Add(Vector(90.0f, jumpStrength));
                    }*/
                }

            }
        }
    }

    if (obj->Type() == FIRE) {
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
        TakeDamage(10);
        GungeonCore::level->GetScene()->Delete(obj, MOVING);
    }

}

bool ShortRangeVillain::CanSeePlayer(Vector direction, float range)
{
    RaycastHit hit = {false, Point(), 0.0f, nullptr};
    Point origin(x, y);

    // máscara de colisão
    unsigned int mask = static_cast<unsigned int>(CollisionLayer::PLAYER) |
        static_cast<unsigned int>(CollisionLayer::SOLIDPLATFORM);

    // raio na direção do jogador para ver se não há nenhuma plataforma na frente
    if (GungeonCore::level->GetScene()->Raycast(origin, direction, range, hit, mask, this))
    {
        // só vê se atingiu realmente um player
        return (hit.objectHit->layer == CollisionLayer::PLAYER);
    }    

    // caso contrário não viu
    return false;
}

void ShortRangeVillain::MachineState()
{
    float playerX = GungeonCore::player->X();
    float playerY = GungeonCore::player->Y();

    float distanceToPlayer = Point::Distance(Point(playerX, playerY), Point(x, y));

    playerSpotted = false; // começa assumindo que não viu o jogador
    
    if (distanceToPlayer <= VISION_RANGE)
    {
        // angulo de visao
        float facingAngle = (state == VILLAIN_WALKING_RIGHT || state == ATTACKING_RIGHT) ? 0.0f : 180.0f;

        // angulo em que o jogador está em relação ao vilão
        float angleToPlayer = Line::Angle(Point(x, y), Point(playerX, playerY));
        angleToPlayer = -angleToPlayer;

		// diferença de angulo entre o vilão e o jogador
        float angleDifference = abs(facingAngle - angleToPlayer);

        if (angleDifference > 180) {
            angleDifference = 360 - angleDifference;
        }

        // se está no campo de visao
        if (angleDifference <= 45.0f)
        {
            Vector directionToPlayer(angleToPlayer, 1.0f);

            if (CanSeePlayer(directionToPlayer, distanceToPlayer))
            {
                playerSpotted = true;
            }
        }
    }

    if (!playerSpotted && distanceToPlayer < 30.0f)
    {
        playerSpotted = true;
    }

    // Determina a direção horizontal para a anima��o
    if (speedX.Magnitude() > 0.1f) {
        if (speedX.XComponent() < 0) { // Se a componente X da velocidade é negativa, está indo para a esquerda
            state = VILLAIN_WALKING_LEFT;
        }
        else { // Se a componente X da velocidade é positiva, está indo para a direita
            state = VILLAIN_WALKING_RIGHT;
        }
    }
    else if (speedX.Angle() > 90.0f && speedX.Angle() < 270.0f) {
        state = ATTACKING_LEFT;
    }
    else {
        state = ATTACKING_RIGHT;
    }

    std::stringstream text;
    switch (currentMachineState) {
    case IDLE_OR_PATROLLING:
        HandlePatrolling();
        // Transi��es
        if (playerSpotted) {
            currentMachineState = PURSUING;
            text << "Vil�o: Jogador detectado! Perseguindo!\n";
            OutputDebugString(text.str().c_str());
            text.str("");
        }
        // Adicionar transi��o para "HURT_OR_STUNNED" se receber dano
        break;

    case PURSUING:
        HandlePursuing();
        // Transi��es

        if (distanceToPlayer < ATTACK_RANGE) {
            currentMachineState = ATTACKING;

            text << "Vil�o: Alvo no alcance! Atacando!\n";
            OutputDebugString(text.str().c_str());
            text.str("");
        }
        else if (distanceToPlayer > VISION_RANGE * 1.5f) { // Se o jogador fugir muito longe
            currentMachineState = IDLE_OR_PATROLLING;

            text << "Vil�o: Jogador muito longe. Voltando a patrulhar.\n";
            OutputDebugString(text.str().c_str());
            text.str("");
        }

        break;

    case ATTACKING:
        HandleAttacking();
        // Transi��es

        if (animationAttacking->Inactive()) {
            if (distanceToPlayer > ATTACK_RANGE) {
                currentMachineState = PURSUING;

                text << "Vil�o: Jogador se afastou. Voltando a perseguir!\n";
                OutputDebugString(text.str().c_str());
                text.str("");
            }
        }

        break;
    }
}

void ShortRangeVillain::HandlePatrolling() {
    // Calculate patrol bounds
    float leftBound = initialX - PATROL_DISTANCE;
    float rightBound = initialX + PATROL_DISTANCE;

    // Reverse direction if out of patrol bounds
    if (walkingRightInPatrol && x >= rightBound) {
        walkingRightInPatrol = false;
    } else if (!walkingRightInPatrol && x <= leftBound) {
        walkingRightInPatrol = true;
    }

    if (walkingRightInPatrol) {
        SetHorizontalVelocity(PATROL_SPEED);
    }
    else {
        SetHorizontalVelocity(-PATROL_SPEED);
    }

    Vector walkRayDirection;
    Point rayOrigin(x, y);
    RaycastHit obstacleHit;

    unsigned int wallMask = static_cast<unsigned int>(CollisionLayer::SOLIDPLATFORM); // só colide com plataformas sólidas

    if (walkingRightInPatrol) {
        walkRayDirection = Vector(0.0f, 1.0f);
    }
    else {
        walkRayDirection = Vector(180.0f, 1.0f);
    }

    // atira um raio pra ver se tem parede na frente, daí muda de direção se tiver
    if (GungeonCore::level->GetScene()->Raycast(rayOrigin, walkRayDirection, 20.0f, obstacleHit, wallMask, this))
    {
        if (obstacleHit.didHit)
        {
            walkingRightInPatrol = !walkingRightInPatrol;
        }
    }
}

void ShortRangeVillain::HandlePursuing() {
    float playerX = GungeonCore::player->X();
    if (playerX < x) {
        SetHorizontalVelocity(-CHASE_SPEED); // Persegue para a esquerda
    }
    else {
        SetHorizontalVelocity(CHASE_SPEED); // Persegue para a direita
    }
}

void ShortRangeVillain::HandleAttacking() {
    SetHorizontalVelocity(0.0f); // Vil�o para de se mover para atacar
    state = ATTACKING; // Define o estado de anima��o para ataque

    float hitboxWidth = 40.0f;
    float hitboxHeight = 40.0f;
    // Distância da hitbox em relação ao centro do vilão
    float offsetX = 15.0f;
    float offsetY = 5.0f;
    // ----------------------------

    float hitboxX = x; // Posição X da hitbox
    float hitboxY = y + offsetY; // Posição Y da hitbox

    // muda a direção da animação para o lado correto




    // Atualiza a posição da BBox de ataque
    // (O Rect é relativo ao seu ponto de origem, então definimos a origem dele)
    if (GungeonCore::player->X() < x)
    {
        state = ATTACKING_LEFT;
        hitboxX -= offsetX; // Desloca a caixa para a esquerda
    }
    else
    {
        state = ATTACKING_RIGHT;
        hitboxX += offsetX; // Desloca a caixa para a direita
    }

    if (attackCooldownTimer.Elapsed() >= attackCooldownDuration) {
        animationAttacking->Restart();
        GungeonCore::audio->Play(SHORTRANGE_ATTACK);
        Hitbox * hitbox = new Hitbox(hitboxWidth, hitboxHeight, hitboxX, hitboxY, 1.0f, 0.6f);

		GungeonCore::level->GetScene()->Add(hitbox, MOVING);
        // Realiza o ataque (dano ao player, spawn de proj�til, etc.)
        //std::cout << "Vil�o: ATAQUE REALIZADO!" << std::endl;
        attackCooldownTimer.Reset();
    }
}

void ShortRangeVillain::TakeDamage(int damage)
{
    // reduz vida do jogador
    health -= damage;
    // se a vida for menor ou igual a zero, o jogador morre
    if (health <= 0)
    {
        health = 0;
        GungeonCore::audio->Play(VILLAIN_DIES);
        GungeonCore::level->GetScene()->Delete(villainShadow, STATIC);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
        /*GungeonCore::hud->SetGameOver(true);
        GungeonCore::hud->SetGameWon(false);*/
    }
}