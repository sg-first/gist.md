#include <map>
#include <typeindex>
#include <memory>

template< typename T >
struct recursion_struct
{
    T t;
    template< typename ... ARG >
    auto operator ( )( ARG && ... arg ) const
    { return t( * this, std::forward< ARG >( arg ) ... ); }
    template< typename ... ARG >
    auto operator ( )( ARG && ... arg )
    { return t( * this, std::forward< ARG >( arg ) ... ); }
};

template< typename T >
recursion_struct< T > recursion( const T & t ) { return recursion_struct< T >( { t } ); }