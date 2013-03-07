#include "contenthandler.h"
#include "settings.h"
#include "dictionarystandardhandler.h"
#include "utils.h"

void
ContentHandler::run(QXmlStreamReader& xml)
{
  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (xml.isStartElement()) {
      if (xml.name() == "FunctionMode") {
      }
      else if (xml.name() == "StepRef") {
        QString stepName = getAttributeOrCharacters(xml, "name");
        m_steps.append(stepName);
      }
      else if (xml.name() == "LayerRef") {
        QString layerName = getAttributeOrCharacters(xml, "name");
        m_layers.append(layerName);
      }
      else if (xml.name() == "BomRef") {
        QString bomName = getAttributeOrCharacters(xml, "name");
        m_boms.append(bomName);
      }
      else if (xml.name() == "AvlRef") {
      }
      else if (xml.name() == "DictionaryStandard") {
        Handler *handler = new DictionaryStandardHandler();
        handler->run(xml);
        // TODO get a list of standard primitives??
        delete handler;
      }
      else if (xml.name() == "DictionaryUser") {
      }
      else if (xml.name() == "DictionaryFont") {
      }
      else if (xml.name() == "DictionaryLineDesc") {
      }
      else if (xml.name() == "DictionaryColor") {
      }
      else if (xml.name() == "DictionaryFirmware") {
      }
    }
    else if (isEndElementWithName(xml, "Content")) {
      updateOdb();
      return;
    }
  }
}

void
ContentHandler::updateOdb()
{
  if (m_steps.empty() || m_layers.empty()) {
    qDebug("ERROR** steps or layers cannot be empty");
    exit(1);
  }
  if (namesStartsWithDot(m_steps) || namesStartsWithDot(m_layers)) {
    qDebug("ERROR** name cannot start with \'.\'");
    exit(1);
  }
  for (int i = 0; i < m_steps.size(); ++i) {
    for (int j = 0; j < m_layers.size(); ++j) {
      createOdbDir(QString("steps/") + m_steps.at(i) + "/" + m_layers.at(j));
    }
  }
  // TODO BOM??
}

bool
ContentHandler::namesStartsWithDot(const QList<QString>& nameList)
{
  for (int i = 0; i < nameList.size(); ++i) {
    if (nameList.at(i).startsWith(".")) {
      return true;
    }
  }
  return false;
}