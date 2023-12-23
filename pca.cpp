#include "pca.h"
#include "image_interest_points.h"

void drawAxis(Mat& img, Point p, Point q, Scalar colour, const float scale = 0.2)
{
    double angle = atan2((double)p.y - q.y, (double)p.x - q.x); // angle in radians
    double hypotenuse = sqrt((double)(p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));

    // Here we lengthen the arrow by a factor of scale
    q.x = (int)(p.x - scale * hypotenuse * cos(angle));
    q.y = (int)(p.y - scale * hypotenuse * sin(angle));
    line(img, p, q, colour, 1, LINE_AA);

    // create the arrow hooks
    p.x = (int)(q.x + 9 * cos(angle + CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle + CV_PI / 4));
    line(img, p, q, colour, 1, LINE_AA);
    p.x = (int)(q.x + 9 * cos(angle - CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle - CV_PI / 4));
    line(img, p, q, colour, 1, LINE_AA);
}

double getOrientation(const std::vector<Point>& pts, Mat& img)
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

    //Store the center of the object
    Point cntr = Point(     static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                            static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

    //Store the eigenvalues and eigenvectors

    // NOTE: For practical purposes this is the information we need:
    // the eigen vectors and eingen values
    // these describe the region of the countours previously found

    std::vector<Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);
    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = Point2d(        pca_analysis.eigenvectors.at<double>(i, 0),
                                        pca_analysis.eigenvectors.at<double>(i, 1));
        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
    }

    // Draw the principal components
    circle(img, cntr, 3, Scalar(255, 0, 255), 2);

    Point p1 = cntr + 0.02 * Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]), static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]), static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
    drawAxis(img, cntr, p1, Scalar(0, 255, 0), 1);
    drawAxis(img, cntr, p2, Scalar(255, 255, 0), 5);
    double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x); // orientation in radians
    return angle;
}

Mat ApplyBasicSegmentation(const Mat& img, int opt)
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

    /*
            retrieves only the extreme outer contours. It sets `hierarchy[i][2]=hierarchy[i][3]=-1` for
            all the contours. 
            RETR_EXTERNAL = 0

            retrieves all of the contours without establishing any hierarchical relationships. 
            RETR_LIST = 1

            retrieves all of the contours and organizes them into a two-level hierarchy. At the top
            level, there are external boundaries of the components. At the second level, there are
            boundaries of the holes. If there is another contour inside a hole of a connected component, it
            is still put at the top level. 
            RETR_CCOMP = 2

            retrieves all of the contours and reconstructs a full hierarchy of nested contours.
            RETR_TREE = 3,
            RETR_FLOODFILL = 4 

    */
    findContours(bw, contours, RETR_CCOMP, CHAIN_APPROX_TC89_KCOS);

    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate the area of each contour
        double area = contourArea(contours[i]);
        // Ignore contours that are too small or too large
        if (area < 1e2 || 1e5 < area) continue;
        // Draw each contour only for visualisation purposes  
        
        if (opt == 0 || opt == 3)
        {
            drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
        }

        // Find the orientation of each shape
        if (opt == 3)
        {
            getOrientation(contours[i], src);
        }
        
    }

    return src;
}

Mat ApplyPCA(const Mat& img)
{
    return ApplyBasicSegmentation(img,3);
}

Mat ApplyFindContourns(const Mat& img)
{
    return ApplyBasicSegmentation(img);
}

std::pair<
    std::vector<Point2d>,
    std::vector<double>
         >
getdata(const std::vector<Point>& pts, Mat& img, center& c)
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

    //Store the center of the object
    Point cntr = Point(     static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                            static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

    c.first = cntr.x;
    c.second = cntr.y;
    std::vector<Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);

    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = Point2d(    pca_analysis.eigenvectors.at<double>(i, 0),
                                    pca_analysis.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
    }

    std::pair< std::vector<Point2d>, std::vector<double>> p;

    p.first = eigen_vecs;
    p.second = eigen_val;

    return p;

}

eigenSpace getEingenSpace(const contourns& contours, Mat& src, centers& _centers)
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
        std::pair< std::vector<Point2d>, std::vector<double>> p = getdata(contours[i], src,c);
        evctors.push_back(p.first);
        evalues.push_back(p.second);
        _centers.push_back(c);
    }

    espace.first = evctors;
    espace.second = evalues;
    return espace;
}

std::vector<std::vector<Point> > getCont(const Mat& img)
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
    findContours(bw, contours, RETR_CCOMP, CHAIN_APPROX_TC89_KCOS);

    return contours;
}

std::stringstream  getEingenSpaceInfo(const Mat& img)
{
    std::stringstream outinfo;
    //------------------------------------------------------------------------
    //   Step 1 : Find contourns
    //------------------------------------------------------------------------

    std::vector<std::vector<Point> > contours = getCont(img);

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

    for (int i = 0; i < evectors.size(); i++)
    {
        outinfo << "---------------------------------------------------------------------" << std::endl;
        outinfo << "Eigenvectors of region " << i << std::endl;
        for (const auto evcts : evectors[i])
        {
            outinfo << "[ " << evcts.x << " , " << evcts.y << " ] ";
        }
        outinfo << std::endl;

        outinfo << "Eigenvalues of region " << i << std::endl;
        for (const auto evalue : evalues[i])
        {
            outinfo << "[ " << evalue << " ] ";
        }
        outinfo << std::endl;

        outinfo << "Center of region " << i << std::endl;
        outinfo << "[ " << _centers[i].first << " , " << _centers[i].second << " ]" << std::endl;
    }

    return outinfo;
}