#ifndef ENUMS
#define ENUMS

// Qt
#include <QObject>

class Enums : public QObject
{
    Q_OBJECT
    Q_ENUMS(ProcessType)
    Q_ENUMS(ProcessState)

public:
    enum ProcessType
    {
        ProcessType_None,
        ProcessType_Encryption,
        ProcessType_Decryption,
    };

    enum ProcessState
    {
        ProcessState_Idle,
        ProcessState_Working,
        ProcessState_Paused,
        ProcessState_Stopped,
        ProcessState_Completed
    };
};

#endif // ENUMS
