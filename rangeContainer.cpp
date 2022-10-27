// Inspiration from Bill Weinman's book C++20.

#include <iostream>

using std::cout;
using std::endl;

template< typename T >
concept Natural = std::integral<T>;

template<Natural T>
class RangeContainer {
    T startValue {};
    T endValue {};
public:
    RangeContainer ( const T& start, const T& end ) : startValue(start), endValue(end) {
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
        using iterator_concept [[maybe_unused]] = std::forward_iterator_tag;
        using iterator_category [[maybe_unused]] = std::forward_iterator_tag;
        using value_type [[maybe_unused]] = std::remove_cv_t<T>;
        using difference_type [[maybe_unused]] = std::ptrdiff_t;
        using pointer [[maybe_unused]] = const T*;
        using reference [[maybe_unused]] = const T&;
        
        explicit Iterator( const T& value ) : data(value) {}

        T operator*() const {
            return data;
        }

        Iterator& operator++() {
            ++data;
            return *this;
        }

        bool operator!=( const Iterator& rhs ) const {
            return data != rhs.data;
        }

        bool operator==( const Iterator& rhs ) const {
            return data == rhs.data;
        }
    };
    // end Iterator class

    Iterator begin() {
        return Iterator{ startValue };
    }

    Iterator end() {
        return Iterator{ endValue };
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
    
    RangeContainer container1{x, y };
    RangeContainer container2{ x + 2, y + 5 };

    for( const Natural auto& i: container1 )
        cout << i << " ";
    cout << endl;

    auto [ min, max ] = std::minmax_element( container1.begin(), container1.end() );
    cout << "Min: " << *min << ", " << "Max: " << *max << endl;

    cout << ( container1 < container2 ) << endl;

    return 0;
}