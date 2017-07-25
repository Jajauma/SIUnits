#ifndef HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED
#define HEADER_700EA8EA_5F41_11E7_93E4_74D4359F3068_INCLUDED

#include <iosfwd>
#include <ratio>
#include <type_traits>

// Forward declaration
namespace SI {
template <typename Rep, typename Period, typename Class>
class Units;
} /* namespace SI */

namespace SI {
namespace Implementation {
// {{{ Greatest common divisor (GCD) calculation code
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
// }}}

// {{{ Helper types for std::common_type specialization
template <typename T>
struct SuccessType
{
    using type = T;
};

struct FailureType
{
};

struct MemberTypeWrapperBase
{
    template <typename T>
    static SuccessType<typename T::type> Test(int);

    template <typename>
    static FailureType Test(...);
};

template <typename T>
struct MemberTypeWrapper : private MemberTypeWrapperBase
{
    using type = decltype(Test<T>(0));
};

template <typename _CommonRep, typename Period1, typename Period2,
          typename Class>
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
    using type = SuccessType<Units<CommonRep, CommonPeriod, Class>>;
};

template <typename Period1, typename Period2, typename Class>
struct CommonTypeWrapper<FailureType, Period1, Period2, Class>
{
    using type = FailureType;
};
// }}}

// {{{ UnitsCast various implementations
template <typename To, typename PeriodQuot, typename CommonRep,
          bool NumIsOne = false, bool DenIsOne = false>
struct UnitsCast
{
    template <typename Rep, typename Period, typename Class>
    static constexpr To Cast(const Units<Rep, Period, Class>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               * static_cast<CommonRep>(PeriodQuot::num)
                               / static_cast<CommonRep>(PeriodQuot::den)));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct UnitsCast<To, PeriodQuot, CommonRep, true, true>
{
    template <typename Rep, typename Period, typename Class>
    static constexpr To Cast(const Units<Rep, Period, Class>& f)
    {
        using ToRep = typename To::Rep;
        return To(static_cast<ToRep>(f.Count()));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct UnitsCast<To, PeriodQuot, CommonRep, true, false>
{
    template <typename Rep, typename Period, typename Class>
    static constexpr To Cast(const Units<Rep, Period, Class>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               / static_cast<CommonRep>(PeriodQuot::den)));
    }
};

template <typename To, typename PeriodQuot, typename CommonRep>
struct UnitsCast<To, PeriodQuot, CommonRep, false, true>
{
    template <typename Rep, typename Period, typename Class>
    static constexpr To Cast(const Units<Rep, Period, Class>& f)
    {
        using ToRep = typename To::Rep;
        return To(
            static_cast<ToRep>(static_cast<CommonRep>(f.Count())
                               * static_cast<CommonRep>(PeriodQuot::num)));
    }
};
// }}}

// {{{ Helpers for static_assert(s)
template <typename T>
struct IsRatio : std::false_type
{
};
template <intmax_t Num, intmax_t Den>
struct IsRatio<std::ratio<Num, Den>> : std::true_type
{
};
template <typename T>
struct IsUnits : std::false_type
{
};
template <typename Rep, typename Period, typename Class>
struct IsUnits<Units<Rep, Period, Class>> : std::true_type
{
};
// }}}

// {{{ Helper types used in literal operators
using FloatLiteral   = long double;
using IntegerLiteral = unsigned long long int;
// }}}
} /* namespace Implementation */
} /* namespace SI */

// {{{ std::common_type specialization for Units
namespace std {
template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
struct common_type<SI::Units<Rep1, Period1, Class>,
                   SI::Units<Rep2, Period2, Class>>
    : public SI::Implementation::CommonTypeWrapper<
          typename SI::Implementation::
              MemberTypeWrapper<common_type<Rep1, Rep2>>::type,
          Period1, Period2, Class>::type
{
};
} /* namespace std */
// }}}

namespace SI {
// {{{ Cast between Units of the same Class
template <typename To, typename Rep, typename Period, typename Class>
constexpr typename std::enable_if<Implementation::IsUnits<To>::value, To>::type
UnitsCast(const Units<Rep, Period, Class>& f)
{
    using ToPeriod   = typename To::Period;
    using ToRep      = typename To::Rep;
    using PeriodQuot = std::ratio_divide<Period, ToPeriod>;
    using CommonRep  = typename std::common_type<ToRep, Rep, intmax_t>::type;
    using Impl       = Implementation::UnitsCast<To,
                                           PeriodQuot,
                                           CommonRep,
                                           PeriodQuot::num == 1,
                                           PeriodQuot::den == 1>;
    return Impl::Cast(f);
}
// }}}

// {{{ Units type definition
template <typename _Rep, typename _Period, typename _Class>
class Units
{
public:
    using Rep    = _Rep;
    using Period = _Period;
    using Class  = _Class;

    static_assert(Implementation::IsRatio<_Period>::value,
                  "period must be a specialization of ratio");
    static_assert(_Period::num > 0, "period must be positive");

    constexpr Units()   = default;
    Units(const Units&) = default;

    template <typename Rep2,
              typename = typename std::
                  enable_if<std::is_convertible<Rep2, Rep>::value
                            && (std::is_floating_point<Rep>::value
                                || !std::is_floating_point<Rep2>::value)>::type>
    constexpr explicit Units(const Rep2& rep)
        : mCount(static_cast<Rep>(rep))
    {
    }

    template <typename Rep2, typename Period2,
              typename = typename std::
                  enable_if<std::is_floating_point<Rep>::value
                            || (std::ratio_divide<Period2, Period>::den == 1
                                && !std::is_floating_point<Rep2>::value)>::type>
    constexpr Units(const Units<Rep2, Period2, Class>& f)
        : mCount(UnitsCast<Units>(f).Count())
    {
    }

    ~Units()       = default;
    Units& operator=(const Units&) = default;

    constexpr Rep Count() const { return mCount; }
    constexpr Units operator+() const { return *this; }
    constexpr Units operator-() const { return Units(-mCount); }

    Units& operator++()
    {
        ++mCount;
        return *this;
    }
    Units& operator--()
    {
        --mCount;
        return *this;
    }

    Units operator++(int) { return Units(mCount++); }
    Units operator--(int) { return Units(mCount--); }

    Units& operator+=(const Units& f)
    {
        mCount += f.Count();
        return *this;
    }
    Units& operator-=(const Units& f)
    {
        mCount -= f.Count();
        return *this;
    }
    Units& operator*=(const Rep& rhs)
    {
        mCount *= rhs;
        return *this;
    }
    Units& operator/=(const Rep& rhs)
    {
        mCount /= rhs;
        return *this;
    }
    template <typename Rep2 = Rep>
    typename std::enable_if<!std::is_floating_point<Rep2>::value, Units&>::type
    operator%=(const Rep& rhs)
    {
        mCount %= rhs;
        return *this;
    }
    template <typename Rep2 = Rep>
    typename std::enable_if<!std::is_floating_point<Rep2>::value, Units&>::type
    operator%=(const Units& f)
    {
        mCount %= f.Count();
        return *this;
    }

private:
    Rep mCount{};
};
// }}}

// {{{ Supported set of binary operators for Units
template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr typename std::common_type<Units<Rep1, Period1, Class>,
                                    Units<Rep2, Period2, Class>>::type
operator+(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(Promote(lhs).Count() + Promote(rhs).Count());
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr typename std::common_type<Units<Rep1, Period1, Class>,
                                    Units<Rep2, Period2, Class>>::type
operator-(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
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

template <typename Rep1, typename Period, typename Rep2, typename Class>
constexpr Units<typename CommonRep<Rep1, Rep2>::type, Period, Class>
operator*(const Units<Rep1, Period, Class>& f, const Rep2& s)
{
    using Promote
        = Units<typename std::common_type<Rep1, Rep2>::type, Period, Class>;
    return Promote(Promote(f).Count() * s);
}

template <typename Rep1, typename Rep2, typename Period, typename Class>
constexpr Units<typename CommonRep<Rep2, Rep1>::type, Period, Class>
operator*(const Rep1& s, const Units<Rep2, Period, Class>& f)
{
    return f * s;
}

template <typename Rep1, typename Period, typename Rep2, typename Class>
constexpr Units<
    typename CommonRep<Rep1, typename std::enable_if<!Implementation::
                                                         IsUnits<Rep2>::value,
                                                     Rep2>::type>::type,
    Period, Class>
operator/(const Units<Rep1, Period, Class>& f, const Rep2& s)
{
    using Promote
        = Units<typename std::common_type<Rep1, Rep2>::type, Period, Class>;
    return Promote(Promote(f).Count() / s);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr typename std::common_type<Rep1, Rep2>::type
operator/(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() / Promote(rhs).Count();
}

template <typename Rep1, typename Period, typename Rep2, typename Class>
constexpr Units<
    typename CommonRep<Rep1, typename std::enable_if<!Implementation::
                                                         IsUnits<Rep2>::value,
                                                     Rep2>::type>::type,
    Period, Class>
operator%(const Units<Rep1, Period, Class>& f, const Rep2& s)
{
    using Promote
        = Units<typename std::common_type<Rep1, Rep2>::type, Period, Class>;
    return Promote(Promote(f).Count() % s);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr typename std::common_type<Units<Rep1, Period1, Class>,
                                    Units<Rep2, Period2, Class>>::type
operator%(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(Promote(lhs).Count() % Promote(rhs).Count());
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator==(const Units<Rep1, Period1, Class>& lhs,
           const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() == Promote(rhs).Count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator<(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    using Arg1    = Units<Rep1, Period1, Class>;
    using Arg2    = Units<Rep2, Period2, Class>;
    using Promote = typename std::common_type<Arg1, Arg2>::type;
    return Promote(lhs).Count() < Promote(rhs).Count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator!=(const Units<Rep1, Period1, Class>& lhs,
           const Units<Rep2, Period2, Class>& rhs)
{
    return !(lhs == rhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator<=(const Units<Rep1, Period1, Class>& lhs,
           const Units<Rep2, Period2, Class>& rhs)
{
    return !(rhs < lhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator>(const Units<Rep1, Period1, Class>& lhs,
          const Units<Rep2, Period2, Class>& rhs)
{
    return rhs < lhs;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2,
          typename Class>
constexpr bool
operator>=(const Units<Rep1, Period1, Class>& lhs,
           const Units<Rep2, Period2, Class>& rhs)
{
    return !(lhs < rhs);
}
// }}}

// {{{ Supported stream insertion and extraction operators
template <typename Rep, typename Period, typename Class>
std::ostream&
operator<<(std::ostream& stream, const Units<Rep, Period, Class>& rhs)
{
    stream << rhs.Count();
    return stream;
}

template <typename Rep, typename Period, typename Class>
std::ostream&
operator>>(std::istream& stream, Units<Rep, Period, Class>& rhs)
{
    Rep tmp{};
    stream >> tmp;
    rhs = tmp;
    return stream;
}
// }}}
} /* namespace SI  */

// {{{ Helper macros
#define SI_UNITS_DEFINE_LITERAL(TYPE, PERIOD, LITERAL)                  \
    constexpr TYPE<SI::Implementation::FloatLiteral, PERIOD>            \
    operator"" LITERAL(SI::Implementation::FloatLiteral count)          \
    {                                                                   \
        return TYPE<SI::Implementation::FloatLiteral, PERIOD>{count};   \
    }                                                                   \
    constexpr TYPE<SI::Implementation::IntegerLiteral, PERIOD>          \
    operator"" LITERAL(SI::Implementation::IntegerLiteral count)        \
    {                                                                   \
        return TYPE<SI::Implementation::IntegerLiteral, PERIOD>{count}; \
    }
// }}}

#endif /* end of the header guard */
