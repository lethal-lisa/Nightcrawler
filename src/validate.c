// Nightcrawler - NSC Validation Module

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
	if (pMove->uNorthAddr == 0 &&
		pMove->uSouthAddr == 0 &&
		pMove->uEastAddr == 0 &&
		pMove->uWestAddr == 0 &&
		pMove->uAltNorthAddr == 0 &&
		pMove->uAltSouthAddr == 0 &&
		pMove->uAltEastAddr == 0 &&
		pMove->uAltWestAddr == 0) return false;
	return true;
}

bool validLook (const scene_LookCluster *pLook) {
	if ((pLook->uAroundAddr == 0) && (pLook->uAltAroundAddr == 0)) return false;
	if (pLook->uNorthAddr == 0 &&
		pLook->uSouthAddr == 0 &&
		pLook->uEastAddr == 0 &&
		pLook->uWestAddr == 0 &&
		pLook->uAltNorthAddr == 0 &&
		pLook->uAltSouthAddr == 0 &&
		pLook->uAltEastAddr == 0 &&
		pLook->uAltWestAddr == 0) return false;
	return true;
}

bool validTalk (const scene_TalkCluster *pTalk) {
	if (pTalk->uInitAddr == 0) return false;
	return true;
}

bool validDia (const talk_DiaNode *pDia) {
	if ((pDia->uTextAddr == 0) && (pDia->uAltTextAddr == 0)) return false;
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
