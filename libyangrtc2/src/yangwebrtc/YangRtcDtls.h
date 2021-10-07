﻿#ifndef YANGWEBRTC_YANGRTCDTLS_H_
#define YANGWEBRTC_YANGRTCDTLS_H_
#include <stdint.h>
#include <string>
#include <string.h>
#include <yangrtp/YangRtpConstant.h>
#include <yangutil/sys/YangSsl.h>

#include <srtp2/srtp.h>
#include <yangutil/sys/YangTimer.h>
using namespace std;

//#define RTMP_SIG_SRS_DOMAIN "ossrs.net"
class YangCertificate;
class YangSRtp;
enum YangDtlsRole {
    YangDtlsRoleClient,
    YangDtlsRoleServer
};
enum YangDtlsState {
    YangDtlsStateInit, // Start.
    YangDtlsStateClientHello, // Should start ARQ thread.
    YangDtlsStateServerHello, // We are in the first ARQ state.
    YangDtlsStateClientCertificate, // Should start ARQ thread again.
    YangDtlsStateServerDone, // We are in the second ARQ state.
    YangDtlsStateClientDone, // Done.
};

enum YangDtlsVersion {
    YangDtlsVersionAuto = -1,
    YangDtlsVersion1_0,
    YangDtlsVersion1_2
};
class YangSendUdpData{
public:
	YangSendUdpData(){};
	virtual ~YangSendUdpData(){};
	virtual int32_t sendData(char* p,int32_t plen)=0;
};
class YangRtcDtls :public YangThread{
public:
	YangRtcDtls();
	virtual ~YangRtcDtls();
	int32_t init(YangSendUdpData* pudp);
	int32_t doHandshake();
	int32_t startHandShake();
	int32_t decodeHandshake(char* p,int32_t len);
	int32_t sendDtlsAlert();
	int32_t filter_data(uint8_t* data, int32_t size);
	bool should_reset_timer();
	int32_t on_dtls_alert(std::string type, std::string desc);
    int32_t get_srtp_key(char* recv_key, int *precvkeylen,char* send_key,int *psendkeylen);
	YangSRtp* getSrtp();
	YangDtlsState getDtlsState();
	int32_t m_isStart;
	int32_t m_loop;
	void stop();
protected:
	void run();
	int32_t startLoop();
	void stopLoop();

	int32_t on_handshake_done();
protected:
	YangCertificate *m_cer;
	YangSRtp* m_srtp;
	YangDtlsState m_state;
    SSL_CTX* m_sslctx;
    bool reset_timer_;
    YangSendUdpData* m_udp;
    SSL* m_ssl;
    BIO* m_bioIn;
    BIO* m_bioOut;
    YangDtlsVersion m_version;

    bool m_handshake_done;
    SSL_CTX* srs_build_dtls_ctx(YangDtlsVersion version, std::string role);
};




class YangSRtp
{
public:
    YangSRtp();
    virtual ~YangSRtp();
public:
    int32_t init(char* recv_key,int precvkeylen, char* send_key,int psendkeylen);
public:
    int32_t enc_rtp(void* packet, int* nb_cipher);
    int32_t enc_rtcp(void* packet, int* nb_cipher);
    int32_t dec_rtp(void* packet, int* nb_plaintext);
    int32_t dec_rtcp(void* packet, int* nb_plaintext);
private:
    srtp_t m_recvCtx;
    srtp_t m_sendCtx;
};

#endif /* YANGWEBRTC_YANGRTCDTLS_H_ */
