#pragma once

class SceneStat
{
public:
    SceneStat();
    int materialsCount() const;
    void setMaterialsCount(int materialsCount);
    void incMaterialsCount(int materialsCount = 1);

    int texturesCount() const;
    void setTexturesCount(int texturesCount);
    void incTexturesCount(int texturesCount = 1);

    int verticiesCount() const;
    void setVerticiesCount(int verticiesCount);
    void incVerticiesCount(int verticiesCount = 1);

    int texVerticiesCount() const;
    void setTexVerticiesCount(int texVerticiesCount);
    void incTexVerticiesCount(int texVerticiesCount = 1);

    int normalsCount() const;
    void setNormalsCount(int normalsCount);
    void incNormalsCount(int normalsCount = 1);

    int facesCount() const;
    void setFacesCount(int facesCount);
    void incFacesCount(int facesCount = 1);

    bool freeFormDetected() const;
    void setFreeFormDetected(bool freeFormDetected);

    bool isLoaded() const;

    void setReadBytes (long long bytes);
    long long readBytes() const;
    void incReadBytes (long long bytes);

    long long totalBytes() const;
    void setTotalBytes(long long totalBytes);

    void setCanceled();
    bool isCanceled() const;
private:
    int _materialsCount = 0;
    int _texturesCount = 0;
    int _verticiesCount = 0;
    int _texVerticiesCount = 0;
    int _normalsCount = 0;
    int _facesCount = 0;

    long long _readBytes = 0;
    long long _totalBytes = 0;

    bool _freeFormDetected = false;
    bool _isLoaded = false;
    bool _isCanceled = false;
};

