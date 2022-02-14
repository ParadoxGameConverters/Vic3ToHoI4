#include "external/commonItems/Log.h"
#include "src/vic3_to_hoi4_converter.h"



int main(const int argc, const char* argv[])
{
   try
   {
      commonItems::ConverterVersion converterVersion;
      converterVersion.loadVersion("../version.txt");
      Log(LogLevel::Info) << converterVersion;
      Log(LogLevel::Progress) << "0%";
      ConvertVic3ToHoi4("default_mod");

      return 0;
   }

   catch (const std::exception& e)
   {
      Log(LogLevel::Error) << e.what();
      return -1;
   }
}