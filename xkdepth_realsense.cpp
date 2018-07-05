/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkdepth_realsense.cpp
 * Author: koos
 * 
 * Created on July 5, 2018, 12:29 PM
 */

#include "xkdepth_realsense.hpp"

namespace xk
{
    xkdepth_realsense::xkdepth_realsense() 
    {
        //enable RGB and Depth streams.
        _pipe_config.enable_stream(RS2_STREAM_COLOR, 1920,1080,RS2_FORMAT_BGR8, 15);
        _pipe_config.enable_stream(RS2_STREAM_DEPTH, 1280,720,RS2_FORMAT_ANY, 15);
        
        _pipe.start(_pipe_config);
        
    }

    xkdepth_realsense::~xkdepth_realsense() 
    {
        _pipe.stop();
    }
    
    uint32_t xkdepth_realsense::get_depth_cm(uint32_t x, uint32_t y)
    {
        return 0;
    }
    
    cv::Mat xkdepth_realsense::get_frame_color()
    {
        cv::Mat retval;
        
                    // get the data for the frame and display the image
            // gathers the rgb image and the depth camera image
            rs2::frameset frames = _pipe.wait_for_frames();
            
            // align depth to the color stream
            // todo: figure out why color stream is 640 by 480
            rs2::align align(RS2_STREAM_COLOR);
            
            rs2::frameset processed = align.process(frames);
            
            // get the data for the frame and display the image
            //cur_depth_frame = std::make_shared<rs2::depth_frame>(processed.first(RS2_STREAM_DEPTH));    
            
            rs2::video_frame cur_color_frame(processed.get_color_frame());
            
            retval = cv::Mat( cur_color_frame.get_height(), cur_color_frame.get_width(), CV_8UC3, (uchar *) cur_color_frame.get_data() );
            
        return retval;
    } 
}
