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
#ifndef TC_USE_REALSENSE
#define TC_USE_REALSENSE
#endif

#include "libterraclear/src/camera_depth_realsense.hpp"
#include "libterraclear/src/camera_usb.hpp"
#include "libterraclear/src/camera_file.hpp"
#include "libterraclear/src/camera_depth_zed.hpp"
#include "libterraclear/src/detector_hsv.hpp"

//OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

//RLS
#include <librealsense2/rs.hpp>


namespace tc = terraclear;

//inches between calibration points.
#define CAL_INCHES 72.0f

//Capture mouse position..
cv::Point _mousepos1(0, 0);
bool _first = false;
bool _firstrange = false;

int trigger_line_offset = 1024.0f;
void mousecallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) 
    {
        trigger_line_offset = y;
    } 
    else if (event == cv::EVENT_RBUTTONDOWN) 
    {

    } 
    else if (event == cv::EVENT_MBUTTONDOWN) 
    {
    } 
    else if (event == cv::EVENT_MOUSEMOVE) 
    {
        _mousepos1 = cv::Point(x, y);
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


    //cast to camera_base to test base classing and virtual functions.
    tc::camera_base* cam = &realsense;
    //    tc:camera_base* cam = &zed;

    
    //cast to 3d depth cam if possible
    tc::camera_depth* depthcam = dynamic_cast<tc::camera_depth*> (cam);

    //check if we have 3D camera functions..
    bool has3d = (depthcam == nullptr) ? false : true;

    //Open CV Window 
    std::string window_rgb = "rgb";
    cv::namedWindow(window_rgb, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO); // | WINDOW_AUTOSIZE);

    //depth window
    std::string window_depth = "depth";
    cv::namedWindow(window_depth, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO); // | WINDOW_AUTOSIZE);

    //get first frames
    cam->update_frames();

    //Image back buffers
    cv::Mat cam_raw = cam->getRGBFrame();
    cv::Mat depth_raw = depthcam->getDepthFrame();

    //front buffers.
    cv::Mat cam_img;
    cv::Mat depth_img;

    //copy to front buffers..
    cam_raw.copyTo(cam_img);
    depth_raw.copyTo(depth_img);

    //set the callback function for all mouse events
    cv::setMouseCallback(window_rgb, mousecallback, (void*) &cam_img);

    //initial position is center of image 
    _mousepos1 = cv::Point(cam_img.cols / 2, cam_img.rows / 2);

    //config values
    double cam_height_above_ground = 44.0f;
    
    do {

        if (has3d) //only get distance of camera supports 3D funcions.
        {
            double ground_angle_rad = depthcam->get_angle_center_rad(depthcam->inches_to_cm(cam_height_above_ground));

            cv::Point a(cam_img.cols / 2, trigger_line_offset);
            cv::Point b(cam_img.cols / 2, _mousepos1.y);
            
            //distance between trigger & object in centimeters..
            double trigger_to_obj_distance_inches = depthcam->cm_to_inches(depthcam->get_distance_over_ground_cm(a, b, ground_angle_rad));
            
            //Draw distance text
            std::stringstream strstrm;
            strstrm << "distance:" << std::fixed << std::setprecision(2) << trigger_to_obj_distance_inches << "\"";
            putText(cam_img, strstrm.str(), cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(0x00, 0x00, 0xff), 2);
            
        } else 
        {
            //Draw distance text
            std::string no3d = "[NO 3D CAM]";
            putText(cam_img, no3d.c_str(), cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(0x00, 0x00, 0xff), 2);
        }

        // Draw mouse intersect lines.
        cv::line(cam_img, cv::Point(cam_img.cols / 2, 0), cv::Point(cam_img.cols / 2, cam_img.rows),
                cv::Scalar(0x00, 0xff, 0x00), 2, cv::LINE_8, 0);

        cv::line(cam_img, cv::Point(0, _mousepos1.y), cv::Point(cam_img.cols, _mousepos1.y),
                cv::Scalar(0x00, 0xff, 0x00), 2, cv::LINE_8, 0);

        //Draw line for for trigger 
        cv::line(cam_img, cv::Point(0, trigger_line_offset), cv::Point(cam_img.cols, trigger_line_offset),
                cv::Scalar(0x00, 0xff, 0xff), 2, cv::LINE_8, 0);

        //circle around target areas..      
        cv::circle(cam_img, _mousepos1, 8, cv::Scalar(0x00, 0xff, 0x00), 2);
        
        //show image
        cv::imshow(window_rgb, cam_img);
        cv::imshow(window_depth, depth_img);

        int x = cv::waitKey(33);
        if (x == 27) // ESC Key = exit
        {
            break;
        } else if (x == 115) {
            //s for SAVE

            //Save frame..
            std::string filename = "frame-grab.jpg";
            cv::imwrite(filename, cam_raw);
        }
        else if (x > 0) {
            std::cout << x << std::endl;
        }

        //update image frame
        cam->update_frames();
        cam_raw .copyTo(cam_img);
        depth_raw.copyTo(depth_img);


    } while (true);



    return 0;
}

