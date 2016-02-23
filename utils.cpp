// Local
#include "utils.h"

const QString Utils::MEF_EXTENSION("mef");
const QString Utils::MEF_FILE_EXTENSION("." + MEF_EXTENSION);
const QString Utils::MEF_NAME_FILTER("*" + MEF_FILE_EXTENSION);
#if defined(Q_OS_WIN)
const QString Utils::URL_FILE_SCHEME("file:///");
#else
const QString Utils::URL_FILE_SCHEME("file://");
#endif
const QChar Utils::LIST_SEPARATOR('|');

const QUrl::FormattingOptions Utils::m_URL_CONVERSION_FLAGS(QUrl::PrettyDecoded | QUrl::PreferLocalFile);
const QRegExp Utils::m_FILE_SCHEME_REG_EXP("^file://");
#if defined(Q_OS_WIN)
const QRegExp Utils::m_LEADING_SLASHES_REG_EXP("^/*");
#endif
const QStringList Utils::m_FILE_SIZE_UNITS(QStringList() << "B" << "kB" << "MB" << "GB" << "TB");
const float Utils::m_KILOBYTE_SIZE(1024.0);

QString Utils::urlToString(const QUrl &url)
{
    QString string = url.toString(m_URL_CONVERSION_FLAGS).remove(m_FILE_SCHEME_REG_EXP);
#if defined(Q_OS_WIN)
    string.remove(m_LEADING_SLASHES_REG_EXP);
#endif
    return string;
}

QString Utils::processStateToString(Enums::ProcessState state)
{
    switch (state)
    {
    case Enums::ProcessState_Idle:
        return "idle";
    case Enums::ProcessState_Working:
        return "working";
    case Enums::ProcessState_Paused:
        return "paused";
    case Enums::ProcessState_Stopped:
        return "stopped";
    case Enums::ProcessState_Completed:
        return "completed";
    default:
        return "unknown";
    }
}

QString Utils::processTypeToString(Enums::ProcessType type)
{
    switch (type)
    {
    case Enums::ProcessType_None:
        return "none";
    case Enums::ProcessType_Encryption:
        return "encryption";
    case Enums::ProcessType_Decryption:
        return "decryption";
    default:
        return "unknown";
    }
}

QString Utils::progressToString(float progress)
{
    return QString("%1 %").arg(progress * 100, 0, 'f', 1);
}

QString Utils::bytesToString(unsigned long long bytes)
{
    QStringListIterator i(m_FILE_SIZE_UNITS);
    QString size_unit(i.next());
    double bytes_double = static_cast<float>(bytes);
    while (bytes_double >= m_KILOBYTE_SIZE
           && i.hasNext())
    {
        size_unit = i.next();
        bytes_double /= m_KILOBYTE_SIZE;
    }

    return QString("%1%2").arg(bytes_double, 0, 'g', 5).arg(size_unit);
}

QString Utils::pointerToString(void *pointer)
{
    return QString("0x%1").arg(QString("%1").arg((quintptr)pointer, QT_POINTER_SIZE * 2, 16, QChar('0')).toUpper());
}

unsigned int Utils::levenshteinDistance(const std::string &from_string, const std::string &to_string)
{
    const size_t from_string_size = from_string.size();
    const size_t to_string_size = to_string.size();

    std::vector<unsigned int> column(to_string_size + 1);
    std::vector<unsigned int> previous_column(to_string_size + 1);

    for (unsigned int i = 0; i < previous_column.size(); i++)
    {
        previous_column[i] = i;
    }
    for (unsigned int i = 0; i < from_string_size; i++)
    {
        column[0] = i + 1;
        for (unsigned int j = 0; j < to_string_size; j++)
        {
            column[j + 1] = std::min(previous_column[j + 1] + 1, std::min(column[j] + 1, previous_column[j] + (from_string[i] == to_string[j] ? 0 : 1)));
        }
        column.swap(previous_column);
    }

    return previous_column[to_string_size];
}
unsigned int Utils::levenshteinDistance(const QString &from_string, const QString &to_string)
{
    return levenshteinDistance(from_string.toStdString(), to_string.toStdString());
}
