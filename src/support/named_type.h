#pragma once



// This template is used to create strong typedefs in C++.
// It helps to avoid accidental mixing of different types that are represented by the same underlying type.
// Is is used like so: using Width = NamedType<double, struct WidthParameter>;
// This implementation was taken from https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/
template <typename T, typename Parameter>
class NamedType
{
  public:
   explicit NamedType(T const& value): value_(value) {}
   explicit NamedType(T&& value): value_(std::move(value)) {}
   [[nodiscard]] T& Get() { return value_; }
   [[nodiscard]] T const& Get() const { return value_; }

  private:
   T value_;
};