#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLTexture>

#include <QTextStream>

#include "BoundingBox.h"
#include "ShapeGroup.h"

class Camera;

class Scene
{
public:
    GLuint loadTexture(QString path);

	QVector<ShapeGroup> shapeGroups;

	Scene();
	explicit Scene(const QString& path);
	~Scene();

	void clear();
	bool genCube();
	bool loadObj(const QString& path);
	bool loadMaterialLibrary(const QString& path);

	QVector3D lightPos() const;
	void setLightPos(const QVector3D& lightPos);
    BoundingBox boundingBox() const;

private:
    QVector3D _lightPos;
    BoundingBox _boundingBox;

    QMap<QString, int> materialNames;
    QMap<int, QOpenGLTexture *> textures;
    QMap<QString, int> getTextureIdByPath;
    int lastMaterialName = 0;

	enum OBJ_ENUM
	{
		OBJ_UNKNOWN = 0,
		OBJ_COMMENT = 1,

		OBJ_CALL = 100, // include file
		OBJ_CSH, // Executes the requested UNIX command.

		OBJ_VERTEX = 200, // geometric vertex
		OBJ_CURV_POINT,
		OBJ_NORMAL,
		OBJ_TEX_VERTEX,

		OBJ_CSTYPE = 300, // free form
		OBJ_DEG,
		OBJ_BMAT,
		OBJ_STEP,

		OBJ_PARM,
		OBJ_TRIM,
		OBJ_HOLE,
		OBJ_SCRV,
		OBJ_SP,
		OBJ_CURV_END,

		OBJ_CON,

		OBJ_POINTS = 400,
		OBJ_LINES,
		OBJ_FACE,
		OBJ_CURV,
		OBJ_CURV2,
		OBJ_SURF,

		OBJ_GROUP = 500,
		OBJ_SMOOTHING_GROUP,
		OBJ_MERGING_GROUP,
		OBJ_OBJECT,

		// render attributes
		OBJ_BEVEL = 600,
		OBJ_COLOR_INTERPOLATION,
		OBJ_DISSOLVE_INTERPOLATION,
		OBJ_LOD,
		OBJ_MAPLIB,
		OBJ_USEMAP,
		OBJ_USEMTL,
		OBJ_MTLLIB,
		OBJ_SHADOW_OBJ,
		OBJ_TRACE_OBJ,
		OBJ_CTECH,
		OBJ_STECH,

		// B-spline
		OBJ_BSP = 700,
		OBJ_BZP,
		OBJ_CDC,
		OBJ_CDP,
		OBJ_RES
	};

	static const QMap<QString, OBJ_ENUM> objTokensContainer;
    static OBJ_ENUM getObjTokenByKeyword(const QString& keyword);

	enum MTL_ENUM
	{
		MTL_UNKNOWN = 0,
		MTL_COMMENT = 1,
		MTL_NEWMTL = 10,

		MTL_AMBIENT_COLOR = 100,
		MTL_DIFFUSE_COLOR,
		MTL_SPECULAR_COLOR,
		MTL_EMISSION_COLOR,
		MTL_TRANSMISSION_FILTER,

		MTL_ILLUMANATION_MODEL = 200,

		MTL_DISSOLVE_FACTOR = 300,
		MTL_TRANSPERANT,
		MTL_SPECULAR_EXPONENT,
		MTL_SHARPNESS,
		MTL_OPTICAL_DENSITY,

		MTL_AMBIENT_TEXMAP = 400,
		MTL_DIFFUSE_TEXMAP,
		MTL_SPECULAR_TEXMAP,
		MTL_EMISSION_TEXMAP,
		MTL_SPECULAR_EXPONENT_TEXMAP,
		MTL_DISSOLVE_TEXMAP,
		MTL_ANTIALIASING_TEXMAP,
		MTL_DECAL_TEXMAP,
		MTL_DISP_TEXMAP,
		MTL_BUMP_TEXMAP,
		MTL_REFLECTION_TEXMAP,
	};

	static const QMap<QString, MTL_ENUM> mtlTokensContainer;
    static MTL_ENUM getMtlTokenByKeyword(const QString& keyword);
};

#endif // OBJECT3D_H


