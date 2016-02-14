// Qt
#include <QStandardPaths>
#include <QUrl>

// Local
#include "settingsmanager.h"

const QString SettingsManager::m_SOURCE_PATH_MODEL_KEY("sourceManager/sourcePathModel");
const QStringList SettingsManager::m_FALL_BACK_SOURCE_PATH_MODEL;
const QString SettingsManager::m_SECURE_PATH_MODEL_KEY("sourceManager/securePathModel");
const QStringList SettingsManager::m_FALL_BACK_SECURE_PATH_MODEL;
const QString SettingsManager::m_DESTINATION_PATH_MODEL_KEY("sourceManager/destinationPathModel");
const QStringList SettingsManager::m_FALL_BACK_DESTINATION_PATH_MODEL;

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

void SettingsManager::onSourcePathModel(QStringList *sourcePathModel)
{
    if (sourcePathModel == NULL)
    {
        return;
    }

    *sourcePathModel = this->sourcePathModel();

    this->debug("Source path model: " + (*sourcePathModel).join('|'));
}

void SettingsManager::onSourcePathModelChanged(const QStringList &sourcePathModel)
{
    QStringList default_source_path_model = this->sourcePathModel();

    if (default_source_path_model == sourcePathModel)
    {
        return;
    }

    m_settings.setValue(m_SOURCE_PATH_MODEL_KEY, sourcePathModel);

    this->debug("Source path model changed: " + sourcePathModel.join('|'));
}

void SettingsManager::onSecurePathModel(QStringList *securePathModel)
{
    if (securePathModel == NULL)
    {
        return;
    }

    *securePathModel = this->securePathModel();

    this->debug("Secure path model: " + (*securePathModel).join('|'));
}

void SettingsManager::onSecurePathModelChanged(const QStringList &securePathModel)
{
    QStringList default_secure_path_model = this->securePathModel();

    if (default_secure_path_model == securePathModel)
    {
        return;
    }

    m_settings.setValue(m_SECURE_PATH_MODEL_KEY, securePathModel);

    this->debug("Secure path model changed: " + securePathModel.join('|'));
}

void SettingsManager::onDestinationPathModel(QStringList *destinationPathModel)
{
    if (destinationPathModel == NULL)
    {
        return;
    }

    *destinationPathModel = this->destinationPathModel();

    this->debug("Destination path model: " + (*destinationPathModel).join('|'));
}

void SettingsManager::onDestinationPathModelChanged(const QStringList &destinationPathModel)
{
    QStringList default_destination_path_model = this->destinationPathModel();

    if (default_destination_path_model == destinationPathModel)
    {
        return;
    }

    m_settings.setValue(m_DESTINATION_PATH_MODEL_KEY, destinationPathModel);

    this->debug("Destination path model changed: " + destinationPathModel.join('|'));
}
