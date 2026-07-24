#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // Проверка аргумента
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <cascade_xml_file>\n";
        return 1;
    }

    // Загрузка каскада
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(argv[1])) {
        std::cerr << "Failed to load cascade file\n";
        return 1;
    }

    // Открытие камеры
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera\n";
        return 1;
    }

    cv::Mat frame, gray;
    double fps = 0;
    cv::TickMeter tm;

    while (true) {
        tm.start();
        cap >> frame;
        if (frame.empty()) break;

        // Конвертация и улучшение контраста
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // Обнаружение лиц
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 5);

        // Отображение результатов
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        // Подсчет FPS
        tm.stop();
        fps = 1.0 / tm.getTimeSec();
        tm.reset();

        cv::putText(frame, "FPS: " + std::to_string(static_cast<int>(fps)),
                   cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

        cv::imshow("Face Detection", frame);
        if (cv::waitKey(1) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}