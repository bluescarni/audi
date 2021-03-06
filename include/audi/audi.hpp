#ifndef AUDI_HPP
#define AUDI_HPP

#include <complex>

#include <audi/config.hpp>

#if defined(AUDI_WITH_MPPP)
#include <audi/real128.hpp>
#endif

#include <audi/functions.hpp>
#include <audi/functions_from_d.hpp>
#include <audi/gdual.hpp>
#include <audi/invert_map.hpp>
#include <audi/vectorized.hpp>

namespace audi
{
using gdual_d = audi::gdual<double>;
using gdual_v = audi::gdual<audi::vectorized<double>>;
using gdual_c = audi::gdual<std::complex<double>>;
#if defined(AUDI_WITH_MPPP)
using gdual_mp = audi::gdual<mppp::real128>;
#endif
} // namespace audi

#endif
