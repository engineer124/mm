#!/usr/bin/env python3

import argparse, os

# "old": "new"
animdict ={
    "Actor_GetSwitchFlag": "Flags_GetSwitch",
    "atan_flip": "Math_Acot2F",
    "atans": "Math_Atan2S",
    "SysMatrix_StateAlloc": "Matrix_StateAlloc",
    "SysMatrix_StatePush": "Matrix_StatePush",
    "SysMatrix_CopyCurrentState": "Matrix_CopyCurrentState",
    "SysMatrix_SetCurrentState": "Matrix_SetCurrentState",
    "SysMatrix_InsertTranslation": "Matrix_InsertTranslation",
    "SysMatrix_InsertMatrix": "Matrix_InsertMatrix",
    "SysMatrix_Scale": "Matrix_Scale",
    "SysMatrix_InsertXRotation_s": "Matrix_InsertXRotation_s",
    "SysMatrix_InsertXRotation_f": "Matrix_InsertXRotation_f",
    "SysMatrix_RotateStateAroundXAxis": "Matrix_RotateStateAroundXAxis",
    "SysMatrix_SetStateXRotation": "Matrix_SetStateXRotation",
    "SysMatrix_RotateY": "Matrix_RotateY",
    "SysMatrix_InsertYRotation_f": "Matrix_InsertYRotation_f",
    "SysMatrix_InsertZRotation_s": "Matrix_InsertZRotation_s",
    "SysMatrix_InsertZRotation_f": "Matrix_InsertZRotation_f",
    "SysMatrix_InsertRotation": "Matrix_InsertRotation",
    "SysMatrix_JointPosition": "Matrix_JointPosition",
    "SysMatrix_SetStateRotationAndTranslation": "Matrix_SetStateRotationAndTranslation",
    "SysMatrix_ToRSPMatrix": "Matrix_ToRSPMatrix",
    "SysMatrix_ToMtx": "Matrix_ToMtx",
    "SysMatrix_NewMtx": "Matrix_NewMtx",
    "SysMatrix_AppendToPolyOpaDisp": "Matrix_AppendToPolyOpaDisp",
    "SysMatrix_MultiplyVector3fByState": "Matrix_MultiplyVector3fByState",
    "SysMatrix_GetStateTranslation": "Matrix_GetStateTranslation",
    "SysMatrix_GetStateTranslationAndScaledX": "Matrix_GetStateTranslationAndScaledX",
    "SysMatrix_GetStateTranslationAndScaledY": "Matrix_GetStateTranslationAndScaledY",
    "SysMatrix_GetStateTranslationAndScaledZ": "Matrix_GetStateTranslationAndScaledZ",
    "SysMatrix_MultiplyVector3fXZByCurrentState": "Matrix_MultiplyVector3fXZByCurrentState",
    "SysMatrix_MtxFCopy": "Matrix_MtxFCopy",
    "SysMatrix_FromRSPMatrix": "Matrix_FromRSPMatrix",
    "SysMatrix_MultiplyVector3fByMatrix": "Matrix_MultiplyVector3fByMatrix",
    "SysMatrix_TransposeXYZ": "Matrix_TransposeXYZ",
    "SysMatrix_NormalizeXYZ": "Matrix_NormalizeXYZ",
    "SysMatrix_InsertRotationAroundUnitVector_f": "Matrix_InsertRotationAroundUnitVector_f",
    "SysMatrix_InsertRotationAroundUnitVector_s": "Matrix_InsertRotationAroundUnitVector_s",
    "func_800B78B8": "Actor_UpdateBgCheckInfo",
    "func_8012403C": "Player_GetMask",
    "func_8012404c": "Player_RemoveMask",
    "Actor_SpawnWithParentAndCutscene": "Actor_SpawnAsChildAndCutscene",
    "Actor_SpawnWithParent": "Actor_SpawnAsChild",
    "func_800BE22C": "Actor_ApplyDamage",
    "func_800F0568": "SoundSource_PlaySoundByPosition",
    "func_8016970C": "Play_CameraSetAtEye",
    "func_800BBA88": "Enemy_StartFinishingBlow",
    "ShrinkWindow_Step": "ShrinkWindow_Update",
    "ShrinkWindow_Fini": "ShrinkWindow_Destroy",
    "func_801A89A8": "Audio_QueueSeqCmd",
    "SkelAnime_LodDrawLimb": "SkelAnime_DrawLimbLod",
    "SkelAnime_LodDraw": "SkelAnime_DrawLod",
    "SkelAnime_LodDrawLimbSV": "SkelAnime_DrawFlexLimbLod",
    "SkelAnime_LodDrawSV": "SkelAnime_DrawFlexLod",
    "SkelAnime_DrawLimb": "SkelAnime_DrawLimbOpa",
    "SkelAnime_Draw": "SkelAnime_DrawOpa",
    "SkelAnime_DrawLimbSV": "SkelAnime_DrawFlexLimbOpa",
    "SkelAnime_DrawSV": "SkelAnime_DrawFlexOpa",
    "SkelAnime_AnimateFrame": "SkelAnime_GetFrameData",
    "SkelAnime_GetTotalFrames": "Animation_GetLength",
    "SkelAnime_GetFrameCount": "Animation_GetLastFrame",
    "SkelAnime_Draw2Limb": "SkelAnime_DrawLimb",
    "SkelAnime_Draw2": "SkelAnime_Draw",
    "SkelAnime_DrawLimbSV2": "SkelAnime_DrawFlexLimb",
    "SkelAnime_DrawSV2": "SkelAnime_DrawFlex",
    "func_80134FFC": "SkelAnime_GetFrameData2",
    "func_801353D4": "Animation_GetLimbCount2",
    "SkelAnime_GetTotalFrames2": "Animation_GetLength2",
    "SkelAnime_GetFrameCount2": "Animation_GetLastFrame2",
    "SkelAnime_InterpolateVec3s": "SkelAnime_InterpFrameTable",
    "SkelAnime_AnimationCtxReset": "AnimationContext_Reset",
    "func_801358D4": "AnimationContext_SetNextQueue",
    "func_801358F4": "AnimationContext_DisableQueue",
    "SkelAnime_NextEntry": "AnimationContext_AddEntry",
    "SkelAnime_LoadLinkAnimetion": "AnimationContext_SetLoadFrame",
    "SkelAnime_LoadAnimationType1": "AnimationContext_SetCopyAll",
    "SkelAnime_LoadAnimationType2": "AnimationContext_SetInterp",
    "SkelAnime_LoadAnimationType3": "AnimationContext_SetCopyTrue",
    "SkelAnime_LoadAnimationType4": "AnimationContext_SetCopyFalse",
    "SkelAnime_LoadAnimationType5": "AnimationContext_SetMoveActor",
    "SkelAnime_LinkAnimetionLoaded": "AnimationContext_LoadFrame",
    "SkelAnime_AnimationType1Loaded": "AnimationContext_CopyAll",
    "SkelAnime_AnimationType2Loaded": "AnimationContext_CopyInterp",
    "SkelAnime_AnimationType3Loaded": "AnimationContext_CopyTrue",
    "SkelAnime_AnimationType4Loaded": "AnimationContext_CopyFalse",
    "SkelAnime_AnimationType5Loaded": "AnimationContext_MoveActor",
    "func_80135EE8": "AnimationContext_Update",
    "SkelAnime_InitLinkAnimetion": "SkelAnime_InitLink",
    "func_801360A8": "LinkAnimation_SetUpdateFunction",
    "func_801360E0": "LinkAnimation_Update",
    "func_80136104": "LinkAnimation_Morph",
    "func_801361BC": "LinkAnimation_AnimateFrame",
    "func_80136288": "LinkAnimation_Loop",
    "func_8013631C": "LinkAnimation_Once",
    "SkelAnime_SetTransition": "Animation_SetMorph",
    "SkelAnime_ChangeLinkAnim": "LinkAnimation_Change",
    "SkelAnime_ChangeLinkAnimDefaultStop": "LinkAnimation_PlayOnce",
    "SkelAnime_ChangeLinkAnimPlaybackStop": "LinkAnimation_PlayOnceSetSpeed",
    "SkelAnime_ChangeLinkAnimDefaultRepeat": "LinkAnimation_PlayLoop",
    "SkelAnime_ChangeLinkAnimPlaybackRepeat": "LinkAnimation_PlayLoopSetSpeed",
    "func_8013670C": "LinkAnimation_CopyJointToMorph",
    "func_8013673C": "LinkAnimation_CopyMorphToJoint",
    "func_8013676C": "LinkAnimation_LoadToMorph",
    "func_801367B0": "LinkAnimation_LoadToJoint",
    "func_801367F4": "LinkAnimation_InterpJointMorph",
    "func_8013682C": "LinkAnimation_BlendToJoint",
    "func_801368CC": "LinkAnimation_BlendToMorph",
    "SkelAnime_SetModeStop": "LinkAnimation_EndLoop",
    "func_80136990": "Animation_OnFrameImpl",
    "func_80136A48": "LinkAnimation_OnFrame",
    "SkelAnime_InitSV": "SkelAnime_InitFlex",
    "func_80136C84": "SkelAnime_SetUpdate",
    "SkelAnime_FrameUpdateMatrix": "SkelAnime_Update",
    "func_80136CF4": "SkelAnime_Morph",
    "func_80136D98": "SkelAnime_MorphTaper",
    "func_80136F04": "SkelAnime_AnimateFrame",
    "func_8013702C": "SkelAnime_LoopFull",
    "func_801370B0": "SkelAnime_LoopPartial",
    "func_8013713C": "SkelAnime_Once",
    "SkelAnime_ChangeAnimImpl": "Animation_ChangeImpl",
    "SkelAnime_ChangeAnim": "Animation_Change",
    "SkelAnime_ChangeAnimDefaultStop": "Animation_PlayOnce",
    "SkelAnime_ChangeAnimTransitionStop": "Animation_MorphToPlayOnce",
    "SkelAnime_ChangeAnimPlaybackStop": "Animation_PlayOnceSetSpeed",
    "SkelAnime_ChangeAnimDefaultRepeat": "Animation_PlayLoop",
    "SkelAnime_ChangeAnimTransitionRepeat": "Animation_MorphToLoop",
    "SkelAnime_ChangeAnimPlaybackRepeat": "Animation_PlayLoopSetSpeed",
    "SkelAnime_AnimSetStop": "Animation_EndLoop",
    "SkelAnime_AnimReverse": "Animation_Reverse",
    "func_80137674": "SkelAnime_CopyFrameTableTrue",
    "func_801376DC": "SkelAnime_CopyFrameTableFalse",
    "func_80137748": "SkelAnime_UpdateTranslation",
    "func_801378B8": "Animation_OnFrame",
    "SkelAnime_CopyVec3s": "SkelAnime_CopyFrameTable",
}

def replace_anim(file):
    with open(file,'r',encoding = 'utf-8') as infile:
        srcdata = infile.read()

    funcs = list(animdict.keys())
    fixes = 0
    for func in funcs:
        newfunc = animdict.get(func)
        if(newfunc is None):
            print("How did this happen?")
            return -1
        if(func in srcdata):
            fixes += 1
            print(func)
            srcdata = srcdata.replace(func, newfunc)

    if(fixes > 0):
        print('Changed', fixes,'entr' + ('y' if fixes == 1 else 'ies') + ' in',file)
        with open(file, 'w', encoding = 'utf-8', newline = '\n') as outfile:
            outfile.write(srcdata)
    return 1

def replace_anim_all(repo):
    for subdir, dirs, files in os.walk(repo + os.sep + 'src'):
        for filename in files:
            if(filename.endswith('.c')):
                file = subdir + os.sep + filename
                replace_anim(file)
    for subdir, dirs, files in os.walk(repo + os.sep + 'asm' + os.sep + 'non_matchings'):
        for filename in files:
            if(filename.endswith('.s')):
                file = subdir + os.sep + filename
                replace_anim(file)
    return 1

parser = argparse.ArgumentParser(description='Update to the new animation names')
parser.add_argument('file', help="source file to be processed. use . to process the whole repo", default = None)

if __name__ == "__main__":
    args = parser.parse_args()
    if(args.file == '.'):
        replace_anim_all(os.curdir)
    else:
        replace_anim(args.file)
