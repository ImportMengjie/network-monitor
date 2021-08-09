#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QString>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

class PathUtils
{
public:
    PathUtils() = delete;

    static QString getAbsPath(const QString& path){
        if(path.startsWith("/"))
            return path;
        else
         return concatDir(QCoreApplication::applicationDirPath(), path);
    }

    static QString concatDir(const QString& dir, const QString& subDir){
        if(dir.endsWith("/")){
            return dir+subDir;
        }else{
            return dir+"/"+subDir;
        }
    }

    static bool isDirExists(const QString& dir){
        QDir d(dir);
        return d.exists();
    }

    static QString getDateDir(const QString& baseDir, const QDateTime& date){
        QString subDir = QString("%1-%2").arg(date.date().year()).arg(date.date().month(), 2, 10, QLatin1Char('0'));
        return concatDir(baseDir, subDir);
    }

    static QString getDateDirAndCreate(const QString& baseDir, const QDateTime& date){
        QString newDir = getDateDir(baseDir, date);
        QDir().mkpath(newDir);
        return newDir;
    }

    static bool mv(const QString& soruce, const QString& target){
        return QFile::rename(soruce, target);
    }

    static QString getFileName(const QString& filePath){
        return QFileInfo(filePath).fileName();
    }

    static bool cp(const QString& soruce, const QString& target){
        return QFile::copy(soruce, target);
    }
};

#endif // PATHUTILS_H
