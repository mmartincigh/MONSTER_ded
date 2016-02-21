#ifndef PATHMANAGERBASE_H
#define PATHMANAGERBASE_H

// Qt
#include <QUrl>

// Local
#include "ipathmanager.h"

class PathManagerBase : public IPathManager
{
    Q_OBJECT
    Q_PROPERTY(QStringList pathModel READ pathModel NOTIFY pathModelChanged)
    Q_PROPERTY(QUrl pathUrl READ pathUrl NOTIFY pathUrlChanged)
    Q_PROPERTY(bool isPathUrlValid READ isPathUrlValid NOTIFY isPathUrlValidChanged)

private:
    static const int m_MAX_MODEL_SIZE;
    QString m_editText;
    QStringList m_pathModel;
    QString m_path;
    QUrl m_pathUrl;
    bool m_isPathUrlValid;

public:
    explicit PathManagerBase(const QString &logTag, QObject *parent = NULL);
    ~PathManagerBase();

public:
    virtual void initialize();
    virtual QStringList pathModel() const;
    virtual QUrl pathUrl() const;
    virtual bool isPathUrlValid() const;

private:
    void setEditText(const QString &editText);
    void setPathModel(const QStringList &pathModel);
    void setPath(const QString &path);
    void setPathUrl(const QUrl &pathUrl);
    void setIsPathUrlValid(bool isPathUrlValid);
    void updatePathModel();

public slots:
    void onPath(QString *path);
    void onIsPathUrlValid(bool *isPathUrlValid);
    void onUpdateEditText(const QString &editText);
    void onUpdateEditText(const QUrl &editText);
    void onProcess();

private slots:
    void onEditTextChanged(const QString &editText);

signals:
    void editTextChanged(const QString &editText);
    void pathModelChanged(const QStringList &pathModel);
    void pathChanged(const QString &path);
    void pathUrlChanged(const QUrl &pathUrl);
    void isPathUrlValidChanged(bool isPathUrlValid);
    void pathModel(QStringList *pathModel);
};

#endif // PATHMANAGERBASE_H
