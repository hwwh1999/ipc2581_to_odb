#ifndef __LOGISTICHEADERHANDLER_H__
#define __LOGISTICHEADERHANDLER_H__

#include "handler.h"

class LogisticHeaderHandler : public Handler
{
public:
  virtual void run(QXmlStreamReader& xml);
};

#endif