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

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

namespace tc = terraclear;

//inches between calibration points.
#define CAL_INCHES 60.0f

//Capture mouse click position..
cv::Point _clickpos1(0,0);
cv::Point _clickpos2(0,0);
cv::Point _mousepos1(0,0);
bool _first = false;

float degtorad(float degrees)
{
    return degrees * (180.0/3.141592653589793238463);
}

float radtodeg(float radians)
{
    return radians * (180.0/3.141592653589793238463);
}

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
         //set measure point where mouse was clicked..
     }
     else if  ( event == cv::EVENT_MBUTTONDOWN )
     {
     }
     else if ( event == cv::EVENT_MOUSEMOVE )
     {
        _mousepos1 = cv::Point(x,y);
     }
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
  //  cv::resizeWindow(window_name, 1024, 768);
    
    //set the callback function for all mouse events
    cv::setMouseCallback(window_name, mousecallback, NULL);

    //get first frames
    cam->update_frames();
    
    //Image ref.
    cv::Mat cam_img = cam->getRGBFrame();

    //initial position is center of image 
    cv::Point center_point = _clickpos1 = _clickpos2 = _mousepos1 = cv::Point(cam_img.cols / 2, cam_img.rows / 2);

   do
    {
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
            //intersect to where image center point
            //i.e. this gives us the camera position on camera plane.
            float center_c = c1 / cos(cal_angle);
            
            //calculate distance from camera (perpendicular ground) 
            //to image center point. camera height above ground
            //can also be inferred from this.
            float cam_a = c1 * cos(cal_angle);
            
            //calculate OFFSET on ground from where camera plane & 
            //ground intersect to where camera is perpendicular to ground.
            float cal_offset = center_c - cam_a;

            //calculate true distance on ground from camera and mouse point.
            float mouse_c = mouse_distance / cos(cal_angle);
            float mouse_corrected = mouse_c - cal_offset;
            
            //Draw distance text
            std::stringstream strstrm;
//            strstrm << "d1:" << std::fixed << std::setprecision(2) << d1 << "\"";
//            strstrm << ",d2:" << std::fixed << std::setprecision(2) << d2 << "\"";
            strstrm << "m:" << std::fixed << std::setprecision(2) << mouse_distance  << "\"";
            strstrm << ",mc:" << std::fixed << std::setprecision(2) << mouse_corrected  << "\"";
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
        
        //show image
        cv::imshow(window_name, cam_img);

        //30 fps roughly
        int x = cv::waitKey(10);
        if (x > 0)
        {
            cout << "EXIT: " << x << endl ;
            break;
        }
     
        //update image
        cam->update_frames();
        
   } while (true);

    

    return 0;
}

