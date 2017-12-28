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

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

Snake::Snake(int argc, char* pArgv[]) {
  initMaze(argc, pArgv);
  generateMaze();
  draw();
}

Snake::~Snake() {
  pCanvas_->Clear();

  if(pCanvas_)
    delete pCanvas_;
}

void Snake::run() { 
  while(true) {
    if (interrupt_received)
     return;

    usleep(1000 * 1000);
  }
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
  
}

int main(int argc, char* pArgv[]) {
  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  Snake snake(argc, pArgv);
  snake.run();

  return 0;
}

