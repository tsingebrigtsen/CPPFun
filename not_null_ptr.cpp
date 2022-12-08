
// std::nullptr_t
#include <cstddef>
// std::forward
#include <algorithm>
// std::cout, endl
#include <iostream>
// std::same_as
#include <concepts>

// Smart pointer created from a raw pointer that cannot be nullptr and is unique.
template<typename T>
requires (!std::same_as<T,std::nullptr_t>)
class not_null_ptr{
    T* ptr;

public:
    explicit not_null_ptr(T* ptr) : ptr(ptr){ std::cout << "Test" << std::endl; assert(ptr != nullptr); };
    explicit not_null_ptr(void* ptr) : ptr(ptr){ std::cout << "Test" << std::endl; assert(ptr != nullptr); };
    ~not_null_ptr(){ delete ptr; }

    // Eliminate copy construction and assignment from nullptr.
    not_null_ptr(std::nullptr_t) = delete;
    not_null_ptr& operator=(std::nullptr_t) = delete;

    // Eliminate copy construction and assignment due to heap allocation.
    not_null_ptr(not_null_ptr& rhs_ptr) = delete;
    not_null_ptr& operator=(not_null_ptr& rhs_ptr) = delete;

    // Eliminate copy construction and assignment due to heap allocation.
    not_null_ptr(not_null_ptr&& rhs_ptr) = delete;
    not_null_ptr& operator=(not_null_ptr&& rhs_ptr) = delete;

    [[nodiscard]] constexpr T* get() const { return ptr; }

    constexpr explicit operator T() const { return get(); }
    constexpr T operator->() const { return get(); }
    constexpr T& operator*() const { return *this->get(); }

    // Should delete many more operators...
};

// factory function similar to make_unique.
template<typename T, typename ... Ts>
not_null_ptr<T> make_not_null(Ts&& ... args){
    return not_null_ptr<T>(new T(std::forward<Ts>(args)...));
}

int main(){
    //auto ptr = make_not_null<std::pair<int,int>>( 2, 3 );
    //std::cout << "not_null_ptr value: " << ptr.get()->first << "," << ptr.get()->second << std::endl;

    not_null_ptr<void> ptr2(void*);
}