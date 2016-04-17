#include "renderwidget.h"

#include <QFileDialog>

RenderWidget::RenderWidget(QWidget *)
{
    QSurfaceFormat frmt;
        frmt.setSamples(4); // for antialiasing
        frmt.setSwapInterval(1); // for vsync
        frmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    this->setFormat(frmt);

    return;
}

RenderWidget::~RenderWidget(){
    delete _scene;
}

void RenderWidget::setCamera(Camera *camera) {
    _camera = camera;
}

void RenderWidget::releaseCamera() {
    delete _camera;
}

void RenderWidget::setScene(Object3D *scene) {
    _scene = scene;
    if (_scene == nullptr) {
        _scene = new Object3D();
        _scene->genCube();
    }

    return;
}

void RenderWidget::releaseScene () {
    delete _scene;
}

void RenderWidget::initializeGL(){
    initializeOpenGLFunctions();


//    glClearColor(0.95, 0.95, 0.95, 0);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_ALPHA_TEST);

//    glEnable(GL_LIGHTING);                 	//enables lighting
    glEnable(GL_LIGHT1);                   	//enables a light
//    glEnable(GL_NORMALIZE);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);  //sets lighting to one-sided

    _scene = new Object3D();

    bool b = false;
    b = _scene->genCube();

    if (b == false)
        throw;

    //    b = _scene->loadObj("e:\\PROJECTS\\levels\\box\\box.obj");
    //    b = _scene->loadObj("e:\\PROJECTS\\levels\\crytek-sponza\\crytek-sponza\\sponza.obj");
    //       b = _scene->loadObj("e:\\PROJECTS\\levels\\cornell-box\\CornellBox-Original.obj");

//    b = _scene->loadObj("e:\\PROJECTS\\levels\\rungholt\\rungholt.obj");
//    b = _scene->loadObj("e:\\PROJECTS\\levels\\Venice\\venice.obj");
//      b = _scene->loadObj("e:\\PROJECTS\\levels\\rungholt\\house.obj");

//    b = _scene->loadObj("e:\\PROJECTS\\levels\\head\\head.obj");
//    b = _scene->loadObj("e:\\PROJECTS\\levels\\powerplant\\powerplant\\powerplant.obj");
//    b = _scene->loadObj("e:\\PROJECTS\\levels\\san-miguel\\san-miguel.obj");



//    _camera->setCenter( (_scene->boundingBox.max_point + _scene->boundingBox.min_point) / 2 );
//    _camera->setEye(_camera->center() + QVector3D(0, 0, -20));
}

void RenderWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_camera->fovy(), 1.0 * width / height, _camera->zNear(), _camera->zFar());

    glMatrixMode(GL_MODELVIEW);
}

void RenderWidget::paintGL(){
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _camera->setLookAt();

    if (lighting) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }

    auto & v = _scene->boundingBox.max_point;
//    float light_position[] = {2 * v.x(), 2 * v.y(), 2 * v.z(), 1};

//    QVector3D v = (_scene->boundingBox.max_point + _scene->boundingBox.min_point) / 2;
    float light_position[] = {v.x(), v.y(), v.z(), 1};


//    float light_dir[] = {0, -1, 0, 1};
//    GLfloat LightAmbient[]= { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
//    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
//    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.05);
//    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);R
//    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

//    GLUquadricObj *quadric;
//    quadric = gluNewQuadric();
//    gluQuadricDrawStyle(quadric, GLU_FILL );

//    glPushMatrix();
//        glTranslatef(light_position[0], light_position[1], light_position[0]);
//        gluSphere( quadric , 5 , 36 , 18 );
//    glPopMatrix();

//    gluDeleteQuadric(quadric);


//    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
//    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60);
//    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < _scene->shapeGroups.size(); ++i) {
        auto & currentShapeGroup = _scene->shapeGroups[i];
        auto & color = currentShapeGroup.material.diffuseColor;

        glColor3f(color.redF(), color.greenF(), color.blueF());

        QColor ambient = currentShapeGroup.material.ambientColor.toRgb();
        QColor diffuse = currentShapeGroup.material.diffuseColor.toRgb();
        float ambient_color[3] = {(float)ambient.redF(), (float)ambient.greenF(), (float)ambient.blueF()};
        float diffuse_color[3] = {(float)diffuse.redF(), (float)diffuse.greenF(), (float)diffuse.blueF()};

        glBindTexture(GL_TEXTURE_2D, currentShapeGroup.material.diffuseTexture_id);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);

        if (currentShapeGroup.triangles.verticies.size() != 0) {
            glVertexPointer(3, GL_FLOAT, 0, &currentShapeGroup.triangles.verticies[0]);
            glTexCoordPointer(2, GL_FLOAT, 0, &currentShapeGroup.triangles.texverticies[0]);
            glNormalPointer(GL_FLOAT, 0, &currentShapeGroup.triangles.normals[0] );

            glDrawArrays(GL_TRIANGLES, 0, currentShapeGroup.triangles.verticies.size());
        }

        if (currentShapeGroup.quads.verticies.size() != 0) {
            glVertexPointer(3, GL_FLOAT, 0, &currentShapeGroup.quads.verticies[0]);
            glTexCoordPointer(2, GL_FLOAT, 0, &currentShapeGroup.quads.texverticies[0]);
            glNormalPointer(GL_FLOAT, 0, &currentShapeGroup.quads.normals[0] );

            glDrawArrays(GL_QUADS, 0, currentShapeGroup.quads.verticies.size());
        }

        glFinish();
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    return;
}

void RenderWidget::mousePressEvent(QMouseEvent * event) {
    _mousePressed = true;

    _lastPos = event->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!_mousePressed)
        return;

//    const QPoint & mousePos = event->pos();
/*
    QVector3D axis = -(_camera->right() * deltaPos.y() + _camera->up() * deltaPos.x());
    float speed = axis.length();

    if ( !_camera->rotationSpeedFixed() )
        _camera->setRotationSpeed(speed); //_rotationSpeed = speed;

    _camera->setRotationAxis(axis);
*/
    //_lastPos = mousePos;
}

void RenderWidget::mouseReleaseEvent(QMouseEvent * event) {
    const QPoint & mousePos = event->pos();

    QPoint deltaPos = mousePos - _lastPos;
    QVector2D v(deltaPos);
    v.setX(v.x() / this->width());
    v.setY(v.y() / this->height());

    emit mouseDownMoveSignal(&v);


    _mousePressed = false;
}
