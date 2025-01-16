#include <opencv2/opencv.hpp>

int main() {
    // 读取输入图像
    cv::Mat image = cv::imread("input.jpg");

    // 创建一个与输入图像大小相同的模糊图像
    cv::Mat blurredImage;
    blurredImage.create(image.size(), image.type());

    // 定义模糊半径
    int blurRadius = 5;

    // 对输入图像进行模糊处理
    cv::blur(image, blurredImage, cv::Size(blurRadius, blurRadius));

    // 显示原图像和模糊图像
    cv::imshow("Original Image", image);
    cv::imshow("Blurred Image", blurredImage);
    cv::waitKey(0);

    return 0;
}
