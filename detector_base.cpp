#include "detector_base.hpp"

namespace terraclear
{
    detector_base::detector_base(cv::Mat imgsrc)
    {
        _imgsrc = imgsrc;
    }

    detector_base::~detector_base() 
    {
    }
    

    void detector_base::mergeBoundingBoxes(std::vector<bounding_box> &object_boxes)
    {
        //TODO: FIX clustering bounding boxes that are close and overlap.. 

        //Group neighbouring rectangles
        /*
        std::vector<bounding_box> object_boxes_copy(object_boxes);
        object_boxes.insert(object_boxes.end(), object_boxes_copy.begin(), object_boxes_copy.end());

        cv::groupRectangles(object_boxes, 1, 0.2);
        */
     }    
}