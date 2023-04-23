#ifndef SRC_SUPPORT_DATEFMT_H
#define SRC_SUPPORT_DATEFMT_H



#include "external/commonItems/Date.h"
#include "external/fmt/include/fmt/format.h"



template <>
struct fmt::formatter<date>
{
   template <typename ParseContext>
   constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin());

   template <typename FormatContext>
   auto format(const date& the_date, FormatContext& ctx) const -> decltype(ctx.out());
};


template <typename ParseContext>
constexpr auto fmt::formatter<date>::parse(ParseContext& ctx) -> decltype(ctx.begin())
{
	return ctx.begin();
}


template <typename FormatContext>
auto fmt::formatter<date>::format(const date& the_date, FormatContext& ctx) const -> decltype(ctx.out())
{
	return fmt::format_to(ctx.out(), "{0}.{1}.{2}", the_date.getYear(), the_date.getMonth(), the_date.getDay());
}



#endif  // SRC_SUPPORT_DATEFMT_H