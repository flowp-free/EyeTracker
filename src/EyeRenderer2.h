#pragma once

#include "Mesh.h"
#include "Config.h"

#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include <vector>

class EyeRenderer
{
public:
    EyeRenderer();
    ~EyeRenderer();

    bool start(const Config& config);
    
    void handleEvents(bool& running);

    void render(
        bool faceDetected,
        float faceX,
        float faceY
    );

    void stop();

private:
    Config config;
    GLuint loadTexture(
        const char* path
    );

    void createWindows();
    void createResources();
    void cleanupResources();
    
    float smoothFaceX = 0.0f;
    float smoothFaceY = 0.0f;
    
    std::vector<EyeWindow> eyes;
    std::vector<Mesh> meshes;

    GLuint shaderProgram = 0;

    GLuint baseColorTexture = 0;
    GLuint normalTexture = 0;

    glm::vec3 renderCameraPosition;
    glm::mat4 view;

    int desktopWidth = 0;
    int desktopHeight = 0;

    // Physical camera position on desktop.
    float cameraX = 0.0f;
    float cameraY = 0.0f;

    bool started = false;
};
