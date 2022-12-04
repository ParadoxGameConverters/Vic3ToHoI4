#include "src/maps/utility.h"



int maps::GetIntFromColor(const commonItems::Color& color)
{
   auto [r, g, b] = color.getRgbComponents();

   return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
}