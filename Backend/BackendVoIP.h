// BackendVoIP.h
#ifndef BACKENDVOIP_H
#define BACKENDVOIP_H

enum CodecType {
    g711, g722, opus, ilbc, g729, g728, g7231MPCMLQ, g7231ACELP
};

float selectCodec(CodecType Codec);
int selectL3L4(bool isCompressed, bool hasControlSum);
int selectL2L1(bool VLAN);
int validatePtime(CodecType Codec, int requestedPtime);
float voiceDetection(bool VAD);
float calculateBandwidth(CodecType Codec, int requestedPtime, bool isCompressed, bool hasControlSum, bool VLAN, bool VAD);

#endif // BACKENDVOIP