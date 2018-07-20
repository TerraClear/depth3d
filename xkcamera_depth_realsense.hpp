/*
 * Specific implementation for RealSense 3D camera class 
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

#include <cstdlib>
#include <iostream>

#include "xkcamera_depth.hpp"
#include <librealsense2/rs.hpp>

#ifndef XCAMERA_KDEPTH_REALSENSE_HPP
#define XCAMERA_KDEPTH_REALSENSE_HPP

namespace xk
{
    class xkcamera_depth_realsense : public xkcamera_depth
    {

        public:
            xkcamera_depth_realsense();
            virtual ~xkcamera_depth_realsense();

            //base class implementations.. 
            double      get_depth_cm(uint32_t x, uint32_t y);
            void        update_frames();
        
        private:
            rs2::pipeline   _pipe;
            rs2::config     _pipe_config;
            
            std::shared_ptr<rs2::depth_frame> _frame_depth;
            
    };
}

#endif /* XCAMERA_KDEPTH_REALSENSE_HPP */
