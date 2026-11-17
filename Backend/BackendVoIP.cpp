#include <iostream>
#include <string>
#include "BackendVoIP.h"
using namespace std;

enum CodecType {
    g711, g722, opus, ilbc, g729, g728, g7231MPCMLQ, g7231ACELP
};


float selectCodec(CodecType Codec){
    switch (Codec){
        case g711:
            return 64.0;
        case g722:
            return 64.0;
        case opus:
            return 32.0;
        case ilbc:
            return 15.2;
        case g729:
            return 8.0;
        case g728:
            return 16.0;
        case g7231MPCMLQ:
            return 6.3;
        case g7231ACELP:
            return 5.3;
        default:
            return 0;
    }

}

int selectL3L4(bool isCompressed, bool hasControlSum){
    if(!isCompressed){
        return 40;
    }
    if(hasControlSum){
        return 4;
    }
    return 2;
}

int selectL2L1(bool VLAN){
    if(!VLAN){
        return 38;
    }
    return 42;
}

int validatePtime(CodecType Codec, int requestedPtime) {
    switch (Codec) {
        case g711:
        case g722:
        case g729:
        case opus:
            if (requestedPtime > 0 && requestedPtime % 10 == 0) {
                return requestedPtime;
            }
            return 20;

        case ilbc:
            if (requestedPtime == 20 || requestedPtime == 30) {
                return requestedPtime;
            }
            return 20;

        case g7231MPCMLQ:
        case g7231ACELP:
            if (requestedPtime > 0 && requestedPtime % 30 == 0) {
                return requestedPtime;
            }
            return 30; 

        default:
            return 20;
    }
}

float voiceDetection(bool VAD){
    if(VAD){
        return 0.65;
    }
    return 1.0;
}

float calculateBandwidth(CodecType Codec, int requestedPtime, bool isCompressed, bool hasControlSum, bool VLAN, bool VAD){
    int ptime = validatePtime(Codec, requestedPtime);
    float codecKbps = selectCodec(Codec);

    float payloadBytes = (codecKbps * ptime)/8.0;

    float totalPacketBytes = payloadBytes + selectL3L4(isCompressed, hasControlSum) + selectL2L1(VLAN);
    float totalBandwidth = ((totalPacketBytes * 8.0)/ptime) * voiceDetection(VAD);
    
    return totalBandwidth;
}
