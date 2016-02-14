#ifndef SECUREMANAGER_H
#define SECUREMANAGER_H

// Qt
#include <QUrl>

// Local
#include "base.h"

class SecureManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(QStringList securePathModel READ securePathModel NOTIFY securePathModelChanged)
    Q_PROPERTY(QUrl securePathUrl READ securePathUrl NOTIFY securePathUrlChanged)
    Q_PROPERTY(bool isSecurePathUrlValid READ isSecurePathUrlValid NOTIFY isSecurePathUrlValidChanged)

private:
    static const int m_MAX_MODEL_SIZE;
    static const QString m_URL_FILE_SCHEME;
    QString m_editText;
    QStringList m_securePathModel;
    QString m_securePath;
    QUrl m_securePathUrl;
    bool m_isSecurePathUrlValid;

public:
    explicit SecureManager(QObject *parent = NULL);
    ~SecureManager();

public:
    void initialize();
    QStringList securePathModel() const;
    QUrl securePathUrl() const;
    bool isSecurePathUrlValid() const;

private:
    void setEditText(const QString &editText);
    void setSecurePathModel(const QStringList &securePathModel);
    void setSecurePath(const QString &securePath);
    void setSecurePathUrl(const QUrl &securePathUrl);
    void setIsSecurePathUrlValid(bool isSecurePathUrlValid);
    void updateSecurePathModel();

public slots:
    void onSecurePath(QString *securePath);
    void onIsSecurePathUrlValid(bool *isSecurePathUrlValid);
    void onUpdateEditText(const QString &editText);
    void onUpdateEditText(const QUrl &editText);
    void onEncryptFiles();
    void onDecryptFiles();

private slots:
    void onEditTextChanged(const QString &editText);

signals:
    void editTextChanged(const QString &editText);
    void securePathModel(QStringList *securePathModel);
    void securePathModelChanged(const QStringList &securePathModel);
    void securePathChanged(const QString &securePath);
    void securePathUrlChanged(const QUrl &securePathUrl);
    void isSecurePathUrlValidChanged(bool isSecurePathUrlValid);
};

#endif // SECUREMANAGER_H
