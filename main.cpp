/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: koos
 *
 * Created on July 5, 2018, 12:04 PM
 */

#include <cstdlib>
#include <iostream>

#include "xkdepth_realsense.hpp"
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
    xk::xkdepth_realsense rls;

    
    //create 3d depth obj.
    xk::xkdepth* depthcam = &rls;
    
    //Open CV Window stuff
    std::string window_name = "rgb";
    cv::namedWindow(window_name, 0 );
    cv::resizeWindow(window_name, 980, 540);
    
    //set the callback function for all mouse events
    cv::setMouseCallback(window_name, mousecallback, NULL);

    //get first frame
    depthcam->update_frames();

    //center of image to get depth..
   _clickpos = cv::Point(depthcam->_frame_color.cols / 2, depthcam->_frame_color.rows / 2);


   do
    {
        //circle around target area..
        cv::circle(depthcam->_frame_color, _clickpos, 10, cv::Scalar(0x00, 0x00, 0xff), 2);

        //get the distance
        double distance = depthcam->get_depth_inches(_clickpos.x, _clickpos.y);       

        //Draw distance text
        char str[12];
        memset(&str, 0x00, sizeof(str));
        
        sprintf(str,"%.2f\"", distance);
        putText(depthcam->_frame_color, str, cv::Point(50,125), cv::FONT_HERSHEY_PLAIN, 4,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        
        //show image
        cv::imshow(window_name, depthcam->_frame_color);

        //30 fps roughly
        int x = cv::waitKey(33);
        if (x > 0)
        {
            cout << "EXIT: " << x << endl ;
            break;
        }
     
        //update camera frames..
        depthcam->update_frames();
        
   } while (true);

    

    return 0;
}

