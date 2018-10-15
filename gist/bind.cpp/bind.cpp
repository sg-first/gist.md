#include <iostream>
using namespace std;
#define SESIZE __1
#define FUNCTION small_bind_t

namespace
{
    //占位符
    class placeholder_ {};
    placeholder_ __1;
}


template <typename R, typename T, typename Arg>
class small_bind_t
{
private:
    typedef R (T::*F)(Arg);
    F f_;
    T* t_;

public:
    small_bind_t(F f, T* t)
        : f_(f), t_(t)
    {}

    R operator()(Arg& a) //注意
    {
        return (t_->*f_)(a);
    }
};

//包装参数
template <typename R, typename T, typename Arg>
small_bind_t<R, T, Arg> small_bind(R (T::*f)(Arg), T* t, placeholder_& a)
{
    return small_bind_t<R, T, Arg>(f, t);
}

class bind_test
{
public:
    void print_string(string str)
    {
        cout<<str<<endl;
    }
};

int main()
{
    bind_test t;
    string h = "test";

    small_bind(&bind_test::print_string,&t,SESIZE)(h);

    FUNCTION<void,bind_test,string> fun = small_bind(&bind_test::print_string,&t,SESIZE);
    fun(h);
}