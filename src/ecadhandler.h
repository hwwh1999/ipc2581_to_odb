#ifndef __ECADHANDLER_H__
#define __ECADHANDLER_H__

#include "handler.h"

class EcadHandler : public Handler
{
public:
  virtual void run(QXmlStreamReader& xml);
};

#endif