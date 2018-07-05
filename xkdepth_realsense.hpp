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
    class xkdepth_realsense : xkdepth
    {

        public:
            xkdepth_realsense();
            virtual ~xkdepth_realsense();

            //base class implementations.. 
            uint32_t    get_depth_cm(uint32_t x, uint32_t y);
            cv::Mat     get_frame_color();
        
        private:
            rs2::pipeline   _pipe;
            rs2::config     _pipe_config;


    };
}

#endif /* XKDEPTH_REALSENSE_HPP */

