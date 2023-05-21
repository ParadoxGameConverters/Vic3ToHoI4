#include "hoi4_country.h"

bool hoi4::Country::AreElectionsAllowed() const
{
   return ideology_ == "democratic";
}
