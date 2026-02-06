#include <ncurses.h>

#include "../firstlab/workFile.h"
#include "hash.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, FALSE);
  srand(static_cast<unsigned>(time(nullptr)));
}

void draw_main_menu() {
  clear();
  int row = 2;
  mvprintw(row++, 2, "Choose option");
  row++;
  mvprintw(row++, 4, "1. Enter hash from console");
  mvprintw(row++, 4, "2. Enter hash from file TXT");
  row++;
  mvprintw(row + 1, 2, "[Q]uit");
  refresh();
}

static std::string trim(const std::string &s) {
  size_t a = 0, b = s.size();
  while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
  while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
  return s.substr(a, b - a);
}

bool is_valid_hash(const std::string &s) {
  if (s.empty()) return false;
  size_t len = s.size();
  if (len != 32 && len != 64) return false;
  for (char c : s)
    if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
  return true;
}

std::vector<std::string> enter_hash_console_checked() {
  while (true) {
    clear();
    mvprintw(1, 2, "Enter hash (press Enter on empty line to finish):");
    int line = 3;
    const int maxlen = 1024;
    char buf[maxlen];
    echo();
    curs_set(1);

    std::vector<std::string> hashes;
    while (true) {
      mvprintw(line, 4, "> ");
      clrtoeol();
      refresh();
      wgetnstr(stdscr, buf, maxlen - 1);
      std::string s = trim(buf);
      if (s.empty()) break;
      hashes.push_back(s);
      line++;
    }

    noecho();
    curs_set(0);

    bool all_valid = !hashes.empty();
    for (auto &h : hashes)
      if (!is_valid_hash(h)) all_valid = false;

    if (all_valid) return hashes;

    clear();
    mvprintw(2, 2, "Error: invalid hash entered!");
    mvprintw(4, 2, "[B]ack    [Q]uit");
    refresh();
    int ch;
    while (true) {
      ch = getch();
      if (ch == 'B' || ch == 'b') return {};
      if (ch == 'Q' || ch == 'q') {
        endwin();
        exit(0);
      }
    }
  }
}

size_t ask_thread_count_checked() {
  while (true) {
    clear();
    mvprintw(2, 2, "Enter number of threads (0 or 1 = single-threaded):");
    mvprintw(4, 2, "> ");
    echo();
    curs_set(1);
    const int maxlen = 32;
    char buf[maxlen];
    wgetnstr(stdscr, buf, maxlen - 1);
    noecho();
    curs_set(0);

    std::string s = trim(buf);

    bool valid = !s.empty();
    for (char c : s)
      if (!std::isdigit(static_cast<unsigned char>(c))) valid = false;

    if (!valid) {
      clear();
      mvprintw(2, 2, "Error: invalid thread count!");
      mvprintw(4, 2, "[T]ry again    [B]ack    [Q]uit");
      refresh();
      int ch;
      while (true) {
        ch = getch();
        if (ch == 'T' || ch == 't') break;
        if (ch == 'B' || ch == 'b') return 0;
        if (ch == 'Q' || ch == 'q') {
          endwin();
          exit(0);
        }
      }
      continue;
    }

    return std::stoul(s);
  }
}

std::vector<std::string> enter_file_and_read_hashes() {
  while (true) {
    clear();
    mvprintw(2, 2, "Enter name file TXT format:");
    mvprintw(4, 2, "> ");
    echo();
    curs_set(1);

    char buf[1024];
    wgetnstr(stdscr, buf, 1023);
    noecho();
    curs_set(0);

    std::string filename = trim(buf);
    if (filename.empty()) {
      clear();
      mvprintw(2, 2, "Error: filename is empty!");
      mvprintw(4, 2, "[B]ack    [Q]uit");
      refresh();
      int ch;
      while (true) {
        ch = getch();
        if (ch == 'B' || ch == 'b') return {};
        if (ch == 'Q' || ch == 'q') {
          endwin();
          exit(0);
        }
      }
    }

    files::filesTXT f(filename);
    if (!f.isValid()) {
      clear();
      mvprintw(2, 2, "Error: invalid file!");
      mvprintw(4, 2, "[B]ack    [Q]uit");
      refresh();
      int ch;
      while (true) {
        ch = getch();
        if (ch == 'B' || ch == 'b') return {};
        if (ch == 'Q' || ch == 'q') {
          endwin();
          exit(0);
        }
      }
    }

    std::vector<std::string> lines;
    if (!f.readFile(lines) || lines.empty()) {
      clear();
      mvprintw(2, 2, "Error: invalid file or read failed!");
      mvprintw(4, 2, "[B]ack    [Q]uit");
      refresh();
      int ch;
      while (true) {
        ch = getch();
        if (ch == 'B' || ch == 'b') return {};
        if (ch == 'Q' || ch == 'q') {
          endwin();
          exit(0);
        }
      }
    }

    std::vector<std::string> hashes;
    for (auto &line : lines) {
      std::istringstream iss(line);
      std::string token;
      while (iss >> token) {
        std::string t = trim(token);
        if (!t.empty()) hashes.push_back(t);
      }
    }

    bool all_valid = !hashes.empty();
    for (auto &h : hashes)
      if (!is_valid_hash(h)) all_valid = false;

    if (!all_valid) {
      clear();
      mvprintw(2, 2, "Error: file contains invalid hashes!");
      mvprintw(4, 2, "[B]ack    [Q]uit");
      refresh();
      int ch;
      while (true) {
        ch = getch();
        if (ch == 'B' || ch == 'b') return {};
        if (ch == 'Q' || ch == 'q') {
          endwin();
          exit(0);
        }
      }
    }

    return hashes;
  }
}

int main() {
  initNcurses();

  while (true) {
    draw_main_menu();
    int ch = getch();

    if (ch == '1') {
      auto hashes = enter_hash_console_checked();
      if (hashes.empty()) continue;

      size_t threads = ask_thread_count_checked();
      if (threads == 0) continue;

      clear();
      mvprintw(1, 2, "Succes enter hash");
      mvprintw(2, 2, "------------------");
      refresh();

      AlgHash::Hash cracker(hashes, threads);
      cracker.AlgorithmHash();

      int row = 4;
      int idx = 1;
      for (auto &kv : cracker.time) {
        unsigned int key = kv.first;
        double seconds = kv.second;
        std::string pw = "(unknown)";
        auto it = cracker.password.find(key);
        if (it != cracker.password.end()) pw = it->second;
        mvprintw(row++, 4, "%d. %s | %.3f", idx++, pw.c_str(), seconds);
      }

      mvprintw(row + 2, 2, "[B]ack    [Q]uit");
      refresh();

      int c;
      while (true) {
        c = getch();
        if (c == 'B' || c == 'b') break;
        if (c == 'Q' || c == 'q') {
          endwin();
          exit(0);
        }
      }

    } else if (ch == '2') {
      auto hashes = enter_file_and_read_hashes();
      if (hashes.empty()) continue;

      size_t threads = ask_thread_count_checked();
      if (threads == 0) continue;

      clear();
      mvprintw(1, 2, "Succes enter hash");
      mvprintw(2, 2, "------------------");
      refresh();

      AlgHash::Hash cracker(hashes, threads);
      cracker.AlgorithmHash();

      int row = 4;
      int idx = 1;
      for (auto &kv : cracker.time) {
        unsigned int key = kv.first;
        double seconds = kv.second;
        std::string pw = "(unknown)";
        auto it = cracker.password.find(key);
        if (it != cracker.password.end()) pw = it->second;
        mvprintw(row++, 4, "%d. %s | %.3f", idx++, pw.c_str(), seconds);
      }

      mvprintw(row + 2, 2, "[B]ack    [Q]uit");
      refresh();

      int c;
      while (true) {
        c = getch();
        if (c == 'B' || c == 'b') break;
        if (c == 'Q' || c == 'q') {
          endwin();
          exit(0);
        }
      }

    } else if (ch == 'q' || ch == 'Q') {
      break;
    }
  }

  endwin();
  return 0;
}
