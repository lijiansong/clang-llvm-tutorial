int main() {
  int x = 1;
  auto lambda = [&]() { ++x; };
  lambda();
  return x;
}
