#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QDir>
#include <QStringList>
#include <QPair>

#include <cstddef>

class GalleryModel : public QObject
{
    Q_OBJECT
private:

    std::size_t scaledWidth;
    std::size_t scaledHeight;

    QList<QString> imagePaths;
    QList<QPixmap> images;

    QString convertToAbsFilePath(const QString& path) const;
    QDir convertToAbsQDir(const QString& path) const;

    /*bool isPathExisting(const QString& path) const;
    bool isPathForFile(const QString& path) const;
    bool isPathForImage(const QString& path) const;
    bool isValidPath(const QString& path) const;*/

    bool isLoadSuccesful(const QPixmap& image) const;

    bool loadImage(const QString& path);
    QPixmap scaleImage(const QPixmap& image) const;

    QStringList getFilesRecursively(const QDir& dir) const;

    QStringList getSupportedImageExtensions() const;
    void reset();

public:
    explicit GalleryModel(std::size_t scaledWidth, std::size_t scaledHeight, QObject *parent = nullptr);

    void loadImages(const QStringList& paths);
    void loadImages(const QDir& dir);

    //QPixmap loadImage(const QString& absPath); // does not save to ımages or ımagePaths load them again but with full size

    const QPixmap& operator[](int offset);
    int size() const;

signals:
    void imageLoadError();
    void imagesLoaded();

public slots:
};

#endif // GALLERYMODEL_H
