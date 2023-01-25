// Nightcrawler - NSC Validation Module Header

#ifndef VALIDATE_H_
#define VALIDATE_H_ 1

#include <stdbool.h>

#include "story.h"

// Routines for validating story files.
bool validNst (const storyFileHdr *pStory);
bool validNsc (const sceneNodeHdr *pScene);
bool validMove (const scene_MoveCluster *pMove);
bool validLook (const scene_LookCluster *pLook);
bool validTalk (const scene_TalkCluster *pTalk);
bool validDia (const talk_DiaNode *pDia);
bool validDol (const dia_OptList *pDol);
bool validOpt (const dia_OptNode *pOpt);
bool validUse (const scene_UseCluster *pUse);
bool validWin (const winNodeHdr *pWin);
bool validDth (const dthNodeHdr *pDth);

#endif /* VALIDATE_H_ */
