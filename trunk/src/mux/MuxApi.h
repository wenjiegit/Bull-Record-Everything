#ifndef MUXAPI_H
#define MUXAPI_H

#include "libmux.h"

#include <QString>

class MuxApi
{
public:
    typedef MuxHandle*  (*create_mux)();
    typedef int         (*start_mux)(MuxHandle*);
    typedef int         (*stop_mux)(MuxHandle*);
    typedef void        (*destroy_mux)(MuxHandle *);

public:
    static MuxApi *instance();
    bool init(const QString &dllPath);

public:
    create_mux  ptr_create_mux;
    start_mux   ptr_start_mux;
    stop_mux    ptr_stop_mux;
    destroy_mux ptr_destroy_mux;
};

#endif // MUXAPI_H
