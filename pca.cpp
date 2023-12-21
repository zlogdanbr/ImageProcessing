#include "pca.h"

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
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // Convert image to binary
    Mat bw;
    threshold(gray, bw, 50, 255, THRESH_BINARY | THRESH_OTSU);
    // Find all the contours in the thresholded image
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate the area of each contour
        double area = contourArea(contours[i]);
        // Ignore contours that are too small or too large
        if (area < 1e2 || 1e5 < area) continue;
        // Draw each contour only for visualisation purposes  
        
        if (opt == 0)
        {
            drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
        }

        // Find the orientation of each shape
        if (opt != 0)
        {
            getOrientation(contours[i], src);
        }
        
    }

    return src;
}

Mat ApplyPCA(const Mat& img)
{
    return ApplyBasicSegmentation(img,1);
}

Mat ApplyFindContourns(const Mat& img)
{
    return ApplyBasicSegmentation(img);
}


/**-----------------------------------------------------------------------------
*       Given a set of counters it returns the eigenspace of them
*       which is the set of all
*       eigenvectors
*       eigenvalues
*       centers
* 
*       INPUT  param: countours of a image img
*       OUTPUT param: centers of all pair of eigenvector and eigenvalues
*       OUTPUT einSpace representing all eigenvectors/eigenvalues
*--------------------------------------------------------------------------------
*/
eigenSpace getEingenSpace(const contourns& contours, centers& c)
{
    eigenSpace espace;
    eigenvectors evctors;
    eigenvalues evalues;

    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<Point> pts = contours[i];
        //Construct a buffer used by the pca analysis
        int sz = static_cast<int>(pts.size());
        Mat data_pts = Mat(sz, _dim2D, CV_64F);
        for (int i = 0; i < data_pts.rows; i++)
        {
            data_pts.at<double>(i, 0) = pts[i].x;
            data_pts.at<double>(i, 1) = pts[i].y;
        }

        //Perform PCA analysis
        PCA pca_analysis(data_pts, Mat(), PCA::DATA_AS_ROW);

        eigenvector eigen_vecs(_dim2D);
        eigenvalue eigen_val(_dim2D);
        for (int i = 0; i < _dim2D; i++)
        {
            eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                    pca_analysis.eigenvectors.at<double>(i, 1));
            eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
        }

        center _c(static_cast<int>(pca_analysis.mean.at<double>(0, 0)), static_cast<int>(pca_analysis.mean.at<double>(0, 1)));
        c.push_back(_c);
        evctors.push_back(eigen_vecs);
        evalues.push_back(eigen_val);
    }

    espace.first = evctors;
    espace.second = evalues;

    return espace;
}