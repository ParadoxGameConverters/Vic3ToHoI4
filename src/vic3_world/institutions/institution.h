#pragma once

namespace vic3
{


class Institution
{
  private:
   std::string type_;
   int investment_;
   int country_id_;

  public:
   Institution(std::string type, int investment, int country_id):
       type_(type),
       investment_(investment),
       country_id_(country_id)
   {
   }
   std::string GetType() { return type_; }
   int GetInvestment() { return investment_; }
   int GetCountry() { return country_id_; }
};
}  // namespace vic3