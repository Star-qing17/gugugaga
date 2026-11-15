// 培训4：相机内参标定与畸变矫正（OpenCV 张正友标定法）
// 用法: ./calib_demo [标定图目录] [输出目录] [最多使用的有效图片数]
// 默认: ./calib_demo ./cam_mix_img . 90
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    const int kCols = 10;         // 棋盘格内角点列数（11 个方格 -> 10 个内角点）
    const int kRows = 7;          // 棋盘格内角点行数（8 个方格 -> 7 个内角点）
    const float kSquare = 0.015f; // 格子边长，单位米（标定板标注 15mm）

    const std::string img_dir = argc > 1 ? argv[1] : "./cam_mix_img";
    const std::string out_dir = argc > 2 ? argv[2] : ".";
    const size_t max_valid = argc > 3 ? std::stoul(argv[3]) : 90;

    std::vector<cv::String> files;
    cv::glob(img_dir + "/*.jpg", files, false);
    if (files.empty()) {
        std::cerr << "未找到图片: " << img_dir << std::endl;
        return -1;
    }

    std::vector<cv::Point3f> object_point;
    for (int r = 0; r < kRows; ++r)
        for (int c = 0; c < kCols; ++c)
            object_point.emplace_back(c * kSquare, r * kSquare, 0.0f);

    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    cv::Size image_size;

    for (const auto& f : files) {
        cv::Mat img = cv::imread(f);
        if (img.empty()) continue;
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        if (!cv::findChessboardCorners(gray, cv::Size(kCols, kRows), corners)) continue;

        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
                                          30, 1e-3));
        object_points.push_back(object_point);
        image_points.push_back(corners);
        image_size = gray.size();

        if (object_points.size() >= max_valid) break;
    }

    if (object_points.size() < 5) {
        std::cerr << "有效图片太少: " << object_points.size() << std::endl;
        return -1;
    }

    cv::Mat K, D;
    std::vector<cv::Mat> rvecs, tvecs;
    double rms = cv::calibrateCamera(object_points, image_points, image_size, K, D, rvecs, tvecs);

    std::cout << "used images: " << object_points.size() << " / " << files.size() << std::endl;
    std::cout << "reprojection rms = " << rms << " px" << std::endl;
    std::cout << "camera matrix K = \n" << K << std::endl;
    std::cout << "distortion D = " << D.t() << std::endl;

    // 畸变矫正：原图与矫正结果并排保存
    cv::Mat sample = cv::imread(files[files.size() / 2]);
    cv::Mat newK = cv::getOptimalNewCameraMatrix(K, D, image_size, 1, image_size);
    cv::Mat undistorted;
    cv::undistort(sample, undistorted, K, D, newK);

    cv::putText(sample, "before", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.9,
                cv::Scalar(0, 0, 255), 2);
    cv::putText(undistorted, "after", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.9,
                cv::Scalar(0, 255, 0), 2);
    cv::Mat compare;
    cv::hconcat(sample, undistorted, compare);
    const std::string out_file = out_dir + "/undistort_result.jpg";
    cv::imwrite(out_file, compare);
    std::cout << "saved: " << out_file << std::endl;

    return 0;
}
