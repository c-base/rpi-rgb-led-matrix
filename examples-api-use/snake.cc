// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Snake game (work in progress)
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)
// Author: coon@c-base.org
// Last Maintenance: 28. dec 2017

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include "snake.h"

Snake::Snake() {
  interruptReceived_ = false;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, interruptHandler);
  signal(SIGINT,  interruptHandler);
}

Snake::~Snake() {
  pCanvas_->Clear();

  if(pCanvas_)
    delete pCanvas_;
}

void Snake::init(int argc, char* pArgv[]) {
  initMaze(argc, pArgv);
  generateMaze();
  draw();
}

int Snake::run() { 
  while(true) {
    if (interruptReceived_)
     return 0;

    tick();
    usleep(1000 * 1000);
  }

  return 0;
}

void Snake::initMaze(int argc, char* pArgv[]) {
 for(int x = 0; x < 128; x++)
   for(int y = 0; y < 32; y++)
     pField_[x][y] = Item::None;

  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.chain_length = 4;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  pCanvas_ = rgb_matrix::CreateMatrixFromFlags(&argc, &pArgv, &defaults);

  if (pCanvas_ == NULL) { 
    printf("Error: failed creating canvas!");
    exit(1);
  }
}

void Snake::generateMaze() {
  for(int x = 0; x < 128; x++) {
    pField_[x][0]  = Item::Wall; 
    pField_[x][31] = Item::Wall; 
  }   

  for(int y = 0; y < 32; y++) {
    pField_[0][y]   = Item::Wall;
    pField_[127][y] = Item::Wall;
  }    
}

void Snake::draw() {
  for(int x=0; x<128; x++) {
    for(int y=0; y<32; y++) {      
      switch(pField_[x][y]) {
        case Item::None:
          pCanvas_->SetPixel(x, y, 0, 0, 0);
          break;

        case Item::Wall:
          pCanvas_->SetPixel(x, y, 255, 255, 255);
          break;

        case Item::Food:
          pCanvas_->SetPixel(x, y, 0, 255, 0);
          break;
      }
    }
  }
}

void Snake::tick() {
  printf("tick\n");

 
}

void Snake::interruptReceived() {
  interruptReceived_ = true;
}

void Snake::interruptHandler(int sigNo) {
  // TODO: don't ignore sigNo

  Snake::_instance()->interruptReceived();
}

int main(int argc, char* pArgv[]) { 
  Snake::_create();
  Snake::_instance()->init(argc, pArgv);

  return Snake::_instance()->run();
}

