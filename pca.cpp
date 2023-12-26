#include "image_interest_points.h"



/*
*  Calculates the PCA object from a contour
*/
PCA getPCAAnalysis(const std::vector<Point>& pts)
{
    //Construct a buffer used by the pca analysis
    int sz = static_cast<int>(pts.size());
    Mat data_pts = Mat(sz, 2, CV_64F);
    for (int i = 0; i < data_pts.rows; i++)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }

    //Perform PCA analysis
    PCA pca_analysis(data_pts, Mat(), PCA::DATA_AS_ROW);
    return pca_analysis;
}

double calculateDistance2D(eigenvector& e)
{
    double _d = pow(e[0].x - e[1].x,2)+ pow(e[0].y - e[1].y, 2);
    return sqrt(_d);
}

double Angle2D(eigenvector& e)
{
    double n1 = calculateNorm2D(e[0]);
    double n2 = calculateNorm2D(e[1]);
    double cos_theta = dotProduct2D(e[0], e[1])/(n1 * n2);
    return acos(cos_theta);
}

int dotProduct2D(Point2d& v1, Point2d& v2)
{
    return(v1.x * v2.x + v1.y * v2.y);
}

double calculateNorm2D(Point2d& v1)
{
    double _p1 = pow(v1.x, 2) + pow(v1.y, 2);
    return sqrt(_p1);
}

/*
*   Calculates the eigenvector and eingenvalue for a contourn
*/
PointValue getEingenFromContourn(const std::vector<Point>& pts, Mat& img, center& c)
{

    //Perform PCA analysis
    PCA pca_analysis = getPCAAnalysis(pts);


    //Store the center of the object
    Point cent = Point(     static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                            static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

    c.first = cent.x;
    c.second = cent.y;

    std::vector<Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);

    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = Point2d(    pca_analysis.eigenvectors.at<double>(i, 0),
                                    pca_analysis.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
    }

    PointValue p;

    p.first = eigen_vecs;
    p.second = eigen_val;

    return p;

}

/*
* Calculates the space of eingenvectors,  eingenvealues and centers for all contourns
*/
eigenSpace getEingenSpace(  const contourns& contours, 
                            Mat& src, 
                            centers& _centers)
{
    eigenSpace espace;
    eigenvectors evctors;
    eigenvalues evalues;

    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate the area of each contour
        double area = contourArea(contours[i]);
        // Ignore contours that are too small or too large
        if (area < 1e2 || 1e5 < area) continue;

        center c;
        PointValue p = getEingenFromContourn(contours[i], src,c);

        evctors.push_back(p.first);
        evalues.push_back(p.second);
        _centers.push_back(c);
    }

    espace.first = evctors;
    espace.second = evalues;
    return espace;
}


/*
*   Get All the contours of a image
*/
std::vector<std::vector<Point> > getContourns(const Mat& img)
{
    Mat src = img.clone();
    // Convert image to grayscale
    Mat gray;

    if (isGrayScaleImage(src) == false)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src.clone();
    }

    // Convert image to binary
    Mat bw;
    threshold(gray, bw, 50, 255, THRESH_BINARY | THRESH_OTSU);
    // Find all the contours in the thresholded image
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, RETR_CCOMP, CHAIN_APPROX_NONE);

    return contours;
}



/*
*   Prints all the relevant information about contourns 
*/
std::stringstream  getEingenSpaceInfo(const Mat& img)
{
    std::stringstream outinfo;
    //------------------------------------------------------------------------
    //   Step 1 : Find contourns
    //------------------------------------------------------------------------

    std::vector<std::vector<Point> > contours = getContourns(img);

    //------------------------------------------------------------------------
    //  Step 2 : Find centers, Eigenvectors and Eigenvalues
    //------------------------------------------------------------------------
    centers _centers;
    Mat clone = img.clone();
    eigenSpace _espace = getEingenSpace(contours, clone, _centers);

    //------------------------------------------------------------------------
    //  Step 3 : Print values to a stringstream
    //------------------------------------------------------------------------
    outinfo << "---------------------------------------------------------------------" << std::endl;
    outinfo << "Image Components Information" << std::endl;
    eigenvectors evectors = _espace.first;
    eigenvalues  evalues = _espace.second;

    /*
                                Angles& angles,
                            Distances& distances)
    */

    for (int i = 0; i < evectors.size(); i++)
    {
        outinfo << "---------------------------------------------------------------------" << std::endl;
        outinfo << "Eigenvectors of region " << i << std::endl;
        for (const auto evcts : evectors[i])
        {
            outinfo << "\t\t[ " << evcts.x << " , " << evcts.y << " ] ";
        }
        outinfo << "Eigenvalues of region " << i << std::endl;
        for (const auto evalue : evalues[i])
        {
            outinfo << "\t\t[ " << evalue << " ] ";
        }
        outinfo << std::endl;

        outinfo << "Center of region " << i << std::endl;
        outinfo << "\t\t[ " << _centers[i].first << " , " << _centers[i].second << " ]" << std::endl;
    }

    return outinfo;
}