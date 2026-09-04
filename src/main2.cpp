#include "Camera.h"
#include "EyeRenderer.h"

#include <iostream>

int main()
{
    // --------------------------------------------------------
    // Eye renderer
    // --------------------------------------------------------

    EyeRenderer renderer;

    if (!renderer.start())
    {
        std::cerr
            << "Eye renderer failed to start.\n";

        return 1;
    }

    // --------------------------------------------------------
    // Face camera
    // --------------------------------------------------------

    Camera faceCamera;

    if (!faceCamera.start())
    {
        std::cerr
            << "Face camera failed to start.\n";

        return 1;
    }

    std::cout
        << "Face camera started.\n";

    // --------------------------------------------------------
    // Main application loop
    // --------------------------------------------------------

    bool running = true;

    while (running)
    {
        renderer.handleEvents(
            running
        );

        FacePosition face =
            faceCamera.update();

        faceCamera.showPreview();

        renderer.render(
            face.detected,
            face.x,
            face.y
        );
    }

    // --------------------------------------------------------
    // Cleanup
    // --------------------------------------------------------

    faceCamera.stop();

    renderer.stop();

    return 0;
}
