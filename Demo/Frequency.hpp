#ifndef HEADER_2A9D1180_712E_11E7_A4B4_74D435E5BEA6_INCLUDED
#define HEADER_2A9D1180_712E_11E7_A4B4_74D435E5BEA6_INCLUDED

#include "SIUnits.hpp"

namespace Demo {
// {{{ Frequency type definition
struct Hertz;
template <typename Rep, typename Period = std::ratio<1>>
using Frequency = SI::Units<Rep, Period, Hertz>;
// }}}

// {{{ Supported set of literal operators
SI_UNITS_DEFINE_LITERAL(Frequency, std::ratio<1>, _Hz)
SI_UNITS_DEFINE_LITERAL(Frequency, std::kilo, _kHz)
SI_UNITS_DEFINE_LITERAL(Frequency, std::mega, _MHz)
SI_UNITS_DEFINE_LITERAL(Frequency, std::giga, _GHz)
SI_UNITS_DEFINE_LITERAL(Frequency, std::tera, _THz)
// }}}
} /* namespace Demo */

#endif /* end of the header guard */
