#include "led-matrix.h"
#include "singleton.h"

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

enum class Item {
  None = 0,
  Wall = 1,
  Food = 2
};

class Snake : public FactorySingleton<Snake> {
public:
  Snake();
  ~Snake();

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

