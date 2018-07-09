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
    
    double xkdepth_realsense::get_depth_cm(uint32_t x, uint32_t y)
    {
        float yscale = _frame_depth->get_height() / _frame_color.rows;
        float xscale = _frame_depth->get_width() / _frame_color.cols;
        
        //get distance, factor in scale between depth & color frames.
        return _frame_depth->get_distance(x * xscale , y * yscale) * 100.00f;
    }
    
    void xkdepth_realsense::update_frames()
    {
        // gathers the rgb image and the depth camera image
        rs2::frameset frames = _pipe.wait_for_frames();

        // align depth to the color stream
        rs2::align align(RS2_STREAM_COLOR);

        rs2::frameset processed = align.process(frames);

        // get the data for the frame and display the image
        _frame_depth = std::make_shared<rs2::depth_frame>(processed.first(RS2_STREAM_DEPTH));

        //grab RGB frame..
        rs2::video_frame cur_color_frame(processed.get_color_frame());

        //convert to cv Mat
        _frame_color = cv::Mat( cur_color_frame.get_height(), cur_color_frame.get_width(), CV_8UC3, (uchar *) cur_color_frame.get_data() );
            
    } 
}
