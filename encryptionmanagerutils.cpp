#include "encryptionmanagerutils.h"

QString EncryptionManagerUtils::stateToString(EncryptionStates state)
{
    QString description;

    switch (state)
    {
    case EncryptionIdle:
        description = "idle";

        break;
    case EncryptionEncrypting:
        description = "encrypting";

        break;
    case EncryptionPaused:
        description = "paused";

        break;
    case EncryptionStopped:
        description = "stopped";

        break;
    case EncryptionCompleted:
        description = "completed";

        break;
    default:
        description = "unknown";

        break;
    }

    return description;
}
