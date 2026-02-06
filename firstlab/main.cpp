#include <ncurses.h>

#include <cstdlib>
#include <ctime>

#include "workFile.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, FALSE);
  curs_set(0);
  srand(time(NULL));
  mousemask(0, NULL);
}

void drawMainMenu() {
  clear();
  mvprintw(1, 2, "Choose option:");
  mvprintw(3, 4, "1. Info disk");
  mvprintw(4, 4, "2. Work TXT file");
  mvprintw(5, 4, "3. Work JSON file");
  mvprintw(6, 4, "4. Work XML file");
  mvprintw(7, 4, "5. Work ZIP file");
  mvprintw(9, 4, "Q - quit program");
  refresh();
}

void workDisk() {
  endwin();

  std::cout << "=== Disk Info ===" << std::endl;
  files::infoDisk::outInfoDisk();
  std::cout << "\n[B]ack or [Q]uit: " << std::flush;

  char ch;
  std::cin >> ch;

  if (ch == 'q' || ch == 'Q') {
    exit(0);
  }

  refresh();
}

void drawDiskMenu() {
  clear();
  mvprintw(1, 2, "Disk Menu:");
  mvprintw(3, 4, "1. Out info disk");
  mvprintw(5, 4, "B - back");
  mvprintw(6, 4, "Q - quit");
  refresh();
}

void createTXTFile() {
  clear();
  mvprintw(1, 2, "Enter directory/filename.txt:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);

  clear();
  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesTXT txt(path);

    if (txt.isValid()) {
      if (txt.createFile()) {
        mvprintw(1, 2, "File created successfully: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Failed to create file: %s", path.c_str());
      }
    } else {
      mvprintw(1, 2, "Unknow paht: %s", path.c_str());
    }
  }

  mvprintw(3, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      break;
    }
  }
}

void writeTXTFile() {
  clear();
  mvprintw(1, 2, "Enter your filename to write text:");
  echo();
  char buffer[256];
  mvgetnstr(3, 4, buffer, 255);
  noecho();
  std::string path(buffer);

  clear();
  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') break;
    }
    return;
  }

  files::filesTXT txt(path);

  if (!txt.isValid()) {
    mvprintw(1, 2, "File not found or invalid path: %s", path.c_str());
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') break;
    }
    return;
  }

  clear();
  mvprintw(1, 2, "Enter text to write:");
  echo();
  char textBuffer[1024];
  mvgetnstr(3, 4, textBuffer, 1023);
  noecho();
  std::string text(textBuffer);

  if (txt.writeLine(text)) {
    mvprintw(5, 2, "Text written successfully.");
  } else {
    mvprintw(5, 2, "Failed to write text.");
  }

  mvprintw(7, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') break;
  }
}

void readTXTFile() {
  clear();
  mvprintw(1, 2, "Enter filename to read:");
  echo();
  char buffer[256];
  mvgetnstr(3, 4, buffer, 255);
  noecho();
  std::string path(buffer);

  clear();
  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') break;
    }
    return;
  }

  std::vector<std::string> content;
  files::filesTXT txt(path);

  if (txt.isValid() && txt.readFile(content)) {
    clear();
    mvprintw(1, 2, "=== File contents ===");
    int y = 3;
    for (const auto& line : content) {
      if (y >= LINES - 2) break;
      mvprintw(y++, 2, "%s", line.c_str());
    }
  } else {
    mvprintw(1, 2, "File not found or empty");
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') break;
  }
}

void deleteTXTFile() {
  clear();
  mvprintw(1, 2, "Enter filename to delete:");
  echo();
  char buffer[256];
  mvgetnstr(3, 4, buffer, 255);
  noecho();
  std::string path(buffer);

  clear();
  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') break;
    }
    return;
  }

  files::filesTXT txt(path);

  if (!txt.isValid() || !txt.deleteFile()) {
    mvprintw(1, 2, "File not found or cannot delete: %s", path.c_str());
  } else {
    mvprintw(1, 2, "File deleted successfully: %s", path.c_str());
  }

  mvprintw(3, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') break;
  }
}

void drawTXTMenu() {
  clear();
  mvprintw(1, 2, "TXT Menu:");
  mvprintw(3, 4, "1. Create TXT file");
  mvprintw(4, 4, "2. Write text to TXT file");
  mvprintw(5, 4, "3. Read TXT file");
  mvprintw(6, 4, "4. Delete TXT file");
  mvprintw(8, 4, "B - back");
  mvprintw(9, 4, "Q - quit");
  refresh();
}

void createJSONFile() {
  clear();
  mvprintw(1, 2, "Enter directory/filename.json:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesJSON jsonFile(path);

    if (jsonFile.isValid()) {
      if (jsonFile.createJson()) {
        mvprintw(1, 2, "File created successfully: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Failed to create file: %s", path.c_str());
      }
    } else {
      mvprintw(1, 2, "Invalid path: %s", path.c_str());
    }
  }

  mvprintw(3, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      break;
    }
  }
}

void writeJSONFile() {
  clear();
  mvprintw(1, 2, "Enter JSON filename:");
  refresh();

  char fileBuffer[256];
  echo();
  mvgetnstr(3, 4, fileBuffer, 255);
  noecho();
  std::string filename(fileBuffer);

  files::filesJSON json(filename);
  if (!json.isValid()) {
    mvprintw(5, 2, "Invalid file path or extension!");
    mvprintw(7, 2, "[B]ack or [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') return;
    }
  }

  std::map<std::string, std::string> data;

  clear();
  mvprintw(1, 2, "Enter key:");
  refresh();
  char keyBuffer[128];
  echo();
  mvgetnstr(3, 4, keyBuffer, 127);
  noecho();
  std::string key(keyBuffer);

  clear();
  mvprintw(1, 2, "Enter value:");
  refresh();
  char valueBuffer[128];
  echo();
  mvgetnstr(3, 4, valueBuffer, 127);
  noecho();
  std::string value(valueBuffer);

  data[key] = value;

  if (!json.writeJson(data)) {
    mvprintw(5, 2, "Failed to write to file!");
  } else {
    mvprintw(5, 2, "Data written successfully.");
  }

  mvprintw(7, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void readJSONFile() {
  clear();
  mvprintw(1, 2, "Enter filename to read JSON:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesJSON jsonFile(path);
    std::map<std::string, std::string> data;

    if (!jsonFile.isValid() || !jsonFile.readJson(data)) {
      mvprintw(1, 2, "Failed to read JSON: %s", path.c_str());
    } else {
      mvprintw(1, 2, "=== File contents ===");
      int line = 2;
      for (const auto& [key, value] : data) {
        mvprintw(line++, 2, "%s : %s", key.c_str(), value.c_str());
      }
    }
  }

  mvprintw(20, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void deleteJSONFile() {
  clear();
  mvprintw(1, 2, "Enter filename to delete JSON:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesJSON jsonFile(path);

    if (!jsonFile.isValid() || !jsonFile.deleteJson()) {
      mvprintw(1, 2, "Failed to delete JSON: %s", path.c_str());
    } else {
      mvprintw(1, 2, "JSON deleted: %s", path.c_str());
    }
  }

  mvprintw(7, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      break;
    }
  }
}

void drawJSONMenu() {
  clear();
  mvprintw(1, 2, "JSON Menu:");
  mvprintw(3, 4, "1. Create JSON file");
  mvprintw(4, 4, "2. Write JSON file");
  mvprintw(5, 4, "3. Read JSON file");
  mvprintw(6, 4, "4. Delete JSON file");
  mvprintw(8, 4, "B - back");
  mvprintw(9, 4, "Q - quit");
  refresh();
}

void createXMLFile() {
  clear();
  mvprintw(1, 2, "Enter directory/filename.xml:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesXML xmlFile(path);

    if (xmlFile.isValid()) {
      if (xmlFile.createXML()) {
        mvprintw(1, 2, "File created successfully: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Failed to create file: %s", path.c_str());
      }
    } else {
      mvprintw(1, 2, "Invalid path: %s", path.c_str());
    }
  }

  mvprintw(3, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      break;
    }
  }
}

void addXMLData() {
  clear();
  mvprintw(1, 2, "Enter filename to add data:");
  refresh();

  char fileBuffer[256];
  echo();
  mvgetnstr(3, 4, fileBuffer, 255);
  noecho();

  std::string filePath(fileBuffer);
  clear();

  if (filePath.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesXML xmlFile(filePath);

    if (!xmlFile.isValid()) {
      mvprintw(1, 2, "Invalid path: %s", filePath.c_str());
    } else {
      mvprintw(1, 2, "Enter tag name:");
      echo();
      char tagBuffer[128];
      mvgetnstr(3, 4, tagBuffer, 127);
      noecho();

      mvprintw(5, 2, "Enter value:");
      echo();
      char valueBuffer[128];
      mvgetnstr(6, 4, valueBuffer, 127);
      noecho();

      if (xmlFile.addDataXML(tagBuffer, valueBuffer)) {
        mvprintw(8, 2, "Data added successfully!");
      } else {
        mvprintw(8, 2, "Failed to add data!");
      }
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void readXMLFile() {
  clear();
  mvprintw(1, 2, "Enter filename to read XML:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesXML xmlFile(path);
    std::map<std::string, std::string> data;

    if (!xmlFile.isValid()) {
      mvprintw(1, 2, "Invalid path: %s", path.c_str());
    } else if (!xmlFile.readXML(data)) {
      mvprintw(1, 2, "Failed to read XML: %s", path.c_str());
    } else {
      int line = 1;
      mvprintw(line++, 2, "Contents of %s:", path.c_str());
      for (const auto& [key, value] : data) {
        if (line >= LINES - 2) break;
        mvprintw(line++, 4, "%s : %s", key.c_str(), value.c_str());
      }
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void deleteXMLFile() {
  clear();
  mvprintw(1, 2, "Enter filename to delete XML:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesXML xmlFile(path);

    if (!xmlFile.isValid()) {
      mvprintw(1, 2, "Invalid path: %s", path.c_str());
    } else if (xmlFile.deleteXML()) {
      mvprintw(1, 2, "XML deleted: %s", path.c_str());
    } else {
      mvprintw(1, 2, "Failed to delete XML: %s", path.c_str());
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void drawXMLMenu() {
  clear();
  mvprintw(1, 2, "XML Menu:");
  mvprintw(3, 4, "1. Create XML file");
  mvprintw(4, 4, "2. Add data to XML file");
  mvprintw(5, 4, "3. Read XML file");
  mvprintw(6, 4, "4. Delete XML file");
  mvprintw(8, 4, "B - back");
  mvprintw(9, 4, "Q - quit");
  refresh();
}

void createZIPFile() {
  clear();
  mvprintw(1, 2, "Enter directory/filename.zip:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesZIP zipFile(path);

    if (zipFile.isValid()) {
      if (zipFile.createZIP()) {
        mvprintw(1, 2, "File created successfully: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Failed to create file: %s", path.c_str());
      }
    } else {
      mvprintw(1, 2, "Invalid path: %s", path.c_str());
    }
  }

  mvprintw(3, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      break;
    }
  }
}

void addFileToZIP() {
  clear();
  mvprintw(1, 2, "Enter ZIP filename:");
  refresh();

  char zipBuf[256];
  echo();
  mvgetnstr(3, 4, zipBuf, 255);
  noecho();
  std::string zipPath(zipBuf);

  if (zipPath.empty()) {
    mvprintw(1, 2, "No ZIP filename provided.");
    mvprintw(5, 2, "[B]ack or [Q]uit");
    refresh();
    while (true) {
      int ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') return;
    }
  }

  files::filesZIP zip(zipPath);
  if (!zip.isValid()) {
    mvprintw(1, 2, "Invalid ZIP path.");
    mvprintw(5, 2, "[B]ack or [Q]uit");
    refresh();
    while (true) {
      int ch = getch();
      if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
      }
      if (ch == 'b' || ch == 'B') return;
    }
  }

  clear();
  mvprintw(1, 2, "Enter filename to add:");
  refresh();
  char fileBuf[256];
  echo();
  mvgetnstr(3, 4, fileBuf, 255);
  noecho();
  std::string fileToAdd(fileBuf);

  if (fileToAdd.empty()) {
    mvprintw(1, 2, "No file provided.");
  } else if (!zip.addFile(fileToAdd)) {
    mvprintw(1, 2, "Failed to add file: %s", fileToAdd.c_str());
  } else {
    mvprintw(1, 2, "File added successfully: %s", fileToAdd.c_str());
  }

  mvprintw(5, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void readZIPContents() {
  clear();
  mvprintw(1, 2, "Enter ZIP filename:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesZIP zipFile(path);
    std::vector<std::string> fileList;

    if (!zipFile.isValid() || !zipFile.readHaveInZip(fileList)) {
      mvprintw(1, 2, "Failed to read ZIP or empty archive: %s", path.c_str());
    } else {
      int line = 1;
      mvprintw(line++, 2, "Contents of %s:", path.c_str());
      for (const auto& f : fileList) {
        if (line >= LINES - 2) break;
        mvprintw(line++, 4, "%s", f.c_str());
      }
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void readFileInZip() {
  clear();
  mvprintw(1, 2, "Enter ZIP filename:");
  refresh();

  char zipBuffer[256];
  echo();
  mvgetnstr(3, 4, zipBuffer, 255);
  noecho();

  std::string zipPath(zipBuffer);
  clear();

  mvprintw(1, 2, "Enter filename in ZIP:");
  refresh();

  char fileBuffer[256];
  echo();
  mvgetnstr(3, 4, fileBuffer, 255);
  noecho();

  std::string fileInZip(fileBuffer);
  clear();

  if (zipPath.empty() || fileInZip.empty()) {
    mvprintw(1, 2, "ZIP or filename is empty!");
  } else {
    files::filesZIP zip(zipPath);
    std::vector<std::string> lines;

    if (!zip.isValid() || !zip.readFileInZIP(fileInZip, lines)) {
      mvprintw(1, 2, "Failed to read file: %s in ZIP: %s", fileInZip.c_str(),
               zipPath.c_str());
    } else {
      int lineNum = 1;
      mvprintw(lineNum++, 2, "Contents of %s in %s:", fileInZip.c_str(),
               zipPath.c_str());
      for (const auto& line : lines) {
        if (lineNum >= LINES - 2) break;
        mvprintw(lineNum++, 2, "%s", line.c_str());
      }
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void extractFileFromZIP() {
  clear();
  mvprintw(1, 2, "Enter ZIP archive filename:");
  refresh();

  char zipBuffer[256];
  echo();
  mvgetnstr(3, 4, zipBuffer, 255);
  noecho();

  std::string zipPath(zipBuffer);
  clear();

  if (zipPath.empty()) {
    mvprintw(1, 2, "No ZIP filename provided.");
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    getch();
    return;
  }

  files::filesZIP zipFile(zipPath);
  if (!zipFile.isValid()) {
    mvprintw(1, 2, "Invalid ZIP path: %s", zipPath.c_str());
    mvprintw(3, 2, "[B]ack or [Q]uit");
    refresh();
    getch();
    return;
  }

  mvprintw(1, 2, "Enter filename to extract from ZIP:");
  echo();
  char fileBuffer[256];
  mvgetnstr(3, 4, fileBuffer, 255);
  noecho();

  std::string fileInZip(fileBuffer);
  clear();

  if (fileInZip.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    if (zipFile.extractFileFromZip(fileInZip)) {
      mvprintw(1, 2, "File '%s' extracted and removed from archive!",
               fileInZip.c_str());
    } else {
      mvprintw(1, 2, "Failed to extract '%s' from ZIP.", fileInZip.c_str());
    }
  }

  mvprintw(5, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void deleteFileInZIP() {
  clear();
  mvprintw(1, 2, "Enter ZIP archive name:");
  refresh();

  char archiveBuffer[256];
  echo();
  mvgetnstr(3, 4, archiveBuffer, 255);
  noecho();

  std::string archivePath(archiveBuffer);
  clear();

  if (archivePath.empty()) {
    mvprintw(1, 2, "No archive name provided.");
  } else {
    files::filesZIP zipFile(archivePath);

    if (!zipFile.isValid()) {
      mvprintw(1, 2, "Invalid archive path: %s", archivePath.c_str());
    } else {
      mvprintw(1, 2, "Enter filename inside ZIP to delete:");
      echo();
      char fileBuffer[256];
      mvgetnstr(3, 4, fileBuffer, 255);
      noecho();

      std::string filenameInArchive(fileBuffer);

      if (filenameInArchive.empty()) {
        mvprintw(5, 2, "No filename provided.");
      } else {
        if (zipFile.deleteFileInZip(filenameInArchive)) {
          mvprintw(5, 2, "File '%s' deleted successfully from '%s'.",
                   filenameInArchive.c_str(), archivePath.c_str());
        } else {
          mvprintw(5, 2, "Failed to delete file '%s' from '%s'.",
                   filenameInArchive.c_str(), archivePath.c_str());
        }
      }
    }
  }

  mvprintw(LINES - 2, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') {
      return;
    }
  }
}

void showZIPSize() {
  clear();
  mvprintw(1, 2, "Enter ZIP filename to check size:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesZIP zipFile(path);

    if (!zipFile.isValid()) {
      mvprintw(1, 2, "Invalid ZIP path: %s", path.c_str());
    } else {
      std::uintmax_t size = zipFile.sizeZIP();
      if (size == 0) {
        mvprintw(1, 2, "ZIP file not found or empty: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Size of ZIP %s: %ju bytes", path.c_str(), size);
      }
    }
  }

  mvprintw(7, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void deleteZIPArchive() {
  clear();
  mvprintw(1, 2, "Enter ZIP filename to delete:");
  refresh();

  char buffer[256];
  echo();
  mvgetnstr(3, 4, buffer, 255);
  noecho();

  std::string path(buffer);
  clear();

  if (path.empty()) {
    mvprintw(1, 2, "No filename provided.");
  } else {
    files::filesZIP zipFile(path);

    if (!zipFile.isValid()) {
      mvprintw(1, 2, "Invalid ZIP path: %s", path.c_str());
    } else {
      if (zipFile.deleteZIP()) {
        mvprintw(1, 2, "ZIP deleted successfully: %s", path.c_str());
      } else {
        mvprintw(1, 2, "Failed to delete ZIP: %s", path.c_str());
      }
    }
  }

  mvprintw(5, 2, "[B]ack or [Q]uit");
  refresh();

  int ch;
  while (true) {
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
      endwin();
      exit(0);
    }
    if (ch == 'b' || ch == 'B') return;
  }
}

void drawZIPMenu() {
  clear();
  mvprintw(1, 2, "ZIP Menu:");
  mvprintw(3, 4, "1. Create ZIP");
  mvprintw(4, 4, "2. Add file to ZIP");
  mvprintw(5, 4, "3. Read what ZIP contains");
  mvprintw(6, 4, "4. Read content of file in ZIP");
  mvprintw(7, 4, "5. Extract file from ZIP");
  mvprintw(8, 4, "6. Delete file from ZIP");
  mvprintw(9, 4, "7. Size of ZIP file");
  mvprintw(10, 4, "8. Delete ZIP file");
  mvprintw(12, 4, "B - back");
  mvprintw(13, 4, "Q - quit");
  refresh();
}
int main() {
  initNcurses();
  drawMainMenu();

  bool running = true;
  int state = 0;

  int ch;
  while (running && (ch = getch())) {
    if (ch == 'q' || ch == 'Q') {
      running = false;
      break;
    }
    if (ch == 'b' || ch == 'B') {
      state = 0;
      drawMainMenu();
      continue;
    }

    switch (state) {
      case 0:
        switch (ch) {
          case '1':
            state = 1;
            drawDiskMenu();
            break;
          case '2':
            state = 2;
            drawTXTMenu();
            break;
          case '3':
            state = 3;
            drawJSONMenu();
            break;
          case '4':
            state = 4;
            drawXMLMenu();
            break;
          case '5':
            state = 5;
            drawZIPMenu();
            break;
        }
        break;

      case 1:
        if (ch == '1') {
          workDisk();
          drawDiskMenu();
        }
        break;

      case 2:
        switch (ch) {
          case '1':
            createTXTFile();
            drawTXTMenu();
            break;
          case '2':
            writeTXTFile();
            drawTXTMenu();
            break;
          case '3':
            readTXTFile();
            drawTXTMenu();
            break;
          case '4':
            deleteTXTFile();
            drawTXTMenu();
            break;
        }
        break;

      case 3:
        switch (ch) {
          case '1':
            createJSONFile();
            drawJSONMenu();
            break;
          case '2':
            writeJSONFile();
            drawJSONMenu();
            break;
          case '3':
            readJSONFile();
            drawJSONMenu();
            break;
          case '4':
            deleteJSONFile();
            drawJSONMenu();
            break;
        }
        break;

      case 4:
        switch (ch) {
          case '1':
            createXMLFile();
            drawXMLMenu();
            break;
          case '2':
            addXMLData();
            drawXMLMenu();
            break;
          case '3':
            readXMLFile();
            drawXMLMenu();
            break;
          case '4':
            deleteXMLFile();
            drawXMLMenu();
            break;
        }
        break;
      case 5:
        switch (ch) {
          case '1':
            createZIPFile();
            drawZIPMenu();
            break;
          case '2':
            addFileToZIP();
            drawZIPMenu();
            break;
          case '3':
            readZIPContents();
            drawZIPMenu();
            break;
          case '4':
            readFileInZip();
            drawZIPMenu();
            break;
          case '5':
            extractFileFromZIP();
            drawZIPMenu();
            break;
          case '6':
            deleteFileInZIP();
            drawZIPMenu();
            break;
          case '7':
            showZIPSize();
            drawZIPMenu();
            break;
          case '8':
            deleteZIPArchive();
            drawZIPMenu();
            break;
        }
        break;
    }
  }

  endwin();
  return 0;
}