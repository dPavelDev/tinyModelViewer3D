#include "ObjReader.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>


const QMap<QString, ObjReader::OBJ_ENUM> ObjReader::objTokensContainer =
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

const QMap<QString, ObjReader::MTL_ENUM> ObjReader::mtlTokensContainer =
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


ObjReader::OBJ_ENUM ObjReader::getObjTokenByKeyword(const QString& keyword)
{
    auto it = objTokensContainer.find(keyword);

    if (it == objTokensContainer.end())
        return OBJ_UNKNOWN;

    return *it;
}

ObjReader::MTL_ENUM ObjReader::getMtlTokenByKeyword(const QString& keyword)
{
    auto it = mtlTokensContainer.find(keyword);

    if (it == mtlTokensContainer.end())
        return MTL_UNKNOWN;

    return *it;
}


ObjReader::ObjReader()
{

}

bool ObjReader::loadMaterialLibrary(Scene * scene, const QString& path, QMap<QString, int> & shapeGroupId, SceneStat* sceneStat)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    QTextStream tin(&file);

    auto currentShapeGroup = scene->shapeGroups.begin();

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

            shapeGroupId.insert(w[1].toString(), scene->addShapeGroup());
            currentShapeGroup = scene->shapeGroups.begin() + shapeGroupId[w[1].toString()];
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
        case MTL_SPECULAR_EXPONENT_TEXMAP:
        case MTL_DISSOLVE_TEXMAP:
        case MTL_EMISSION_TEXMAP:
        case MTL_BUMP_TEXMAP:
        {
            if (w.size() <= 1)
                break;

            QFileInfo currentFile(path);
            QDir dir = currentFile.absoluteDir();
            QString pathToTexture = dir.absoluteFilePath(w.back().toString());
            pathToTexture = pathToTexture.replace("\\", "/");


            auto currentMaterial = &currentShapeGroup->material;

            switch (token)
            {
            case MTL_AMBIENT_TEXMAP:
                currentMaterial->ambientTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_DIFFUSE_TEXMAP:
                currentMaterial->diffuseTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_SPECULAR_TEXMAP:
                currentMaterial->specularTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_SPECULAR_EXPONENT_TEXMAP:
                currentMaterial->specularExponentTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_DISSOLVE_TEXMAP:
                currentMaterial->dissolveTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_EMISSION_TEXMAP:
                currentMaterial->emissionTexture = scene->addTexture(pathToTexture);
                break;
            case MTL_BUMP_TEXMAP:
                currentMaterial->bumpTexture = scene->addTexture(pathToTexture);
                break;
            default:
                break;

            }

            sceneStat->setTexturesCount(scene->texturesCount());
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

Scene* ObjReader::loadScene(const QString& path, SceneStat* sceneStat)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return nullptr;

    auto* scene = new Scene();

    QTextStream tin(&file);

    std::vector<QVector3D> v(1), vn(1);
    std::vector<QVector2D> vt(1);

    QMap<QString, int> shapeGroupId;
    shapeGroupId.insert("", scene->addShapeGroup());

    std::vector<ShapeGroup>::iterator currentShapeGroup = scene->shapeGroups.begin() + shapeGroupId[""];

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
            scene->setBoundingBox( BoundingBox(scene->boundingBox(), result) );

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

                FormVertex(int _vertex_id = 0, int _tex_id = 0, int _normal_id = 0) :
                    vertex_id(_vertex_id), tex_id(_tex_id), normal_id(_normal_id) {}
            };

            QVector<FormVertex> formVerticies;

            for (int i = 1; i < w.size(); ++i)
            {
                QVector<QStringRef> v_id = w[i].split('/');

                int vertex_id = v_id[0].toInt(),
                        tex_id = (v_id.size() >= 2) ? v_id[1].toInt() : 0,
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

            break;
        }
        case OBJ_MTLLIB:
        {
            QFileInfo currentFile(path);
            QDir dir = currentFile.absoluteDir();

            for (int i = 1; i < w.size(); ++i)
            {
                loadMaterialLibrary(scene, dir.absoluteFilePath(w[i].toString()), shapeGroupId, sceneStat);
            }

            currentShapeGroup = scene->shapeGroups.begin();
            break;
        }
        case OBJ_USEMTL:
            currentShapeGroup->triangles.shrink_to_fit();
            currentShapeGroup->quads.shrink_to_fit();

            currentShapeGroup = scene->shapeGroups.begin() + shapeGroupId[ w[1].toString()];
            break;
        default:
            break;
        }
    }

    currentShapeGroup->triangles.shrink_to_fit();
    currentShapeGroup->quads.shrink_to_fit();

    if (sceneStat->isCanceled()) {
        delete scene;
        return nullptr;
    }

    return scene;
}
