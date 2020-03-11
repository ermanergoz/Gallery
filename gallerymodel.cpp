#include "gallerymodel.h"
#include <QFileInfo>
#include <QDir>
#include <QVector>
#include <QStack>
#include <QStringList>

#include <QDebug>

GalleryModel::GalleryModel(std::size_t scaledWidth, std::size_t scaledHeight, QObject *parent) :
    QObject(parent), scaledWidth(scaledWidth), scaledHeight(scaledHeight)
{

}

/*bool GalleryModel::isPathExisting(const QString &path) const
{
    return QFileInfo::exists(path); //static
}

bool GalleryModel::isPathForFile(const QString& path) const
{
    //QFileInfo fileInfo(path);
    //
    //return fileInfo.isFile();
    return QFileInfo(path).isFile();
}

bool GalleryModel::isPathForImage(const QString& path) const
{
    QFileInfo fi(path);
    QString fileExtension = fi.completeSuffix();  // example "tar.gz"

    QVector<QString> supportedExtensions = getSupportedImageExtensions();

    for(auto imageExtension : supportedExtensions)
    {
        if(fileExtension==imageExtension)
            return true;
    }

    return false;
}

bool GalleryModel::isValidPath(const QString& path) const
{
    QString absPath = convertToAbsPath(path);

    if( isPathExisting(absPath) &&
        isPathForFile(absPath) &&
        isPathForImage(absPath)
       )
    {
        return true;
    }
    else
    {
        return false;
    }

}
*/

QStringList GalleryModel::getSupportedImageExtensions() const
{
    //QVector<QString> supportedExtensions(3);
    //supportedExtensions[0] = "jpg";
    //supportedExtensions[1] = "jpeg";
    //supportedExtensions[2] = "png";

    return QStringList() << "jpg" << "jpeg" << "png";
}


QString GalleryModel::convertToAbsFilePath(const QString& path) const
{
     QFileInfo fi(path);
     return fi.absoluteFilePath();
}

QDir GalleryModel::convertToAbsQDir(const QString& path) const
{
    QFileInfo fi(path);
    return fi.absoluteDir();
}


bool GalleryModel::loadImage(const QString& path)
{
    QString absPath = convertToAbsFilePath(path);
    QPixmap image(absPath);

    bool loadSuccessful = isLoadSuccesful(image);
    if(!loadSuccessful)
    {
        emit imageLoadError();
        return false;
    }

    QPixmap scaledImage = scaleImage(image);

    images.push_back(scaledImage);
    imagePaths.push_back(absPath);

    return loadSuccessful;
}

bool GalleryModel::isLoadSuccesful(const QPixmap& image) const
{
    return  !image.isNull();
}

QPixmap GalleryModel::scaleImage(const QPixmap& image) const
{
    return image.scaled(static_cast<int>(scaledWidth), static_cast<int>(scaledHeight), Qt::AspectRatioMode::KeepAspectRatio);
}

QStringList GalleryModel::getFilesRecursively(const QDir &dir) const
{
    QStack<QDir> dirsToExplore;
    QStringList filePaths;

    QStringList extensions = getSupportedImageExtensions();

    QStringList nameFilter;
    for(const auto& extension : extensions)
    {
        nameFilter << ("*." + extension);
    }

    dirsToExplore.push(dir);

    while(!dirsToExplore.isEmpty())
    {
        QDir currentlyExplored(dirsToExplore.pop());
        QStringList subDirectoryPaths =
                currentlyExplored.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

        for(const auto& dirPath : subDirectoryPaths)
        {
            dirsToExplore.push(QDir(currentlyExplored.path() + "/" + dirPath));
        }

        QStringList containedFilePaths = currentlyExplored.entryList(nameFilter, QDir::Files);
        for(const auto& filePath : containedFilePaths)
        {
            filePaths << (currentlyExplored.path() + "/" + filePath);
        }
        //filePaths += containedFilePaths;
    }

    return filePaths;
}

void GalleryModel::loadImages(const QStringList& paths)
{
    for(const auto& path : paths)
    {
        if(!loadImage(path))
        {
            return; //just ending it it is not returning anything
        }
    }

    emit imagesLoaded();
}

void GalleryModel::loadImages(const QDir& dir) // recursive
{
    QStringList files = getFilesRecursively(dir);
    loadImages(files);
}

void GalleryModel::reset()
{
    images.clear();
    imagePaths.clear();
}

const QPixmap& GalleryModel::operator[](int offset)
{
    return images[offset];
}

int GalleryModel::size() const
{
    return images.size();
}
