// Qt
#include <QFileInfo>

// Local
#include "pathmanagerbase.h"
#include "utils.h"

const int PathManagerBase::m_MAX_MODEL_SIZE(5);

PathManagerBase::PathManagerBase(const QString &logTag, QObject *parent) :
    IPathManager(logTag, parent),
    m_editText(""),
    m_pathModel(),
    m_path(""),
    m_pathUrl(""),
    m_isPathUrlValid(false)
{
    QObject::connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(onEditTextChanged(QString)));
}

PathManagerBase::~PathManagerBase()
{
}

void PathManagerBase::initialize()
{
    QStringList path_model;
    emit this->pathModel(&path_model);
    this->setPathModel(path_model);

    this->debug("Initialized");
}

QStringList PathManagerBase::pathModel() const
{
    return m_pathModel;
}

QUrl PathManagerBase::pathUrl() const
{
    return m_pathUrl;
}

bool PathManagerBase::isPathUrlValid() const
{
    return m_isPathUrlValid;
}

void PathManagerBase::setEditText(const QString &editText)
{
    if (m_editText == editText)
    {
        return;
    }

    m_editText = editText;

    this->debug("Edit text changed: " + m_editText);

    emit this->editTextChanged(m_editText);
}

void PathManagerBase::setPathModel(const QStringList &pathModel)
{
    if (m_pathModel == pathModel)
    {
        return;
    }

    m_pathModel = pathModel;

    this->debug("Path model changed: " + m_pathModel.join(Utils::LIST_SEPARATOR));

    emit this->pathModelChanged(m_pathModel);
}

void PathManagerBase::setPath(const QString &path)
{
    if (m_path == path)
    {
        return;
    }

    m_path = path;

    this->debug("Path changed: " + m_path);

    emit this->pathChanged(m_path);
}

void PathManagerBase::setPathUrl(const QUrl &pathUrl)
{
    if (m_pathUrl == pathUrl)
    {
        return;
    }

    m_pathUrl = pathUrl;

    this->debug("Path URL changed: " + m_pathUrl.toString());

    emit this->pathUrlChanged(m_pathUrl);
}

void PathManagerBase::setIsPathUrlValid(bool isPathUrlValid)
{
    if (m_isPathUrlValid == isPathUrlValid)
    {
        return;
    }

    m_isPathUrlValid = isPathUrlValid;

    this->debug("Is path URL valid changed: " + QString(m_isPathUrlValid ? "true" : "false"));

    emit this->isPathUrlValidChanged(m_isPathUrlValid);
}

void PathManagerBase::updatePathModel()
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

void PathManagerBase::onPath(QString *path)
{
    if (path == NULL)
    {
        return;
    }

    *path = m_path;
}

void PathManagerBase::onIsPathUrlValid(bool *isPathUrlValid)
{
    if (isPathUrlValid == NULL)
    {
        return;
    }

    *isPathUrlValid = m_isPathUrlValid;
}

void PathManagerBase::onUpdateEditText(const QString &editText)
{
    this->setEditText(editText);
}

void PathManagerBase::onUpdateEditText(const QUrl &editText)
{
    QString edit_text = Utils::urlToString(editText);
    this->setEditText(edit_text);
}

void PathManagerBase::onProcess()
{
    this->updatePathModel();
}

void PathManagerBase::onEditTextChanged(const QString &editText)
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
    QUrl source_path_url(source_path.prepend(Utils::URL_FILE_SCHEME));
    this->setPathUrl(source_path_url);
}
