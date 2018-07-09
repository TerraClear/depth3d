/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkdepth_realsense.hpp
 * Author: koos
 *
 * Created on July 5, 2018, 12:29 PM
 */

#include <cstdlib>
#include <iostream>

#include "xkdepth.hpp"
#include <librealsense2/rs.hpp>

#ifndef XKDEPTH_REALSENSE_HPP
#define XKDEPTH_REALSENSE_HPP

namespace xk
{
    class xkdepth_realsense : public xkdepth
    {

        public:
            xkdepth_realsense();
            virtual ~xkdepth_realsense();

            //base class implementations.. 
            double      get_depth_cm(uint32_t x, uint32_t y);
            void        update_frames();
        
        private:
            rs2::pipeline   _pipe;
            rs2::config     _pipe_config;
            
            std::shared_ptr<rs2::depth_frame> _frame_depth;
            
    };
}

#endif /* XKDEPTH_REALSENSE_HPP */

