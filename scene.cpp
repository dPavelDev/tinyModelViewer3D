#include <QFile>
#include <QFileInfo>

#include "scene.h"
#include <QtCore/QDir>
#include "ShapeGroup.h"

#include <QtDebug>
#include <ctime>

const QMap<QString, Scene::OBJ_ENUM> Scene::objTokensContainer =
{
	{ "#", OBJ_COMMENT },
	{ "call", OBJ_CALL},
	{"csh", OBJ_CSH},

	{"v", OBJ_VERTEX},
	{"vp", OBJ_CURV_POINT},
	{"vn", OBJ_NORMAL},
	{"vt", OBJ_TEX_VERTEX},

	{"cstype", OBJ_CSTYPE}, // free form
	{"deg", OBJ_DEG},
	{"bmat", OBJ_BMAT},
	{"step", OBJ_STEP},

	{"parm", OBJ_PARM},
	{"trim", OBJ_TRIM},
	{"hole", OBJ_HOLE},
	{"scrv", OBJ_SCRV},
	{"sp", OBJ_SP},
	{"end", OBJ_CURV_END},

	{"con", OBJ_CON},

	{"p", OBJ_POINTS},
	{"l", OBJ_LINES},
	{"f", OBJ_FACE},
	{"curv", OBJ_CURV},
	{"curv2", OBJ_CURV2},
	{"surf", OBJ_SURF},

	{"g", OBJ_GROUP},
	{"s", OBJ_SMOOTHING_GROUP},
	{"mg", OBJ_MERGING_GROUP},
	{"o", OBJ_OBJECT},

	// render attributes
	{ "bevel", OBJ_BEVEL},
	{ "c_interp", OBJ_COLOR_INTERPOLATION },
	{ "d_interp", OBJ_DISSOLVE_INTERPOLATION },
	{ "lod", OBJ_LOD },
	{ "maplib",OBJ_MAPLIB },
	{ "usemap",OBJ_USEMAP },
	{ "usemtl",OBJ_USEMTL },
	{ "mtllib",OBJ_MTLLIB },
	{ "shadow_obj",OBJ_SHADOW_OBJ },
	{ "trace_obj",OBJ_TRACE_OBJ },
	{ "ctech",OBJ_CTECH },
	{ "stech",OBJ_STECH },

	// B-spline
	{"bsp", OBJ_BSP},
	{ "bzp", OBJ_BZP},
	{ "cdc", OBJ_CDC},
	{ "cdp", OBJ_CDP},
	{ "res", OBJ_RES}
};

const QMap<QString, Scene::MTL_ENUM> Scene::mtlTokensContainer =
{
	{"newmtl", MTL_NEWMTL},
	{"#", MTL_COMMENT},

	{"Ka", MTL_AMBIENT_COLOR},
	{"Kd", MTL_DIFFUSE_COLOR},
	{"Ks", MTL_SPECULAR_COLOR},
	{"Ke", MTL_EMISSION_COLOR},
	{"Tf", MTL_TRANSMISSION_FILTER},

	{"illum", MTL_ILLUMANATION_MODEL},

	{"d", MTL_DISSOLVE_FACTOR},
	{"Tr", MTL_TRANSPERANT},
	{"Ns", MTL_SPECULAR_EXPONENT},
	{"sharpness", MTL_SHARPNESS},
	{"Ni", MTL_OPTICAL_DENSITY},

	{"map_Ka", MTL_AMBIENT_TEXMAP},
	{"map_Kd", MTL_DIFFUSE_TEXMAP},
	{"map_Ks", MTL_SPECULAR_TEXMAP},
	{"map_Ke", MTL_EMISSION_TEXMAP},
	{"map_Ns", MTL_SPECULAR_EXPONENT_TEXMAP},
	{"map_d", MTL_DISSOLVE_TEXMAP},
	{"map_aat", MTL_ANTIALIASING_TEXMAP},
	{"decal", MTL_DECAL_TEXMAP},
	{"disp", MTL_DISP_TEXMAP},
	{"bump", MTL_BUMP_TEXMAP},
	{"map_bump", MTL_BUMP_TEXMAP},
	{"refl", MTL_REFLECTION_TEXMAP}
};

Scene::Scene()
{
}

Scene::Scene(const QString& path)
{
	this->loadObj(path);
}

bool Scene::loadMaterialLibrary(const QString& path)
{
	QFile file(path);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
        return false;
	}

	QTextStream tin(&file);
	ShapeGroup* currentShapeGroup = &shapeGroups.front();
	Material* currentMaterial = &currentShapeGroup->material;

    int k = 0; // current line
	while (!tin.atEnd())
	{
		QString line = tin.readLine();

		++k;

        QVector<QStringRef> w = line.splitRef(QRegExp("(\\s)"), QString::SkipEmptyParts);
		//QVector<QStringRef> w = line.splitRef(" ", QString::SkipEmptyParts);

		if (w.empty())
            continue;

        auto token = getMtlTokenByKeyword(w[0].toString());
		switch (token)
		{
		case MTL_COMMENT:
			break;
		case MTL_NEWMTL:
            if (materialNames.find(w[1].toString()) != materialNames.end()) // if material is already added
			{
				throw;
			}

            materialNames[w[1].toString()] = ++lastMaterialName;
			shapeGroups.push_back(ShapeGroup());
			if (lastMaterialName != shapeGroups.size() - 1)
			{
				throw;
			}

			currentShapeGroup = &shapeGroups.back();
			currentMaterial = &currentShapeGroup->material;
			break;
		case MTL_AMBIENT_COLOR:
		case MTL_DIFFUSE_COLOR:
		case MTL_SPECULAR_COLOR:
		case MTL_EMISSION_COLOR:
		case MTL_TRANSMISSION_FILTER:
		{
            QColor color;

			if (w[1] == "spectral")
			{
				qDebug() << "Spectral colors don't support";
			}
			else if (w[1] == "xyz") // CIE XYZ color
			{
				qDebug() << "CIE XYZ colors don't support";
			}
			else // rgb color
			{
				color.setRedF(w[1].toDouble());
				color.setGreenF(w[2].toDouble());
				color.setBlueF(w[3].toDouble());
			}

			switch (token)
			{
			case MTL_AMBIENT_COLOR:
				currentMaterial->ambientColor = color;
				break;
			case MTL_DIFFUSE_COLOR:
				currentMaterial->diffuseColor = color;
				break;
			case MTL_SPECULAR_COLOR:
				currentMaterial->specularColor = color;
				break;
			case MTL_EMISSION_COLOR:
				currentMaterial->emissionColor = color;
				break;
			case MTL_TRANSMISSION_FILTER:
				currentMaterial->transmissionFilter = color;
				break;
			default:
				qDebug() << QString("Error : ") << line;
				break;
			}			
		}
			break;
		case MTL_AMBIENT_TEXMAP:
		case MTL_DIFFUSE_TEXMAP:
		case MTL_SPECULAR_TEXMAP:
		case MTL_EMISSION_TEXMAP:
		case MTL_SPECULAR_EXPONENT_TEXMAP:
		case MTL_DISSOLVE_TEXMAP:
		case MTL_BUMP_TEXMAP: 
		{
			QFileInfo currentFile(path);
            QDir dir = currentFile.absoluteDir();

            QString pathToTexture = dir.absoluteFilePath(w.back().toString());
            //qDebug() << pathToTexture;

			switch (token)
			{
			case MTL_AMBIENT_TEXMAP:
				currentMaterial->ambientTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_DIFFUSE_TEXMAP:
				currentMaterial->diffuseTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_SPECULAR_TEXMAP:
				currentMaterial->specularTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_SPECULAR_EXPONENT_TEXMAP:
				currentMaterial->opticalDensityTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_DISSOLVE_TEXMAP:
				currentMaterial->dissolveTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_EMISSION_TEXMAP:
				currentMaterial->emissionTexture_id = this->loadTexture(pathToTexture);
				break;
			case MTL_BUMP_TEXMAP:
				currentMaterial->bumpTexture_id = this->loadTexture(pathToTexture);
				break;
			default:
				qDebug() << "Error : " << line; 	
				break;
			}
		}
			break;
		case MTL_OPTICAL_DENSITY:
            currentMaterial->opticalDensity = w[1].toFloat();
			break;
		case MTL_SPECULAR_EXPONENT:
            currentMaterial->specularExponent = w[1].toFloat();
			break;
		case MTL_TRANSPERANT:
			currentMaterial->transperancy = w[1].toFloat();
			break;
		case MTL_DISSOLVE_FACTOR:
			if (w[1] == "-halo") // dissolve = 1.0 - (n * v)(1.0 - factor)
				currentMaterial->transperancy = 1.0 - w[2].toFloat();
			else
				currentMaterial->transperancy = 1.0 - w[1].toFloat();
			break;
		case MTL_SHARPNESS:
            currentMaterial->sharpness = w[1].toFloat();
			break;
		case MTL_ILLUMANATION_MODEL:
			currentMaterial->illuminationMode = w[1].toInt();
			break;
		default:
            qWarning() << "Warning: unresolved keyword " << w[0];
			break;
		}
	}

	qInfo() << "Materials are loaded: " << materialNames.size() << " counts";

	return true;
}

QVector3D Scene::lightPos() const
{
	return _lightPos;
}

void Scene::setLightPos(const QVector3D& lightPos)
{
    _lightPos = lightPos;
}

BoundingBox Scene::boundingBox() const
{
    return _boundingBox;
}

Scene::OBJ_ENUM Scene::getObjTokenByKeyword(const QString &keyword)
{
    if (keyword == "#")
        return OBJ_COMMENT;
    if (keyword == "v")
        return OBJ_VERTEX;


    auto it = objTokensContainer.find(keyword);

    if (it == objTokensContainer.end())
        return OBJ_UNKNOWN;

    return *it;
}

Scene::MTL_ENUM Scene::getMtlTokenByKeyword(const QString &keyword)
{
    auto it = mtlTokensContainer.find(keyword);

    if (it == mtlTokensContainer.end())
        return MTL_UNKNOWN;

    return *it;
}


bool Scene::loadObj(const QString& path)
{
    auto t = clock();

    QFile _tin(path);

    if (!_tin.open(QFile::ReadOnly | QFile::Text))
        return false;

    QTextStream tin(&_tin);

    this->clear();

	std::vector<QVector3D> v(1), vn(1);
	std::vector<QVector2D> vt(1);

	int f_count = 0, v_count = 0;

	ShapeGroup* currentShapeGroup = &shapeGroups[0];

	int k = 0;
	while (!tin.atEnd())
	{
		QString line = tin.readLine();
		++k;
		if (k % 100000 == 0)
		{
			qDebug() << k;
			qDebug() << currentShapeGroup->triangles.verticies.size() << " " << currentShapeGroup->triangles.verticies.capacity();
			qDebug() << currentShapeGroup->quads.verticies.size() << " " << currentShapeGroup->quads.verticies.capacity();
		}


		//QVector<QStringRef> w = line.splitRef(QRegExp("(\\s)"), QString::SkipEmptyParts);
        QVector<QStringRef> w = line.splitRef(" ", QString::SkipEmptyParts);
		if (w.empty())
			continue;

        OBJ_ENUM token; //= getObjTokenByKeyword(w[0].toString());
        if (w[0] == "#")
			token = OBJ_COMMENT;
        else if (w[0] == "v" )
		{
			token = OBJ_VERTEX;
		}
		else 
            token = getObjTokenByKeyword(w[0].toString());

		switch (token)
		{
		case OBJ_COMMENT:
			break;
		case OBJ_VERTEX:
			v.push_back(QVector3D(w[1].toFloat(), w[2].toFloat(), w[3].toFloat()));
            _boundingBox = BoundingBox(_boundingBox, v.back());
			
			++v_count;
			break;
		case OBJ_NORMAL:
			vn.push_back(QVector3D(w[1].toFloat(), w[2].toFloat(), w[3].toFloat()));
			break;
		case OBJ_TEX_VERTEX:
			vt.push_back(QVector2D(w[1].toFloat(), w[2].toFloat()));
			break;
		case OBJ_FACE:
		{
			++f_count;

			if (w.size() - 1 == 3)
			{ // triangle

				for (int i = 1; i <= 3; ++i)
				{
					QVector<QStringRef> v_id = w[i].split("/");
					int vertex_id = v_id[0].toInt(),
						tex_id = (v_id.size() >= 3) ? v_id[1].toInt() : 0,
						normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

					if (vertex_id < 0)
						vertex_id = int(v.size()) + vertex_id;
					if (tex_id < 0)
						tex_id = int(vt.size()) + tex_id;
					if (normal_id < 0)
						normal_id = int(vn.size()) + normal_id;

					currentShapeGroup->triangles.verticies.push_back(v[vertex_id]);
					currentShapeGroup->triangles.texverticies.push_back(vt[tex_id]);
					currentShapeGroup->triangles.normals.push_back(vn[normal_id]);
				}
			}
			else if (w.size() - 1 == 4)
			{ // quad

				for (int i = 1; i <= 4; ++i)
				{
					QVector<QStringRef> v_id = w[i].split("/");
					int vertex_id = v_id[0].toInt(),
						tex_id = (v_id.size() >= 2) ? v_id[1].toInt() : 0,
						normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

					if (vertex_id < 0)
						vertex_id = int(v.size()) + vertex_id;
					if (tex_id < 0)
						tex_id = int(vt.size()) + tex_id;
					if (normal_id < 0)
						normal_id = int(vn.size()) + normal_id;

					currentShapeGroup->quads.verticies.push_back(v[vertex_id]);
					currentShapeGroup->quads.texverticies.push_back(vt[tex_id]);
					currentShapeGroup->quads.normals.push_back(vn[normal_id]);
				}
			}
			else
			{
				qDebug() << "Warning! This parser supports only triangles and quads";
			}

			break;
		}
		case OBJ_GROUP:
		case OBJ_OBJECT:
		case OBJ_CALL:
		case OBJ_SMOOTHING_GROUP:
			break;
		case OBJ_MTLLIB:
		{
			QFileInfo currentFile(path);
			QDir dir = currentFile.absoluteDir();

			for (int i = 1; i < w.size(); ++i)
			{
				this->loadMaterialLibrary(dir.absoluteFilePath(w[i].toString()));
			}

			currentShapeGroup = &shapeGroups[0];
			break;
		}
		case OBJ_USEMTL:
			currentShapeGroup->triangles.shrink_to_fit();
			currentShapeGroup->quads.shrink_to_fit();

			currentShapeGroup = &shapeGroups[materialNames[w[1].toString()]];
			break;
		default:
            qDebug() << "Warning! Command not supported: " << w[0];
			break;
		}
	}

	qDebug() << "Loaded. Forms: " << f_count << ", verticies: " << v_count;
	qDebug() << "Time: " << (1.0f * clock() - t) / CLOCKS_PER_SEC;

    _lightPos = _boundingBox.maxPoint();
	_lightPos.setY(_lightPos.y() * 2);

	return true;
}

void Scene::clear()
{
	shapeGroups.clear();
	shapeGroups.push_back(ShapeGroup());

	materialNames.clear();
	materialNames.insert("", 0);
	lastMaterialName = 0;
	getTextureIdByPath.clear();

    _boundingBox = BoundingBox();
}

bool Scene::genCube()
{
	this->clear();
	auto& currentShapeGroup = shapeGroups[0];

	QVector<QVector3D> v = {
		{-0.5, -0.5, -0.5},
		{-0.5, -0.5, 0.5},
		{-0.5, 0.5, -0.5},
		{-0.5, 0.5, 0.5},
		{0.5, -0.5, -0.5},
		{0.5, -0.5, 0.5},
		{0.5, 0.5, -0.5},
		{0.5, 0.5, 0.5},
	};

	QVector<QVector2D> vt = {
		{1, 0},
		{0, 0.0},
		{0, 1},
		{1, 1}
	};

	QVector<QVector<int>> f = {
		{3, 1, 5, 7},
		{7, 5, 4, 6},
		{6, 4, 0, 2},
		{2, 0, 1, 3},
		{0, 1, 5, 4},
		{2, 3, 7, 6}
	};


	currentShapeGroup.material.diffuseTexture_id = loadTexture(":/tex/texture.jpg");

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
            _boundingBox = BoundingBox(_boundingBox, QVector3D(v[f[i][j]][0], v[f[i][j]][1], v[f[i][j]][2]));

			currentShapeGroup.quads.verticies.push_back(v[f[i][j]]);
			currentShapeGroup.quads.texverticies.push_back(vt[j]);
		}
	}

	return true;
}

GLuint Scene::loadTexture(QString path)
{
    path = path.replace("\\", "/");


	if (getTextureIdByPath[path] != 0)
		return getTextureIdByPath[path];

	QImage image(path);

	if (image.isNull())
	{
		return 0;
	}

	auto tex = new QOpenGLTexture(image.mirrored());
	tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	tex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
	tex->setMaximumAnisotropy(16.0);

	textures.insert(tex->textureId(), tex);
	getTextureIdByPath[path] = tex->textureId();

	return tex->textureId();
}

Scene::~Scene()
{
    for (auto it : textures)
    {
        delete it;
    }
}

