#ifndef SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGY_H
#define SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGY_H



#include <map>
#include <string>



namespace vic3
{


using Law = std::string;
using ApprovalAmount = int;



class Ideology
{
  public:
   explicit Ideology(std::map<Law, ApprovalAmount> law_approvals): law_approvals_(std::move(law_approvals)) {}

   [[nodiscard]] const std::map<Law, ApprovalAmount>& GetLawApprovals() const { return law_approvals_; }

  private:
   std::map<Law, ApprovalAmount> law_approvals_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_IDEOLOGIES_IDEOLOGY_H