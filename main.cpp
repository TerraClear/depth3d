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
*/

#include <cstdlib>
#include <iostream>

#include "xkcamera_depth_realsense.hpp"
#include "xkcamera_usb.hpp"
#include "xkcamera_file.hpp"
#include "xkcamera_depth_zed.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;


//Capture mouse click position..

cv::Point _clickpos(0,0);

void mousecallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
         //set measure point where mouse was clicked..
            _clickpos = cv::Point(x,y);
     }
     else if  ( event == cv::EVENT_RBUTTONDOWN )
     {
     }
     else if  ( event == cv::EVENT_MBUTTONDOWN )
     {
     }
     else if ( event == cv::EVENT_MOUSEMOVE )
     {

     }
}

int main(int argc, char** argv) 
{
    //testing realsense cam implementation..
    xk::xkcamera_depth_realsense realsense;

    //testing ZED cam implementation..
    //xk::xkcamera_depth_zed zed(0, xk::CameraPosition::Left);

    //testing regular usb cam implementation..
  //  xk::xkcamera_usb usbcam(3, 1280, 720);

    //testing video file implementation..
  //  xk::xkcamera_file filecam("/home/koos/Downloads/rocks4.mp4");
    
    
    //cast to camera
    xk::xkcamera* cam = &realsense;
//    xk::xkcamera* cam = &zed;
//    xk::xkcamera* cam = &usbcam;
//    xk::xkcamera* cam = &filecam;


    
    // ----
    
    //cast to 3d depth cam if possible
    xk::xkcamera_depth* depthcam = dynamic_cast<xk::xkcamera_depth*>(cam);

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
   _clickpos = cv::Point(cam_img.cols / 2, cam_img.rows / 2);

   do
    {
        //circle around target area..
        cv::circle(cam_img, _clickpos, 10, cv::Scalar(0x00, 0x00, 0xff), 2);

        //get the mean distance of a X*Y square
        double distance = 0; 
        if (has3d) //only get distance of camera supports 3D funcions..
        {

            distance = depthcam->get_depth_cm(_clickpos.x, _clickpos.y);       

            //Draw distance text
            std::stringstream strstrm;
//            strstrm << std::fixed << std::setprecision(2) << distance << " cm";
            strstrm << std::fixed << std::setprecision(2) << (uint32_t) distance << "\"";
            putText(cam_img, strstrm.str(), cv::Point(50,125), cv::FONT_HERSHEY_PLAIN, 4,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        }
        else
        {
            //Draw distance text
            std::string no3d = "[NO 3D CAM]";
            putText(cam_img, no3d.c_str(), cv::Point(50,125), cv::FONT_HERSHEY_PLAIN, 4,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        }
        
        //show image
        cv::imshow(window_name, cam_img);

        //30 fps roughly
        int x = cv::waitKey(33);
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

