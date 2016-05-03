#ifndef OGLRENDER_H
#define OGLRENDER_H

#include <QOpenGLWidget>
#include <QBasicTimer>
#include <QTimer>

#include "scene.h"
#include "camera.h"

class RenderWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
    explicit RenderWidget(QWidget*);
	~RenderWidget() Q_DECL_OVERRIDE;

	bool lighting = false;
    QColor backgroundColor () const;
    void setBackgroundColor(const QColor & color);

	void setCamera(Camera* camera);
	void releaseCamera();
    Camera * getCamera() const;

	void setScene(Scene* scene = nullptr);
	Scene* getScene() const;
	void releaseScene();

	int getFrameCount() const;
    void updateView(int width = 0, int height = 0);

    void setGameMode(bool gameMode = true);

    void startTimer(const int fps = 60);
    void pauseTimer();
    void resumeTimer();

    void timerEvent(QTimerEvent *);

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    void mouseMoveSignal(QMouseEvent * event);
    void mousePressSignal(QMouseEvent * event);
    void mouseReleaseSignal(QMouseEvent * event);

    void initialized();

private:
	Camera* _camera = nullptr;
    QBasicTimer _timer;
    int _timerUpdateInterval = -1;

	Scene* _scene = nullptr;

    int _frameCount = 0;
	QColor _backgroundColor;

    bool _gameMode = false;
    int _mouseMoveCounter = 0;
};

#endif // OGLRENDER_H


