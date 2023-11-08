#ifndef _CVWDMOS_
#define _CVWDMOS_

#include "opcvwrapper.h"


#define image_path_eu 		"C:\\Users\\Administrador\\Documents\\GitHub\\images\\data\\eu.jpg"
#define image_path_faces 	"C:\\sers\\Administrador\\Documents\\GitHub\\images\\data\\f.jpg"


void FindAndDrawCircles(const std::string& image_path);
void FindAndDrawRectangles(const std::string& image_path);
void FindAndDFacesAndEyes(const std::string& image_path);
void FindAndDFacesAndEyes(Mat& img);
void segmentImage(const std::string& image_path);
/**
* ImageAdjustmentsTesting
* it tests adjusments in a photo specified by the path image_path
*/
void ImageAdjustmentsTesting(const std::string& image_path);
/**
* detecting
* Finds edges in a poto specified by the path image_path using the laplacian
* algorithm.
*/
void FindEdgesOnPhoto(const std::string& image_path);
void ApplyCustomFilter(const std::string& image_path);
void FindTheCountours(const std::string& image_path);

#endif