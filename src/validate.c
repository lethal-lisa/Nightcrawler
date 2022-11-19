
#include <stdbool.h>

#include "validate.h"
#include "story.h"

bool validNst (const storyFileHdr *pStory) {
	if (pStory->uVersion > NST_HIGHEST_SUPPORTED_VER) return false;
	if (pStory->cItems > NST_MAX_ITEM_COUNT) return false;
	return true;
}

bool validNsc (const sceneNodeHdr *pScene) {
	if (pScene->uLookClustAddr == 0) return false;
	if (pScene->uMoveClustAddr == 0) return false;
	return true;
}

bool validMove (const scene_MoveCluster *pMove) {
	return true;
}

bool validLook (const scene_LookCluster *pLook) {
	if ((pLook->uAroundAddr == 0) || (pLook->uAltAroundAddr == 0)) return false;
	return true;
}

bool validTalk (const scene_TalkCluster *pTalk) {
	if (pTalk->uInitAddr == 0) return false;
	return true;
}

bool validDia (const talk_DiaNode *pDia) {
	if ((pDia->uTextAddr == 0) || (pDia->uAltTextAddr == 0)) return false;
	return true;
}

bool validDol (const dia_OptList *pDol) {
	return true;
}

bool validOpt (const dia_OptNode *pOpt) {
	if (pOpt->uTextAddr == 0) return false;
	return true;
}

bool validUse (const scene_UseCluster *pUse) {
	if (pUse->uStrAddr == 0) return false;
	return true;
}

bool validWin (const winNodeHdr *pWin) {
	if (pWin->uStrAddr == 0) return false;
	return true;
}

bool validDth (const dthNodeHdr *pDth) {
	if (pDth->uStrAddr == 0) return false;
	return true;
}
