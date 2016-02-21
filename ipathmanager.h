#ifndef IPATHMANAGER_H
#define IPATHMANAGER_H

// Local
#include "base.h"

class IPathManager : public Base
{
    Q_OBJECT

public:
    explicit IPathManager(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IPathManager() {}

public:
    virtual void initialize() = 0;
    virtual QStringList pathModel() const = 0;
    virtual QUrl pathUrl() const = 0;
    virtual bool isPathUrlValid() const = 0;
};

#endif // IPATHMANAGER_H
