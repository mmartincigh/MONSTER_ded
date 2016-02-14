// Qt
#include <QFileInfo>

// Local
#include "securemanager.h"
#include "utils.h"

const int SecureManager::m_MAX_MODEL_SIZE(5);
#if defined(Q_OS_WIN)
const QString SecureManager::m_URL_FILE_SCHEME("file:///");
#else
const QString SecureManager::m_URL_FILE_SCHEME("file://");
#endif

SecureManager::SecureManager(QObject *parent) :
    Base("SCRM", parent),
    m_editText(""),
    m_securePathModel(),
    m_securePath(""),
    m_securePathUrl(""),
    m_isSecurePathUrlValid(false)
{
    QObject::connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(onEditTextChanged(QString)));

    this->debug("Secure manager created");
}

SecureManager::~SecureManager()
{
    this->debug("Secure manager disposed of");
}

void SecureManager::initialize()
{
    QStringList source_path_model;
    emit this->securePathModel(&source_path_model);
    this->setSecurePathModel(source_path_model);

    this->debug("Initialized");
}

QStringList SecureManager::securePathModel() const
{
    return m_securePathModel;
}

QUrl SecureManager::securePathUrl() const
{
    return m_securePathUrl;
}

bool SecureManager::isSecurePathUrlValid() const
{
    return m_isSecurePathUrlValid;
}

void SecureManager::setEditText(const QString &editText)
{
    if (m_editText == editText)
    {
        return;
    }

    m_editText = editText;

    this->debug("Edit text changed: " + m_editText);

    emit this->editTextChanged(m_editText);
}

void SecureManager::setSecurePathModel(const QStringList &securePathModel)
{
    if (m_securePathModel == securePathModel)
    {
        return;
    }

    m_securePathModel = securePathModel;

    this->debug("Secure path model changed: " + m_securePathModel.join('|'));

    emit this->securePathModelChanged(m_securePathModel);
}

void SecureManager::setSecurePath(const QString &securePath)
{
    if (m_securePath == securePath)
    {
        return;
    }

    m_securePath = securePath;

    this->debug("Secure path changed: " + m_securePath);

    emit this->securePathChanged(m_securePath);
}

void SecureManager::setSecurePathUrl(const QUrl &securePathUrl)
{
    if (m_securePathUrl == securePathUrl)
    {
        return;
    }

    m_securePathUrl = securePathUrl;

    this->debug("Secure path URL changed: " + m_securePathUrl.toString());

    emit this->securePathUrlChanged(m_securePathUrl);
}

void SecureManager::setIsSecurePathUrlValid(bool isSecurePathUrlValid)
{
    if (m_isSecurePathUrlValid == isSecurePathUrlValid)
    {
        return;
    }

    m_isSecurePathUrlValid = isSecurePathUrlValid;

    this->debug("Is secure path URL valid changed: " + QString(m_isSecurePathUrlValid ? "true" : "false"));

    emit this->isSecurePathUrlValidChanged(m_isSecurePathUrlValid);
}

void SecureManager::updateSecurePathModel()
{
    // Do not change the model if the current path is invalid.
    if (!m_isSecurePathUrlValid)
    {
        return;
    }

    // Get the secure path model.
    QStringList source_path_model = m_securePathModel;

    // Check whether the model already contains the current path.
    if (source_path_model.contains(m_editText))
    {
        // Move the current path to the top.
        source_path_model.removeAll(m_editText);
        source_path_model.prepend(m_editText);
        this->setSecurePathModel(source_path_model);

        return;
    }

    // Make room for another entry if necessary.
    while (source_path_model.size() >= m_MAX_MODEL_SIZE)
    {
        // Remove the oldest (last) entry from the model.
        source_path_model.removeLast();
    }

    // Prepend the current path to the model.
    source_path_model.prepend(m_editText);
    this->setSecurePathModel(source_path_model);
}

void SecureManager::onSecurePath(QString *securePath)
{
    if (securePath == NULL)
    {
        return;
    }

    *securePath = m_securePath;
}

void SecureManager::onIsSecurePathUrlValid(bool *isSecurePathUrlValid)
{
    if (isSecurePathUrlValid == NULL)
    {
        return;
    }

    *isSecurePathUrlValid = m_isSecurePathUrlValid;
}

void SecureManager::onUpdateEditText(const QString &editText)
{
    this->setEditText(editText);
}

void SecureManager::onUpdateEditText(const QUrl &editText)
{
    QString edit_text = Utils::urlToString(editText);
    this->setEditText(edit_text);
}

void SecureManager::onEncryptFiles()
{
    this->updateSecurePathModel();
}

void SecureManager::onDecryptFiles()
{
    this->updateSecurePathModel();
}

void SecureManager::onEditTextChanged(const QString &editText)
{
    QFileInfo source_path_info(editText);
    bool is_source_path_valid = source_path_info.exists() && source_path_info.isDir();
    this->setIsSecurePathUrlValid(is_source_path_valid);

    if (!m_isSecurePathUrlValid)
    {
        // Do not update the secure path URL.

        return;
    }

    this->setSecurePath(editText);

    QString source_path(editText);
    QUrl source_path_url(source_path.prepend(m_URL_FILE_SCHEME));
    this->setSecurePathUrl(source_path_url);
}
