/*
 * Copyright (C) codedump
 */

#ifndef __QNODE_CORE_LOGTHREAD_H__
#define __QNODE_CORE_LOGTHREAD_H__

#include "base/singleton.h"
#include "core/io_thread.h"
#include "core/message.h"

class Message;

class LogThread : public IOThread {
public:                    
  LogThread();
  virtual ~LogThread();

  virtual void Process(Message*);

  virtual void Timeout();
private:

private:
  string path_;
};

#define gLogThread Singleton<LogThread>::Instance()
#define InitLogThread Singleton<LogThread>::Instance

#endif  // __QNODE_CORE_LOGTHREAD_H__
