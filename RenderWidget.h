#pragma once

#include <QOpenGLWidget>
#include <QBasicTimer>
#include <QOpenGLFunctions>

#include "Scene.h"
#include "Camera.h"

class RenderWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit RenderWidget(QWidget*);
    ~RenderWidget() Q_DECL_OVERRIDE;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    void setCamera(Camera* camera);
    Camera* camera() const;

    void setScene(Scene* scene = nullptr);
    Scene* scene() const;
    void releaseScene();

    int frameCount() const;
    void updateView(int width = 0, int height = 0);

    bool isLighting() const;
    void setLighting(bool isLighting);

    bool isPowerSaveMode() const;
    void setPowerSaveMode(bool isPowerSaveMode);

    void startTimer(const int fps = 60);
    void pauseTimer();
    void resumeTimer();

    void timerEvent(QTimerEvent*);
    void updateLater();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    void mouseMoveSignal(QMouseEvent* event);
    void mousePressSignal(QMouseEvent* event);
    void mouseReleaseSignal(QMouseEvent* event);

    void initialized();

private:
    bool _isRendering = false;
    bool _requireUpdate = false;
    bool _isPowerSaveMode = false;

    Camera* _camera = nullptr;
    QBasicTimer _timer;
    int _timerUpdateInterval = -1;

    Scene* _scene = nullptr;
    bool _isLighting = false;

    int _frameCount = 0;
    QColor _backgroundColor;

    //bool _gameMode = false;
    int _mouseMoveCounter = 0;

    void _updateLight();

    void _setMaterial(const Material &material);
    void _drawTriangles(const Triangles &triangles);
    void _drawQuads(const Quads &quads);

    void _releaseBuffers();
};
