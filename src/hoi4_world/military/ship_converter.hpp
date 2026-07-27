#pragma once


#include <map>
#include <string>
#include <vector>

#include "src/hoi4_world/military/ship.h"



namespace hoi4
{

class ShipConverter
{
  public:
   ShipConverter(std::map<std::string, hoi4::Ship> ship_mappings): ship_mappings_(std::move(ship_mappings)) {}

   std::vector<hoi4::Ship> ConvertShips(const std::vector<std::string>& vic3_ships,
       std::map<std::string, int>& ship_counts) const;

  private:
   std::map<std::string, hoi4::Ship> ship_mappings_;
};

}  // namespace hoi4