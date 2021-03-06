#ifndef UTILS_H
#define UTILS_H

// Qt
#include <QString>
#include <QUrl>
#include <QSystemTrayIcon>

// Local
#include "encryptionmanagerimpl.h"

class Utils
{
public:
    static const QString MEF_EXTENSION;
    static const QString MEF_FILE_EXTENSION;
    static const QString MEF_NAME_FILTER;
    static const QString URL_FILE_SCHEME;
    static const QChar LIST_SEPARATOR;

private:
    static const QUrl::FormattingOptions m_URL_CONVERSION_FLAGS;
    static const QRegExp m_FILE_SCHEME_REG_EXP;
#if defined(Q_OS_WIN)
    static const QRegExp m_LEADING_SLASHES_REG_EXP;
#endif
    static const QStringList m_FILE_SIZE_UNITS;
    static const float m_KILOBYTE_SIZE;

public:
    static QString urlToString(const QUrl &url);
    static QString processStateToString(Enums::ProcessState state);
    static QString processTypeToString(Enums::ProcessType type);
    static QString progressToString(float progress);
    static QString bytesToString(unsigned long long bytes);
    static QString pointerToString(void *pointer);
    static unsigned int levenshteinDistance(const std::string &from_string, const std::string &to_string);
    static unsigned int levenshteinDistance(const QString &from_string, const QString &to_string);
    static QString activationReasonToString(QSystemTrayIcon::ActivationReason reason);
};

#endif // UTILS_H
