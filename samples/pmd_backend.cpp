
#include <ntk/camera/pmd_grabber.h>
#include <ntk/camera/pmd_rgb_grabber.h>
#include <ntk/camera/rgbd_processor.h>
#include <ntk/utils/opencv_utils.h>

#include <QApplication>

using namespace ntk;
using namespace cv;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    PmdGrabber grabber;
    grabber.connectToDevice();
    grabber.start();

    // New opencv window
    namedWindow("color");
    namedWindow("amplitude");
    namedWindow("depth");
    namedWindow("depth_as_color");

    PmdRgbProcessor processor;

    RGBDImage current_frame;
    cv::Mat3b depth_as_color;
    while (true)
    {
        grabber.waitForNextFrame();
        grabber.copyImageTo(current_frame);
        processor.processImage(current_frame);

        int fps = grabber.frameRate();
        cv::putText(current_frame.rgbRef(),
                    cv::format("%d fps", fps),
                    Point(10,20), 0, 0.5, Scalar(255,0,0,255));

        // Display the image
        imshow("color", current_frame.rgb());

        // Show depth as normalized gray scale
        imshow_normalized("depth", current_frame.depth());

        // Compute color encoded depth.
        compute_color_encoded_depth(current_frame.depth(), depth_as_color);
        imshow("depth_as_color", depth_as_color);

        unsigned char c = (cv::waitKey(10) & 0xff);
        if (c == 27)
            break;
    }
}
