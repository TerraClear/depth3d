/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkdepth.cpp
 * Author: koos
 * 
 * Created on July 5, 2018, 12:23 PM
 */

#include "xkdepth.hpp"
namespace xk
{
    xkdepth::xkdepth() 
    {
    }

    xkdepth::~xkdepth() 
    {
    }
    
    double xkdepth::get_depth_inches(uint32_t x, uint32_t y)
    {
        //cm to inches
        return get_depth_cm(x, y) * 0.393701;
    }
}
