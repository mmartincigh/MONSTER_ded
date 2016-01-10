#ifndef ENCRYPTIONMANAGERUTILS_H
#define ENCRYPTIONMANAGERUTILS_H

#include <QObject>

#include "encryptionstates.h"

class EncryptionManagerUtils : public QObject
{
    Q_OBJECT

public:
    static QString stateToString(EncryptionStates state);
};

#endif // ENCRYPTIONMANAGERUTILS_H
