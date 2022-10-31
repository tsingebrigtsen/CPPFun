// Inspired by Bill Weinman's C++20 book.

#include <numeric>

#include <iostream>
using std::cout;
using std::endl;

template< typename T >
concept Natural = std::integral<T>;

template< Natural T >
class RangeContainer {
    T startValue {};
    T endValue {};
public:
    constexpr RangeContainer ( const T& start, const T& end ) : startValue(start), endValue(end) {
        if( startValue > endValue ){
            startValue = 0;
            endValue = 0;
        }
    }

    // begin Iterator class.
    class Iterator {
    private:
        T data {};
    public:
        using iterator_concept = std::forward_iterator_tag;
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::remove_cv_t<T>;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        explicit constexpr Iterator( const T& value ) : data(value) {}

        constexpr T operator*() const {
            return data;
        }

        constexpr Iterator& operator++() {
            ++data;
            return *this;
        }

        constexpr bool operator!=( const Iterator& rhs ) const {
            return data != rhs.data;
        }

        constexpr bool operator==( const Iterator& rhs ) const {
            return data == rhs.data;
        }
    };
    // end Iterator class

    [[nodiscard]] constexpr Iterator begin() const {
        return Iterator{ startValue };
    }

    [[nodiscard]] constexpr Iterator end() const {
        return Iterator{ endValue + 1 };
    }

    // Overload of usual comparison e.g. <
    auto operator<=>( RangeContainer& rhs ) {
        return ( endValue <=> rhs.endValue );
    };

    bool operator==( const RangeContainer& rhs ){
        return ( startValue == rhs.startValue ) && ( endValue == rhs.endValue );
    }
};

int main() {
    int x = 100;
    int y = 110;

    // Testing the container with forward iterator.
    RangeContainer container1( x, y );
    RangeContainer container2{ x + 2, y + 5 };

    for( const Natural auto& i : container1 )
        cout << i << " ";
    cout << endl;

    auto [ min, max ] = std::minmax_element( container1.begin(), container1.end() );
    cout << "Min: " << *min << ", " << "Max: " << *max << endl;

    cout << ( container1 < container2 ) << endl;

    // Evaluated at compile time.
    constexpr RangeContainer container3{ 1, 10 };
    constexpr auto result = std::accumulate( container3.begin(), container3.end(), 0 );

    constexpr int compare = 55;
    static_assert( result == compare );

    cout << result << endl;

    return 0;
}