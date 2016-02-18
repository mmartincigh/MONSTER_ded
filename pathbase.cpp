// Qt
#include <QFileInfo>

// Local
#include "pathbase.h"
#include "utils.h"

const int PathBase::m_MAX_MODEL_SIZE(5);
#if defined(Q_OS_WIN)
const QString PathBase::m_URL_FILE_SCHEME("file:///");
#else
const QString PathBase::m_URL_FILE_SCHEME("file://");
#endif

PathBase::PathBase(const QString &logTag, QObject *parent) :
    IPath(logTag, parent),
    m_editText(""),
    m_pathModel(),
    m_path(""),
    m_pathUrl(""),
    m_isPathUrlValid(false)
{
    QObject::connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(onEditTextChanged(QString)));
}

PathBase::~PathBase()
{
}

void PathBase::initialize()
{
    QStringList path_model;
    emit this->pathModel(&path_model);
    this->setPathModel(path_model);

    this->debug("Initialized");
}

QStringList PathBase::pathModel() const
{
    return m_pathModel;
}

QUrl PathBase::pathUrl() const
{
    return m_pathUrl;
}

bool PathBase::isPathUrlValid() const
{
    return m_isPathUrlValid;
}

void PathBase::setEditText(const QString &editText)
{
    if (m_editText == editText)
    {
        return;
    }

    m_editText = editText;

    this->debug("Edit text changed: " + m_editText);

    emit this->editTextChanged(m_editText);
}

void PathBase::setPathModel(const QStringList &pathModel)
{
    if (m_pathModel == pathModel)
    {
        return;
    }

    m_pathModel = pathModel;

    this->debug("Path model changed: " + m_pathModel.join('|'));

    emit this->pathModelChanged(m_pathModel);
}

void PathBase::setPath(const QString &path)
{
    if (m_path == path)
    {
        return;
    }

    m_path = path;

    this->debug("Path changed: " + m_path);

    emit this->pathChanged(m_path);
}

void PathBase::setPathUrl(const QUrl &pathUrl)
{
    if (m_pathUrl == pathUrl)
    {
        return;
    }

    m_pathUrl = pathUrl;

    this->debug("Path URL changed: " + m_pathUrl.toString());

    emit this->pathUrlChanged(m_pathUrl);
}

void PathBase::setIsPathUrlValid(bool isPathUrlValid)
{
    if (m_isPathUrlValid == isPathUrlValid)
    {
        return;
    }

    m_isPathUrlValid = isPathUrlValid;

    this->debug("Is path URL valid changed: " + QString(m_isPathUrlValid ? "true" : "false"));

    emit this->isPathUrlValidChanged(m_isPathUrlValid);
}

void PathBase::updatePathModel()
{
    // Do not change the model if the current path is invalid.
    if (!m_isPathUrlValid)
    {
        return;
    }

    // Get the path model.
    QStringList path_model = m_pathModel;

    // Check whether the model already contains the current path.
    if (path_model.contains(m_editText))
    {
        // Move the current path to the top.
        path_model.removeAll(m_editText);
        path_model.prepend(m_editText);
        this->setPathModel(path_model);

        return;
    }

    // Make room for another entry if necessary.
    while (path_model.size() >= m_MAX_MODEL_SIZE)
    {
        // Remove the oldest (last) entry from the model.
        path_model.removeLast();
    }

    // Prepend the current path to the model.
    path_model.prepend(m_editText);
    this->setPathModel(path_model);
}

void PathBase::onPath(QString *path)
{
    if (path == NULL)
    {
        return;
    }

    *path = m_path;
}

void PathBase::onIsPathUrlValid(bool *isPathUrlValid)
{
    if (isPathUrlValid == NULL)
    {
        return;
    }

    *isPathUrlValid = m_isPathUrlValid;
}

void PathBase::onUpdateEditText(const QString &editText)
{
    this->setEditText(editText);
}

void PathBase::onUpdateEditText(const QUrl &editText)
{
    QString edit_text = Utils::urlToString(editText);
    this->setEditText(edit_text);
}

void PathBase::onProcess()
{
    this->updatePathModel();
}

void PathBase::onEditTextChanged(const QString &editText)
{
    QFileInfo source_path_info(editText);
    bool is_source_path_valid = source_path_info.exists() && source_path_info.isDir();
    this->setIsPathUrlValid(is_source_path_valid);

    if (!m_isPathUrlValid)
    {
        // Do not update the path URL.

        return;
    }

    this->setPath(editText);

    QString source_path(editText);
    QUrl source_path_url(source_path.prepend(m_URL_FILE_SCHEME));
    this->setPathUrl(source_path_url);
}
