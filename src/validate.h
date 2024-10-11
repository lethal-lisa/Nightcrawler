// Nightcrawler - NSC Validation Module Header

#ifndef VALIDATE_H_
#define VALIDATE_H_ 1

#include <stdbool.h>

#include "story.h"

// Routines for validating story files.

// Validate NST type node.
bool validNst (const storyFileHdr *pStory);

// Validate NSC type node.
bool validNsc (const sceneNodeHdr *pScene);

// Validate MOV type node.
bool validMove (const scene_MoveCluster *pMove);

// Validate LOK type node.
bool validLook (const scene_LookCluster *pLook);

// Validate TLK type node.
bool validTalk (const scene_TalkCluster *pTalk);

// Validate DIA type node.
bool validDia (const talk_DiaNode *pDia);

// Validate DOL type node.
bool validDol (const dia_OptList *pDol);

// Validate OPT type node.
bool validOpt (const dia_OptNode *pOpt);

// Validate USE type node.
bool validUse (const scene_UseCluster *pUse);

// Validate WIN type node.
bool validWin (const winNodeHdr *pWin);

// Validate DTH type node.
bool validDth (const dthNodeHdr *pDth);

#endif /* VALIDATE_H_ */