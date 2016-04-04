#include "renderwidget.h"

RenderWidget::RenderWidget(QWidget *)
{
    QSurfaceFormat frmt; // for antialiasing
    frmt.setSamples(4);
    this->setFormat(frmt);

    _camera = Camera(
                QVector3D(0, 0, 1), // eye
                QVector3D(0, 0, 0), // center
                QVector3D(0, 1, 0),  // up

                45, // viewing angle
                0.001, // zNear plane
                1000 // zFar plane
    );

    _rotationSpeed = 0; // in degrees

    _rotationAxis = _camera.up();
    return;
}

RenderWidget::~RenderWidget(){
}

void RenderWidget::initializeGL(){
    initializeOpenGLFunctions();

    //glClearColor(0, 0, 0, 0);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_POLYGON_SMOOTH);
    //glShadeModel(GL_FLAT);
    //glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    _timer.start(1000.0 / fps, this); // set redraw timer
}

void RenderWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_camera.fovy(), 1.0 * width / height, _camera.zNear(), _camera.zFar());

    glMatrixMode(GL_MODELVIEW);
}

void RenderWidget::paintGL(){
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _camera.setGluLookAt();


//    float v[3][3] = {
//        {0.5, -0.5, 0},
//        {0.0, 0.5, 0},
//        {-0.5, -0.5, 0},
//    };

//    glBegin(GL_TRIANGLES);
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex3fv(v[0]);

//        glColor3f(0.0, 1.0, 0.0);
//        glVertex3fv(v[1]);

//        glColor3f(0.0, 0.0, 1.0);
//        glVertex3fv(v[2]);
//    glEnd();

    // draw cube

    float v[9][3] = {
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5,  0.5},
        {-0.5,  0.5, -0.5},
        {-0.5,  0.5,  0.5},
        { 0.5, -0.5, -0.5},
        { 0.5, -0.5,  0.5},
        { 0.5,  0.5, -0.5},
        { 0.5,  0.5,  0.5},
    };

    int indexis[12][3] = {
        {1, 0, 2},
        {1, 3, 2},

        {1, 5, 7},
        {1, 3, 7},

        {5, 4, 6},
        {5, 7, 6},

        {0, 4, 6},
        {0, 2, 6},

        {3, 7, 6},
        {3, 2, 6},

        {1, 0, 4},
        {1, 5, 4}
    };

    for (int i = 0; i < 12; ++i) {
        glBegin(GL_TRIANGLES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3fv(v[indexis[i][0]]);
            glColor3f(0.0, 1.0, 0.0);
            glVertex3fv(v[indexis[i][1]]);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3fv(v[indexis[i][2]]);
        glEnd();
    }

//    for (int i = 0; i < 8; ++i) {
//        for (int j = i + 1; j < 8; ++j) {
//            for (int k = j + 1; k < 8; ++k) {
//                glBegin(GL_TRIANGLES);
//                    glColor3f(1.0, 0.0, 0.0);
//                    glVertex3fv(v[i]);

//                    glColor3f(0.0, 1.0, 0.0);
//                    glVertex3fv(v[j]);

//                    glColor3f(0.0, 0.0, 1.0);
//                    glVertex3fv(v[k]);
//                glEnd();
//            }
//        }
//    }

}

void RenderWidget::setScale(int sliderPosition) {
    if (sliderPosition == 0)
        sliderPosition = 1;

    _camera.setEye(_camera.center() - _camera.dir() * (100.0f / sliderPosition) * (100.f / sliderPosition));

    update();
}

void RenderWidget::setRotationSpeed(int sliderPosition) {
    if (fabs(_rotationSpeed - sliderPosition) < 1)
        return;

    _rotationSpeed = sliderPosition;
}

void RenderWidget::setFixedSpeed(bool fixed) {
    _rotationFixed = fixed;
}

void RenderWidget::mousePressEvent(QMouseEvent * event) {
    _mousePressed = true;

    _lastPos = event->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent * event) {
    if (!_mousePressed)
        return;

    const QPoint & mousePos = event->pos();

    QPoint deltaPos = mousePos - _lastPos;
    QVector3D axis = -(_camera.right() * deltaPos.y() + _camera.up() * deltaPos.x());
    float speed = axis.length();

    _rotationAxis = axis;

    if (!_rotationFixed)
        _rotationSpeed = speed;

    _lastPos = mousePos;
}

void RenderWidget::mouseReleaseEvent(QMouseEvent * ) {
    _mousePressed = false;
}

void RenderWidget::timerEvent(QTimerEvent *) {
    if (_rotationSpeed < 1) {
        return;
    }

    if (!_rotationFixed) {
        _rotationSpeed -= 5.0 / fps; // -5 degrees per second
        speedChanged(_rotationSpeed);
    }

    _rotation = QQuaternion::fromAxisAndAngle(_rotationAxis, _rotationSpeed / fps);

    _camera.rotate(_rotation);

    update();
}
