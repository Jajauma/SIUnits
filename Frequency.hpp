#ifndef HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED
#define HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED

#include <ratio>
#include <type_traits>

namespace Types {
// forward declaration
template <typename Rep, typename Period>
class Frequency;
} /* namespace Types */

namespace Types {
namespace Implementation {
template <intmax_t P>
struct Sign : std::integral_constant<intmax_t, (P < 0) ? -1 : 1>
{
};
template <intmax_t P>
struct Abs : std::integral_constant<intmax_t, P * Sign<P>::value>
{
};
template <intmax_t P, intmax_t Q>
struct GCD : GCD<Q, (P % Q)>
{
};
template <intmax_t P>
struct GCD<P, 0> : std::integral_constant<intmax_t, Abs<P>::value>
{
};
template <intmax_t Q>
struct GCD<0, Q> : std::integral_constant<intmax_t, Abs<Q>::value>
{
};

template <typename _CommonRep, typename Period1, typename Period2>
struct CommonTypeWrapper
{
private:
    using NumGCD    = GCD<Period1::num, Period2::num>;
    using DenGCD    = GCD<Period1::den, Period2::den>;
    using CommonRep = typename _CommonRep::type;
    using CommonPeriod
        = std::ratio<NumGCD::value,
                     (Period1::den / DenGCD::value) * Period2::den>;

public:
    // TODO: FIXME: get rid of libstdc++ internals
    using type = std::__success_type<Frequency<CommonRep, CommonPeriod>>;
};

template <typename Period1, typename Period2>
// TODO: FIXME: get rid of libstdc++ internals
struct CommonTypeWrapper<std::__failure_type, Period1, Period2>
{
    // TODO: FIXME: get rid of libstdc++ internals
    typedef std::__failure_type type;
};

template <typename T>
struct IsRatio : std::false_type
{
};
template <intmax_t Num, intmax_t Den>
struct IsRatio<std::ratio<Num, Den>> : std::true_type
{
};
template <typename T>
struct IsFrequency : std::false_type
{
};
template <typename Rep, typename Period>
struct IsFrequency<Frequency<Rep, Period>> : std::true_type
{
};

template <typename To, typename PeriodQuot, typename CommonRep,
          bool NumIsOne = false, bool DenIsOne = false>
struct FrequencyCast
{
    template <typename Rep, typename Period>
    static constexpr To Cast(const Frequency<Rep, Period>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               * static_cast<CommonRep>(PeriodQuot::num)
                               / static_cast<CommonRep>(PeriodQuot::den)));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct FrequencyCast<To, PeriodQuot, CommonRep, true, true>
{
    template <typename Rep, typename Period>
    static constexpr To Cast(const Frequency<Rep, Period>& f)
    {
        using ToRep = typename To::Rep;
        return To(static_cast<ToRep>(f.Count()));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct FrequencyCast<To, PeriodQuot, CommonRep, true, false>
{
    template <typename Rep, typename Period>
    static constexpr To Cast(const Frequency<Rep, Period>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               / static_cast<CommonRep>(PeriodQuot::den)));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct FrequencyCast<To, PeriodQuot, CommonRep, false, true>
{
    template <typename Rep, typename Period>
    static constexpr To Cast(const Frequency<Rep, Period>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               * static_cast<CommonRep>(PeriodQuot::num)));
    }
};
} /* namespace Implementation */
} /* namespace Types */

namespace std {
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
struct common_type<Types::Frequency<Rep1, Period1>,
                   Types::Frequency<Rep2, Period2>>
    : public Types::Implementation::CommonTypeWrapper<
          // TODO: FIXME: get rid of libstdc++ internals
          typename __member_type_wrapper<common_type<Rep1, Rep2>>::type,
          Period1, Period2>::type
{
};
} /* namespace std */

namespace Types {

template <typename To, typename Rep, typename Period>
constexpr
    typename std::enable_if<Implementation::IsFrequency<To>::value, To>::type
    FrequencyCast(const Frequency<Rep, Period>& f)
{
    using ToPeriod   = typename To::Period;
    using ToRep      = typename To::Rep;
    using PeriodQuot = std::ratio_divide<Period, ToPeriod>;
    using CommonRep  = typename std::common_type<ToRep, Rep, intmax_t>::type;
    using Impl       = Implementation::FrequencyCast<To,
                                               PeriodQuot,
                                               CommonRep,
                                               PeriodQuot::num == 1,
                                               PeriodQuot::den == 1>;
    return Impl::Cast(f);
}

template <typename _Rep, typename _Period = std::ratio<1>>
class Frequency
{
public:
    using Rep    = _Rep;
    using Period = _Period;

    static_assert(Implementation::IsRatio<_Period>::value,
                  "period must be a specialization of ratio");
    static_assert(_Period::num > 0, "period must be positive");

    constexpr Frequency()       = default;
    Frequency(const Frequency&) = default;

    template <typename Rep2,
              typename = typename std::
                  enable_if<std::is_convertible<Rep2, Rep>::value
                            && (std::is_floating_point<Rep>::value
                                || !std::is_floating_point<Rep2>::value)>::type>
    constexpr explicit Frequency(const Rep2& rep)
        : mCount(static_cast<Rep>(rep))
    {
    }

    template <typename Rep2, typename Period2,
              typename = typename std::
                  enable_if<std::is_floating_point<Rep>::value
                            || (std::ratio_divide<Period2, Period>::den == 1
                                && !std::is_floating_point<Rep2>::value)>::type>
    constexpr Frequency(const Frequency<Rep2, Period2>& f)
        : mCount(FrequencyCast<Frequency>(f).Count())
    {
    }

    ~Frequency()       = default;
    Frequency& operator=(const Frequency&) = default;

    constexpr Rep Count() const { return mCount; }
    constexpr Frequency operator+() const { return *this; }
    constexpr Frequency operator-() const { return Frequency(-mCount); }

    Frequency& operator++()
    {
        ++mCount;
        return *this;
    }
    Frequency& operator--()
    {
        --mCount;
        return *this;
    }

    Frequency operator++(int) { return Frequency(mCount++); }
    Frequency operator--(int) { return Frequency(mCount--); }

    Frequency& operator+=(const Frequency& f)
    {
        mCount += f.Count();
        return *this;
    }
    Frequency& operator-=(const Frequency& f)
    {
        mCount -= f.count();
        return *this;
    }
    Frequency& operator*=(const Rep& rhs)
    {
        mCount *= rhs;
        return *this;
    }
    Frequency& operator/=(const Rep& rhs)
    {
        mCount /= rhs;
        return *this;
    }
    template <typename Rep2 = Rep>
    typename std::enable_if<!std::is_floating_point<Rep2>::value,
                            Frequency&>::type
    operator%=(const Rep& rhs)
    {
        mCount %= rhs;
        return *this;
    }
    template <typename Rep2 = Rep>
    typename std::enable_if<!std::is_floating_point<Rep2>::value,
                            Frequency&>::type
    operator%=(const Frequency& f)
    {
        mCount %= f.Count();
        return *this;
    }

private:
    Rep mCount{};
};

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr typename std::common_type<Frequency<Rep1, Period1>,
                                    Frequency<Rep2, Period2>>::type
operator+(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(Promote(lhs).Count() + Promote(rhs).Count());
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr typename std::common_type<Frequency<Rep1, Period1>,
                                    Frequency<Rep2, Period2>>::type
operator-(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(Promote(lhs).Count() - Promote(rhs).Count());
}

template <
    typename Rep1, typename Rep2,
    bool
    = std::is_convertible<Rep2,
                          typename std::common_type<Rep1, Rep2>::type>::value>
struct CommonRep
{
};

template <typename Rep1, typename Rep2>
struct CommonRep<Rep1, Rep2, true>
{
    using type = typename std::common_type<Rep1, Rep2>::type;
};

template <typename Rep1, typename Period, typename Rep2>
constexpr Frequency<typename CommonRep<Rep1, Rep2>::type, Period>
operator*(const Frequency<Rep1, Period>& f, const Rep2& s)
{
    using Promote
        = Frequency<typename std::common_type<Rep1, Rep2>::type, Period>;
    return Promote(Promote(f).Count() * s);
}

template <typename Rep1, typename Rep2, typename Period>
constexpr Frequency<typename CommonRep<Rep2, Rep1>::type, Period>
operator*(const Rep1& s, const Frequency<Rep2, Period>& f)
{
    return f * s;
}

template <typename Rep1, typename Period, typename Rep2>
constexpr Frequency<
    typename CommonRep<Rep1,
                       typename std::
                           enable_if<!Implementation::IsFrequency<Rep2>::value,
                                     Rep2>::type>::type,
    Period>
operator/(const Frequency<Rep1, Period>& f, const Rep2& s)
{
    using Promote
        = Frequency<typename std::common_type<Rep1, Rep2>::type, Period>;
    return Promote(Promote(f).Count() / s);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr typename std::common_type<Rep1, Rep2>::type
operator/(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() / Promote(rhs).Count();
}

template <typename Rep1, typename Period, typename Rep2>
constexpr Frequency<
    typename CommonRep<Rep1,
                       typename std::
                           enable_if<!Implementation::IsFrequency<Rep2>::value,
                                     Rep2>::type>::type,
    Period>
operator%(const Frequency<Rep1, Period>& f, const Rep2& s)
{
    using Promote
        = Frequency<typename std::common_type<Rep1, Rep2>::type, Period>;
    return Promote(Promote(f).Count() % s);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr typename std::common_type<Frequency<Rep1, Period1>,
                                    Frequency<Rep2, Period2>>::type
operator%(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(Promote(lhs).Count() % Promote(rhs).Count());
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator==(const Frequency<Rep1, Period1>& lhs,
           const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() == Promote(rhs).Count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator<(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    using Arg1    = Frequency<Rep1, Period1>;
    using Arg2    = Frequency<Rep2, Period2>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() < Promote(rhs).Count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator!=(const Frequency<Rep1, Period1>& lhs,
           const Frequency<Rep2, Period2>& rhs)
{
    return !(lhs == rhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator<=(const Frequency<Rep1, Period1>& lhs,
           const Frequency<Rep2, Period2>& rhs)
{
    return !(rhs < lhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator>(const Frequency<Rep1, Period1>& lhs,
          const Frequency<Rep2, Period2>& rhs)
{
    return rhs < lhs;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool
operator>=(const Frequency<Rep1, Period1>& lhs,
           const Frequency<Rep2, Period2>& rhs)
{
    return !(lhs < rhs);
}

namespace Literals {
using Float   = long double;
using Integer = unsigned long long int;

constexpr Frequency<Float> operator"" _Hz(Float count)
{
    return Frequency<Float>{count};
}
constexpr Frequency<Float, std::kilo> operator"" _kHz(Float count)
{
    return Frequency<Float, std::kilo>{count};
}
constexpr Frequency<Float, std::mega> operator"" _MHz(Float count)
{
    return Frequency<Float, std::mega>{count};
}
constexpr Frequency<Float, std::giga> operator"" _GHz(Float count)
{
    return Frequency<Float, std::giga>{count};
}

constexpr Frequency<Integer> operator"" _Hz(Integer count)
{
    return Frequency<Integer>{count};
}
constexpr Frequency<Integer, std::kilo> operator"" _kHz(Integer count)
{
    return Frequency<Integer, std::kilo>{count};
}
constexpr Frequency<Integer, std::mega> operator"" _MHz(Integer count)
{
    return Frequency<Integer, std::mega>{count};
}
constexpr Frequency<Integer, std::giga> operator"" _GHz(Integer count)
{
    return Frequency<Integer, std::giga>{count};
}
} /* namespace Literals  */
} /* namespace Types  */

#endif /* end of the header guard */
