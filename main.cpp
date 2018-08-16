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

namespace tc = terraclear;

//inches between calibration points.
#define CAL_INCHES 72.0f

//Capture mouse click position..
cv::Point _clickpos1(0,0);
cv::Point _clickpos2(0,0);
cv::Point _mousepos1(0,0);
bool _first = false;

bool _firstrange = false;
cv::Scalar _lowrange = cv::Scalar(94,130,190);
cv::Scalar _highrange = cv::Scalar(105,220,250);

float degtorad(float degrees)
{
    return degrees * (180.0/3.141592653589793238463);
}

float radtodeg(float radians)
{
    return radians * (180.0/3.141592653589793238463);
}

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
         //show_hsv(*((cv::Mat*)userdata),x ,y);
     }
     else if ( event == cv::EVENT_MOUSEMOVE )
     {
        _mousepos1 = cv::Point(x,y);
     }
}


void mergeBoundingBoxes(std::vector<cv::Rect> &object_boxes)
{
    //Group neighbouring rectangles
    std::vector<cv::Rect> object_boxes_copy(object_boxes);
    object_boxes.insert(object_boxes.end(), object_boxes_copy.begin(), object_boxes_copy.end());
    cv::groupRectangles(object_boxes, 1, 0.2);
 }



//Find objects in a scene based on specific criteria
//Return contours for objects
std::vector<cv::Rect> findObjectBoundingBoxes(cv::Mat imgsrc, cv::Scalar lowrange, cv::Scalar highrange)
{
    
    
    //ret vector
    std::vector<cv::Rect> ret_vect;
    
    // hard coded color hue range for BLUE
    cv::Scalar lowRange = lowrange;// cv::Scalar(77,201,200);
    cv::Scalar highRange = highrange; //cv::Scalar(81,209,250);
 
    cv::Mat mat_filtered;
    
    //blur Image a bit first.
    cv::blur(imgsrc, mat_filtered, cv::Size(20,20));
    
    /// Transform it to HSV color space
    cv::cvtColor(mat_filtered, mat_filtered, cv::COLOR_BGR2HSV);
    
    //find all objects in range
    cv::inRange(mat_filtered, lowRange, highRange, mat_filtered);
  
  
    //morphological opening (remove small objects from the foreground)
    cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)) );
    cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) ); 

     //morphological closing (fill small holes in the foreground)
    cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) ); 
    cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)) );
    
    //Vector for all contours.
    std::vector<std::vector<cv::Point>> contours;
    findContours(mat_filtered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    
    //create bounding boxes from contours
    for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); it != contours.end(); it++)
    {
        std::vector<cv::Point> contour = *it;
        ret_vect.push_back(cv::boundingRect(cv::Mat(contour)));
    }
    
    //duplicate all rectangles, hack to make every rect at least appear twice
    //so it can be grouped..
    mergeBoundingBoxes(ret_vect);

    
    return ret_vect;
}

// gets the location of all of the blue lights for location of paddles
// there may be more than actually present, so probably should take the largest ones
std::vector<cv::Rect> get_blues_lights(cv::Mat img)
{
    // a collection of all of the blue_lights
    std::vector<cv::Rect> blue_lights;
    cv::Mat hsvimg;
    cv::cvtColor(img, hsvimg, cv::COLOR_BGR2HSV);
    hsvimg.forEach<cv::Vec3b>
    (
      [&](cv::Vec3b &pixel, const void * position) -> void
      {
        if(pixel.val[0]>76 && pixel.val[0]<106 && pixel.val[1] < 55 &&
           pixel.val[2] > 240)
            pixel = cv::Vec3b(120,100,100);
        else
            pixel = cv::Vec3b(0,0,0);
      }
    );
    cv::cvtColor(hsvimg, img, cv::COLOR_HSV2BGR);
    cv::Mat erode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    cv::Mat dilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    // erodes many times so that any small amounts of blue are blasted away to
    // leave behind the large blue spots, may have to adjust the amount of erosion
    // if the objects are further away
    cv::erode(img, img, erode);
    cv::dilate(img,img, dilate);
    cv::Mat gray;
    // contours requires gray scale
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray, contours, CV_RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
    // has the indices of all of the contours used in current rectangles, just
    // used to know when all of the contours have been used
    std::set<int> contours_included;
    // this is purposely skewed for the height, since the objects are expected
    // to be vertical
    int height_valid = 400;
    int width_valid = 100;
    // so that if there are no contours, no seg fault
    if(contours.size() > 0)
    {
        while(contours.size()>contours_included.size())
        {
            std::cout << contours.size() << " " << contours_included.size() << std::endl;
             // find the first element that has not been found yet
            cv::Moments moment;
            if(contours_included.empty())
            {
                moment = cv::moments((cv::Mat)contours[0]);
            }
            else
            {
                for(int i = 0; i< contours.size(); i++)
                {
                    if(contours_included.find(i) == contours_included.end())
                    {
                        moment = cv::moments((cv::Mat)contours[i]);
                        break;
                    }
                }
            }
            // get that first contours place
            cv::Rect_<double> paddle(moment.m10/moment.m00, moment.m01/moment.m00, 10.0,10.0);
            // expands the rectangle to encompass the whole area, the center of the
            // rectangle should be the actual location
            // this for loop is just to grow the rectangle over many increments
            for(int index = 0; index < 5; index++)
            {
                for(int i = 0; i < contours.size(); i++)
                {
                    moment = cv::moments((cv::Mat)contours[i]);
                    double curx = moment.m10/moment.m00;
                    double cury = moment.m01/moment.m00;
                    // has to have less distance away so do squared so no absolute value needed
                    if(curx<paddle.x+paddle.width+width_valid && curx>paddle.x-width_valid &&
                       cury<paddle.y+paddle.height+height_valid && cury>paddle.y-height_valid &&
                       contours_included.find(i) == contours_included.end()) // this is so things already on here are not taken
                    {
                        // so that we know that this is already included in the contours done
                        // otherwise this will infinite loop even after finding all of them
                        contours_included.insert(i);
                        if(paddle.x > curx)
                        {
                            // adds to width since it needs to still extend to
                            // the previous width
                            paddle.width += paddle.x - curx;
                            paddle.x = curx;
                        }
                        else if(paddle.x+paddle.width < curx)
                        {
                            paddle.width = curx-paddle.x;
                        }
                        if(paddle.y > cury)
                        {
                            paddle.height += paddle.y - cury;
                            paddle.y = cury;
                        }
                        else if(paddle.y+paddle.height<cury)
                        {
                            paddle.height = cury-paddle.y;
                        }
                    }
                }
            }
            blue_lights.push_back(paddle);
        }
    }
    return blue_lights;
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
    cv::Mat cam_img = cam->getRGBFrame();

    //set the callback function for all mouse events
    cv::setMouseCallback(window_name, mousecallback, (void*) &cam_img);
    
    //initial position is center of image 
    cv::Point center_point = _clickpos1 = _clickpos2 = _mousepos1 = cv::Point(cam_img.cols / 2, cam_img.rows / 2);

    int hlow = _lowrange[0];
    int slow = _lowrange[1];
    int vlow = _lowrange[2];
    int hhigh = _highrange[0];
    int shigh = _highrange[1];
    int vhigh= _highrange[2];
    
    cv::createTrackbar("HL", window_name, &hlow, 175, sliderCallBack);
    cv::createTrackbar("HH", window_name, &hhigh, 175, sliderCallBack);
    cv::createTrackbar("SL", window_name, &slow, 255, sliderCallBack);
    cv::createTrackbar("SH", window_name, &shigh, 255, sliderCallBack);
    cv::createTrackbar("VL", window_name, &vlow, 255, sliderCallBack);
    cv::createTrackbar("VH", window_name, &vhigh, 255, sliderCallBack);

   do
    {
       
       _lowrange = cv::Scalar(hlow, slow, vlow);
       _highrange = cv::Scalar(hhigh, shigh, vhigh);
       
        //find all blue objects.
       std::vector<cv::Rect> blue_balls = findObjectBoundingBoxes(cam_img, _lowrange, _highrange);
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

        //30 fps roughly
        int x = cv::waitKey(100);
        if (x > 0)
        {
            std::cout << "EXIT: " << x << std::endl ;
            break;
        }
     
        //update image
        cam->update_frames();
        
   } while (true);

    

    return 0;
}

