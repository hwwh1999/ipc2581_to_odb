#ifndef __HISTORYRECORDHANDLER_H__
#define __HISTORYRECORDHANDLER_H__

#include "handler.h"

class HistoryRecordHandler : public Handler
{
public:
  virtual void run(QXmlStreamReader& xml);
};

#endif