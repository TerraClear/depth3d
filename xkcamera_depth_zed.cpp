
#include "xkcamera_depth_zed.hpp"

namespace xk
{

    xkcamera_depth_zed::xkcamera_depth_zed(uint32_t camera_index, CameraPosition selected_cam) 
    {
        _selected_camera = selected_cam;
        if(!_videofeed.open(camera_index))
        {
            //TODO: Add custom exception handlers..
            throw std::exception();
        }
        
        //Set Camera Resolution
        _videofeed.set(3, ZED_RESX);
        _videofeed.set(4, ZED_RESY);

    }

    xkcamera_depth_zed::~xkcamera_depth_zed() 
    {
        _videofeed.release();
    }

    void xkcamera_depth_zed::update_frames()
    {
        //get next frame;
        cv::Mat video_frame;
        _videofeed >> video_frame;
        

        //Clip the left or right image if needed..
        cv::Rect rect_roi(0, 0, video_frame.cols, video_frame.rows);
        
        if (_selected_camera == CameraPosition::Left)
            rect_roi = cv::Rect(0, 0, video_frame.cols / 2, video_frame.rows);
        else if (_selected_camera == CameraPosition::Right)
            rect_roi = cv::Rect(video_frame.cols / 2, 0, video_frame.cols, video_frame.rows);
            
        video_frame(rect_roi).copyTo(_frame_color);
        
        
    }

    double xkcamera_depth_zed::get_depth_cm(uint32_t x, uint32_t y)
    {
        return 0;
    }    
    
}