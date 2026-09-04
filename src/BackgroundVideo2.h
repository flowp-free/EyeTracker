#pragma once

#include <GLES3/gl3.h>
#include <opencv2/opencv.hpp>

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

class BackgroundVideo
{
public:
    BackgroundVideo();
    ~BackgroundVideo();

    bool openFolder(
        const std::string& folderPath
	 );

    bool update();

    GLuint getTexture() const;

    int getWidth() const;
    int getHeight() const;

    void close();

private:
    bool openCurrentVideo();

    cv::VideoCapture video;

    cv::Mat frame;
    cv::Mat rgbFrame;

    GLuint texture = 0;

    int width = 0;
    int height = 0;

    double videoFPS = 24.0;

    std::chrono::steady_clock::time_point
        lastFrameTime;

    std::vector<std::string> playlist;

    std::size_t currentVideoIndex = 0;

    bool opened = false;
};
