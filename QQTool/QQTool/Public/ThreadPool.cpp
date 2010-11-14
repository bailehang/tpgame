#include "stdafx.h"
#include "ThreadPool.h"

volatile long  tp_ipc_peer_namespace::ctpool::pool_Count = 0;
volatile bool  tp_ipc_peer_namespace::ctpool::Ras_running_ = 0;