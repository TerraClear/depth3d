#ifndef DETECTOR_BASE_HPP
#define DETECTOR_BASE_HPP

#include <vector>

//OPENCV
#include <opencv2/opencv.hpp>

namespace terraclear
{
    struct bounding_box : cv::Rect
    {
        float confidence;                 
        unsigned int class_id;        
        unsigned int track_id;        
        unsigned int frame_count; 
    };

    class detector_base 
    {
        public:

            detector_base(cv::Mat imgsrc);
            virtual ~detector_base();

            //pure virtual 
            virtual std::vector<bounding_box> detect_objects() = 0;

       
        protected:
            cv::Mat _imgsrc;
            void mergeBoundingBoxes(std::vector<bounding_box> &object_boxes);
           
    };

}
#endif /* DETECTOR_BASE_HPP */

