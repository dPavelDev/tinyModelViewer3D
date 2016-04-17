#include "object3d.h"

Object3D::Object3D()
{
}

Object3D::Object3D(const QString &path) {
    this->loadObj(path);
}

bool Object3D::loadMaterialLibrary(const QString &path) {
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QTextStream tin(&file);
    ShapeGroup * currentShapeGroup = &shapeGroups[0];
    Material * currentMaterial = &currentShapeGroup->material;

    int k = 0;
    while (!tin.atEnd()) {
        QString line = tin.readLine();

        ++k;

        QVector<QStringRef> w = line.splitRef(QRegExp("(\\s)"), QString::SkipEmptyParts);
        //QVector<QStringRef> w = line.splitRef(" ", QString::SkipEmptyParts);
        if (w.empty())
            continue;

        if (w[0] == "#") {
            //comment
        }
        else if (w[0] == "newmtl") {
            if (materialNames.find(w[1].toString()) != materialNames.end()) {
                throw;
            }

            materialNames[w[1].toString()] = ++lastMaterialName;
            shapeGroups.push_back(ShapeGroup());
            if (lastMaterialName != shapeGroups.size() - 1) {
                throw;
            }

            currentShapeGroup = &shapeGroups.back();
            currentMaterial = &currentShapeGroup->material;
        }
        else if (w[0].toString()[0] == 'K') { // color
            QColor color;
            color.setRedF(w[1].toDouble());
            color.setGreenF(w[2].toDouble());
            color.setBlueF(w[3].toDouble());

            if (w[0] == "Ka") // ambient color
                currentMaterial->ambientColor = color;
            else if (w[0] == "Kd") // diffuse color
                currentMaterial->diffuseColor = color;
            else if (w[0] == "Ks") // specular color
                currentMaterial->specularColor = color;
            else if (w[0] == "Ke") // emission color
                currentMaterial->emissionColor = color;
            else
                std::cerr << "Error : " << line.toStdString() << std::endl;
        }
        else if (w[0].contains("map_")) {
            QFileInfo currentFile(path);
            QDir dir = currentFile.absoluteDir();

            QString pathToTexture = dir.absoluteFilePath(w[1].toString());

            if (w[0] == "map_Ka")
                currentMaterial->ambientTexture_id = this->loadTexture(pathToTexture);
            else if (w[0] == "map_Kd")
                currentMaterial->diffuseTexture_id = this->loadTexture(pathToTexture);
            else if (w[0] == "map_Ks")
                currentMaterial->specularTexture_id = this->loadTexture(pathToTexture);
            else if (w[0] == "map_Ke")
                currentMaterial->emissionTexture_id = this->loadTexture(pathToTexture);
            else if (w[0] == "map_bump")
                currentMaterial->bumpTexture_id = this->loadTexture(pathToTexture);
            else
                std::cerr << "Error : " << line.toStdString() << std::endl;
        }
        else if (w[0] == "Ni") {
            currentMaterial->opticalDensity = w[1].toInt();
        }
        else if (w[0] == "Ns") {
            currentMaterial->specularExponent = w[1].toInt();
        }
        else if (w[0] == "d") {
            currentMaterial->transperancy = 1.0 - w[1].toFloat();
        }
        else if (w[0] == "illum") {
            currentMaterial->illuminationMode = w[1].toInt();
        }
        else {
            std::cerr << "-Error: " << line.toStdString() << std::endl;
        }
    }

    return true;
}

bool Object3D::loadObj(const QString &path) {
    //std::ifstream tin(path.toStdString().c_str());
    QFile _tin(path);

    if (!_tin.open(QFile::ReadOnly | QFile::Text))
        return false;

    QTextStream tin(&_tin);

    this->clear();

    std::vector<QVector3D> v(1), vn(1);
    std::vector<QVector2D> vt(1);

    int f_count = 0, v_count = 0;

    ShapeGroup * currentShapeGroup = &shapeGroups[0];

    int k = 0;
    while (!tin.atEnd()) {
        QString line = tin.readLine();
        ++k;
        if (k % 100000 == 0) {
            std::cerr << k << std::endl;
            std::cerr << currentShapeGroup->triangles.verticies.size() << " " << currentShapeGroup->triangles.verticies.capacity() << std::endl;
            std::cerr << currentShapeGroup->quads.verticies.size() << " " << currentShapeGroup->quads.verticies.capacity() << std::endl;
        }


        //QVector<QStringRef> w = line.splitRef(QRegExp("(\\s)"), QString::SkipEmptyParts);
        QVector<QStringRef> w = line.splitRef(" ", QString::SkipEmptyParts);
        if (w.empty())
            continue;

        if (w[0] == "#") {
            //std::cerr << raw_line << std::endl;
        }
        else if (w[0] == "mtllib") {
            QFileInfo currentFile(path);
            QDir dir = currentFile.absoluteDir();

            for (int i = 1; i < w.size(); ++i) {
                this->loadMaterialLibrary(dir.absoluteFilePath( w[i].toString()) );
            }

            currentShapeGroup = &shapeGroups[0];
        }
        else if (w[0] == "usemtl") {
            currentShapeGroup->triangles.shrink_to_fit();
            currentShapeGroup->quads.shrink_to_fit();

            currentShapeGroup = &shapeGroups[ materialNames[ w[1].toString() ] ];
        }
        else if (w[0] == "v") {
            QVector3D vertex = QVector3D(w[1].toFloat(), w[2].toFloat(), w[3].toFloat());
            boundingBox = BoundingBox(boundingBox, vertex);

            v.push_back(vertex);

            ++v_count;
        }
        else if (w[0] == "vn") {
            vn.push_back(QVector3D(w[1].toFloat(), w[2].toFloat(), w[3].toFloat()));
        }
        else if (w[0] == "vt") {
            vt.push_back(QVector2D(w[1].toFloat(), w[2].toFloat()));
        }
        else if (w[0] == "f") {
            ++f_count;

            if (w.size() - 1 == 3) { // triangle

                for (int i = 1; i <= 3; ++i) {
                    QVector<QStringRef> v_id = w[i].split("/");
                    int vertex_id = v_id[0].toInt(),
                        tex_id = (v_id.size() >= 3) ? v_id[1].toInt() : 0,
                        normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

                    if (vertex_id < 0)
                        vertex_id = v.size() + vertex_id;
                    if (tex_id < 0)
                        tex_id = vt.size() + tex_id;
                    if (normal_id < 0)
                        normal_id = vn.size() + normal_id;

                    currentShapeGroup->triangles.verticies.push_back(v[ vertex_id ]);
                    currentShapeGroup->triangles.texverticies.push_back(vt[ tex_id ]);
                    currentShapeGroup->triangles.normals.push_back(vn[ normal_id ]);
                }


            }
            else if (w.size() - 1 == 4) { // quad

                for (int i = 1; i <= 4; ++i) {
                    QVector<QStringRef> v_id = w[i].split("/");
                    int vertex_id = v_id[0].toInt(),
                        tex_id = (v_id.size() >= 2) ? v_id[1].toInt() : 0,
                        normal_id = (v_id.size() >= 3) ? v_id[2].toInt() : 0;

                    if (vertex_id < 0)
                        vertex_id = v.size() + vertex_id;
                    if (tex_id < 0)
                        tex_id = vt.size() + tex_id;
                    if (normal_id < 0)
                        normal_id = vn.size() + normal_id;

                    currentShapeGroup->quads.verticies.push_back(v[ vertex_id ]);
                    currentShapeGroup->quads.texverticies.push_back(vt[ tex_id ]);
                    currentShapeGroup->quads.normals.push_back(vn[ normal_id ]);
                }

            }
            else {
                std::cerr << "Error! This parser supports only triangles and quads\n";
            }
        }
        else if (w[0] == "g") {

        }
        else if (w[0] == "o") {

        }
        else {
            std::cerr << "Unknown line: " << line.toStdString() << std::endl;
        }
    }

    std::cerr << "Loaded. Forms: " << f_count << ", verticies: " << v_count << std::endl;

    return true;
}

void Object3D::clear() {
    shapeGroups.clear();
    shapeGroups.push_back(ShapeGroup());

    materialNames.clear();
    materialNames.insert("", 0);
    lastMaterialName = 0;

    boundingBox = BoundingBox();
}

bool Object3D::genCube()
{
    this->clear();
    auto & currentShapeGroup = shapeGroups[0];

    QVector<QVector3D> v = {
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5,  0.5},
        {-0.5,  0.5, -0.5},
        {-0.5,  0.5,  0.5},
        { 0.5, -0.5, -0.5},
        { 0.5, -0.5,  0.5},
        { 0.5,  0.5, -0.5},
        { 0.5,  0.5,  0.5},
    };

    QVector<QVector2D> vt = {
        {1, 0},
        {0, 0.0},
        {0, 1},
        {1, 1}
    };

    QVector<QVector<int> > f = {
        {3, 1, 5, 7},
        {7, 5, 4, 6},
        {6, 4, 0, 2},
        {2, 0, 1, 3},
        {0, 1, 5, 4},
        {2, 3, 7, 6}
    };


    currentShapeGroup.material.diffuseTexture_id = loadTexture(":/tex/texture.jpg");

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            boundingBox = BoundingBox(boundingBox, QVector3D(v[ f[i][j] ][0], v[ f[i][j] ][1], v[ f[i][j] ][2]));

            currentShapeGroup.quads.verticies.push_back(v[ f[i][j] ]);
            currentShapeGroup.quads.texverticies.push_back(vt[ j ]);
        }
    }

    return true;
}

GLuint Object3D::loadTexture(const QString &path) {
    QImage image(path);

    if (image.isNull()) {
        return 0;
    }

    QOpenGLTexture * tex = new QOpenGLTexture(image.mirrored());
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMaximumAnisotropy(16.0);

    textures.insert(tex->textureId(), tex);

    return tex->textureId();
}

Object3D::~Object3D() {
    for (auto it : textures) {
        delete it;
    }
}
