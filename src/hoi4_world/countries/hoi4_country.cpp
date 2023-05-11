#include "hoi4_country.h"

bool hoi4::Country::AreElectionsAllowed() const
{
   // Strictly from V3 or ideology mapping dependent?
   return ideology_ == "democratic";
}
