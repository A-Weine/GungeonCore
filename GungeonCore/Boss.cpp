//#include "Boss.h"
//#include "GungeonCore.h"
//#include "Fire.h"
//#include "Scene.h"
//#include <cmath>
//#include <sstream>
//#include <windows.h>
//
//Player*& Boss::player = GungeonCore::player;
//
//// --- Boss State Management ---
//enum BossAttackState { BOSS_LONGRANGE, BOSS_SHORTRANGE };
//
//Boss::Boss()
//{
//    layer = CollisionLayer::ENEMY;
//
//    bossMagic = new Image("Resources/Magic.png");
//
//    spriteIdle = new TileSet("Resources/boss_sprite_sheet.png", 85, 93, 9, 79);
//    animationIdle = new Animation(spriteIdle, 0.095f, true);
//
//	spriteAction = new TileSet("Resources/boss_sprite_sheet.png", 85, 93, 9, 79);
//	animationAction = new Animation(spriteAction, 0.095f, true);
//
//
//
//	uint Seq1[18] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
//	uint Seq2[20] = { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37 };
//	uint Seq3[21] = { 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 };
//	uint Seq4[20] = { 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78 };
//
//    animationIdle->Add(IDLE, Seq1, 18);
//    animationIdle->Select(STOPPED_RIGHT);
//
//    // action
//    animationAction->Add(DASHING, Seq2, 20);
//    animationAction->Add(SUMMONING, Seq3, 21);
//    animationAction->Add(THROWING, Seq4, 20);
//    animationAction->Select(DASHING);
//
//    BBox(new Rect(-(spriteIdle->TileWidth() / 2.0f) + 20 , -(spriteIdle->TileHeight() / 2.0f), (spriteIdle->TileWidth() / 2.0f) - 20, (spriteIdle->TileHeight() / 2.0f)));
//
//    initialX = 400;
//    initialY = 200;
//    MoveTo((float) initialX, (float) initialY);
//
//    sinkAmount = 6.0f;
//    attackCooldownTimer.Reset();
//    stateSwitchTimer.Reset();
//
//    speedY.ScaleTo(0.0f); // Initialize vertical speed
//
//    // Initialize state variables
//    attackState = BOSS_LONGRANGE;
//    isDashing = false;
//    dashHitPlayer = false;
//    dashCooldown = 0.0f;
//
//    villainShadow = nullptr;
//
//    dead = false;
//
//    attackCooldownDuration = 0.5f; // <--- ADICIONE ESTA LINHA (2s de espera)
//}
//
//Boss::~Boss()
//{
//    delete spriteIdle;
//    delete spriteAction;
//    delete animationIdle;
//    delete animationAction;
//	delete bossMagic;
//}
//
//void Boss::Reset()
//{
//    MoveTo((float) initialX, (float) initialY);
//    health = 50;
//    attackCooldownTimer.Reset();
//    stateSwitchTimer.Reset();
//    speedY.ScaleTo(0.0f);
//
//    // Reset state variables
//    attackState = BOSS_LONGRANGE;
//    isDashing = false;
//    dashHitPlayer = false;
//    dashCooldown = 0.0f;
//}
//
//void Boss::OnCollision(Object* obj)
//{
//    // Copy the vertical collision logic from LongRangeVillain for platforms
//    if (obj->Type() == SOFTPLATFORM)
//    {
//        Rect* bossRect = static_cast<Rect*>(this->BBox());
//        Rect* platformRect = static_cast<Rect*>(obj->BBox());
//        if (!bossRect || !platformRect)
//            return;
//
//        float previousBossBottom = previousY + bossRect->bottom;
//        float platformTop = platformRect->Top();
//        bool isFalling = speedY.YComponent() < 0;
//        bool wasAbove = previousBossBottom <= platformTop + sinkAmount;
//
//        if (isFalling && wasAbove)
//        {
//            float correctY = platformTop - bossRect->bottom;
//            float finalY = correctY + sinkAmount;
//            MoveTo(x, finalY);
//            speedY.ScaleTo(0.0f);
//        }
//    }
//
//    if (obj->Type() == SOLIDPLATFORM) {
//        Rect* bossRect = static_cast<Rect*>(this->BBox());
//        Rect* platformRect = static_cast<Rect*>(obj->BBox());
//        if (!bossRect || !platformRect)
//            return;
//
//        float bossWidth = bossRect->Right() - bossRect->Left();
//        float bossHeight = bossRect->Bottom() - bossRect->Top();
//        float platformWidth = platformRect->Right() - platformRect->Left();
//        float platformHeight = platformRect->Bottom() - platformRect->Top();
//
//        float bossCenterX = bossRect->Left() + bossWidth / 2.0f;
//        float bossCenterY = (bossRect->Top() - sinkAmount) + bossHeight / 2.0f;
//        float platformCenterX = platformRect->Left() + platformWidth / 2.0f;
//        float platformCenterY = platformRect->Top() + platformHeight / 2.0f;
//
//        float diffX = bossCenterX - platformCenterX;
//        float diffY = bossCenterY - platformCenterY;
//
//        float combinedHalfWidths = bossWidth / 2.0f + platformWidth / 2.0f;
//        float combinedHalfHeights = bossHeight / 2.0f + platformHeight / 2.0f;
//
//        float overlapX = combinedHalfWidths - abs(diffX);
//        float overlapY = combinedHalfHeights - abs(diffY);
//
//        if (overlapX > 0 && overlapY > 0)
//        {
//            if (overlapX < overlapY)
//            {
//                if (diffX > 0)
//                    MoveTo(x + overlapX, y);
//                else
//                    MoveTo(x - overlapX, y);
//            }
//            else
//            {
//                if (diffY > 0)
//                {
//                    MoveTo(x, y + overlapY);
//                    speedY.ScaleTo(0.0f);
//                }
//                else
//                {
//                    MoveTo(x, y - overlapY);
//                    speedY.ScaleTo(0.0f);
//                }
//            }
//        }
//    }
//
//    // --- DASH COLLISION WITH PLAYER ---
//    if (attackState == BOSS_SHORTRANGE && isDashing && !dashHitPlayer && obj->Type() == PLAYER) {
//        player->TakeDamage(8);
//
//        // Push player away from boss
//        float pushStrength = 10.0f;
//        float px = player->X() - x;
//        float py = player->Y() - y;
//        float plen = sqrt(px * px + py * py);
//        if (plen > 0.1f) {
//            float pushX = (px / plen) * pushStrength;
//            float pushY = (py / plen) * pushStrength;
//        }
//
//        isDashing = false;
//        dashCooldown = dashCooldownDuration;
//
//        // Move boss away from player
//        float moveAwayDist = 80.0f;
//        float awayX = x - dashDirX * moveAwayDist;
//        float awayY = y - dashDirY * moveAwayDist;
//        MoveTo(awayX, awayY);
//
//        dashHitPlayer = true;
//    }
//
//    // --- DASH COLLISION WITH WALL ---
//    if (attackState == BOSS_SHORTRANGE && isDashing && obj->Type() == SOLIDPLATFORM) {
//        isDashing = false;
//        dashCooldown = dashCooldownDuration;
//    }
//
//    if (obj->Type() == FIRE) {
//        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
//        TakeDamage(10);
//        GungeonCore::level->GetScene()->Delete(obj, MOVING);
//    }
//}
//void Boss::Update()
//{
//    previousY = y;
//
//    if (state == IDLE) {
//        animationIdle->NextFrame();
//    }
//    else {
//        animationAction->NextFrame();
//    }
//
//    // Calculate distance to player
//    float playerX = player->X();
//    float playerY = player->Y();
//    float dx = playerX - x;
//    float dy = playerY - y;
//    float dist = sqrt(dx * dx + dy * dy);
//
//    // --- State switching logic ---
//    if (stateSwitchTimer.Elapsed() >= stateSwitchInterval) {
//        attackState = (attackState == BOSS_LONGRANGE) ? BOSS_SHORTRANGE : BOSS_LONGRANGE;
//        stateSwitchTimer.Reset();
//        isDashing = false;
//        dashHitPlayer = false;
//        dashCooldown = 0.0f;
//
//        std::stringstream text;
//        text << "Boss: Switching to " << (attackState == BOSS_LONGRANGE ? "LONG RANGE" : "SHORT RANGE") << " mode!\n";
//        OutputDebugString(text.str().c_str());
//    }
//
//    // --- Dash cooldown logic ---
//    if (dashCooldown > 0.0f) {
//        dashCooldown -= gameTime;
//        if (dashCooldown < 0.0f) dashCooldown = 0.0f;
//    }
//
//    // Initial velocity settings
//    float vx = 0.0f;
//    float vy = 0.0f;
//
//    // State-specific behavior
//    if (attackState == BOSS_LONGRANGE) {
//        // Only move horizontally if outside shooting range
//        const float SHOOTING_RANGE = 240.0f;
//        float moveSpeed = 60.0f * gameTime;
//        if (dist > SHOOTING_RANGE) {
//            vx = (dx / dist) * moveSpeed;
//        }
//
//        // Flying logic
//        const float FLY_THRESHOLD = 40.0f;
//        const float FLY_SPEED = 50.0f * gameTime;
//        if (playerY < y - FLY_THRESHOLD) {
//            // Player is above, fly up
//            vy = -FLY_SPEED;
//            speedY.ScaleTo(0.0f);
//        }
//
//        // Gravity
//        float gravityForce = 19.6f * gameTime;
//        if (speedY.Magnitude() < 15.0f)
//            speedY.Add(Vector(270.0f, gravityForce));
//        vy -= speedY.YComponent() * 50 * gameTime;
//
//        // Shooting logic
//        if (attackCooldownTimer.Elapsed() >= attackCooldownDuration && dist <= SHOOTING_RANGE) {
//            state = THROWING;
//            Point bossPos(x, y);
//            Point playerPos(player->X(), player->Y());
//            float firingAngle = Line::Angle(bossPos, playerPos);
//
//            //Level2::scene->Add(new Fire(this, firingAngle, bossMagic, VILLAIN), MOVING);
//
//            std::stringstream text;
//            text << "Player Take Damage: " << GungeonCore::player->takeDamage << '\n';
//            OutputDebugString(text.str().c_str());
//            text.str("");
//
//            //std::stringstream text;
//            text << "Boss: ATTACK EXECUTED!\n";
//            OutputDebugString(text.str().c_str());
//            attackCooldownTimer.Reset();
//        }
//    }
//    else if (attackState == BOSS_SHORTRANGE) {
//        // Flying logic (same as long range)
//        const float FLY_THRESHOLD = 40.0f;
//        const float FLY_SPEED = 50.0f * gameTime;
//        if (playerY < y - FLY_THRESHOLD) {
//            // Player is above, fly up
//            vy = -FLY_SPEED;
//            speedY.ScaleTo(0.0f);
//        }
//
//        // Gravity (same as long range)
//        float gravityForce = 19.6f * gameTime;
//        if (speedY.Magnitude() < 15.0f)
//            speedY.Add(Vector(270.0f, gravityForce));
//
//        // Only apply gravity if not dashing
//        if (!isDashing) {
//            vy -= speedY.YComponent() * 50 * gameTime;
//        }
//
//        const float dashSpeedValue = 400.0f; // Higher speed for more noticeable dash
//
//        if (!isDashing) {
//            state = IDLE;
//            // Regular movement when not dashing
//            const float PREFERRED_DISTANCE = 150.0f;
//            float moveSpeed = 70.0f * gameTime;
//
//            if (dist > PREFERRED_DISTANCE) {
//                vx = (dx / dist) * moveSpeed;
//            }
//
//            // Try to dash if cooldown is over
//            if (dashCooldown <= 0.0f) {
//                state = DASHING;
//                // Prepare for dash
//                dashDirX = (dx != 0) ? dx / dist : 0.0f;
//                dashDirY = (dy != 0) ? dy / dist : 0.0f;
//                isDashing = true;
//                dashHitPlayer = false;
//
//                GungeonCore::audio->Play(BOSS_DASH);
//
//                std::stringstream text;
//                text << "Boss: DASH STARTED at position (" << x << ", " << y << ") toward player!\n";
//                OutputDebugString(text.str().c_str());
//
//                // Reset attack cooldown timer to track dash duration
//                attackCooldownTimer.Reset();
//            }
//        }
//        else {
//            // Dashing logic - move directly toward target at high speed
//            vx = dashDirX * dashSpeedValue * gameTime;
//            vy = dashDirY * dashSpeedValue * gameTime;
//
//            // Auto-cancel dash after a short time (if didn't hit anything)
//            if (attackCooldownTimer.Elapsed() >= 0.75f) {
//                std::stringstream text;
//                text << "Boss: DASH TIMED OUT after 0.75s at position (" << x << ", " << y << ")\n";
//                OutputDebugString(text.str().c_str());
//
//                isDashing = false;
//                dashCooldown = dashCooldownDuration * 0.7f; // Cooldown after dash
//            }
//        }
//    }
//
//    // Move
//    Translate(vx, vy);
//
//    // Restrict to game area
//    if (x < 0) MoveTo(0, y);
//    if (y < 0) MoveTo(x, 0);
//    if (x > game->Width()) MoveTo(game->Width(), y);
//    if (y > game->Height()) MoveTo(x, game->Height());
//}
//
//void Boss::Draw()
//{
//    if (state == IDLE) {
//        animationIdle->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
//    }
//    else {
//		animationAction->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
//    }
//}
//
//void Boss::TakeDamage(int damage)
//{
//    health -= damage;
//    if (health <= 0) {
//        health = 0;
//        if (villainShadow)
//            GungeonCore::level->GetScene()->Delete(villainShadow, STATIC);
//
//        dead = true;
//
//        GungeonCore::audio->Play(BOSS_DYING);
//        GungeonCore::level->GetScene()->Delete(this, MOVING);
//    }
//}