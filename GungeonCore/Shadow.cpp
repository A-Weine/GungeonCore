#include "Shadow.h"
#include "Types.h"
#include "GungeonCore.h"

Shadow::Shadow(Object* followTarget, Scene* scene)
{
    owner = followTarget;
    sprite = new Sprite("Resources/sombra.png");
    parentScene = scene;

    //debugRayStartSprite = new Sprite("Resources/debug_point.png");
    //debugHitPointSprite = new Sprite("Resources/debug_point.png");

    originalScale = 1.0f;
    scaleFactor = 1.0f;
    ScaleTo(originalScale);
    
    //debugHitX = 0.0f;
    //debugHitY = 0.0f;
    //debugStartX = 0.0f;
    //debugStartY = 0.0f;
}

Shadow::~Shadow()
{
    delete sprite;
    //delete debugRayStartSprite;
    //delete debugHitPointSprite;
}

void Shadow::Update()
{
    if (!owner)
    {
        ScaleTo(0.0f);
        return;
    }

    // testa se a sombra é de um player

    Point playerFeet(owner->X(), owner->Y());
    Vector down(90.0f, 1.0f);
    RaycastHit shadowHit;
    float maxShadowDistance = 300.0f;

    //debugStartX = playerFeet.X();
    //debugStartY = playerFeet.Y();

    unsigned int mask = static_cast<unsigned int>(CollisionLayer::SOLIDPLATFORM) |
                        static_cast<unsigned int>(CollisionLayer::SOFTPLATFORM);

    if (parentScene->Raycast(playerFeet, down, maxShadowDistance, shadowHit, mask, this))
    {
        if (GungeonCore::level->GetSinkAmount())
            MoveTo(shadowHit.hitPoint.X(), shadowHit.hitPoint.Y() + GungeonCore::level->GetSinkAmount());
        else
			MoveTo(shadowHit.hitPoint.X(), shadowHit.hitPoint.Y());


        float distanceToGround = shadowHit.distance;

        scaleFactor = 1.0f - (distanceToGround / maxShadowDistance);

        if (scaleFactor < 0.1f) scaleFactor = 0.1f;
        if (scaleFactor > 1.0f) scaleFactor = 1.0f;

        ScaleTo(originalScale * scaleFactor);
        
        //debugHitX = shadowHit.hitPoint.X();
        //debugHitY = shadowHit.hitPoint.Y();
    }
    else
    {
        ScaleTo(0.0f);

        //debugHitX = -1000.0f;
        //debugHitY = -1000.0f;
    }
}

void Shadow::Draw()
{
    if (scale > 0.0f)
    {
       sprite->Draw(x, y, Layer::MIDDLE, scale, 0.0f, { 1.0f, 1.0f, 1.0f, 0.4f * scaleFactor });
    }

    //debugRayStartSprite->Draw(debugStartX, debugStartY, Layer::UPPER);
    //debugHitPointSprite->Draw(debugHitX, debugHitY, Layer::UPPER);
}