#pragma once



// adapted from https://brevzin.github.io/c++/2025/03/12/polymorphic-equals/



template <class D, class Base>
[[nodiscard]] bool PolymorphicEquality(const D& lhs, const Base& rhs)
{
   if (typeid(rhs) == typeid(D))
   {
      return lhs == static_cast<const D&>(rhs);
   }
   else
   {
      return false;
   }
}


template <class D, class Base>
[[nodiscard]] bool PolymorphicLess(const D& lhs, const Base& rhs)
{
   if (typeid(rhs) == typeid(D))
   {
      return lhs < static_cast<const D&>(rhs);
   }
   else
   {
      return typeid(rhs).name() < typeid(D).name();
   }
}