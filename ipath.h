#ifndef IPATH_H
#define IPATH_H

// Local
#include "base.h"

class IPath : public Base
{
    Q_OBJECT

public:
    explicit IPath(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IPath() {}

public:
    virtual void initialize() = 0;
    virtual QStringList pathModel() const = 0;
    virtual QUrl pathUrl() const = 0;
    virtual bool isPathUrlValid() const = 0;
};

#endif // IPATH_H
