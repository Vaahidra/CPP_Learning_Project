#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <functional>

template<typename ... T>
using Arithmetic = std::enable_if_t<(std::is_arithmetic<std::remove_reference_t<T>>::value && ...), int>;

template<size_t Size, typename T>
class Point {
static_assert(Size >= 1);
friend std::ostream& operator<<(std::ostream& stream, const Point& point) {
    return stream << point.to_string();
}
public:
    std::array<T, Size> values;

    Point() {}
    Point(Point& other) : values {other.values} {}
    Point(Point&& other) : values {other.values} {}
    Point(const Point& other) : values {other.values} {}
    Point(const Point&& other) : values {other.values} {}

    template<typename ... U, typename = Arithmetic<U...>>
    Point(U&& ... val) : values {std::forward<U>(val)...} {
        static_assert(sizeof...(U) == Size);
    }
    T& x() { return get<0>(); }
    T x() const { return get<0>(); }
    T& y() { return get<1>(); }
    T y() const { return get<1>(); }
    T& z() { return get<2>(); }
    T z() const { return get<2>(); }

    template<int pos>
    [[nodiscard]] T get() const {
        static_assert(Size > pos);
        return values[pos];
    }
    template<int pos>
    [[nodiscard]] T& get() {
        static_assert(Size > pos);
        return values[pos];
    }

    Point& operator+=(const Point& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::plus<T>());
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::minus<T>());
        return *this;
    }
    Point& operator*=(const Point& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::multiplies<T>());
        return *this;
    }

    Point& operator*=(const T scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](T x){return x*scalar;});
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const T scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }
    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point operator-() const {
        Point result;
        return result - *this;
    }

    T length() const {
        const T l = std::accumulate(values.begin(), values.end(), static_cast<T>(0),
                                    [](T sum, T x){ return sum + x*x;});
        return std::sqrt(l);
    }

    T distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const T target_len = 1.0f)
    {
        const T current_len = length();
        if (current_len == 0) throw std::logic_error("cannot normalize vector of length 0");

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const T max_len)
    {
        assert(max_len > 0);

        const T current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }
        return *this;
    }

    std::string to_string() const {
        using namespace std::string_literals;
        return "[" + std::accumulate(std::next(values.begin()), values.end(), std::to_string(values[0]),
                               [](const std::string& s, const T& x){return s + ", "s + std::to_string(x);}) + "]";
    }
};
using Point2D = Point<2, float>;
using Point3D = Point<3, float>;




// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}


