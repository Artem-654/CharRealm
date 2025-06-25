#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "Game.h"
using namespace std;
vector<vector<vector<vector<MapBlock*>>>> GAME::MAP_BLOCKS;
vector<vector<Entity*>> GAME::MAP_BLOCKS_BUFFER;
int GAME::ScreenPosY;
int GAME::ScreenPosX;
int GAME::ScreenChunkPosY;
int GAME::ScreenChunkPosX;
bool GAME::UpChunkGenerated = false;
bool GAME::LeftChunkGenerated = false;
bool GAME::RightChunkGenerated = false;
bool GAME::DownChunkGenerated = false;
const chrono::milliseconds GAME::FrameDuration = chrono::milliseconds(1000 / TargetFPS);
int GAME::FrameCount = 0;
double GAME::FPS = 0;
Player* GAME::player = nullptr;
int Player::screenPosY, Player::screenPosX, Player::screenChunkPosY, Player::screenChunkPosX;

GAME::GAME()
{
    MAP_BLOCKS.resize(ChunkSizeY);
    for (int i = 0; i < ChunkSizeY; ++i)
    {
        MAP_BLOCKS[i].resize(ChunkSizeX);
    }
    MAP_BLOCKS_BUFFER.resize(RenderSize +4);
    for (int i = 0; i < RenderSize +4; ++i)
    {
        MAP_BLOCKS_BUFFER[i].resize(RenderSize + 4);
        for (int j = 0; j < RenderSize + 4; j++)
            MAP_BLOCKS_BUFFER[i][j] = nullptr;
    }

    std::srand(std::time(0));

    int chunkplayerX = rand() % ChunkSizeX;
    int chunkplayerY = rand() % ChunkSizeY;
    int playerX = rand() % GlobalSizeX;
    int playerY = rand() % GlobalSizeY;
    chunkplayerY = 1;
    chunkplayerX = 1;
    playerY = 0;
    playerX = 0;
    
    spawnNewPlayer(chunkplayerY, chunkplayerX, playerY, playerX);
}

GAME::~GAME()
{
    delete player;
    for (int iy = 0; iy < GlobalSizeY; iy++)
        for (int jx = 0; jx < GlobalSizeX; jx++)
            for (int i = 0; i < GlobalSizeY; i++)
                for (int j = 0; j < GlobalSizeX; j++)
                    delete MAP_BLOCKS[iy][jx][i][j];
}

void GAME::update()
{   
    int chunkindexY,chunkindexX,indexY, indexX;
    for (int i = 0, indexi = ScreenPosY - (RenderSize / 2); i < RenderSize; indexi++, i++)
    {
        for (int j = 0, indexj = ScreenPosX - (RenderSize / 2); j < RenderSize; indexj++, j++)
        {
            chunkindexY = 0, chunkindexX = 0, indexY = 0, indexX = 0;
            if (indexi < 0)
            {
                if (!ScreenChunkPosY)
                    continue;
                if (!UpChunkGenerated)
                {
                    if (ScreenChunkPosX != 0)
                    {
                        generateChunk(ScreenChunkPosY - 1, ScreenChunkPosX - 1);
                    }
                    if (ScreenChunkPosX != (ChunkSizeX - 1))
                    {
                        generateChunk(ScreenChunkPosY - 1, ScreenChunkPosX + 1);
                    }
                    generateChunk(ScreenChunkPosY - 1, ScreenChunkPosX);
                    UpChunkGenerated = true;
                }
                chunkindexY = -1, indexY = GlobalSizeY;
            }
            if (indexj < 0)
            {
                if (!ScreenChunkPosX)
                    continue;
                if (!LeftChunkGenerated)
                {
                    if (ScreenChunkPosY != 0)
                    {
                        generateChunk(ScreenChunkPosY - 1, ScreenChunkPosX - 1);
                    }
                    if (ScreenChunkPosY != (ChunkSizeX - 1))
                    {
                        generateChunk(ScreenChunkPosY + 1, ScreenChunkPosX - 1);
                    }
                    generateChunk(ScreenChunkPosY, ScreenChunkPosX - 1);
                    LeftChunkGenerated = true;
                }
                chunkindexX = -1, indexX = GlobalSizeX;
            }
            if (indexi >= GlobalSizeY)
            {
                if (ScreenChunkPosY == ChunkSizeY)
                    continue;
                if (!DownChunkGenerated)
                {
                    if (ScreenChunkPosX != 0)
                    {
                        generateChunk(ScreenChunkPosY + 1, ScreenChunkPosX - 1);
                    }
                    if (ScreenChunkPosX != (ChunkSizeX - 1))
                    {
                        generateChunk(ScreenChunkPosY + 1, ScreenChunkPosX + 1);
                    }
                    generateChunk(ScreenChunkPosY + 1, ScreenChunkPosX);
                    DownChunkGenerated = true;
                }
                chunkindexY = +1,  indexY = -GlobalSizeY;
            }
            if (indexj >= GlobalSizeX)
            {
                if (ScreenChunkPosX == ChunkSizeX)
                    continue;
                if (!RightChunkGenerated)
                {
                    if (ScreenChunkPosY != 0)
                    {
                        generateChunk(ScreenChunkPosY - 1, ScreenChunkPosX + 1);
                    }
                    if (ScreenChunkPosY != (ChunkSizeX - 1))
                    {
                        generateChunk(ScreenChunkPosY + 1, ScreenChunkPosX + 1);
                    }
                    generateChunk(ScreenChunkPosY, ScreenChunkPosX + 1);
                    RightChunkGenerated = true;
                }
                 chunkindexX = +1, indexX = -GlobalSizeX;
            }
            // if (((indexi + indexY) < 0) || ((indexj + indexX) < 0) || ((indexi + indexY) < 0) >= GlobalSizeY || ((indexj + indexX) < 0) >= GlobalSizeX)
            //     continue;
            if ((indexi + indexY) < 0 || (indexi + indexY) >= GlobalSizeY || (indexj + indexX) < 0 || (indexj + indexX) >= GlobalSizeX)
                continue;

            int chunkY = ScreenChunkPosY + chunkindexY;
            int chunkX = ScreenChunkPosX + chunkindexX;
            int localY = indexi + indexY;
            int localX = indexj + indexX;

            if (chunkY < 0 || chunkY >= ChunkSizeY ||
                chunkX < 0 || chunkX >= ChunkSizeX ||
                localY < 0 || localY >= GlobalSizeY ||
                localX < 0 || localX >= GlobalSizeX)
                continue;

            if (MAP_BLOCKS[ScreenChunkPosY + chunkindexY][ScreenChunkPosX + chunkindexX][indexi + indexY][indexj + indexX] == nullptr)
            {
                if ((rand() % 100) == 1)
                {
                    MAP_BLOCKS[ScreenChunkPosY + chunkindexY][ScreenChunkPosX + chunkindexX][indexi + indexY][indexj + indexX] = new StoneWall(ScreenChunkPosY + chunkindexY,ScreenChunkPosX + chunkindexX,indexi + indexY,indexj + indexX);
                }
                else
                {
                    MAP_BLOCKS[ScreenChunkPosY + chunkindexY][ScreenChunkPosX + chunkindexX][indexi + indexY][indexj + indexX] = new MapBlock(ScreenChunkPosY + chunkindexY, ScreenChunkPosX + chunkindexX, indexi + indexY, indexj + indexX);
                    if(rand()% 100 == 1)
                       setEntity(ScreenChunkPosY + chunkindexY, ScreenChunkPosX + chunkindexX, indexi + indexY, indexj + indexX, new Mob(ScreenChunkPosY + chunkindexY, ScreenChunkPosX + chunkindexX, indexi + indexY, indexj + indexX));
                    else
                        if(rand() % 1000 == 1)
                            setEntity(ScreenChunkPosY + chunkindexY, ScreenChunkPosX + chunkindexX, indexi + indexY, indexj + indexX, new AngryMob(ScreenChunkPosY + chunkindexY, ScreenChunkPosX + chunkindexX, indexi + indexY, indexj + indexX));
                }
            }
            MAP_BLOCKS[ScreenChunkPosY + chunkindexY][ScreenChunkPosX + chunkindexX][indexi + indexY][indexj + indexX]->update();
        }
    }
}

void GAME::showScreen(ScreenHandler& screenHandler)
{
    int chunkindexY, chunkindexX, indexY, indexX;
    for (int i = 0, indexi = ScreenPosY - (ScrenSizeY / 2); i < ScrenSizeY; indexi++, i++)
    {
        for (int j = 0, indexj = ScreenPosX - (ScrenSizeX / 2); j < ScrenSizeX; indexj++, j++)
        {
            chunkindexY = 0, chunkindexX = 0, indexY = 0, indexX = 0;
            if (indexi < 0)
            {
                if (ScreenChunkPosY <= 0)
                {
                    setDefaultValues(i,j, screenHandler);
                    continue;
                }

                chunkindexY = -1, indexY = GlobalSizeY;
            }
            if (indexj < 0)
            {
                if (ScreenChunkPosX <= 0)
                {
                    setDefaultValues(i, j, screenHandler);
                    continue;
                }
                chunkindexX = -1, indexX = GlobalSizeX;
            }
            if (indexi >= GlobalSizeY)
            {
                if (ScreenChunkPosY >= ChunkSizeY - 1)
                {
                    setDefaultValues(i, j, screenHandler);
                    continue;
                }
                chunkindexY = +1, indexY = -GlobalSizeY;
            }
            if (indexj >= GlobalSizeX)
            {
                if (ScreenChunkPosX >= ChunkSizeX - 1)
                {
                    setDefaultValues(i, j, screenHandler);
                    continue;
                }
                chunkindexX = +1, indexX = -GlobalSizeX;
            }
            MapBlock* currentBlock = MAP_BLOCKS[ScreenChunkPosY + chunkindexY][ScreenChunkPosX + chunkindexX][indexi + indexY][indexj + indexX];
            if(currentBlock == nullptr)
            {
                    setDefaultValues(i, j, screenHandler);
                    continue;
            }
            screenHandler.putPixel(j, i, Pixel(currentBlock->getModel(), currentBlock->getfg(), currentBlock->getbg()));
        }
    }
    screenHandler.putPixel(0, ScrenSizeY, Pixel(ScreenPosY));
    screenHandler.putPixel(2, ScrenSizeY, Pixel(ScreenPosX));
    screenHandler.putPixel(4, ScrenSizeY, Pixel(ScreenChunkPosY));
    screenHandler.putPixel(6, ScrenSizeY, Pixel(ScreenChunkPosX));
}


void GAME::setEntity(int chunkY, int chunkX, int Y, int X, Entity* Entity)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->setEntity(Entity);
}
void GAME::setEntityHealth(int chunkY, int chunkX, int Y, int X, int damage)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->setEntityHealth(damage);
}
void GAME::setEntityInBuffer(int chunkY, int chunkX, int Y, int X, Entity* Entity)
{
    if (!Entity) return;

    int chunkindexY = 0, chunkindexX = 0;
    if (chunkY < ScreenChunkPosY)
        chunkindexY = GlobalSizeY;
    if (chunkX < ScreenChunkPosX)
        chunkindexX = GlobalSizeX;
    if (chunkY > ScreenChunkPosY)
        chunkindexY = -GlobalSizeY;
    if (chunkX > ScreenChunkPosX)
        chunkindexX = -GlobalSizeX;
    int bufferY = (Y - ScreenPosY) + ((RenderSize+2) / 2) - chunkindexY, bufferX = (X - ScreenPosX) + ((RenderSize+2) / 2) - chunkindexX;
    MAP_BLOCKS_BUFFER[bufferY][bufferX] = Entity;
    Entity->setChunkPosY(chunkY);
    Entity->setChunkPosX(chunkX);
    Entity->setPosY(Y);
    Entity->setPosX(X);
}

void GAME::setScreenPosX(int X)
{
    ScreenPosX = X;
}

void GAME::setScreenPosY(int Y)
{
    ScreenPosY = Y;
}

void GAME::setScreenChunkPosX(int chunkPosX)
{
    if (ScreenChunkPosX != chunkPosX)
    {
        int tempChunkpos = ScreenChunkPosX;
        ScreenChunkPosX = chunkPosX;
        if (tempChunkpos < chunkPosX)
        {
            RightChunkGenerated = false;
            return;
        }
        if (tempChunkpos > chunkPosX)
        {
            LeftChunkGenerated = false;
        }
    }
}

void GAME::setScreenChunkPosY(int chunkPosY)
{
    if (ScreenChunkPosY != chunkPosY)
    {
        int tempChunkpos = ScreenChunkPosY;
        ScreenChunkPosY = chunkPosY;
        if (tempChunkpos < chunkPosY)
        {
            DownChunkGenerated = false;
            return;
        }
        if (tempChunkpos > chunkPosY)
            UpChunkGenerated = false;
    }
}

void GAME::setFromPlayerScreenPos()
{
    GAME::setScreenPosY(Player::getPlrScrPosY());
    GAME::setScreenPosX(Player::getPlrScrPosX());
    GAME::setScreenChunkPosY(Player::getPlrScrChunkPosY());
    GAME::setScreenChunkPosX(Player::getPlrScrChunkPosX());
}

void GAME::setDefaultValues(int i, int j, ScreenHandler& screenHandler) {
    screenHandler.putPixel(j,i, Pixel(L"--", Pixel::RGB{0,0,0}, Pixel::RGB{0,0,0}));
}

void GAME::setFromBufferToMap()
{
    int chunkindexY, chunkindexX, indexY, indexX;
    for (int i = 0 ; i < RenderSize+2; i++)
    {
        for (int j = 0; j < RenderSize+2;j++)
        {
            if (MAP_BLOCKS_BUFFER[i][j] != nullptr)
            {
                MAP_BLOCKS[MAP_BLOCKS_BUFFER[i][j]->getChunkPosY()][MAP_BLOCKS_BUFFER[i][j]->getChunkPosX()][MAP_BLOCKS_BUFFER[i][j]->getPosY()][MAP_BLOCKS_BUFFER[i][j]->getPosX()]->setEntity(MAP_BLOCKS_BUFFER[i][j]);
                MAP_BLOCKS_BUFFER[i][j] = nullptr;
            }
        }
    }
}

void GAME::generateChunk(int ChunkY, int ChunkX)
{
    if (ChunkY < 0 || ChunkY >= ChunkSizeY || ChunkX < 0 || ChunkX >= ChunkSizeX)
        return;
    MAP_BLOCKS[ChunkY][ChunkX].resize(GlobalSizeY);
    for (int i = 0; i < GlobalSizeY; i++)
        MAP_BLOCKS[ChunkY][ChunkX][i].resize(GlobalSizeX);
}

int GAME::getRenderSize()
{
    return RenderSize;
}

int GAME::getGlobalSizeY()
{
    return GlobalSizeY;
}

int GAME::getGlobalSizeX()
{
    return GlobalSizeX;
}

int GAME::getChunkSizeY()
{
    return ChunkSizeY;
}

int GAME::getChunkSizeX()
{
    return ChunkSizeX;
}

Entity* GAME::getEntityPtr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X]->getEntityPtr();
}
MapBlock* GAME::getMapBlockPtr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X];
}

bool GAME::checkEntityInBuffer(int chunkY, int chunkX, int Y, int X)
{
    int chunkindexY = 0, chunkindexX = 0;
    if (chunkY < ScreenChunkPosY)
        chunkindexY = GlobalSizeY;
    if (chunkX < ScreenChunkPosX)
        chunkindexX = GlobalSizeX;
    if (chunkY > ScreenChunkPosY)
        chunkindexY = -GlobalSizeY;
    if (chunkX > ScreenChunkPosX)
        chunkindexX = -GlobalSizeX;
    int bufferY = (Y - ScreenPosY) + ((RenderSize + 2) / 2) - chunkindexY, bufferX = (X - ScreenPosX) + ((RenderSize + 2) / 2) - chunkindexX;
    return MAP_BLOCKS_BUFFER[bufferY][bufferX] != nullptr;
}

bool GAME::checkBlockPtr(int chunkY, int chunkX, int Y, int X)
{
    if (GAME::getMapBlockPtr(chunkY, chunkX, Y, X) == nullptr)
        return true;
    if (!GAME::getMapBlockPtr(chunkY, chunkX, Y, X)->isEmpty())
        return true;
    if (!GAME::getMapBlockPtr(chunkY, chunkX, Y, X)->getCanWalkThêough())
        return true;
    if (GAME::checkEntityInBuffer(chunkY, chunkX, Y, X))
        return true;
    return false;
}

bool GAME::isPlayer(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X]->isPlayer();
}

int GAME::checkAttackBlockPtr(int chunkY, int chunkX, int Y, int X)
{
    if (GAME::getMapBlockPtr(chunkY, chunkX, Y, X) == nullptr)
        return 0;
    if (GAME::getMapBlockPtr(chunkY, chunkX, Y, X)->isEmpty())
        return 0;
    /*if (!GAME::isPlayer(chunkY, chunkX, Y, X))
        return 0;*/
    if (GAME::checkEntityInBuffer(chunkY, chunkX, Y, X))
        return 0;
    return 1;
}

void GAME::spawnNewPlayer()
{
    int chunkY, chunkX, Y, X;

    do{
        chunkX = rand() % ChunkSizeX;
        chunkY = rand() % ChunkSizeY;
        X = rand() % GlobalSizeX;
        Y = rand() % GlobalSizeY;
    }while (checkEntity(chunkY, chunkX, Y, X));
    
    if(player == nullptr)
        player = new Player(chunkY, chunkX, Y, X);
    else
    {
        clearEntityAtBlock(player);
        player->reset(chunkY, chunkX, Y, X);
    }
    genChunksAfterSpawn(chunkY, chunkX, Y, X);
    setEntity(chunkY, chunkX, Y, X, player);
    setFromPlayerScreenPos();
}
void GAME::spawnNewPlayer(int chunkY, int chunkX, int Y, int X)
{
    player = new Player(chunkY, chunkX, Y, X);
    genChunksAfterSpawn(chunkY, chunkX, Y, X);
    setEntity(chunkY, chunkX, Y, X, player);
    setFromPlayerScreenPos();
}

void GAME::countFPS(chrono::steady_clock::time_point&startTime, chrono::steady_clock::time_point&frameStart)
{
    FrameCount++;
    chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();
    chrono::duration<double> frameDuration = frameEnd - frameStart;
    frameStart = frameEnd;
    if (chrono::duration_cast<chrono::seconds>(frameEnd - startTime).count() >= 1) {
        FPS = FrameCount / std::chrono::duration<double>(frameEnd - startTime).count();
        FrameCount = 0;
        startTime = frameEnd;
    }
    //this_thread::sleep_for(FrameDuration - frameDuration);
}

Player* GAME::getPLayerPtr()
{
    return player;
}

void GAME::genChunksAfterSpawn(int chunkY, int chunkX, int Y, int X)
{
    for (int i = -1;i<2;i++)
        for (int j = -1 ;j<2;j++)
        {
            if (((chunkY + i) == (ChunkSizeY)) || ((chunkY + i) == -1) || ((chunkX + j) == -1) || ((chunkX + j) == ChunkSizeX))
            {
                continue;
            }
            MAP_BLOCKS[chunkY + i][chunkX + j].resize(GlobalSizeY);
            for (int k = 0; k < GlobalSizeY; k++)
                MAP_BLOCKS[chunkY + i][chunkX + j][k].resize(GlobalSizeX);
        }
    MAP_BLOCKS[chunkY][chunkX][Y][X] = new MapBlock(chunkY, chunkX,Y, X);
}

int GAME::checkEntity(int chunkY, int chunkX, int Y, int X)
{
    if(MAP_BLOCKS[chunkY][chunkX][Y][X] == nullptr)
        return 0;
    else
        return !MAP_BLOCKS[chunkY][chunkX][Y][X]->isEmpty();;
}

void GAME::clearEntityAtBlock(Entity* entity)
{
    MAP_BLOCKS[entity->getChunkPosY()][entity->getChunkPosX()][entity->getPosY()][entity->getPosX()]->setEntity(nullptr);
}

//-----------------------------------------------------------------------

MapBlock::MapBlock(int chunkY, int chunkX, int Y,int X)
{
    posY = Y;
    posX = X;
    chunkPosY = chunkY;
    chunkPosX = chunkX;
    fg = {0,128,43};
    bg = {135,75,7};
    blockModel = L". ";
    canWalkThêough = true;
}

MapBlock::~MapBlock()
{
    delete entityPtr;
}

int MapBlock::getPosY() 
{
    return posY;
}

int MapBlock::getPosX() 
{
    return posX;
}

wstring MapBlock::getModel() const
{
    if (entityPtr != nullptr) {
        return entityPtr->getModel();
    }
    else {
        return blockModel;
    }
}

void MapBlock::setEntityHealth(int damage)
{
    entityPtr->setHealth(damage);
}

void MapBlock::update()
{
    if (entityPtr != nullptr)
    {
        if (entityPtr->update())
            return;
        entityPtr->action();
    }
}

bool MapBlock::isEmpty()
{
    return this->entityPtr == nullptr;
}

bool MapBlock::getCanWalkThêough()
{
    return canWalkThêough;
}

bool MapBlock::isPlayer()
{
    return entityPtr->getIsPlayer();
}

Pixel::RGB MapBlock::getfg()
{
    if (entityPtr == nullptr)
        return fg;
    else
        return entityPtr->getfg();
}

Pixel::RGB MapBlock::getbg()
{
    if (entityPtr == nullptr)
        return bg;
    else
        return entityPtr->getbg();
}

void MapBlock::setEntity(Entity* Entity)
{
    entityPtr = Entity;
}

void MapBlock::setPosEntity(int chunkY, int chunkX,int Y, int X)
{
    entityPtr->setChunkPosY(chunkY);
    entityPtr->setChunkPosX(chunkX);
    entityPtr->setPosY(Y);
    entityPtr->setPosX(X);
}

Entity* MapBlock::getEntityPtr()
{
    return entityPtr;
}

StoneWall::StoneWall(int chunkY, int chunkX, int Y, int X) : MapBlock( chunkY, chunkX, Y, X)
{
    blockModel = L"##";
    canWalkThêough = false;
    fg = {81,81,81};
    bg = {135,75,7};
}

Entity::Entity()
{
    fg = {255,255,255};
    bg = {135,75,7};
    health = 90;
    damage = 10;
    walkingDelay = 0;
    standartDelay = 10;
    animationCooldown = 10;
    chunkPosY = 0;
    chunkPosX = 0;
    posY = 0;
    posX = 0;
}

Entity::~Entity()
{
    GAME::setEntity(chunkPosY, chunkPosX, posY, posX, nullptr);
}

Entity::Entity(int chunkY, int chunkX, int Y, int X)
{
    fg = {255,255,255};
    bg = {135,75,7};
    health = 90;
    damage = 10;
    walkingDelay = 0;
    standartDelay = 100;
    animationCooldown = 10;
    chunkPosY = chunkY;
    chunkPosX = chunkX;
    posY = Y;
    posX = X;
}

int Entity::move(int result)
{
    int copyposY = posY, copyposX = posX, copychunkposY = chunkPosY, copychunkposX = chunkPosX,constposY = posY,constposX = posX,constchunkposY = chunkPosY,constchunkposX = chunkPosX,chunkindex = 0;
    switch (result)
    {
    case 1://w
        if (copyposY == 0)
        {
            if (chunkPosY == 0)
                return 0;
            copychunkposY--;
            copyposY = GAME::getGlobalSizeY();
        } 
        if (Player::getPlrScrChunkPosY() != chunkPosY)
            chunkindex = GAME::getGlobalSizeY()+1;
        if (Player::getPlrScrPosY() -(constposY - chunkindex) == GAME::getRenderSize() / 2)
            break;
        if (GAME::checkBlockPtr(copychunkposY, copychunkposX, copyposY - 1, copyposX))
            break;
        walkingDelay = standartDelay;
        GAME::setEntityInBuffer(copychunkposY, copychunkposX, copyposY - 1, copyposX, GAME::getEntityPtr(chunkPosY, chunkPosX, posY, posX));
        GAME::setEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 2://a
        if (turnedRight)
        {
            swap(entitysModel[0], entitysModel[1]);
            swap(constEntityModel[0], constEntityModel[1]);
            swap(attackEntityModel[0], attackEntityModel[1]);
            turnedRight = false;
        }
        if (copyposX == 0)
        {
            if (chunkPosX == 0)
                return 0;
            copychunkposX--;
            copyposX = GAME::getGlobalSizeX();
        }
        if (Player::getPlrScrChunkPosX() != chunkPosX)
            chunkindex = GAME::getGlobalSizeX()+1;
        if (Player::getPlrScrPosX() -(constposX - chunkindex) == GAME::getRenderSize() / 2)
            break;
        if (GAME::checkBlockPtr(copychunkposY, copychunkposX, copyposY, copyposX - 1))
            break;
        walkingDelay = standartDelay;
        GAME::setEntityInBuffer(copychunkposY, copychunkposX, copyposY, copyposX - 1, GAME::getEntityPtr(chunkPosY, chunkPosX, posY, posX));
        GAME::setEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 3://s
        if (copyposY == (GAME::getGlobalSizeY() - 1))
        {
            if (chunkPosY == GAME::getChunkSizeY() - 1)
                return 0;
            copychunkposY++;
            copyposY = -1;
        }
        if (Player::getPlrScrChunkPosY() != chunkPosY)
            chunkindex = GAME::getGlobalSizeY()+1;
        if (constposY - (Player::getPlrScrPosY() - chunkindex) == GAME::getRenderSize() / 2)
            break;
        if (GAME::checkBlockPtr(copychunkposY, copychunkposX, copyposY + 1, copyposX))
            break;
        walkingDelay = standartDelay;
        GAME::setEntityInBuffer(copychunkposY, copychunkposX, copyposY + 1, copyposX, GAME::getEntityPtr(chunkPosY, chunkPosX, posY, posX));
        GAME::setEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 4://d
        if (!turnedRight)
        {
            swap(entitysModel[0], entitysModel[1]);
            swap(constEntityModel[0], constEntityModel[1]);
            swap(attackEntityModel[0], attackEntityModel[1]);
            turnedRight = true;
        }
        if (copyposX == (GAME::getGlobalSizeX() - 1))
        {
            if (chunkPosX == GAME::getChunkSizeX() - 1)
                return 0;
            copychunkposX++;
            copyposX = -1;
        }
        if (Player::getPlrScrChunkPosX() != chunkPosX)
            chunkindex = GAME::getGlobalSizeX() + 1;
        if (constposX - (Player::getPlrScrPosX() - chunkindex) == GAME::getRenderSize() / 2)
            break;
        if (GAME::checkBlockPtr(copychunkposY, copychunkposX, copyposY, copyposX + 1))
            break;
        walkingDelay = standartDelay;
        GAME::setEntityInBuffer(copychunkposY, copychunkposX, copyposY, copyposX + 1, GAME::getEntityPtr(chunkPosY, chunkPosX, posY, posX));
        GAME::setEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 5:
        if (copyposY == 0)
        {
            if (chunkPosY != 0)
            {
                copychunkposY--;
                copyposY = GAME::getGlobalSizeY();
                punch(copychunkposY, copychunkposX, copyposY - 1, copyposX, damage);
                copychunkposY = constchunkposY;
                copyposY = constposY;
            }
        }
        else
        {
            punch(copychunkposY, copychunkposX, copyposY - 1, copyposX, damage);
        }
        if (copyposX == 0)
        {
            if (chunkPosX != 0)
            {
                copychunkposX--;
                copyposX = GAME::getGlobalSizeX();
                punch(copychunkposY, copychunkposX, copyposY, copyposX - 1, damage);
                copychunkposX = constchunkposX;
                copyposX = constposX;
            }
        }
        else
        {
            punch(copychunkposY, copychunkposX, copyposY, copyposX - 1, damage);
        }
        if (copyposY == (GAME::getGlobalSizeY() - 1))
        {
            if (chunkPosY != GAME::getChunkSizeY() - 1)
            {
                copychunkposY++;
                copyposY = -1;
                punch(copychunkposY, copychunkposX, copyposY + 1, copyposX, damage);
                copychunkposY = constchunkposY;
                copyposY = constposY;
            }
        }
        else
        {
            punch(copychunkposY, copychunkposX, copyposY + 1, copyposX, damage);
        }
        if (copyposX == (GAME::getGlobalSizeX() - 1))
        {
            if (chunkPosX != GAME::getChunkSizeX() - 1)
            {
                copychunkposX++;
                copyposX = -1;
                punch(copychunkposY, copychunkposX, copyposY, copyposX + 1, damage);
                copychunkposX = constchunkposX;
                copyposX = constposX;
            }
        }
        else
        {
            punch(copychunkposY, copychunkposX, copyposY, copyposX + 1, damage);
        }
        animationCooldown = 10;
        walkingDelay = standartDelay;
        return 1;
    }

    return 0;
}

bool Entity::update()
{
    if (animationCooldown == 0)
        entitysModel = constEntityModel;
    else
        animationCooldown--;
    if(walkingDelay)
        walkingDelay--;
    return false;
}

void Entity::action()
{
    if(!walkingDelay&&(!(rand()%10)))
        if (move(pathFinding()))//pathFinding()
        {
            entitysModel = attackEntityModel;
        }
}

wstring Entity::getModel()
{
    return entitysModel;
}

void Entity::setPosY(int Y)
{
    posY = Y;
}

void Entity::setPosX(int X)
{
    posX = X;
}

int Entity::getPosY()
{
    return posY;
}

int Entity::getPosX()
{
    return posX;
}

void Entity::setChunkPosY(int Y)
{
    chunkPosY = Y;
}

void Entity::setChunkPosX(int X)
{
    chunkPosX = X;
}

void Entity::setHealth(int damage)
{
    health -= damage;
    if (health <= 0)
        killThisEntity();
}

void Entity::punch(int chunkY, int chunkX, int Y, int X, int damage)
{
    switch (GAME::checkAttackBlockPtr(chunkY, chunkX, Y, X))
    {
    case 0:
        break;
    case 1:
        GAME::setEntityHealth(chunkY, chunkX, Y, X, damage);
        break;
    }

}

void Entity::killThisEntity()
{
    GAME::setEntity(chunkPosY, chunkPosX, posY, posX, nullptr);
    delete this;
}

int Entity::getChunkPosY()
{
    return chunkPosY;
}

int Entity::getChunkPosX()
{
    return chunkPosX;
}

Pixel::RGB Entity::getfg()
{
    return fg;
}
Pixel::RGB Entity::getbg()
{
    return bg;
}

bool Entity::getIsPlayer()
{
    return isPlayer;
}

int Entity::pathFinding()
{
    return (rand() % 5) + 1;
}

Player::Player()
{
    fg = {255,170,0};
    bg = {135,75,7};
    damage = 50;
    health = 100;
    standartDelay = 0;
    isPlayer = true;
    entitysModel = L"@!";
    constEntityModel = L"@!";
    attackEntityModel = L"@-";
    screenPosY = 0;
    screenPosX = 0;
    screenChunkPosY = 0;
    screenChunkPosX = 0;
}

Player::Player(int chunkY, int chunkX, int Y,int X) : Entity( chunkY,  chunkX, Y, X)
{
    fg = {255,170,0};
    bg = {135,75,7};
    damage = 50;
    health = 100;
    standartDelay = 0;
    isPlayer = true;
    entitysModel = L"@!";
    constEntityModel = L"@!";
    attackEntityModel = L"@-";
    screenPosY = Y;
    screenPosX = X;
    screenChunkPosY = chunkY;
    screenChunkPosX = chunkX;
}

void Player::killThisEntity()
{
    GAME::spawnNewPlayer();
}

//void Player::update()
//{
//
//}

void Player::action()
{

}

void Player::action(wchar_t way)
{
    if (walkingDelay)
        return;
    switch (way)
    {
    case L'w':
        move(1);
        break;
    case L'a':
        move(2);
        if (turnedRight)
        {
            swap(entitysModel[0], entitysModel[1]);
            swap(constEntityModel[0], constEntityModel[1]);
            swap(attackEntityModel[0], attackEntityModel[1]);
            turnedRight = false;
        }
        break;
    case L's':
        move(3);
        break;
    case L'd':
        move(4);
        if (!turnedRight)
        {
            swap(entitysModel[0], entitysModel[1]);
            swap(constEntityModel[0], constEntityModel[1]);
            swap(attackEntityModel[0], attackEntityModel[1]);
            turnedRight = true;
        }
        break;
    case L'f':
        if (move(5))
        {
            entitysModel = attackEntityModel;
        }
        break;
    }
    screenPosY = posY;
    screenPosX = posX;
    screenChunkPosY = chunkPosY;
    screenChunkPosX = chunkPosX;
}

void Player::reset(int chunkY, int chunkX, int Y,int X)
{
    fg = {255,170,0};
    bg = {135,75,7};
    damage = 50;
    health = 100;
    standartDelay = 0;
    isPlayer = true;
    entitysModel = L"@!";
    constEntityModel = L"@!";
    attackEntityModel = L"@-";
    screenPosY = Y;
    screenPosX = X;
    screenChunkPosY = chunkY;
    screenChunkPosX = chunkX;
    chunkPosY = chunkY;
    chunkPosX = chunkX;
    posY = Y;
    posX = X;
}

int Player::getPlrScrPosY()
{
    return screenPosY;
}

int Player::getPlrScrPosX()
{
    return screenPosX;
}

int Player::getPlrScrChunkPosY()
{
    return screenChunkPosY;
}

int Player::getPlrScrChunkPosX()
{
    return screenChunkPosX;
}

//int Player::GetPlrDamage()
//{
//    return damage;
//}
//int Player::GetPlrHealth()
//{
//    return health;
//}

Mob::Mob(int chunkY, int chunkX, int Y, int X) : Entity( chunkY,  chunkX, Y,X)
{
    fg = {128,255,0};
    bg = {135,75,7};
    entitysModel = L"g ";
    constEntityModel = L"g ";
    attackEntityModel = L"g-";
}

AngryMob::AngryMob(int chunkY, int chunkX, int Y, int X) : Entity(chunkY, chunkX, Y, X)
{
    fg = {149,0,0};
    bg = {135,75,7};
    damage = 30;
    entitysModel = L"G ";
    constEntityModel = L"G ";
    attackEntityModel = L"G-";
}

int AngryMob::pathFinding()
{
    int chunkindexY = 0, chunkindexX = 0, distanceY,distanceX;
    if (Player::getPlrScrChunkPosY() != chunkPosY)
    {
        if (Player::getPlrScrChunkPosY() < chunkPosY)
        {
            chunkindexY = GAME::getGlobalSizeY();
        }
        else
        {
            chunkindexY = -GAME::getGlobalSizeY();
        }
    }
    if (Player::getPlrScrChunkPosX() != chunkPosX)
    {
        if (Player::getPlrScrChunkPosX() < chunkPosX)
        {
            chunkindexX = GAME::getGlobalSizeX();
        }
        else
        {
            chunkindexX = -GAME::getGlobalSizeX();
        }
    }
    distanceY = Player::getPlrScrPosY() - posY - chunkindexY;
    distanceX = Player::getPlrScrPosX() - posX - chunkindexX;
    if ((abs(distanceY) + abs(distanceX))<2)
        return 5;
    if (abs(distanceY) < abs(distanceX))
    {
        if (distanceX == abs(distanceX))
        {
            return 4;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (distanceY == abs(distanceY))
        {
            return 3;
        }
        else
        {
            return 1;
        }
    }
}