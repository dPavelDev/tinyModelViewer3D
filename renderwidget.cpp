#include "renderwidget.h"

#ifdef _MSC_VER // for glu library
#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "GLU32.LIB")
#endif

#include <GL/glu.h>
#include <GL/gl.h>

RenderWidget::RenderWidget(QWidget*)
{
	setMouseTracking(true); // allow to get mouse pos when mouse is moving without press button

	QSurfaceFormat frmt;
    frmt.setSamples(1); // for antialiasing
	frmt.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    frmt.setSwapInterval(100);
	this->setFormat(frmt);

	_backgroundColor = this->palette().color(this->backgroundRole()); // set background color
}

RenderWidget::~RenderWidget()
{
	delete _scene;
	delete _camera;
}

QColor RenderWidget::backgroundColor() const
{
	return _backgroundColor;
}

void RenderWidget::setBackgroundColor(const QColor& color)
{
	_backgroundColor = color;
	makeCurrent();
	glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0);
	doneCurrent();
}

void RenderWidget::setCamera(Camera* camera)
{
	_camera = camera;
    updateView();
}

Camera* RenderWidget::getCamera() const
{
	return _camera;
}

void RenderWidget::setScene(Scene* scene)
{
	makeCurrent();
	_scene = scene;
	if (_scene == nullptr)
	{
		_scene = new Scene();
		_scene->genCube();
	}

	_scene->initMaterials();
    _scene->initGeometry();

	doneCurrent();
}

Scene* RenderWidget::getScene() const
{
	return _scene;
}

void RenderWidget::releaseScene()
{
	makeCurrent();
	delete _scene;
	_scene = nullptr;
	doneCurrent();
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

bool RenderWidget::isLighting() const
{
	return _isLighting;
}

void RenderWidget::setLighting(bool isLighting)
{
	_isLighting = isLighting;
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

void RenderWidget::timerEvent(QTimerEvent*)
{
    //if (_requireUpdate)
        update();
    _requireUpdate = false;
}

void RenderWidget::updateLater()
{
    _requireUpdate = true;
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
	glClearColor(_backgroundColor.redF(), _backgroundColor.greenF(), _backgroundColor.blueF(), 1.0f); // set background color

	glShadeModel(GL_SMOOTH); // enable smooth for color and texverticies

	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_TEXTURE_2D); // enable texture support

	glEnable(GL_LIGHT1); //enable a light
	glDisable(GL_NORMALIZE); // disable normalizing because it reduce performance

	emit initialized();
}

void RenderWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_camera->fovy(), 1.0 * width / height, _camera->zNear(), _camera->zFar());

    glMatrixMode(GL_MODELVIEW);
}

void RenderWidget::paintGL()
{
	makeCurrent();
	if (_scene == nullptr)
		return;
	if (_camera == nullptr)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	_camera->setLookAt(); // set camera position

	if (_isLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

    _updateLight();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

    for (const ShapeGroup& currentShapeGroup : _scene->shapeGroups)
	{
        const auto& material = currentShapeGroup.material;
		const auto& color = material.diffuseColor;

		// set material
		glColor3f(color.redF(), color.greenF(), color.blueF());

		auto ambient = material.ambientColor.toRgb();
		auto diffuse = material.diffuseColor.toRgb();
        auto specular = material.specularColor.toRgb();
        auto emission = material.emissionColor.toRgb();

        float ambient_color[3] = {float(ambient.redF()), float(ambient.greenF()), float(ambient.blueF())};
        float diffuse_color[3] = {float(diffuse.redF()), float(diffuse.greenF()), float(diffuse.blueF())};
        float specular_color[3] = {float(specular.redF()), float(specular.greenF()), float(specular.blueF())};
        float emission_color[3] = {float(emission.redF()), float(emission.greenF()), float(emission.blueF())};

        glBindTexture(GL_TEXTURE_2D, currentShapeGroup.material.diffuseTexture_id);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_color);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.specularExponent);

		// draw triangles
		if (currentShapeGroup.triangles.verticies.size() != 0)
		{
			if (currentShapeGroup.triangles.verticies.empty())
				glDisableClientState(GL_VERTEX_ARRAY);
            else {
				glEnableClientState(GL_VERTEX_ARRAY);
                glBindBuffer(GL_ARRAY_BUFFER, currentShapeGroup.triangles.verticiesBuffer.bufferId());
                glVertexPointer(3, GL_FLOAT, 0, nullptr);
            }

            if (currentShapeGroup.triangles.texverticies.empty())
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            else{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glBindBuffer(GL_ARRAY_BUFFER, currentShapeGroup.triangles.texverticiesBuffer.bufferId());
                glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
            }
			if (currentShapeGroup.triangles.normals.empty())
				glDisableClientState(GL_NORMAL_ARRAY);
            else {
				glEnableClientState(GL_NORMAL_ARRAY);
                glBindBuffer(GL_ARRAY_BUFFER, currentShapeGroup.triangles.normalsBuffer.bufferId());
                glNormalPointer(GL_FLOAT, 0, nullptr);
            }

			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(currentShapeGroup.triangles.verticies.size()));
            glBindBuffer(GL_ARRAY_BUFFER, 0); //release vbo
		}

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // draw quads (without buffers)
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
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    emit mouseReleaseSignal(event);
}

void RenderWidget::_updateLight()
{
    auto v = _scene->lightPos(); // get light position
    float light_position[] = {v.x(), v.y(), v.z(), 1};

    GLfloat LightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);

    float dim = _scene->boundingBox().max_dim();

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1 / dim);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1 / dim / dim);
}

