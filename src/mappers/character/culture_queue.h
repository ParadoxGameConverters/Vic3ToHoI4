#ifndef SRC_MAPPERS_CULTUREQUEUE_H
#define SRC_MAPPERS_CULTUREQUEUE_H



namespace mappers
{
using CharacterId = int;

struct IdeologyCultureQueue
{
   std::vector<CharacterId> communism;
   std::vector<CharacterId> democratic;
   std::vector<CharacterId> fascism;
   std::vector<CharacterId> neutrality;

   std::partial_ordering operator<=>(const IdeologyCultureQueue&) const = default;
};

struct CultureQueue  // Mirrors PortraitPaths of GraphicsBlock
{
   std::vector<CharacterId> army;
   std::vector<CharacterId> navy;
   IdeologyCultureQueue leader;
   std::vector<CharacterId> female_leader;
   IdeologyCultureQueue advisor;
   std::vector<CharacterId> male_operative;
   std::vector<CharacterId> female_operative;
   std::vector<CharacterId> male_monarch;
   std::vector<CharacterId> female_monarch;
   std::vector<CharacterId> council;

   std::partial_ordering operator<=>(const CultureQueue&) const = default;
};
}  // namespace mappers
#endif  // SRC_MAPPERS_CULTUREQUEUE_H