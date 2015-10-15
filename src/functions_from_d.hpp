#ifndef AUDI_FUNCTIONS_FROM_D_HPP
#define AUDI_FUNCTIONS_FROM_D_HPP

#include <cmath>
#include "gdual.hpp"
#include <boost/math/constants/constants.hpp>

namespace audi
{

/// Computes the gdual of a function composition when the first derivative is known 
/** 
 * Finds the Taylor expansion for \f$ g(f(\mathbf x))\f$ when the 
 * Taylor expansion of \f$ f(\mathbf x)\f$ is known as well as
 * the derivative \f$ \frac{dg}{df} \f$ and the value \f$ g_0 = g(f(\mathbf x_0))\f$
 *
 * Essentially it exploits the identity:
 *
 * \f[
 * g(f(\mathbf x)) = \int \frac{dg}{df} \frac{\partial f}{\partial x_1}dx_1 + h(x_2..x_n)    
 * \f]
 * 
 * and computes \f$ h(x_2..x_n)\f$ applying the same formula. Note that \f$ \mathbf x = [x_1, x_2, ..., x_n]\f$
 *
 * \note This way of computing derivatives is slower with respect to the corresponding
 * methods based on the nilpotency of \f$ \hat f\f$ and should thus be used only when 
 * necessary (that is when no formula is found to exploit nilpotency.)
 *
 * @param[in] f Taylor expansion of the inner function
 * @param[in] dg Taylor expansion of the derivative of the outer function
 * @param[in] g0 Value of the outer function at the expansion point
*/
inline gdual _compose_from_derivative(gdual f, gdual dg, double g0)
{
    auto ss = f.get_symbol_set();
    if (ss.size() == 0) {
        return gdual(g0);
    }
    auto retval = (dg * f.partial(ss[0])).integrate(ss[0]);
    for (auto i = 1u; i < ss.size(); ++i) {
        f = f.subs(ss[i-1], 0);
        dg = dg.subs(ss[i-1], 0);
        retval += (dg * f.partial(ss[i])).integrate(ss[i]);
    }
    return g0 + retval;
}

/// Possible overload for the inverse hyperbolic tangent
/**
 * Implements the inverse hyperbolic tangent of an audi::gdual. The audi:atanh
 * overload is actually faster and this is only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse hyperbolic tangent of \p d
 *
*/
inline gdual atanh_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::atanh(f0);
    auto dg = 1. / (1 - f*f);
    return _compose_from_derivative(f, dg, g0);
}

/// Possible overload for the inverse tangent
/**
 * Implements the inverse tangent of an audi::gdual. The audi:atan
 * overload is actually faster and this is only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse tangent of \p d
 *
*/
inline gdual atan_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::atan(f0);
    auto dg = 1. / (1 + f*f);
    return _compose_from_derivative(f, dg, g0);
}

/// Possible overload for the inverse sine
/**
 * Implements the inverse sine of an audi::gdual. The audi:asin
 * overload may be slower, but this is currently only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse sine of \p d
 *
*/
inline gdual asin_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::asin(f0);
    auto dg = 1. / sqrt(1 - f*f);
    return _compose_from_derivative(f, dg, g0);
}

/// Possible overload for the inverse hyperbolic sine
/**
 * Implements the inverse hyperbolic sine of an audi::gdual. The audi:asinh
 * overload may be slower, but this is currently only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse hyperbolic sine of \p d
 *
*/
inline gdual asinh_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::asinh(f0);
    auto dg = 1. / sqrt(1 + f*f);
    return _compose_from_derivative(f, dg, g0);
}

/// Possible overload for the inverse cosine
/**
 * Implements the inverse cosine of an audi::gdual. The audi:acos
 * overload may be slower, but this is currently only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse cosine of \p d
 *
*/
inline gdual acos_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::acos(f0);
    auto dg = - 1. / sqrt(1 - f*f);
    return _compose_from_derivative(f, dg, g0);
}

/// Possible overload for the inverse hyperbolic cosine
/**
 * Implements the inverse hyperbolic cosine of an audi::gdual. The audi:cosh
 * is faster, and this is currently only provided as to benchmark the
 * performances of audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the inverse hyperbolic cosine of \p d
 *
*/
inline gdual acosh_d(const gdual& f)
{
    auto f0 = f.constant_cf();
    double g0 = std::acosh(f0);
    auto dg = 1. / sqrt((f - 1)*(f + 1));
    return _compose_from_derivative(f, dg, g0);
}

/// Overload for the error function
/**
 * Implements the error function of an audi::gdual. Essentially it 
 * makes use of the fact that \f$ \frac{d erf(x)}{dx} = \frac{2}{\sqrt{\pi}}\exp(-x^2)\f$
 * to then use audi::_compose_from_derivative
 *
 * @param[in] f audi::gdual argument
 *
 * @return an audi:gdual containing the Taylor expansion of the error function of \p d
 *
*/
inline gdual erf(const gdual& d)
{
    auto f0 = d.constant_cf();
    double g0 = std::erf(f0);
    auto dg = (2. / std::sqrt(boost::math::constants::pi<double>())) * exp(- d * d);
    return _compose_from_derivative(d, dg, g0);
}


} // end of namespace audi 

#endif


