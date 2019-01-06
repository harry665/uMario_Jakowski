#pragma once

#ifndef WIND_H
#define WIND_H

#include "Minion.h"

class Wind : public Minion
{
private:
    int iMaxXPos;
    bool end;

    bool followPlayer;

    int nextWindFrameID;


public:
    Wind(int iXPos, int iYPos, int iMaxXPos);
    ~Wind(void);

    void Update();
    void Draw(SDL_Renderer* rR, CIMG* iIMG);

    void minionPhysics();

    void collisionWithPlayer(bool TOP);
};

#endif
