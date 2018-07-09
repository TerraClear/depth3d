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

/*
 * 
 */
int main(int argc, char** argv) 
{
    xk::xkdepth_realsense rls;
    
    std::string window_name = "rgb";
    cv::namedWindow(window_name, 0 );
    cv::resizeWindow(window_name, 980, 540);

    while (true)
    {
        rls.update_frames();
        
        double distance = rls.get_depth_inches(rls._frame_color.cols / 2, rls._frame_color.rows / 2);       

        //Draw distance..
        char str[12];
        sprintf(str,"%.2f\"\0", distance);
        putText(rls._frame_color, str, cv::Point(50,125), cv::FONT_HERSHEY_PLAIN, 4,  cv::Scalar(0x00, 0x00, 0xff), 2);                                    
        
        //circle around target area..
        cv::circle(rls._frame_color, cv::Point(rls._frame_color.cols / 2, rls._frame_color.rows / 2), 10, cv::Scalar(0x00, 0x00, 0xff), 2);

        cv::imshow(window_name, rls._frame_color);

        int x = cv::waitKey(1 );
        if (x > 0)
        {
            cout << "EXIT: " << x << endl ;
            break;
        }
        
    }
    

    return 0;
}

