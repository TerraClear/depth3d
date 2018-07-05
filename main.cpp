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
        cv::imshow(window_name, rls.get_frame_color());

        int x = cv::waitKey( 100 );
        if (x > 0)
        {
            cout << "EXIT: " << x << endl ;
            break;
        }
        
    }
    

    return 0;
}

