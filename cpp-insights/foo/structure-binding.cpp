int main() {
  struct point {
    int x;
    int y;
  };
  point pt{1, 2};
  auto[x, y] = pt;
}
