#ifndef CHOCO_VEC2_HPP
#define CHOCO_VEC2_HPP

#include <utility>
#include <algorithm>

template <class T>
struct vec2 {
    T x = 0;
    T y = 0;

    vec2(T x, T y) : x(std::move(x)), y(std::move(y)) {}
    vec2(T s) : x(s), y(std::move(s)) {}

    vec2& operator+=(vec2 const& v) {
        *this = *this + v;
        return *this;
    }
    vec2& operator-=(vec2 const& v) {
        *this = *this - v;
        return *this;
    }

    vec2& operator+=(T s) {
        *this = *this + s;
        return *this;
    }
    vec2& operator-=(T s) {
        *this = *this - s;
        return *this;
    }
    vec2& operator*=(T s) {
        *this = *this * s;
        return *this;
    }
    vec2& operator/=(T s) {
        *this = *this / s;
        return *this;
    }

    void clamp (T min, T max) {
        x = std::clamp(x, min, max);
        y = std::clamp(y, min, max);
    }
};

template <class T, class U>
auto operator+(vec2<T> const& lhs, vec2<U> const& v) -> vec2<decltype(lhs.x + v.x)> {
    return {lhs.x + v.x, lhs.y + v.y};
}
template <class T, class U>
auto operator-(vec2<T> const& lhs, vec2<U> const& v) -> vec2<decltype(lhs.x - v.x)> {
    return {lhs.x - v.x, lhs.y - v.y};
}

template <class T, class U>
auto operator+(vec2<T> const& lhs, U s) -> vec2<decltype(lhs.x + s)> {
    return {lhs.x + s, lhs.y + s};
}
template <class T, class U>
auto operator-(vec2<T> const& lhs, U s) -> vec2<decltype(lhs.x - s)> {
    return {lhs.x - s, lhs.y - s};
}
template <class T, class U>
auto operator*(vec2<T> const& lhs, U s) -> vec2<decltype(lhs.x * s)> {
    return {lhs.x * s, lhs.y * s};
}
template <class T, class U>
auto operator/(vec2<T> const& lhs, U s) -> vec2<decltype(lhs.x / s)> {
    return {lhs.x / s, lhs.y / s};
}

template <class T>
std::ostream& operator<< (std::ostream& os, vec2<T> const& v) {
    os << v.x << ' ' << v.y << '\n';
}


#endif
