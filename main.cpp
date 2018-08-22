/*
 * Basic driver app for 3D camera
 * 
 * Copyright (C) 2017 TerraClear, Inc.
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
 * 
 * CREATED BY: Koos du Preez - koos@terraclear.com
 * 
*/

#include <cstdlib>
#include <iostream>

//libterraclear
#include "camera_depth_realsense.hpp"
#include "camera_usb.hpp"
#include "camera_file.hpp"
#include "camera_depth_zed.hpp"
#include "hsvcalibration.hpp"

//OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "detector_hsv.hpp"

namespace tc = terraclear;

//inches between calibration points.
#define CAL_INCHES 72.0f

//Capture mouse click position..
cv::Point _clickpos1(0,0);
cv::Point _clickpos2(0,0);
cv::Point _mousepos1(0,0);
cv::Point _hsvpos1(20,10);
bool _first = false;

bool _firstrange = false;

float degtorad(float degrees)
{
    return degrees * (180.0/3.141592653589793238463);
}

float radtodeg(float radians)
{
    return radians * (180.0/3.141592653589793238463);
}

/*
void show_hsv(cv::Mat imgsrc, int x, int y)
{
        cv::Mat hsv;
        //blur Image a bit first.
        cv::blur(imgsrc, hsv, cv::Size(20,20));
        /// Transform it to HSV color space
        cv::cvtColor(hsv, hsv, cv::COLOR_BGR2HSV);  

        cv::Vec3b val = hsv.at< cv::Vec3b >(y,x); // Matis row X col matrix

    if (_firstrange)
    {
        _lowrange = cv::Scalar(val.val[0], val.val[1], val.val[2]);
        std::cout << "LOW ";
    }
    else
    {
        _highrange = cv::Scalar(val.val[0], val.val[1], val.val[2]);
        std::cout << "HIGH ";
    }
    
    std::cout << "x=" << x << ", y=" << y << ", H="<< int(val.val[0]) << ", S="<< int(val.val[1]) << ",V="<< int(val.val[2]) << std::endl;
        
    _firstrange = !_firstrange;
    
}
*/

void mousecallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
         if (_first)
            _clickpos1 = cv::Point(x,y);
         else
            _clickpos2 = cv::Point(x,y);
         
         _first = !_first;
     }
     else if  ( event == cv::EVENT_RBUTTONDOWN )
     {
         
     }
     else if  ( event == cv::EVENT_MBUTTONDOWN )
     {
          _hsvpos1 = cv::Point(x,y);
     }
     else if ( event == cv::EVENT_MOUSEMOVE )
     {
        _mousepos1 = cv::Point(x,y);
     }
}

void sliderCallBack(int pos, void* data) 
{


}

int main(int argc, char** argv) 
{
    //testing realsense cam implementation..
    tc::camera_depth_realsense realsense;
    realsense.avgerage_square_pixels = 10;

    //testing ZED cam implementation..
    //tc::camera_depth_zed zed(0, tc::CameraPosition::Left);

    //testing regular usb cam implementation..
  //  tc::camera_usb usbcam(3, 1280, 720);

    //testing video file implementation..
  //  tc::camera_file filecam("/home/koos/Downloads/rocks4.mp4");
    
    
    //cast to camera_base
    tc::camera_base* cam = &realsense;
//    tc:camera_base* cam = &zed;
//    tc::camera_base* cam = &usbcam;
//    tc::camera_base* cam = &filecam;


    
    // ----
    
    //cast to 3d depth cam if possible
    tc::camera_depth* depthcam = dynamic_cast<tc::camera_depth*>(cam);

    //check if we have 3D camera functions..
    bool has3d = (depthcam == nullptr) ? false : true;

    //Open CV Window stuff
    std::string window_name = "rgb";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);// | WINDOW_AUTOSIZE);
//    namedWindow(windowName,  CV_GUI_EXPANDED);
  //  cv::resizeWindow(window_name, 1024, 768);
    
    //get first frames
    cam->update_frames();
    
    //Image ref.
    cv::Mat cam_raw = cam->getRGBFrame();
    
    cv::Mat cam_img;    
    cam_raw.copyTo(cam_img);

    //set the callback function for all mouse events
    cv::setMouseCallback(window_name, mousecallback, (void*) &cam_img);
    
    //initial position is center of image 
    cv::Point center_point = _clickpos1 = _clickpos2 = _mousepos1 = cv::Point(cam_img.cols / 2, cam_img.rows / 2);

    //create HSV based detector
    tc::detector_hsv hsvd(cam_img);
    
    int hlow =  hsvd._lowrange[0];
    int slow = hsvd._lowrange[1];
    int vlow = hsvd._lowrange[2];
    int hhigh = hsvd._highrange[0];
    int shigh = hsvd._highrange[1];
    int vhigh= hsvd._highrange[2];
    
   do
    {
       
       //set low & high HSV ranges.
       hsvd._lowrange = cv::Scalar(hlow, slow, vlow);
       hsvd._highrange = cv::Scalar(hhigh, shigh, vhigh);
       
        //find all blue objects.
       std::vector<tc::bounding_box> blue_balls = hsvd.detect_objects();
       
     //  std::vector<cv::Rect> blue_balls = get_blues_lights(cam_img);
        for (auto bbox : blue_balls)
        {
          cv::rectangle(cam_img, bbox, cv::Scalar(0x00, 0x00, 0xff), 2, cv::LINE_8, 0);
        }
        
        //circle around target areas..      
        cv::circle(cam_img, _clickpos1, 8, cv::Scalar(0x00, 0xff, 0xff), 2);
        cv::circle(cam_img, _clickpos2, 8, cv::Scalar(0x00, 0x00, 0xff), 2);
        cv::circle(cam_img, _mousepos1, 8, cv::Scalar(0x00, 0xff, 0x00), 2);

        float c1, d1, d2, mouse_distance;
        
        if (has3d) //only get distance of camera supports 3D funcions..
        {

            c1 = depthcam->get_depth_inches(center_point.x, center_point.y);
            d1 = depthcam->get_depth_inches(_clickpos1.x, _clickpos1.y);       
            d2 = depthcam->get_depth_inches(_clickpos2.x, _clickpos2.y);    
            mouse_distance = depthcam->get_depth_inches(_mousepos1.x, _mousepos1.y);    
            
            //calculate camera angle from two calibration points.
            float cal_a_diff = (d1 > d2) ? d1 - d2 : d2 - d1;
            float cal_c = CAL_INCHES;
            float cal_angle = acos(cal_a_diff / cal_c); //in RADIANS..
            
            //calculate distance on ground from where camera plane & ground 
            //intersect to where image center point is
            //i.e. this gives us the camera position on camera plane.
            float center_c = c1 / cos(cal_angle);
            
            //calculate distance from camera (perpendicular ground) 
            //to image center point. camera height above ground
            //can also be inferred from this.
            float cam_a = c1 * cos(cal_angle);
            
            //calculate OFFSET on ground from where camera plane & 
            //ground intersect to where camera is perpendicular to ground.
            float cal_offset = center_c - cam_a;

            //calculate distance on ground from where camera plane & ground 
            //intersect to where mouse cursor is.
            float mouse_c = mouse_distance / cos(cal_angle);
            
            //calculate distance from camera (perpendicular ground) 
            //to mouse cursor point. 
            float mouse_ground_distance = mouse_c - cal_offset;            
            
            //Draw distance text
            std::stringstream strstrm;
//            strstrm << "d1:" << std::fixed << std::setprecision(2) << d1 << "\"";
//            strstrm << ",d2:" << std::fixed << std::setprecision(2) << d2 << "\"";
            strstrm << "m:" << std::fixed << std::setprecision(2) << mouse_ground_distance  << "\"";
//            strstrm << ", a: " << std::fixed << std::setprecision(2) << side_a  << "\"";
//            strstrm << ", c: " << std::fixed << std::setprecision(2) << side_c  << "\"";
            putText(cam_img, strstrm.str(), cv::Point(10,50), cv::FONT_HERSHEY_PLAIN, 3,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        }
        else
        {
            //Draw distance text
            std::string no3d = "[NO 3D CAM]";
            putText(cam_img, no3d.c_str(), cv::Point(10,50), cv::FONT_HERSHEY_PLAIN, 3,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        }
        
        //Draw line through center of image to aid "alignment" of calibration device.
        cv::line(   cam_img, cv::Point(cam_img.cols / 2, 0), cv::Point(cam_img.cols / 2, cam_img.rows), 
                    cv::Scalar(0x00, 0xff, 0x00), 3, cv::LINE_8, 0);
        
        //show image
        cv::imshow(window_name, cam_img);

        int x = cv::waitKey(33);
        if(x == 27) // ESC Key = exit
        {
            break;       
        }
        else if (x == 99) //c key = CALIBRATE 
        {
            tc::hsvcalibration hsvcal(hlow, hhigh ,slow, shigh, vlow, vhigh);
            cv::Mat hsvroi = cam_raw(cv::Rect(_hsvpos1.x-20, _hsvpos1.y-10, 40, 20));
            hsvcal.setimage(hsvroi);
            
        }
        else if (x > 0)
        {
            std::cout << x << std::endl ;
        }
     
        //update image
        cam->update_frames();
        cam_raw.copyTo(cam_img);
        
        
   } while (true);

    

    return 0;
}

