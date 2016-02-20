// Local
#include "statsmanager.h"
#include "utils.h"

const QString StatsManager::m_CURRENT_INPUT_FILE_NONE("none");

StatsManager::StatsManager(QObject *parent) :
    Base("STSM", parent),
    m_processedBytes(0),
    m_processedBytesString(Utils::bytesToString(m_processedBytes)),
    m_bytesToProcess(0),
    m_bytesToProcessString(Utils::bytesToString(m_bytesToProcess)),
    m_errors(0),
    m_warnings(0),
    m_skipped(0),
    m_overwritten(0),
    m_processed(0),
    m_currentInputFile(m_CURRENT_INPUT_FILE_NONE)
{
    this->debug("Stats manager created");
}

StatsManager::~StatsManager()
{
    this->debug("Stats manager disposed of");
}

unsigned long long StatsManager::processedBytes() const
{
    return m_processedBytes;
}

QString StatsManager::processedBytesString() const
{
    return m_processedBytesString;
}

unsigned long long StatsManager::bytesToProcess() const
{
    return m_bytesToProcess;
}

QString StatsManager::bytesToProcessString() const
{
    return m_bytesToProcessString;
}

int StatsManager::errors() const
{
    return m_errors;
}

int StatsManager::warnings() const
{
    return m_warnings;
}

int StatsManager::skipped() const
{
    return m_skipped;
}

int StatsManager::overwritten() const
{
    return m_overwritten;
}

int StatsManager::processed() const
{
    return m_processed;
}

QString StatsManager::currentInputFile() const
{
    return m_currentInputFile;
}

void StatsManager::setProcessedBytes(unsigned long long processedBytes)
{
    if (m_processedBytes == processedBytes)
    {
        return;
    }

    m_processedBytes = processedBytes;

    //this->debug("Processed bytes changed: " + QString::number(m_processedBytes));

    emit this->processedBytesChanged(m_processedBytes);
}

void StatsManager::setProcessedBytesString(const QString &processedBytesString)
{
    if (m_processedBytesString == processedBytesString)
    {
        return;
    }

    m_processedBytesString = processedBytesString;

    //this->debug("Processed bytes string changed: " + m_processedBytesString);

    emit this->processedBytesStringChanged(m_processedBytesString);
}

void StatsManager::setBytesToProcess(unsigned long long bytesToProcess)
{
    if (m_bytesToProcess == bytesToProcess)
    {
        return;
    }

    m_bytesToProcess = bytesToProcess;

    this->debug("Bytes to encrypt changed: " + QString::number(m_bytesToProcess));

    emit this->bytesToProcessChanged(m_bytesToProcess);
}

void StatsManager::setBytesToProcessString(const QString &bytesToProcessString)
{
    if (m_bytesToProcessString == bytesToProcessString)
    {
        return;
    }

    m_bytesToProcessString = bytesToProcessString;

    this->debug("Bytes to encrypt string changed: " + m_bytesToProcessString);

    emit this->bytesToProcessStringChanged(m_bytesToProcessString);
}

void StatsManager::setErrors(int errors)
{
    if (m_errors == errors)
    {
        return;
    }

    m_errors = errors;

    this->debug("Errors changed: " + QString::number(m_errors));

    emit this->errorsChanged(m_errors);
}

void StatsManager::setWarnings(int warnings)
{
    if (m_warnings == warnings)
    {
        return;
    }

    m_warnings = warnings;

    this->debug("Warnings changed: " + QString::number(m_warnings));

    emit this->warningsChanged(m_warnings);
}

void StatsManager::setSkipped(int skipped)
{
    if (m_skipped == skipped)
    {
        return;
    }

    m_skipped = skipped;

    this->debug("Skipped changed: " + QString::number(m_skipped));

    emit this->skippedChanged(m_skipped);
}

void StatsManager::setOverwritten(int overwritten)
{
    if (m_overwritten == overwritten)
    {
        return;
    }

    m_overwritten = overwritten;

    this->debug("Overwritten changed: " + QString::number(m_overwritten));

    emit this->overwrittenChanged(m_overwritten);
}

void StatsManager::setProcessed(int processed)
{
    if (m_processed == processed)
    {
        return;
    }

    m_processed = processed;

    this->debug("Processed changed: " + QString::number(m_processed));

    emit this->processedChanged(m_processed);
}

void StatsManager::setCurrentInputFile(const QString &currentInputFile)
{
    if (m_currentInputFile == currentInputFile)
    {
        return;
    }

    m_currentInputFile = currentInputFile;

    this->debug("Current input file changed: " + m_currentInputFile);

    emit this->currentInputFileChanged(m_currentInputFile);
}

void StatsManager::onProcessedBytesChanged(unsigned long long processedBytes)
{
    this->setProcessedBytes(processedBytes);
    this->setProcessedBytesString(Utils::bytesToString(processedBytes));
}
void StatsManager::onBytesToProcessChanged(unsigned long long bytesToProcess)
{
    this->setBytesToProcess(bytesToProcess);
    this->setBytesToProcessString(Utils::bytesToString(bytesToProcess));
}

void StatsManager::onErrorsChanged(int errors)
{
    this->setErrors(errors);
}

void StatsManager::onWarningsChanged(int warnings)
{
    this->setWarnings(warnings);
}

void StatsManager::onSkippedChanged(int skipped)
{
    this->setSkipped(skipped);
}

void StatsManager::onOverwrittenChanged(int overwritten)
{
    this->setOverwritten(overwritten);
}

void StatsManager::onProcessedChanged(int processed)
{
    this->setProcessed(processed);
}

void StatsManager::onCurrentInputFileChanged(const QString &currentInputFile)
{
    this->setCurrentInputFile(currentInputFile);
}
