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
#include "xkcamera_depth_realsense.hpp"

namespace xk
{
    xkcamera_depth_realsense::xkcamera_depth_realsense() 
    {
        //enable RGB and Depth streams.
        _pipe_config.enable_stream(RS2_STREAM_COLOR, 1920,1080,RS2_FORMAT_BGR8, 15);
        _pipe_config.enable_stream(RS2_STREAM_DEPTH, 1280,720,RS2_FORMAT_ANY, 15);
        
        _pipe.start(_pipe_config);
    }

    xkcamera_depth_realsense::~xkcamera_depth_realsense() 
    {
        _pipe.stop();
    }
    
    double xkcamera_depth_realsense::get_depth_cm(uint32_t x, uint32_t y)
    {
        float yscale = _frame_depth->get_height() / _frame_color.rows;
        float xscale = _frame_depth->get_width() / _frame_color.cols;
        
        //get distance, factor in scale between depth & color frames.
        return _frame_depth->get_distance(x * xscale , y * yscale) * 100.00f;
    }
    
    void xkcamera_depth_realsense::update_frames()
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