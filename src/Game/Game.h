#pragma once
#include <vector>
#include <string>
#include "Pixel.h"
#include "ScreenHandler.h"
using namespace std;
class Entity;
class MapBlock;
class ScreenCell;
class Player;

class GAME
{
    static vector<vector<vector<vector<MapBlock*>>>> MAP_BLOCKS;
    static vector<vector<Entity*>> MAP_BLOCKS_BUFFER;
    static const int RenderSize = 40;
    static const int ChunkSizeX = 2;
    static const int ChunkSizeY = 2;
    static const int GlobalSizeX = 38;
    static const int GlobalSizeY = 38;
    static const int ScrenSizeX = 38;
    static const int ScrenSizeY = 38;
    static int ScreenPosX;
    static int ScreenPosY;
    static int ScreenChunkPosX;
    static int ScreenChunkPosY;
    static bool UpChunkGenerated;
    static bool LeftChunkGenerated;
    static bool RightChunkGenerated;
    static bool DownChunkGenerated;
    static const int TargetFPS = 60;
    static const chrono::milliseconds FrameDuration;
    static int FrameCount;
    static double FPS;
    static Player* player;

    static void genChunksAfterSpawn(int chunkY, int chunkX, int Y, int X);
    static int checkEntity(int chunkY, int chunkX, int Y, int X);
    static void clearEntityAtBlock(Entity*);
public:
    GAME();
    ~GAME();
    static void showScreen(ScreenHandler& screenHandler);
    static void update();
    static void setEntity(int chunkY, int chunkX, int Y, int X,Entity*);
    static void setEntityInBuffer(int chunkY, int chunkX, int Y, int X, Entity*);
    static void setScreenPosY(int Y);
    static void setScreenPosX(int X);
    static void setScreenChunkPosY(int chunkposY);
    static void setScreenChunkPosX(int chunkposX);
    static void setFromPlayerScreenPos();
    static void setDefaultValues(int i ,int j, ScreenHandler& screenHandler);
    static void setFromBufferToMap();
    static void setEntityHealth(int chunkY, int chunkX, int Y, int X,int damage);
    static void generateChunk(int ChunkY, int ChunkX);
    static int getRenderSize();
    static int getGlobalSizeY();
    static int getGlobalSizeX();
    static int getChunkSizeY();
    static int getChunkSizeX();
    static Entity* getEntityPtr(int chunkY, int chunkX, int Y, int X);
    static MapBlock* getMapBlockPtr(int chunkY, int chunkX, int Y, int X);
    static bool checkEntityInBuffer(int chunkY, int chunkX, int Y, int X);
    static bool checkBlockPtr(int chunkY, int chunkX, int Y, int X);
    static bool isPlayer(int chunkY, int chunkX, int Y, int X);
    static int checkAttackBlockPtr(int chunkY, int chunkX, int Y, int X);
    static void spawnNewPlayer();
    static void spawnNewPlayer(int chunkY, int chunkX, int Y, int X);
    static void countFPS(chrono::steady_clock::time_point& startTime, chrono::steady_clock::time_point& frameStart);
    static Player* getPLayerPtr();
};
class MapBlock
{
protected:
    wstring blockModel;
    Entity* entityPtr = nullptr;
    Pixel::RGB fg, bg;
    int chunkPosY,chunkPosX, posY, posX;
    bool transperent, roof, canWalkThêough;
public:
    MapBlock(int chunkY, int chunkX, int Y, int X);
    ~MapBlock();
    void update();
    //void UpdateEntity_model();
    int getPosY();
    int getPosX();
    wstring getModel() const;
    void setEntity(Entity*);
    void setPosEntity(int chunkY, int chunkX, int Y,int X);
    void setEntityHealth(int damage);
    bool isEmpty();
    bool getCanWalkThêough();
    bool isPlayer();
    //int Move(int result);
    Pixel::RGB getfg();
    Pixel::RGB getbg();
    Entity* getEntityPtr();
};

class StoneWall : public MapBlock
{
public:
    StoneWall(int chunkY, int chunkX, int Y, int X);
};

class Entity
{
protected:
    bool turnedRight = true,isPlayer = false;
    int chunkPosY, chunkPosX, posY, posX,damage,health,animationCooldown,walkingDelay,standartDelay;
    wstring entitysModel, constEntityModel, attackEntityModel;
    Pixel::RGB fg, bg;
public:
    Entity();
    ~Entity();
    Entity(int chunkY, int chunkX ,int Y,int X);
    virtual int move(int result);
    bool update();
    virtual void action();
    wstring getModel();
    void setPosY(int Y);
    void setPosX(int X);
    int getPosY();
    int getPosX();
    void setChunkPosY(int Y);
    void setChunkPosX(int X);
    void setHealth(int damage);
    static void punch(int chunkY, int chunkX, int Y, int X,int damage);
    virtual void killThisEntity();
    int getChunkPosY();
    int getChunkPosX();
    Pixel::RGB getfg();
    Pixel::RGB getbg();
    bool getIsPlayer();
    virtual int pathFinding();
};
class Player : public Entity
{
    static int screenPosY, screenPosX, screenChunkPosY, screenChunkPosX;
public:
    Player();
    Player(int chunkY, int chunkX, int Y, int X);
    //void Update() override;
    void killThisEntity() override;
    void action();
    void action(wchar_t way);
    void reset(int chunkY, int chunkX, int Y,int X);
    static int getPlrScrPosY();
    static int getPlrScrPosX();
    static int getPlrScrChunkPosY();
    static int getPlrScrChunkPosX();
    //static int GetPlrDamage();
    //static int GetPlrHealth();
};
class Mob : public Entity
{
public:
    Mob(int chunkY, int chunkX, int Y,int X);
};
class AngryMob : public Entity
{
public:
    AngryMob(int chunkY, int chunkX, int Y, int X);
    int pathFinding() override;
};