#ifndef SRC_MAPPERS_CULTUREQUEUE_H
#define SRC_MAPPERS_CULTUREQUEUE_H



namespace mappers
{
using CharacterId = int;
using PortraitType = std::string;
using CultureQueue = std::map<PortraitType, std::vector<CharacterId>>;
}  // namespace mappers
#endif  // SRC_MAPPERS_CULTUREQUEUE_H