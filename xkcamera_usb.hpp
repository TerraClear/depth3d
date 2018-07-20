/*
 * Specific implementation for USB camera class 
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

#ifndef XKCAMERA_USB_HPP
#define XKCAMERA_USB_HPP

#include "xkcamera.hpp"

namespace xk
{  
    class xkcamera_usb : public xkcamera
    {
        public:
            xkcamera_usb(uint32_t camera_index);
            xkcamera_usb(uint32_t camera_index, uint32_t resolutionX, uint32_t resolutionY);
            virtual ~xkcamera_usb();
            
            //pure virtual implementation..
            void update_frames();
                        
        private:
            cv::VideoCapture _videofeed;

    };
}
#endif /* XKCAMERA_USB_HPP */

