#include "nif_ssl_socket.h"
#include "nif_ssl_context.h"
#include "tlssocket.h"
#include "erltls_nif.h"
#include "macros.h"
#include "nif_utils.h"

static const char kErrorFailedToAllocNifSocket[] = "failed to alloc enif_ssl_socket";
static const char kErrorFailedToAllocSslSocket[] = "failed to alloc ssl socket";
static const char kErrorFailedToInitSslSocket[]  = "failed to init ssl socket";

struct enif_ssl_socket
{
    TlsSocket* socket;
};

enif_ssl_socket* new_nif_socket(ErlNifResourceType* res)
{
    return static_cast<enif_ssl_socket*>(enif_alloc_resource(res, sizeof(enif_ssl_socket)));
}

ERL_NIF_TERM enif_ssl_socket_new(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    
    int role;
    long flags;
    SSL_CTX* ctx = get_context(env, data, argv[0]);
    
    if(!ctx)
        return make_badarg(env);

    if(!enif_get_int(env, argv[1], &role))
        return make_badarg(env);
    
    if(!enif_get_long(env, argv[2], &flags))
        return make_badarg(env);
    
    scoped_ptr(nif_socket, enif_ssl_socket, new_nif_socket(data->res_ssl_sock), enif_release_resource);
    
    if(nif_socket.get() == NULL)
        return make_error(env, kErrorFailedToAllocNifSocket);
    
    TlsSocket* socket = new TlsSocket();
    
    if(socket == NULL)
        return make_error(env, kErrorFailedToAllocSslSocket);
    
    if(!socket->Init(ctx, static_cast<TlsSocket::kSslRole>(role), flags))
        return make_error(env, kErrorFailedToInitSslSocket);
    
    nif_socket->socket = socket;
    
    ERL_NIF_TERM term = enif_make_resource(env, nif_socket.get());
    return enif_make_tuple2(env, ATOMS.atomOk, term);
}

void enif_ssl_socket_free(ErlNifEnv* env, void* obj)
{
    UNUSED(env);
    
    enif_ssl_socket *data = static_cast<enif_ssl_socket*>(obj);
    
    if(data->socket != NULL)
        delete data->socket;
}

ERL_NIF_TERM enif_ssl_socket_handshake(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    
    enif_ssl_socket* wp = NULL;
    
    if(!enif_get_resource(env, argv[0], data->res_ssl_sock, (void**) &wp))
        return make_badarg(env);
        
    return wp->socket->Handshake(env);
}

ERL_NIF_TERM enif_ssl_socket_send_pending(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    
    enif_ssl_socket* wp = NULL;
    
    if(!enif_get_resource(env, argv[0], data->res_ssl_sock, (void**) &wp))
        return make_badarg(env);
    
    return wp->socket->SendPending(env);
}

ERL_NIF_TERM enif_ssl_socket_feed_data(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    
    enif_ssl_socket* wp = NULL;
    ErlNifBinary bin;
    
    if(!enif_get_resource(env, argv[0], data->res_ssl_sock, (void**) &wp))
        return make_badarg(env);
    
    if(!get_binary(env, argv[1], &bin))
        return make_badarg(env);
    
    return wp->socket->FeedData(env, &bin);
}

ERL_NIF_TERM enif_ssl_socket_send_data(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    
    enif_ssl_socket* wp = NULL;
    ErlNifBinary bin;
    
    if(!enif_get_resource(env, argv[0], data->res_ssl_sock, (void**) &wp))
        return make_badarg(env);
    
    if(!get_binary(env, argv[1], &bin))
        return make_badarg(env);
    
    return wp->socket->SendData(env, &bin);
}

ERL_NIF_TERM enif_ssl_socket_shutdown(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UNUSED(argc);
    
    erltls_data* data = static_cast<erltls_data*>(enif_priv_data(env));
    enif_ssl_socket* wp = NULL;
    
    if(!enif_get_resource(env, argv[0], data->res_ssl_sock, (void**) &wp))
        return make_badarg(env);
    
    return wp->socket->Shutdown(env);
}
