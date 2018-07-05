/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkdepth.hpp
 * Author: koos
 *
 * Created on July 5, 2018, 12:23 PM
 */


#ifndef XKDEPTH_HPP
#define XKDEPTH_HPP

#include <cstdint>
#include <opencv2/opencv.hpp>


namespace xk
{
    class xkdepth 
    {
        public:
            xkdepth();
            virtual ~xkdepth();

            //pure virtual function for acquiring depth (Z) and specific X Y  
            virtual uint32_t    get_depth_cm(uint32_t x, uint32_t y) = 0;
            virtual cv::Mat     get_frame_color() = 0;

        protected:

        private:

    };    
}


#endif /* XKDEPTH_HPP */

