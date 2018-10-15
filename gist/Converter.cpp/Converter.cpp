#include <type_traits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>
using namespace std;

//一般参数都是from，根据自己情况写
#define GenericConvert(TYPE,EXP) template <typename From>\
                                 struct Converter<TYPE, From>{\
                                 static TYPE convert(const From& from){return EXP;}\
                                 }

#define SingleConvert(TOTYPE,FROMTYPE,EXP) template <>\
                                           struct Converter<TOTYPE,FROMTYPE>{\
                                           static TOTYPE convert(const FROMTYPE& from){return EXP;}\
                                             }

namespace detail
{
  const char* strue = "true";
  const char* sfalse = "false";

  //traits
  template <typename To, typename From>
  struct Converter
  {
  };
  //to numeric
  GenericConvert(int,std::atoi(from));
  GenericConvert(long,std::atol(from));
  GenericConvert(long long,std::atoll(from));
  GenericConvert(double,std::atof(from));
  GenericConvert(float,(float)std::atof(from));
  //to bool
  template <typename From>
  struct Converter<bool, From>
  {
    static typename std::enable_if<std::is_integral<From>::value, bool>::type convert(From from)
    {
      return !!from;
    }
  };
  static bool boolconvert(const char* from)
  {
    const unsigned int len = strlen(from);
    if (len != 4 && len != 5)
      throw std::invalid_argument("argument is invalid");
    bool r = true;

    auto checkbool=[](const char* from, const size_t len, const char* s)->bool{
        for (size_t i = 0; i < len; i++)
        {
          if (from[i] != s[i])
          {
            return false;
          }
        }
        return true;
    };

    if (len == 4)
    {
      r = checkbool(from, len, strue);
      if (r)
        return true;
    }
    else
    {
      r = checkbool(from, len, sfalse);
      if (r)
        return false;
    }
    throw std::invalid_argument("argument is invalid");
  }

  SingleConvert(bool,string,detail::boolconvert(from.c_str()));
  template <> //这个由于类型里有const不能用宏了
  struct Converter<bool, const char*>
  {
    static bool convert(const char* from)
    {
      return detail::boolconvert(from);
    }
  };
  SingleConvert(bool,char*,detail::boolconvert(from));
  template <unsigned N> //这个由于还有别的模板参数也不能用了
  struct Converter<bool, const char[N]>
  {
    static bool convert(const char(&from)[N])
    {
      return detail::boolconvert(from);
    }
  };
  template <unsigned N> //同上
  struct Converter<bool, char[N]>
  {
    static bool convert(const char(&from)[N])
    {
      return detail::boolconvert(from);
    }
  };
  //to string
  GenericConvert(string,std::to_string(from));
}

int main()
{
    string a=detail::Converter<string,int>::convert(1);
}