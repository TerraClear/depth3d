/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xkdepth.hpp
 * Author: koos
 *
 * Created on July 5, 2018, 12:23 PM
 */


#ifndef XKDEPTH_HPP
#define XKDEPTH_HPP
namespace xk
{
    class xkdepth 
    {
        public:
            xkdepth();
            virtual ~xkdepth();

        protected:
            //pure virtual function for acquiring depth (Z) and specific X Y  
            virtual void get_depth_meters() = 0;

        private:

    };    
}


#endif /* XKDEPTH_HPP */

