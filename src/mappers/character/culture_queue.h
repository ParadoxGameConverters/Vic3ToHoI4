#ifndef SRC_MAPPERS_CULTUREQUEUE_H
#define SRC_MAPPERS_CULTUREQUEUE_H



namespace mappers
{
struct IdeologyCultureQueue
{
   std::vector<int> communism;
   std::vector<int> democratic;
   std::vector<int> fascism;
   std::vector<int> neutrality;

   std::partial_ordering operator<=>(const IdeologyCultureQueue&) const = default;
};

struct CultureQueue  // Mirrors PortraitPaths of GraphicsBlock
{
   std::vector<int> army;
   std::vector<int> navy;
   IdeologyCultureQueue leader;
   std::vector<int> female_leader;
   IdeologyCultureQueue advisor;
   std::vector<int> male_operative;
   std::vector<int> female_operative;
   std::vector<int> male_monarch;
   std::vector<int> female_monarch;
   std::vector<int> council;

   std::partial_ordering operator<=>(const CultureQueue&) const = default;
};
}  // namespace mappers
#endif  // SRC_MAPPERS_CULTUREQUEUE_H