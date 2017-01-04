#ifndef ERLTLS_C_SRC_ERLTLS_NIF_H_
#define ERLTLS_C_SRC_ERLTLS_NIF_H_

#include "erl_nif.h"

struct atoms
{
    ERL_NIF_TERM atomOk;
    ERL_NIF_TERM atomTrue;
    ERL_NIF_TERM atomFalse;
    ERL_NIF_TERM atomError;
    ERL_NIF_TERM atomBadArg;
    ERL_NIF_TERM atomOptions;

    ERL_NIF_TERM atomVerifyNone;
    ERL_NIF_TERM atomVerifyPeer;

    ERL_NIF_TERM atomSslNotStarted;

    ERL_NIF_TERM atomCtxCertfile;
    ERL_NIF_TERM atomCtxDhfile;
    ERL_NIF_TERM atomCtxCacerts;
    ERL_NIF_TERM atomCtxCiphers;
    ERL_NIF_TERM atomCtxReuseSessionsTtl;
    ERL_NIF_TERM atomCtxUseSessionTicket;

    ERL_NIF_TERM atomCtxVerify;
    ERL_NIF_TERM atomCtxFailIfNoPeerCert;
    ERL_NIF_TERM atomCtxDepth;
};

struct erltls_data
{
    ErlNifResourceType* res_ssl_ctx;
    ErlNifResourceType* res_ssl_sock;
};

extern atoms ATOMS;

#endif
