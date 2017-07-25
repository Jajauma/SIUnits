#ifndef HEADER_DE162D8A_7130_11E7_A4B4_74D435E5BEA6_INCLUDED
#define HEADER_DE162D8A_7130_11E7_A4B4_74D435E5BEA6_INCLUDED

#include "SIUnits.hpp"

namespace Demo {
// {{{ Length type definition
struct Meter;
template <typename Rep, typename Period = std::ratio<1>>
using Length = SI::Units<Rep, Period, Meter>;
// }}}

// {{{ Supported set of literal operators
SI_UNITS_DEFINE_LITERAL(Length, std::nano, _nm)
SI_UNITS_DEFINE_LITERAL(Length, std::micro, _um)
SI_UNITS_DEFINE_LITERAL(Length, std::milli, _mm)
SI_UNITS_DEFINE_LITERAL(Length, std::centi, _cm)
SI_UNITS_DEFINE_LITERAL(Length, std::ratio<1>, _m)
SI_UNITS_DEFINE_LITERAL(Length, std::kilo, _km)
// }}}
} /* namespace Demo  */

#endif /* end of the header guard */
