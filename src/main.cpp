#include <iostream>
#include <opencv2/opencv.hpp>

// Real-time face detection from the default webcam using a Haar cascade
// classifier. The cascade XML file is supplied as a command-line argument,
// so any cascade shipped with OpenCV (frontal face, profile face, eye, ...)
// can be used without changing this code.
int main(int argc, char** argv) {
    // Exactly one argument is expected: the path to the cascade XML file.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <cascade_xml_file>\n";
        return 1;
    }

    // Load the trained cascade. This fails if the path is wrong or the file
    // is not a valid OpenCV cascade description.
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(argv[1])) {
        std::cerr << "Failed to load cascade file\n";
        return 1;
    }

    // Open the default camera (device index 0).
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera\n";
        return 1;
    }

    // Buffers are declared outside the loop so they are reallocated only
    // when the frame size actually changes.
    cv::Mat frame, gray;
    double fps = 0;
    cv::TickMeter tm;  // measures the wall-clock time of a single iteration

    while (true) {
        tm.start();
        cap >> frame;
        if (frame.empty()) break;  // camera disconnected or end of stream

        // Haar features work on intensity only, so drop the colour channels,
        // then equalise the histogram to make detection more robust against
        // backlighting and uneven illumination.
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // Scan the image at increasing scales.
        // scaleFactor = 1.1  -> the image shrinks by 10% on every pass;
        // minNeighbors = 5   -> a region needs five overlapping detections
        //                       to be reported, which filters out most
        //                       false positives.
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 5);

        // Draw the detections on the original colour frame.
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        // Derive the frame rate from the duration of this iteration and reset
        // the meter so the next frame is timed independently.
        tm.stop();
        fps = 1.0 / tm.getTimeSec();
        tm.reset();

        cv::putText(frame, "FPS: " + std::to_string(static_cast<int>(fps)),
                   cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

        cv::imshow("Face Detection", frame);
        // waitKey also pumps the GUI event loop; 27 is the ESC key code.
        if (cv::waitKey(1) == 27) break;
    }

    // Release the camera and close the display window.
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
