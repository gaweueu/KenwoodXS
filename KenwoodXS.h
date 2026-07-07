// SPDX-License-Identifier: Apache-2.0

#ifndef KenwoodXS_H__
#define KenwoodXS_H__

#include <Arduino.h>

// Kenwood XS System Control Commands
enum KenwoodXSCommand {
    // Miscellaneous
    POWER_ON     = 181,
    POWER_OFF    = 185,
    BAND         = 123,
    PLUS_10      = 164,
    
    // Input Selection
    TAPE_2_ON    = 20,
    TAPE_2_OFF   = 24,
    VIDEO_2      = 113,
    PHONO        = 115,
    TAPE_1       = 121,
    VIDEO_1      = 122,
    TUNER        = 123,
    CD           = 125,

    // Tape A Controls
    TA_FAST_FORWARD = 67,
    TA_STOP         = 69,
    TA_PLAY         = 71,
    TA_RECORD       = 73,
    TA_REWIND       = 75,
    TA_PAUSE        = 77,
    TA_REWIND_SLOW  = 79,

    // Tape B Controls
    TB_FAST_FORWARD = 66,
    TB_STOP         = 68,
    TB_PLAY         = 70,
    TB_RECORD       = 72,
    TB_REWIND       = 74,
    TB_PAUSE        = 76,
    TB_REWIND_SLOW  = 78,

    // CD Controls
    CD_PLAY_PAUSE   = 194,
    CD_STOP         = 198,
    CD_DISC         = 174,
    CD_FAST_FORWARD = 161,
    CD_REWIND       = 169,
    CD_NEXT_TRACK   = 192,
    CD_PREV_TRACK   = 200,
    CD_SCRUB_STOP   = 177,
    CD_START        = 222
};

enum KenwoodXSMode {
    KENWOOD_XS_TRANSMIT_ONLY,
    KENWOOD_XS_BIDIRECTIONAL
};

class KenwoodXS {

public:
typedef void (*OnCommandSentCallback)(byte command);
typedef void (*OnCommandReceivedCallback)(byte command);

    KenwoodXS(int ctrlPin, int sdatPin);
    void begin(KenwoodXSMode mode = KENWOOD_XS_TRANSMIT_ONLY);
    void begin(bool bidirectional);
    void setMode(KenwoodXSMode mode);
    KenwoodXSMode mode() const;
    void sendCommand(byte command);
    void sendCommand(KenwoodXSCommand command);
    void poll();
    bool available();
    bool readCommand(byte &command);
    bool readCommand(byte &command, unsigned long timeoutMs);
    void powerOn();
    void powerOff();
    void selectInput(KenwoodXSCommand input);
    void playTapeA();
    void stopTapeA();
    void playTapeB();
    void stopTapeB();
    void playCD();
    void stopCD();
    void startCD();
    void tryAllCommands(unsigned long delayMs = 1000);
    void onCommandSentCallback(OnCommandSentCallback callback);
    void onCommandReceivedCallback(OnCommandReceivedCallback callback);
    void setDebugOutput(bool enabled);

private:
    enum ReceiveState {
        RX_IDLE,
        RX_WAIT_START_RISE,
        RX_WAIT_START_FALL,
        RX_WAIT_BIT_RISE,
        RX_WAIT_BIT_FALL,
        RX_WAIT_CTRL_LOW
    };

    void sendByte(byte b);
    void driveIdle();
    void releaseBus();
    void resetReceiveState();
    void finishReceivedCommand(byte command);
    bool classifyPulse(unsigned long widthUs, bool &bitValue) const;
    int _ctrlPin;
    int _sdatPin;
    KenwoodXSMode _mode;
    bool _debugOutput;
    bool _hasReceivedCommand;
    byte _lastReceivedCommand;
    ReceiveState _rxState;
    byte _rxCommand;
    byte _rxBitCount;
    unsigned long _rxStateStartedAt;
    unsigned long _rxLowStartedAt;
    OnCommandSentCallback _onCommandSentCallback;
    OnCommandReceivedCallback _onCommandReceivedCallback;
    
    // Protocol timing constants
    static const int BIT_0 = 15000;  // 15ms for bit 0
    static const int BIT_1 = 7500;   // 7.5ms for bit 1
    static const int START_BIT_DELAY = 15;  // 15ms start bit
    static const int CTRL_SETUP_DELAY = 2;  // 2ms CTRL setup time
    static const int FRAME_SIGNAL_DELAY = 7500;  // 7.5ms frame signal
    static const unsigned long RX_ONE_ZERO_THRESHOLD_US = 11250;
    static const unsigned long RX_MIN_PULSE_US = 4000;
    static const unsigned long RX_MAX_PULSE_US = 20000;
    static const unsigned long RX_START_TIMEOUT_US = 30000;
    static const unsigned long RX_FRAME_TIMEOUT_US = 30000;
};

#endif // KenwoodXS_H__
