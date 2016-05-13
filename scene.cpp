#include <QFile>
#include <QFileInfo>

#include "scene.h"
#include <QtCore/QDir>
#include "ShapeGroup.h"

const QMap<QString, Scene::OBJ_ENUM> Scene::objTokensContainer =
{
	{"#", OBJ_COMMENT},
	{"call", OBJ_CALL},
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
	{"bevel", OBJ_BEVEL},
	{"c_interp", OBJ_COLOR_INTERPOLATION},
	{"d_interp", OBJ_DISSOLVE_INTERPOLATION},
	{"lod", OBJ_LOD},
	{"maplib",OBJ_MAPLIB},
	{"usemap",OBJ_USEMAP},
	{"usemtl",OBJ_USEMTL},
	{"mtllib",OBJ_MTLLIB},
	{"shadow_obj",OBJ_SHADOW_OBJ},
	{"trace_obj",OBJ_TRACE_OBJ},
	{"ctech",OBJ_CTECH},
	{"stech",OBJ_STECH},

	// B-spline
	{"bsp", OBJ_BSP},
	{"bzp", OBJ_BZP},
	{"cdc", OBJ_CDC},
	{"cdp", OBJ_CDP},
	{"res", OBJ_RES}
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

bool Scene::loadMaterialLibrary(const QString& path, SceneStat* sceneStat)
{
	QFile file(path);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}

	QTextStream tin(&file);
	ShapeGroup* currentShapeGroup = getShapeGroup(""); // get shape group without material

	while (!tin.atEnd())
	{
        if (sceneStat->isCanceled())
            return false;

		QString line = tin.readLine();

		auto w = stringSplitBySpace(line);
		if (w.empty())
			continue;

		auto token = getMtlTokenByKeyword(w[0].toString());
		switch (token)
		{
		case MTL_COMMENT:
			break;
		case MTL_NEWMTL:
            if (w.size() <= 1)
                break;

			sceneStat->incMaterialsCount();
			addShapeGroup(w[1].toString());

			currentShapeGroup = getShapeGroup(w[1].toString());
			break;
		case MTL_AMBIENT_COLOR:
		case MTL_DIFFUSE_COLOR:
		case MTL_SPECULAR_COLOR:
		case MTL_EMISSION_COLOR:
		case MTL_TRANSMISSION_FILTER:
			{
                if (w.size() <= 1)
                    break;
                QColor color;


				if (w[1] == "spectral")
				{
//                    sceneStat->spectralColorDetected();
//                    qDebug() << "Spectral colors don't support";
				}
				else if (w[1] == "xyz") // CIE XYZ color
				{
//                    sceneStat->cieXyzColorDetected();
//                    qDebug() << "CIE XYZ colors don't support";
				}
				else // rgb color
				{
                    if (w.size() <= 1)
                        break;

					color.setRedF(w[1].toDouble());
                    color.setGreenF( (w.size() > 2) ? w[2].toDouble() : color.red() );
                    color.setBlueF ( (w.size() > 3) ? w[3].toDouble() : color.green() );
				}

				auto currentMaterial = &currentShapeGroup->material;

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
                if (w.size() <= 1)
                    break;

                QFileInfo currentFile(path);
				QDir dir = currentFile.absoluteDir();
				QString pathToTexture = dir.absoluteFilePath(w.back().toString());
                pathToTexture = pathToTexture.replace("\\", "/");


				auto currentMaterial = &currentShapeGroup->material;

                addTexture(pathToTexture);
				switch (token)
				{
				case MTL_AMBIENT_TEXMAP:
                    currentMaterial->ambientTexturePath = pathToTexture;
					break;
				case MTL_DIFFUSE_TEXMAP:
                    currentMaterial->diffuseTexturePath = pathToTexture;
					break;
				case MTL_SPECULAR_TEXMAP:
                    currentMaterial->specularTexturePath = pathToTexture;
					break;
				case MTL_SPECULAR_EXPONENT_TEXMAP:
                    currentMaterial->specularTexturePath = pathToTexture;
					break;
				case MTL_DISSOLVE_TEXMAP:
                    currentMaterial->dissolveTexturePath = pathToTexture;
					break;
				case MTL_EMISSION_TEXMAP:
                    currentMaterial->emissionTexturePath = pathToTexture;
					break;
				case MTL_BUMP_TEXMAP:
                    currentMaterial->bumpTexturePath = pathToTexture;
					break;
                default:
                    break;

                }

                sceneStat->setTexturesCount(_getTexImageByPath.count());
			}
			break;

		case MTL_OPTICAL_DENSITY:
            if (w.size() <= 1)
                break;

			currentShapeGroup->material.opticalDensity = w[1].toFloat();
			break;
		case MTL_SPECULAR_EXPONENT:
            if (w.size() <= 1)
                break;

            currentShapeGroup->material.specularExponent = w[1].toFloat();
			break;
		case MTL_TRANSPERANT:
            if (w.size() <= 1)
                break;

			currentShapeGroup->material.transperancy = w[1].toFloat();
			break;
		case MTL_DISSOLVE_FACTOR:
            if (w.size() <= 1)
                break;

            if (w[1] == "-halo") {// dissolve = 1.0 - (n * v)(1.0 - factor)
                if (w.size() >= 2)
                    currentShapeGroup->material.transperancy = 1.0 - w[2].toFloat();
            }
            else
				currentShapeGroup->material.transperancy = 1.0 - w[1].toFloat();

			break;
		case MTL_SHARPNESS:
            if (w.size() <= 1)
                break;

			currentShapeGroup->material.sharpness = w[1].toFloat();
			break;
		case MTL_ILLUMANATION_MODEL:
            if (w.size() <= 1)
                break;

            currentShapeGroup->material.illuminationMode = w[1].toInt();
			break;

		default:
            break;
		}
	}

	return true;
}

void Scene::initMaterials()
{
    for (ShapeGroup& shapeGroup : shapeGroups)
    {
        Material* material = &shapeGroup.material;

        material->ambientTexture_id = _loadTexture(material->ambientTexturePath);
        material->diffuseTexture_id = _loadTexture(material->diffuseTexturePath);
        material->dissolveTexture_id = _loadTexture(material->dissolveTexturePath);
        material->emissionTexture_id = _loadTexture(material->emissionTexturePath);
        material->opticalDensityTexture_id = _loadTexture(material->opticalDensityTexturePath);
        material->specularTexture_id = _loadTexture(material->specularTexturePath);
        material->bumpTexture_id = _loadTexture(material->bumpTexturePath);
    }
}

void Scene::initGeometry()
{
    for (ShapeGroup & shapeGroup : shapeGroups) {
        shapeGroup.triangles.initBuffers();
    }
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

void Scene::addShapeGroup(const QString& materialName)
{
	int id = shapeGroups.size();
	_materialNames.insert(materialName, id);
	shapeGroups.push_back(ShapeGroup());
}

ShapeGroup* Scene::getShapeGroup(const QString& materialName)
{
    return &shapeGroups[_materialNames[materialName]];
}

void Scene::addTexture(QString path)
{
    if (_getTexImageByPath.contains(path))
        return;

    QImage image(path);
    if (image.isNull())
        return;

    _getTexImageByPath.insert(path, image);
}

Scene::OBJ_ENUM Scene::getObjTokenByKeyword(const QString& keyword)
{
	auto it = objTokensContainer.find(keyword);

	if (it == objTokensContainer.end())
		return OBJ_UNKNOWN;

	return *it;
}

Scene::MTL_ENUM Scene::getMtlTokenByKeyword(const QString& keyword)
{
	auto it = mtlTokensContainer.find(keyword);

	if (it == mtlTokensContainer.end())
		return MTL_UNKNOWN;

	return *it;
}


Scene* Scene::loadObj(const QString& path, SceneStat* sceneStat)
{
    QFile file(path);

	if (!file.open(QFile::ReadOnly | QFile::Text))
		return nullptr;

	auto* scene = new Scene();
    scene->clear();

    QTextStream tin(&file);

    std::vector<QVector3D> v(1), vn(1);
	std::vector<QVector2D> vt(1);

	ShapeGroup* currentShapeGroup = &scene->shapeGroups[0];

    sceneStat->setTotalBytes(file.size());

    while (!tin.atEnd())
	{
        if (sceneStat->isCanceled())
            break;

        sceneStat->setReadBytes(file.pos());

        QString line = tin.readLine();
		auto w = stringSplitBySpace(line);
		if (w.empty())
			continue;

        OBJ_ENUM token = getObjTokenByKeyword(w[0].toString());

		switch (token)
		{
		case OBJ_COMMENT:
			break;
        case OBJ_VERTEX:
        {
            QVector3D result;

            if (w.size() >= 2)
                result.setX(w[1].toFloat());
            if (w.size() >= 3)
                result.setY(w[2].toFloat());
            if (w.size() >= 4)
                result.setZ(w[3].toFloat());

            v.push_back(result);
            scene->_boundingBox = BoundingBox(scene->_boundingBox, result);

            sceneStat->incVerticiesCount();
            break;
        }
        case OBJ_NORMAL:
        {
            QVector3D result;

            if (w.size() >= 2)
                result.setX(w[1].toFloat());
            if (w.size() >= 3)
                result.setY(w[2].toFloat());
            if (w.size() >= 4)
                result.setZ(w[3].toFloat());

            vn.push_back(result);

            sceneStat->incNormalsCount();
            break;
        }
        case OBJ_TEX_VERTEX:
        {
            QVector2D result;

            if (w.size() >= 2)
                result.setX(w[1].toFloat());
            if (w.size() >= 3)
                result.setY(w[2].toFloat());


            vt.push_back(QVector2D(result));

            sceneStat->incTexVerticiesCount();
            break;
        }
		case OBJ_FACE:
			{
				sceneStat->incFacesCount();

				struct FormVertex
				{
					int vertex_id, tex_id, normal_id;

					FormVertex(int _vertex_id = 0, int _tex_id = 0, int _normal_id = 0) : vertex_id(_vertex_id), tex_id(_tex_id), normal_id(_normal_id)
					{
					}
				};

				QVector<FormVertex> formVerticies;

				for (int i = 1; i < w.size(); ++i)
				{
                    QVector<QStringRef> v_id = w[i].split('/');

					int vertex_id = v_id[0].toInt(),
						tex_id = (v_id.size() >= 3) ? v_id[1].toInt() : 0,
						normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

					if (vertex_id < 0)
						vertex_id = int(v.size()) + vertex_id;
					if (tex_id < 0)
						tex_id = int(vt.size()) + tex_id;
					if (normal_id < 0)
						normal_id = int(vn.size()) + normal_id;

					formVerticies.push_back(FormVertex(vertex_id, tex_id, normal_id));
				}

				if (formVerticies.size() < 3)
					break;

                // triangulation
				for (int i = 2; i < formVerticies.size(); ++i)
				{
					int id[3] = {0, i - 1, i};

                    bool hasNormal = true;

					for (int j = 0; j < 3; ++j)
					{
						int vertex_id = formVerticies[id[j]].vertex_id;
						int tex_id = formVerticies[id[j]].tex_id;
						int normal_id = formVerticies[id[j]].normal_id;

                        if (normal_id == 0)
                            hasNormal = false;

						currentShapeGroup->triangles.verticies.push_back(v[vertex_id]);
						currentShapeGroup->triangles.texverticies.push_back(vt[tex_id]);
						currentShapeGroup->triangles.normals.push_back(vn[normal_id]);
					}

                    if (!hasNormal) {
                        auto & verticies = currentShapeGroup->triangles.verticies;

                        auto & v1 = verticies[verticies.size() - 1];
                        auto & v2 = verticies[verticies.size() - 2];
                        auto & v3 = verticies[verticies.size() - 3];

                        QVector3D normal = QVector3D::crossProduct(v3 - v1, v2 - v1);
                        normal.normalize();

                        for (int i = verticies.size() - 3; i < (int)verticies.size(); ++i)
                            currentShapeGroup->triangles.normals[i] = normal;
                    }
				}

				//            if (w.size() - 1 == 3)
				//            { // triangle

				//                for (int i = 1; i <= 3; ++i)
				//                {
				//                    QVector<QStringRef> v_id = w[i].split("/");
				//                    int vertex_id = v_id[0].toInt(),
				//                        tex_id = (v_id.size() >= 3) ? v_id[1].toInt() : 0,
				//                        normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

				//                    if (vertex_id < 0)
				//                        vertex_id = int(v.size()) + vertex_id;
				//                    if (tex_id < 0)
				//                        tex_id = int(vt.size()) + tex_id;
				//                    if (normal_id < 0)
				//                        normal_id = int(vn.size()) + normal_id;

				//                    currentShapeGroup->triangles.verticies.push_back(v[vertex_id]);
				//                    currentShapeGroup->triangles.texverticies.push_back(vt[tex_id]);
				//                    currentShapeGroup->triangles.normals.push_back(vn[normal_id]);
				//                }
				//            }
				//            else if (w.size() - 1 == 4)
				//            { // quad

				//                for (int i = 1; i <= 4; ++i)
				//                {
				//                    QVector<QStringRef> v_id = w[i].split("/");
				//                    int vertex_id = v_id[0].toInt(),
				//                        tex_id = (v_id.size() >= 2) ? v_id[1].toInt() : 0,
				//                        normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

				//                    if (vertex_id < 0)
				//                        vertex_id = int(v.size()) + vertex_id;
				//                    if (tex_id < 0)
				//                        tex_id = int(vt.size()) + tex_id;
				//                    if (normal_id < 0)
				//                        normal_id = int(vn.size()) + normal_id;

				//                    currentShapeGroup->quads.verticies.push_back(v[vertex_id]);
				//                    currentShapeGroup->quads.texverticies.push_back(vt[tex_id]);
				//                    currentShapeGroup->quads.normals.push_back(vn[normal_id]);
				//                }
				//            }
				//            else
				//            {
				//                qDebug() << "Warning! This parser supports only triangles and quads";
				//            }

				break;
			}
		case OBJ_MTLLIB:
			{
				QFileInfo currentFile(path);
				QDir dir = currentFile.absoluteDir();

				for (int i = 1; i < w.size(); ++i)
				{
					scene->loadMaterialLibrary(dir.absoluteFilePath(w[i].toString()), sceneStat);
				}

				currentShapeGroup = &scene->shapeGroups[0];
				break;
			}
		case OBJ_USEMTL:
			currentShapeGroup->triangles.shrink_to_fit();
			currentShapeGroup->quads.shrink_to_fit();

			currentShapeGroup = &scene->shapeGroups[scene->_materialNames[w[1].toString()]];
			break;
		default:
			break;
		}
	}

    if (sceneStat->isCanceled()) {
        delete scene;
        scene = nullptr;
    }

	return scene;
}

void Scene::clear()
{
	shapeGroups.clear();
	_materialNames.clear();

	addShapeGroup(""); //add default shape group
    _getTextureIdByPath.clear();

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


    currentShapeGroup.material.diffuseTexturePath = ":/tex/texture.jpg";
    addTexture(":/tex/texture.jpg");

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

GLuint Scene::_loadTexture(QString path)
{
    if (path.isEmpty())
        return 0;

    if (_getTextureIdByPath[path] != 0) {
        return _getTextureIdByPath[path];
    }

    QImage image = _getTexImageByPath[path];

    if (image.isNull())
    {
        return 0;
    }

    auto tex = new QOpenGLTexture(image.mirrored());
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMaximumAnisotropy(8.0);

    _oglTextures.insert(tex);
    _getTextureIdByPath[path] = tex->textureId();

    return tex->textureId();
}

Scene::~Scene()
{
    for (auto it : _oglTextures)
	{
		delete it;
	}
}


QVector<QStringRef> stringSplitBySpace(const QString& s)
{
	QVector<QStringRef> result;

	for (int l = 0, r = 0; l < s.length(); ++l , r = std::max(l, r))
	{
		if (s[l].isSpace())
			continue;

		while (r < s.length() && !s[r].isSpace())
			++r;

		result.push_back(QStringRef(&s, l, r - l));

		l = r - 1;
	}

	return result;
}

