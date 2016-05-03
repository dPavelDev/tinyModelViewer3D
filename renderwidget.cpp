#include "renderwidget.h"

#ifdef _MSC_VER // for glu library
#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "GLU32.LIB")
#endif

#include <GL/glu.h>

RenderWidget::RenderWidget(QWidget *)
{
    setMouseTracking(true); // allow to get mouse pos when mouse is moving without press button

	QSurfaceFormat frmt;
	frmt.setSamples(4); // for antialiasing
	//frmt.setSwapInterval(1); // for vsync
	frmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	this->setFormat(frmt);

    _backgroundColor = this->palette().color(this->backgroundRole()); // set background color
    return;
}

RenderWidget::~RenderWidget()
{
    delete _scene;
}

QColor RenderWidget::backgroundColor() const
{
    return _backgroundColor;
}

void RenderWidget::setBackgroundColor(const QColor &color)
{
    _backgroundColor = color;
    makeCurrent();
    glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0);
    doneCurrent();
}

void RenderWidget::setCamera(Camera* camera)
{
	_camera = camera;
}

void RenderWidget::releaseCamera()
{
	delete _camera;
    _camera = nullptr;
}

Camera *RenderWidget::getCamera() const
{
    return _camera;
}

void RenderWidget::setScene(Scene* scene)
{
	_scene = scene;
	if (_scene == nullptr)
	{
		_scene = new Scene();
		_scene->genCube();
	}
}

Scene* RenderWidget::getScene() const
{
	return _scene;
}

void RenderWidget::releaseScene()
{
	delete _scene;
	_scene = nullptr;
}

int RenderWidget::getFrameCount() const
{
	return _frameCount;
}

void RenderWidget::updateView(int width, int height)
{
    if (width == 0)
        width = this->width();
    if (height == 0)
        height = this->height();

	makeCurrent();
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(_camera->fovy(), 1.0 * width / height, _camera->zNear(), _camera->zFar());

        glMatrixMode(GL_MODELVIEW);
        doneCurrent();
}

void RenderWidget::startTimer(const int fps)
{
    _timerUpdateInterval = 1000.0 / fps;
    this->resumeTimer();
}

void RenderWidget::pauseTimer()
{
    _timer.stop();
}

void RenderWidget::resumeTimer()
{
    if (_timerUpdateInterval == -1)
        return;

    _timer.start(_timerUpdateInterval, Qt::CoarseTimer, this);
}

void RenderWidget::timerEvent(QTimerEvent *)
{
    update();
}

void RenderWidget::initializeGL()
{
    glClearColor(_backgroundColor.redF(), _backgroundColor.greenF(), _backgroundColor.blueF(), 1.0f); // set background color

    glShadeModel(GL_SMOOTH); // enable smooth for color and texverticies

    glEnable(GL_DEPTH_TEST); // enable depth test
    glEnable(GL_TEXTURE_2D); // enable texture support

	//    glEnable(GL_LIGHTING);                 	//enables lighting
    glEnable(GL_LIGHT1); //enable a light
    glDisable(GL_NORMALIZE); // disable normalizing because it reduce performance

    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //sets lighting to one-sided

    _scene = new Scene(); // set default model
    _scene->genCube();

    emit initialized();
}

void RenderWidget::resizeGL(int width, int height)
{
    updateView(width, height);
}

void RenderWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    _camera->setLookAt(); // set camera position

    if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

    auto v = _scene->lightPos(); // get light position
    float light_position[] = {v.x(), v.y(), v.z(), 1};

    GLfloat LightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	for (auto& currentShapeGroup : _scene->shapeGroups)
	{
		auto& color = currentShapeGroup.material.diffuseColor;

        // set material
		glColor3f(color.redF(), color.greenF(), color.blueF());

		auto ambient = currentShapeGroup.material.ambientColor.toRgb();
		auto diffuse = currentShapeGroup.material.diffuseColor.toRgb();

		float ambient_color[3] = {static_cast<float>(ambient.redF()), static_cast<float>(ambient.greenF()), static_cast<float>(ambient.blueF())};
        float diffuse_color[3] = {static_cast<float>(diffuse.redF()), static_cast<float>(diffuse.greenF()), static_cast<float>(diffuse.blueF())};

		glBindTexture(GL_TEXTURE_2D, currentShapeGroup.material.diffuseTexture_id);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);

        // draw triangles
		if (currentShapeGroup.triangles.verticies.size() != 0)
		{
			if (currentShapeGroup.triangles.verticies.empty())
				glDisableClientState(GL_VERTEX_ARRAY);
			else
				glEnableClientState(GL_VERTEX_ARRAY);

			if (currentShapeGroup.triangles.texverticies.empty())
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			else
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (currentShapeGroup.triangles.normals.empty())
				glDisableClientState(GL_NORMAL_ARRAY);
			else
				glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, &currentShapeGroup.triangles.verticies[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &currentShapeGroup.triangles.texverticies[0]);

			if (!currentShapeGroup.triangles.normals.empty())
				glNormalPointer(GL_FLOAT, 0, &currentShapeGroup.triangles.normals[0]);

			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(currentShapeGroup.triangles.verticies.size()));
		}

        // draw quads
		if (currentShapeGroup.quads.verticies.size() != 0)
		{
			if (currentShapeGroup.quads.verticies.empty())
				glDisableClientState(GL_VERTEX_ARRAY);
			else
				glEnableClientState(GL_VERTEX_ARRAY);

			if (currentShapeGroup.quads.texverticies.empty())
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			else
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (currentShapeGroup.quads.normals.empty())
				glDisableClientState(GL_NORMAL_ARRAY);
			else
				glEnableClientState(GL_NORMAL_ARRAY);

			
			glVertexPointer(3, GL_FLOAT, 0, &currentShapeGroup.quads.verticies[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &currentShapeGroup.quads.texverticies[0]);

			if (!currentShapeGroup.quads.normals.empty())
				glNormalPointer(GL_FLOAT, 0, &currentShapeGroup.quads.normals[0]);

			glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(currentShapeGroup.quads.verticies.size()));
		}
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

    // update frame count
	++_frameCount;
}

void RenderWidget::mousePressEvent(QMouseEvent* event)
{
    emit mousePressSignal(event);
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    emit mouseMoveSignal(event);

    //cursor().setPos(QWidget::mapToGlobal(QPoint(width() / 2, height() / 2)));

/*
    if (!_mousePressed)
        return;

    qDebug() << "Mouse move";


    const auto& mousePos = event->pos();

    QPoint deltaPos = mousePos - _lastPos;
    QVector2D v(deltaPos);
    v.setX(v.x() / this->width());
    v.setY(v.y() / this->height());

    _lastPos = event->pos();*/

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

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    emit mouseReleaseSignal(event);
}

