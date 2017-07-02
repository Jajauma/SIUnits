#ifndef HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED
#define HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED

#include <ratio>
#include <stdexcept>

namespace Types {
template <typename _Rep, typename _Period = std::ratio<1>>
class Frequency
{
public:
    using Rep    = _Rep;
    using Period = _Period;
    using Self   = Frequency<_Rep, _Period>;

    explicit Frequency(Rep count = Rep{})
        : mCount{count}
    {
    }

    Frequency(const Self& other) = default;
    Self& operator=(const Self& other) = default;

    template <typename XRep>
    Frequency(const Frequency<XRep, Period>& other)
        : mCount{CheckedCast<Rep>(other.Count())}
    {
    }

    template <typename XRep>
    Self& operator=(const Frequency<XRep, Period>& other)
    {
        mCount = CheckedCast<Rep>(other.Count());
        return *this;
    }

    Self& operator=(Rep value)
    {
        mCount = value;
        return *this;
    }

    Self operator+() const { return Self{mCount}; }

    Self operator-() const { return Self{-mCount}; }

    Rep Count() const { return mCount; }

private:
    template <typename Target, typename Source>
    Target CheckedCast(Source value)
    {
        auto result = static_cast<Target>(value);
        if (static_cast<Source>(result) != value)
            throw std::runtime_error("CheckedCast<>() failed");
        return result;
    }

    Rep mCount{};
};

namespace Literals {
using Float   = long double;
using Integer = unsigned long long;

Frequency<Float> operator"" _Hz(Float count)
{
    return Frequency<Float>{count};
}

Frequency<Integer> operator"" _Hz(Integer count)
{
    return Frequency<Integer>{count};
}

Frequency<Float, std::kilo> operator"" _kHz(Float count)
{
    return Frequency<Float, std::kilo>{count};
}

Frequency<Integer, std::kilo> operator"" _kHz(Integer count)
{
    return Frequency<Integer, std::kilo>{count};
}
} /* namespace Literals  */
} /* namespace Types  */

#endif /* end of the header guard */
