#ifndef _SNAKE_H
#define _SNAKE_H

#include "led-matrix.h"
#include "singleton.h"

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

enum class Item {
  None,
  Wall,
  Food
};

//-----------------------------------------------------
// Entity
//-----------------------------------------------------

class Entity {
public:
  Entity(const Item& type) : type_(type) {}
  ~Entity() {}

  void render() {
  
  }

private:
  Item pField_[128][32];
  const Item type_;
};

//-----------------------------------------------------
// Maze
//-----------------------------------------------------

class Maze : public Entity {
public:
  Maze() : Entity(Item::Wall) {}

private:
};

//-----------------------------------------------------
// Food
//-----------------------------------------------------

class Food : public Entity {
public:
  Food() : Entity(Item::Food) {}

private:
};

//-----------------------------------------------------
// LedMatrixApp
//-----------------------------------------------------

class LedMatrixApp : public FactorySingleton<LedMatrixApp> {
public:
  LedMatrixApp();
  ~LedMatrixApp();

  void init(int argc, char* pArgv[]);
  int run();

private:
  void tick();
  void initMaze(int argc, char* pArgv[]);
  void generateMaze();
  void draw();
  void interruptReceived();
  static void interruptHandler(int sigNo);

  volatile bool interruptReceived_;
  Item pField_[128][32]; // 128x32
  Canvas* pCanvas_{NULL};
};

#endif // !_SNAKE_H

