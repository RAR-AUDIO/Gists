#pragma once

//==================================================================================
/* IPlug2 Includes */
#include "IControls.h"
//==================================================================================
/* Standard Library Includes */
#include <iomanip>
#include <sstream>
#include <string>
//==================================================================================

namespace rar::utils::funcs
{
    
static constexpr double TWO_PI = static_cast<double> (2 * 3.141592653589793238L);

inline auto percentage_display_func = IParam::DisplayFunc ([] (double value, WDL_String& out)
                                                           {
    std::ostringstream s;
    s << std::fixed << std::setprecision (0) << value * 100.0 << " %";
    out = WDL_String (s.str().c_str()); });

inline auto db_display_func = IParam::DisplayFunc ([] (double value, WDL_String& out)
                                                   {
    std::ostringstream s;
    if (value <= - 100)
    {
        s << std::fixed << std::setprecision (1) << "-inf" << " dB";
    }
    else
    {
        s << std::fixed << std::setprecision (1) << value << " dB";
    }
    out = WDL_String (s.str().c_str()); });

inline auto panning_display_func = IParam::DisplayFunc ([] (double value, WDL_String& out)
                                                        {
    std::ostringstream s;
    s << std::fixed << std::setprecision (0) << value / ( TWO_PI / 4.0) * 100.0 << " %";
    out = WDL_String (s.str().c_str()); });

inline auto frequency_display_func = IParam::DisplayFunc ([] (double value, WDL_String& out) 
                                                          {
    // TODO (Roberto A. Ramirez): Change value when above 1000 to show 1 Khz
    std::ostringstream s;
    if (value >= 1000.0)
    {
        s << std::fixed << std::setprecision (0) << value << " kHz";
    }
    else if (value <= 999.9)
    {
        s << std::fixed << std::setprecision (0) << value << " Hz";
    }
    out = WDL_String (s.str().c_str()); });
} // namespace rar::utils::funcs
