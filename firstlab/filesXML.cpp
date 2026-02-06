#include "workFile.h"

namespace files {
// PRIVATE
// PUBLIC
bool filesXML::createXML() {
  if (fsystem::exists(filePath_)) return false;

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertFirstChild(root);
  if (doc.SaveFile(filePath_.string().c_str()) != tinyxml2::XML_SUCCESS)
    return false;
  return true;
}

bool filesXML::addDataXML(const string& tagName, const string& value) {
  if (!fsystem::exists(filePath_)) return false;
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(filePath_.string().c_str()) != tinyxml2::XML_SUCCESS)
    return false;

  tinyxml2::XMLElement* root = doc.FirstChildElement("root");
  if (!root) {
    root = doc.NewElement("root");
    doc.InsertFirstChild(root);
  }

  tinyxml2::XMLElement* elem = doc.NewElement(tagName.c_str());
  elem->SetText(value.c_str());
  root->InsertEndChild(elem);

  return doc.SaveFile(filePath_.string().c_str()) == tinyxml2::XML_SUCCESS;
}

bool filesXML::addDataXML(const std::map<string, string>& data) {
  for (auto& [tag, value] : data)
    if (!addDataXML(tag, value)) return false;
  return true;
}

bool filesXML::readXML(std::map<std::string, std::string>& data) {
  if (!fsystem::exists(filePath_)) return false;

  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(filePath_.string().c_str()) != tinyxml2::XML_SUCCESS)
    return false;

  tinyxml2::XMLElement* root = doc.RootElement();
  if (!root) return false;

  data.clear();
  for (tinyxml2::XMLElement* elem = root->FirstChildElement(); elem != nullptr;
       elem = elem->NextSiblingElement()) {
    const char* name = elem->Name();
    const char* text = elem->GetText();
    if (name && text) {
      data[name] = text;
    }
  }

  return !data.empty();
}

bool filesXML::deleteXML() {
  if (fsystem::exists(filePath_)) return fsystem::remove(filePath_);
  return false;
}
}  // namespace files