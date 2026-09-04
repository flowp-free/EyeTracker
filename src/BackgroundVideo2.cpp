#include "BackgroundVideo.h"
#include <filesystem>
#include <algorithm>
#include <iostream>

BackgroundVideo::BackgroundVideo()
{
}

BackgroundVideo::~BackgroundVideo()
{
    close();
}

bool BackgroundVideo::open(
    const std::string& path
)
{
   
    video.open(path);
    
    videoFPS =
       video.get(
        cv::CAP_PROP_FPS
       );

    if (videoFPS <= 0.0){
    videoFPS = 30.0;
    }

    lastFrameTime = std::chrono::steady_clock::now();

    std::cout << "Background FPS: " << videoFPS << '\n';

    if (!video.isOpened())
    {
        std::cerr
            << "Could not open background video: "
            << path
            << '\n';

        return false;
    }
    
    std::cout
       << "Video backend: "
       << video.getBackendName()
       << '\n';

    width =
        static_cast<int>(
            video.get(
                cv::CAP_PROP_FRAME_WIDTH
            )
        );

    height =
        static_cast<int>(
            video.get(
                cv::CAP_PROP_FRAME_HEIGHT
            )
        );

    std::cout
        << "Background video: "
        << width
        << "x"
        << height
        << '\n';

    // Read first frame so we know the actual format.
    if (!video.read(frame))
    {
        std::cerr
            << "Could not read first video frame.\n";

        return false;
    }

    cv::cvtColor(
        frame,
        rgbFrame,
        cv::COLOR_BGR2RGB
    );

    // --------------------------------------------------------
    // Create OpenGL texture
    // --------------------------------------------------------

    glGenTextures(
        1,
        &texture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        rgbFrame.cols,
        rgbFrame.rows,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        rgbFrame.data
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    opened = true;

    return true;
}

bool BackgroundVideo::update()
{
    if (!opened)
    {
        return false;
    }

    auto now =
        std::chrono::steady_clock::now();

    double elapsed =
        std::chrono::duration<double>(
            now - lastFrameTime
        ).count();

    double frameDuration =
        1.0 / videoFPS;

    // Not time for next video frame yet.
    if (elapsed < frameDuration)
    {
        return true;
    }

    lastFrameTime = now;

    // --------------------------------------------
    // Decode next frame
    // --------------------------------------------

    if (!video.read(frame))
    {
        // Loop back to beginning.
        video.set(
            cv::CAP_PROP_POS_FRAMES,
            0
        );

        if (!video.read(frame))
        {
            return false;
        }
    }

    cv::cvtColor(
        frame,
        rgbFrame,
        cv::COLOR_BGR2RGB
    );

    // --------------------------------------------
    // Upload new frame to OpenGL
    // --------------------------------------------

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        rgbFrame.cols,
        rgbFrame.rows,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        rgbFrame.data
    );

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    return true;
}

GLuint BackgroundVideo::getTexture() const
{
    return texture;
}

int BackgroundVideo::getWidth() const
{
    return width;
}

int BackgroundVideo::getHeight() const
{
    return height;
}

void BackgroundVideo::close()
{
    if (video.isOpened())
    {
        video.release();
    }

    if (texture != 0)
    {
        glDeleteTextures(
            1,
            &texture
        );

        texture = 0;
    }

    frame.release();
    rgbFrame.release();

    opened = false;
}
