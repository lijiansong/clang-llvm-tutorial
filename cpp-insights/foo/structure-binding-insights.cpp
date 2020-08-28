int main()
{
  struct point
  {
    int x;
    int y;
    // inline constexpr point(const point &) noexcept = default;
  };
  
  point pt = {1, 2};
  point __pt10 = point(pt);
  int& x = __pt10.x;
  int& y = __pt10.y;
}
