#include "Wind.h"
#include "Core.h"
#include "Player.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

Wind::Wind(int iXPos, int iYPos, int iMaxXPos) {
    this->fXPos = (float)iXPos + 16;
    this->fYPos = (float)iYPos;

    this->iMaxXPos = iMaxXPos;
    this->followPlayer = false;
    this->end = false;

    this->iBlockID = 66;

    this->collisionOnlyWithPlayer = true;
    this->iHitBoxX = 32;
    this->iHitBoxY = 26;

    this->moveDirection = true;

    this->nextWindFrameID = 128;

    this->minionSpawned = true;

    srand((unsigned)time(NULL));
}

Wind::~Wind(void) {

}

/* ******************************************** */

void Wind::Update() {
    if(minionState == -2) {
        Minion::minionDeathAnimation();
    } else {
        if(fXPos > iMaxXPos) {
            end = true;
        }

        if(end) {
            fXPos -= 1;

            if(fXPos < -iHitBoxX) {
                minionState = -1;
            }

            return;
        }

        if(fXPos < -CCore::getMap()->getXPos() - 64) {
            fXPos = -CCore::getMap()->getXPos() - 32;
        }

        if(fXPos <= CCore::getMap()->getPlayer()->getXPos() - CCore::getMap()->getXPos() + CCore::getMap()->getPlayer()->getHitBoxX()/2 + 32*CCore::getMap()->getPlayer()->getMoveSpeed() && CCore::getMap()->getPlayer()->getMove() && CCore::getMap()->getPlayer()->getMoveDirection()) {
            moveSpeed = CCore::getMap()->getPlayer()->getMoveSpeed() + (fXPos < CCore::getMap()->getPlayer()->getXPos() - CCore::getMap()->getXPos() + CCore::getMap()->getPlayer()->getHitBoxX()/2 + 32*CCore::getMap()->getPlayer()->getMoveSpeed());
            fXPos += moveSpeed;
            followPlayer = true;
            moveDirection = true;
        } else {
            if(followPlayer) {
                if(moveSpeed > 0) {
                    fXPos += moveSpeed;
                    --moveSpeed;
                } else {
                    followPlayer = false;
                }
            } else {
                if(!moveDirection) {
                    fXPos -= 1;

                    if(fXPos < CCore::getMap()->getPlayer()->getXPos() - CCore::getMap()->getXPos() + CCore::getMap()->getPlayer()->getHitBoxX()/2 - 128) {
                        moveDirection = true;
                    }
                } else {
                    fXPos += 1;
                    if(fXPos > CCore::getMap()->getPlayer()->getXPos() - CCore::getMap()->getXPos() + CCore::getMap()->getPlayer()->getHitBoxX()/2 + 128) {
                        moveDirection = false;
                    }
                }
            }
        }

        if(nextWindFrameID < rand()%85) {
            iBlockID = 67;
        }

        if(nextWindFrameID<= 0) {
            //the minion blast a wind and mario must get slowed - changing physics
            CCore::getMap()->getPlayer()->setCurrentJumpSpeed(0.650f);   //7,65 normal, 0,10 ninja
            CCore::getMap()->getPlayer()->setCurrentFallingSpeed(0.65f);
            CCore::getMap()->getPlayer()->setCurrentJumpDistance(0.1f);
            //CCore::getMap()->getPlayer()->setjumpState(0);
            nextWindFrameID = 35 + rand()%175;
            iBlockID = 66;

        } else {
            --nextWindFrameID;
        }
    }
}

void Wind::Draw(SDL_Renderer* rR, CIMG* iIMG) {
    if(minionState != -2) {
        iIMG->Draw(rR, (int)fXPos + (int)CCore::getMap()->getXPos(), (int)fYPos - 16, !moveDirection);
    } else {
        iIMG->DrawVert(rR, (int)fXPos + (int)CCore::getMap()->getXPos(), (int)fYPos - 16);
    }
}

void Wind::minionPhysics() { }

/* ******************************************** */

void Wind::collisionWithPlayer(bool TOP) {
    if(CCore::getMap()->getPlayer()->getStarEffect() || TOP) {
        setMinionState(-2);
    } else if(!end) {
        CCore::getMap()->playerDeath(true, false);
    }
}