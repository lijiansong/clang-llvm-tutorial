int main()
{
  int x = 1;
    
  class __lambda_3_18
  {
    public: 
    inline /*constexpr */ void operator()() const
    {
      ++x;
    }
    
    private: 
    int & x;
    
    public:
    __lambda_3_18(int & _x)
    : x{_x}
    {}
    
  };
  
  __lambda_3_18 lambda = __lambda_3_18{x};
  lambda.operator()();
  return x;
}
