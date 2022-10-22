// From Bill Weinman's book.

#include <iostream>

using std::cout;
using std::endl;

template<typename T>
class CustomContainer {
private:
    T startValue {};
    T endValue {};
public:
   CustomContainer ( T start, T end ) : startValue(start), endValue(end) {}

    // begin Iterator class.
    class Iterator {
    private:
        T data;
    public:
        using iterator_concept [[maybe_unused]] = std::forward_iterator_tag;
        using iterator_category [[maybe_unused]] = std::forward_iterator_tag;
        using value_type [[maybe_unused]] = std::remove_cv_t<T>;
        using difference_type [[maybe_unused]] = std::ptrdiff_t;
        using pointer [[maybe_unused]] = const T*;
        using reference [[maybe_unused]] = const T&;
        
        explicit Iterator( T value ) : data(value) {}

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

    auto operator<=>( CustomContainer& rhs ) {
        return endValue <=> rhs.endValue;
    };

    auto operator==( const CustomContainer& rhs ){
        return endValue == rhs.endValue;
    }
};

int main() {
    int x = 100;
    int y = 110;
    
    CustomContainer container1{ x, y };
    CustomContainer container2{ x + 20, y + 20 };

    for( const auto& i: container1 )
        cout << i << " ";
    cout << endl;

    auto [ min, max ] = std::minmax_element( container1.begin(), container1.end() );
    cout << "Min: " << *min << ", " << "Max: " << *max << endl;

    cout << ( container1 < container2 ) << endl;

    return 0;
}