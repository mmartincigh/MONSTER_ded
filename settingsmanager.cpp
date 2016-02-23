// Qt
#include <QStandardPaths>
#include <QUrl>

// Local
#include "settingsmanager.h"
#include "utils.h"

const QString SettingsManager::m_SOURCE_PATH_MODEL_KEY("sourcePathManager/sourcePathModel");
const QStringList SettingsManager::m_FALL_BACK_SOURCE_PATH_MODEL;
const QString SettingsManager::m_SECURE_PATH_MODEL_KEY("securePathManager/securePathModel");
const QStringList SettingsManager::m_FALL_BACK_SECURE_PATH_MODEL;
const QString SettingsManager::m_DESTINATION_PATH_MODEL_KEY("destinationPathManager/destinationPathModel");
const QStringList SettingsManager::m_FALL_BACK_DESTINATION_PATH_MODEL;
const QString SettingsManager::m_OPEN_FILE_KEY("processManager/openFile");
const bool SettingsManager::m_FALL_BACK_OPEN_FILE(false);

SettingsManager::SettingsManager(QObject *parent) :
    Base("SM", parent),
    m_settings(this)
{
    this->debug("Settings manager created");
}

SettingsManager::~SettingsManager()
{
    this->debug("Settings manager disposed of");
}

QStringList SettingsManager::sourcePathModel() const
{
    return m_settings.value(m_SOURCE_PATH_MODEL_KEY, m_FALL_BACK_SOURCE_PATH_MODEL).toStringList();
}

QStringList SettingsManager::securePathModel() const
{
    return m_settings.value(m_SECURE_PATH_MODEL_KEY, m_FALL_BACK_SECURE_PATH_MODEL).toStringList();
}

QStringList SettingsManager::destinationPathModel() const
{
    return m_settings.value(m_DESTINATION_PATH_MODEL_KEY, m_FALL_BACK_DESTINATION_PATH_MODEL).toStringList();
}

bool SettingsManager::openFile() const
{
    return m_settings.value(m_OPEN_FILE_KEY, m_FALL_BACK_OPEN_FILE).toBool();
}

void SettingsManager::onSourcePathModel(QStringList *sourcePathModel)
{
    if (sourcePathModel == NULL)
    {
        return;
    }

    *sourcePathModel = this->sourcePathModel();

    this->debug("Source path model: " + (*sourcePathModel).join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onSourcePathModelChanged(const QStringList &sourcePathModel)
{
    QStringList default_source_path_model = this->sourcePathModel();

    if (default_source_path_model == sourcePathModel)
    {
        return;
    }

    m_settings.setValue(m_SOURCE_PATH_MODEL_KEY, sourcePathModel);

    this->debug("Source path model changed: " + sourcePathModel.join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onSecurePathModel(QStringList *securePathModel)
{
    if (securePathModel == NULL)
    {
        return;
    }

    *securePathModel = this->securePathModel();

    this->debug("Secure path model: " + (*securePathModel).join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onSecurePathModelChanged(const QStringList &securePathModel)
{
    QStringList default_secure_path_model = this->securePathModel();

    if (default_secure_path_model == securePathModel)
    {
        return;
    }

    m_settings.setValue(m_SECURE_PATH_MODEL_KEY, securePathModel);

    this->debug("Secure path model changed: " + securePathModel.join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onDestinationPathModel(QStringList *destinationPathModel)
{
    if (destinationPathModel == NULL)
    {
        return;
    }

    *destinationPathModel = this->destinationPathModel();

    this->debug("Destination path model: " + (*destinationPathModel).join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onDestinationPathModelChanged(const QStringList &destinationPathModel)
{
    QStringList default_destination_path_model = this->destinationPathModel();

    if (default_destination_path_model == destinationPathModel)
    {
        return;
    }

    m_settings.setValue(m_DESTINATION_PATH_MODEL_KEY, destinationPathModel);

    this->debug("Destination path model changed: " + destinationPathModel.join(Utils::LIST_SEPARATOR));
}

void SettingsManager::onOpenFile(bool *openFile)
{
    if (openFile == NULL)
    {
        return;
    }

    *openFile = this->openFile();

    this->debug("Open file: " + QString(*openFile ? "true" : "false"));
}

void SettingsManager::onOpenFileChanged(bool openFile)
{
    bool default_open_file = this->openFile();

    if (default_open_file == openFile)
    {
        return;
    }

    m_settings.setValue(m_OPEN_FILE_KEY, openFile);

    this->debug("Open file changed: " + QString(openFile ? "true" : "false"));
}
