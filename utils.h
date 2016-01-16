#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QUrl>

#include "encryptionmanagerimpl.h"

class Utils
{
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
    static QString thumbnailGenerationStateToString(Enums::State state);
    static QString progressToString(float progress);
    static QString humanReadableFileSize(unsigned long long fileSize);
};

#endif // UTILS_H
