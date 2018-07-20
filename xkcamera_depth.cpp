/*
 * Base Depth Camera class 
 * Copyright (C) 2017 Jacobus du Preez / kdupreez@hotmail.com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "xkcamera_depth.hpp"
namespace xk
{
    xkcamera_depth::xkcamera_depth() 
    {
    }

    xkcamera_depth::~xkcamera_depth() 
    {
    }
    
    double xkcamera_depth::get_depth_inches(uint32_t x, uint32_t y)
    {
        //cm to inches
        return get_depth_cm(x, y) * INCHES_PER_CM;
    }

}
