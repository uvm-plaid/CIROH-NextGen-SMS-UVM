#include <stdint.h>
#include <TimeLib.h>

namespace sat {
    enum class SatCode: uint8_t {
        Okay,
        CmdTooLong,
        ResponseTooLong,
        RecvErr,
        InvalidResponse,
        NetworkErr,
    };

    // SBDIX: Short Burst Data Extended
    // Response from SBDIX command to initiate packet transfer
    struct SbdixResponse {
        enum class MoStatus: uint8_t {
            Success = 0,                // MO message transferred successfully
            SuccessButTooBig = 1,       // MO message transferred successfully, but MT message too big
            SuccessButLocationUpdateFailed = 2, // MO message transferred successfully, but location update not accepted
            Reserved1 = 3,              // Reserved, indicates MO session success if used
            Reserved2 = 4,              // Reserved, indicates MO session success if used
            Reserved3 = 5,              // Reserved, indicates MO session failure if used
            Reserved4 = 6,              // Reserved, indicates MO session failure if used
            Reserved5 = 7,              // Reserved, indicates MO session failure if used
            Reserved6 = 8,              // Reserved, indicates MO session failure if used
            GssTimeout = 10,            // GSS reported that the call did not complete in time
            MqFull = 11,                // MO message queue at GSS is full
            TooManySegments = 12,       // MO message has too many segments
            GssSessionFailure = 13,     // GSS reported that session did not complete
            InvalidSegmentSize = 14,    // Invalid segment size
            AccessDenied = 15,          // Access denied
            IsuLocked = 16,             // ISU locked, cannot make SBD calls
            GatewayTimeout = 17,        // Gateway not responding, local session timeout
            ConnectionLost = 18,        // Connection lost (RF drop)
            LinkFailure = 19,           // Link failure (protocol error)
            ReservedFailureStart = 20,  // Reserved, indicates failure if used (20-31)
            ReservedFailureEnd = 31,
            NoNetworkService = 32,      // No network service, unable to initiate call
            AntennaFault = 33,          // Antenna fault, unable to initiate call
            RadioDisabled = 34,         // Radio disabled, unable to initiate call
            IsuBusy = 35,               // ISU busy, unable to initiate call
            TryLater = 36,              // Try later, wait 3 minutes since last registration
            SbdServiceDisabled = 37,    // SBD service temporarily disabled
            TrafficMgmtPeriod = 38,     // Try later, traffic management period
            ReservedFailure2Start = 39, // Reserved, indicates failure if used (39-63)
            ReservedFailure2End = 63,
            BandViolation = 64,         // Band violation, outside permitted frequency
            PllLockFailure = 65         // PLL lock failure, hardware error during transmission
        };

        enum class MtStatus: uint8_t {
            NoMsg,
            MsgRecv,
            Err,
        };

        MoStatus mo_status;  // Small subset of all possible codes
        MtStatus mt_status; // Disposition of mobile terminated transaction
        uint16_t momsn;  // Mobile Originated Message Sequence Number
        uint32_t mt_length;  // Length in bytes of the mobile terminated message
        uint32_t mt_queued;  // # queued messages waiting at GSS
    };

    void set_echo(bool echo); 
    sat::SatCode initiate_transfer(sat::SbdixResponse &response);
    sat::SatCode send_packet(const uint8_t buf[], uint16_t nbytes);
    sat::SatCode get_time(tmElements_t &time);
    sat::SatCode get_manufacturer();
}
