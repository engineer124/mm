#include "global.h"

#define DYNAPOLY_STATE_NONE 0
#define DYNAPOLY_STATE_RIDING_FALLING (1 << 0)
#define DYNAPOLY_STATE_RIDING_MOVING (1 << 1)
#define DYNAPOLY_STATE_RIDING_ROTATING (1 << 2)
#define DYNAPOLY_STATE_SWITCH_PRESSED (1 << 3)
#define DYNAPOLY_STATE_HEAVY_SWITCH_PRESSED (1 << 4)

void DynaPolyActor_Init(DynaPolyActor* dynaActor, s32 flags) {
    dynaActor->bgId = -1;
    dynaActor->pushForce = 0.0f;
    dynaActor->unk14C = 0.0f;
    dynaActor->flags = flags;
    dynaActor->stateFlags = DYNAPOLY_STATE_NONE;
}

void DynaPolyActor_LoadMesh(PlayState* play, DynaPolyActor* dynaActor, CollisionHeader* meshHeader) {
    CollisionHeader* header = NULL;

    CollisionHeader_GetVirtual(meshHeader, &header);
    dynaActor->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &dynaActor->actor, header);
}

void DynaPolyActor_ResetState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags = DYNAPOLY_STATE_NONE;
}

void DynaPolyActor_SetRidingFallingState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags |= DYNAPOLY_STATE_RIDING_FALLING;
}

void DynaPolyActor_SetRidingMovingState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags |= DYNAPOLY_STATE_RIDING_MOVING;
}

void DynaPolyActor_SetRidingMovingStateByIndex(CollisionContext* colCtx, s32 bgId) {
    DynaPolyActor* dynaActor = DynaPoly_GetActor(colCtx, bgId);

    if (dynaActor != NULL) {
        DynaPolyActor_SetRidingMovingState(dynaActor);
    }
}

void DynaPolyActor_SetRidingRotatingState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags |= DYNAPOLY_STATE_RIDING_ROTATING;
}

void DynaPolyActor_SetRidingRotatingStateByIndex(CollisionContext* colCtx, s32 bgId) {
    DynaPolyActor* dynaActor = DynaPoly_GetActor(colCtx, bgId);

    if (dynaActor != NULL) {
        DynaPolyActor_SetRidingRotatingState(dynaActor);
    }
}

void DynaPolyActor_SetSwitchPressedState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags |= DYNAPOLY_STATE_SWITCH_PRESSED;
}

void DynaPolyActor_SetHeavySwitchPressedState(DynaPolyActor* dynaActor) {
    dynaActor->stateFlags |= DYNAPOLY_STATE_HEAVY_SWITCH_PRESSED;
}

s32 DynaPolyActor_IsInRidingFallingState(DynaPolyActor* dynaActor) {
    if (dynaActor->stateFlags & DYNAPOLY_STATE_RIDING_FALLING) {
        return true;
    } else {
        return false;
    }
}

s32 DynaPolyActor_IsInRidingMovingState(DynaPolyActor* dynaActor) {
    if (dynaActor->stateFlags & DYNAPOLY_STATE_RIDING_MOVING) {
        return true;
    } else {
        return false;
    }
}

s32 DynaPolyActor_IsInRidingRotatingState(DynaPolyActor* dynaActor) {
    if (dynaActor->stateFlags & DYNAPOLY_STATE_RIDING_ROTATING) {
        return true;
    } else {
        return false;
    }
}

s32 DynaPolyActor_IsInSwitchPressedState(DynaPolyActor* dynaActor) {
    if (dynaActor->stateFlags & DYNAPOLY_STATE_SWITCH_PRESSED) {
        return true;
    } else {
        return false;
    }
}

s32 DynaPolyActor_IsInHeavySwitchPressedState(DynaPolyActor* dynaActor) {
    if (dynaActor->stateFlags & DYNAPOLY_STATE_HEAVY_SWITCH_PRESSED) {
        return true;
    } else {
        return false;
    }
}

s32 DynaPolyActor_ValidateMove(PlayState* play, DynaPolyActor* dynaActor, s16 startRadius, s16 endRadius,
                               s16 startHeight) {
    Vec3f startPos;
    Vec3f endPos;
    Vec3f intersectionPos;
    f32 sin = Math_SinS(dynaActor->yRotation);
    f32 cos = Math_CosS(dynaActor->yRotation);
    s32 bgId;
    CollisionPoly* poly;
    f32 adjustedStartRadius;
    f32 adjustedEndRadius;
    f32 sign = (0.0f <= dynaActor->pushForce) ? 1.0f : -1.0f;

    adjustedStartRadius = (f32)startRadius - 0.1f;
    startPos.x = dynaActor->actor.world.pos.x + (adjustedStartRadius * cos);
    startPos.y = dynaActor->actor.world.pos.y + startHeight;
    startPos.z = dynaActor->actor.world.pos.z - (adjustedStartRadius * sin);

    adjustedEndRadius = (f32)endRadius - 0.1f;
    endPos.x = sign * adjustedEndRadius * sin + startPos.x;
    endPos.y = startPos.y;
    endPos.z = sign * adjustedEndRadius * cos + startPos.z;

    if (BgCheck_EntityLineTest3(&play->colCtx, &startPos, &endPos, &intersectionPos, &poly, true, false, false, true,
                                &bgId, &dynaActor->actor, 0.0f)) {
        return false;
    }
    startPos.x = (dynaActor->actor.world.pos.x * 2.0f) - startPos.x;
    startPos.z = (dynaActor->actor.world.pos.z * 2.0f) - startPos.z;
    endPos.x = sign * adjustedEndRadius * sin + startPos.x;
    endPos.z = sign * adjustedEndRadius * cos + startPos.z;
    if (BgCheck_EntityLineTest3(&play->colCtx, &startPos, &endPos, &intersectionPos, &poly, true, false, false, true,
                                &bgId, &dynaActor->actor, 0.0f)) {
        return false;
    }
    return true;
}
