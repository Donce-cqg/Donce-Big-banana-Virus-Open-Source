#include <opencv2/opencv.hpp>

int main() {
    // ��ȡ����ͼ��
    cv::Mat image = cv::imread("input.jpg");

    // ����һ��������ͼ���С��ͬ��ģ��ͼ��
    cv::Mat blurredImage;
    blurredImage.create(image.size(), image.type());

    // ����ģ���뾶
    int blurRadius = 5;

    // ������ͼ�����ģ������
    cv::blur(image, blurredImage, cv::Size(blurRadius, blurRadius));

    // ��ʾԭͼ���ģ��ͼ��
    cv::imshow("Original Image", image);
    cv::imshow("Blurred Image", blurredImage);
    cv::waitKey(0);

    return 0;
}
