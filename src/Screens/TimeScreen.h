#pragma once

#include "Screen.h"

class TimeScreen : public Screen {
 public:
  TimeScreen(uint16_t bg = GxEPD_BLACK) : Screen(bg) {}
  void show() override;
};
