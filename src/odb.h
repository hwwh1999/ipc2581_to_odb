#ifndef __ODB_H__
#define __ODB_H__

#include <QtCore>
#include "toplevelhandler.h"

class Odb
{
public:
  Odb(TopLevelHandler& h);
  void run();

private:
  // member functions
  void deleteOdbDir(const QString path); // path relative to odb root
  void createOdbDir(const QString path); // path relative to odb root
  void createDictionary(); // collect all dictionary to m_dictionary
  void createFileSystem(); // Job->xxx
  void createMatrix(); // Job->matrix->matrix
  void createStepLayerHierarchy(); // Job->step->layers->xxx
  void createLayerFeature(); // Job->step->layers->features

  // data members
  TopLevelHandler m_handler;
  QDir            m_dir;
  QString         m_odbRootPath;
  QList<QString>  m_allSteps;
  QList<QString>  m_allLayers;
  Dictionary      m_dictionary;
};

#endif
