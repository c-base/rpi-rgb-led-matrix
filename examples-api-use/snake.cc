// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// LedMatrixApp game (work in progress)
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

//-----------------------------------------------------
// LedMatrixApp
//-----------------------------------------------------

LedMatrixApp::LedMatrixApp() {
  // Clean exit when user hits CTRL-C:
  interruptReceived_ = false;

  signal(SIGTERM, interruptHandler);
  signal(SIGINT,  interruptHandler);
}

LedMatrixApp::~LedMatrixApp() {
  pCanvas_->Clear();

  if(pCanvas_)
    delete pCanvas_;
}

void LedMatrixApp::init(int argc, char* pArgv[]) {
  Maze m;
  m.render();

  initMaze(argc, pArgv);
  generateMaze();
  draw();
}

int LedMatrixApp::run() { 
  while(true) {
    if (interruptReceived_)
     return 0;

    tick();
    usleep(1000 * 1000);
  }

  return 0;
}

void LedMatrixApp::initMaze(int argc, char* pArgv[]) {
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

void LedMatrixApp::generateMaze() {
  for(int x = 0; x < 128; x++) {
    pField_[x][0]  = Item::Wall; 
    pField_[x][31] = Item::Wall; 
  }   

  for(int y = 0; y < 32; y++) {
    pField_[0][y]   = Item::Wall;
    pField_[127][y] = Item::Wall;
  }    
}

void LedMatrixApp::draw() {
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

void LedMatrixApp::tick() {
  printf("tick\n");

 
}

void LedMatrixApp::interruptReceived() {
  interruptReceived_ = true;
}

void LedMatrixApp::interruptHandler(int sigNo) {
  // TODO: don't ignore sigNo

  LedMatrixApp::_instance()->interruptReceived();
}

//-----------------------------------------------------
// main
//-----------------------------------------------------

int main(int argc, char* pArgv[]) { 
  LedMatrixApp::_create();
  LedMatrixApp::_instance()->init(argc, pArgv);

  return LedMatrixApp::_instance()->run();
}

