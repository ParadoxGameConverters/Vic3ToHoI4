#include <external/commonItems/Log.h>

#include "src/configuration/configuration_importer.h"
#include "src/vic3_to_hoi4_converter.h"



int main()
{
   try
   {
      commonItems::ConverterVersion converter_version;
      converter_version.loadVersion("../version.txt");
      Log(LogLevel::Info) << converter_version;

      const auto configuration = configuration::ImportConfiguration("configuration.txt", converter_version);

      Log(LogLevel::Progress) << "0%";
      ConvertVic3ToHoi4(configuration, converter_version);

      return 0;
   }

   catch (const std::exception& e)
   {
      Log(LogLevel::Error) << e.what();
      return -1;
   }
}