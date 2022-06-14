#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

constexpr int N_VALUE = 4;
constexpr bool IS_PRINT_DITRIGRAMS = false;
constexpr bool IS_PRINT_WORDS = true;

bool get_user_variant(bool);

using ditrimap = std::unordered_map<std::string, std::size_t>;

struct cmp {
  bool operator()(const std::pair<std::string, double> &a,
                  const std::pair<std::string, double> &b) const {
    return a.second > b.second;
  }
};

bool is_letter(char l) { return (l >= 'a' && l <= 'z') || (l >= 'A' && l <= 'Z'); }
std::string get_dotted(const std::string &s) { return '.' + s + '.'; }

std::string remove_non_word_characters(const std::string &str) {
  std::string result;
  result.reserve(str.length());
  for (char i : str) {
    if (is_letter(i)) { result.push_back(i); }
  }
  result.resize(result.size());
  return result;
}

void add_digrams_from_word(ditrimap &dc, const std::string &str) {
  std::string dotted = get_dotted(str);
  for (std::size_t i = 0; i < dotted.length() - 1; ++i) {
    std::string sstr = dotted.substr(i, 2);
    if (auto it = dc.find(sstr); it != dc.end()) {
      it->second += 1;
    } else {
      dc.emplace(sstr, 1);
    }
  }
}

void add_trigrams_from_word(ditrimap &tc, const std::string &str) {
  std::string dotted = get_dotted(str);
  for (std::size_t i = 0; i < dotted.length() - 2; ++i) {
    std::string sstr = dotted.substr(i, 3);
    if (auto it = tc.find(sstr); it != tc.end()) {
      it->second += 1;
    } else {
      tc.emplace(sstr, 1);
    }
  }
}

double calculate_index_i(const ditrimap &dc, const ditrimap &tc, const std::string &str) {
  double logzero = 10.;
  std::string dotted = get_dotted(str);
  double s = 0.;
  int cnt = 0;
  double logab, logbc, logabc;
  for (std::size_t i = 0; i < dotted.length() - 2; ++i, ++cnt) {
    auto it_ab  = dc.find(dotted.substr(i, 2));
    auto it_bc  = dc.find(dotted.substr(i + 1, 2));
    auto it_abc = tc.find(dotted.substr(i, 3));
    logab = it_ab   == dc.end() ? logzero : std::log(it_ab->second);
    logbc = it_bc   == dc.end() ? logzero : std::log(it_bc->second);
    logabc = it_abc == tc.end() ? logzero : std::log(it_abc->second);
    s += std::pow(1. / 2 * (logab + logbc) - logabc, 2);
  }
  return std::pow(s / cnt, 0.5);
}

void print_ditrigrams(const ditrimap &dtc) {
  for (const auto &[sstr, count] : dtc) {
    std::cout << "  " << sstr << " : " << count << std::endl;
  }
}

void print_words(const std::set<std::pair<std::string, double>, cmp> &sp, double n) {
  auto it = sp.begin();
  while (it != sp.end() && it->second >= n) {
    std::cout << it++->first << " ";
  }
  std::cout << std::endl;
}

int main() {
  ditrimap digram_container;
  ditrimap trigram_container;
  std::set<std::pair<std::string, double>, cmp> word_weights;

  std::cout << "Enter filename: ";
  std::string str;
  std::getline(std::cin, str);
  std::ifstream input_file(str);
  if (!input_file.is_open()) {
    std::cout << "ERROR: No file found '" << str << "'." << std::endl;
    return -1;
  }

  double n = N_VALUE;
  std::cout << "Enter value N(default=" << n << "): ";
  std::getline(std::cin, str);
  // std::cin >> str;
  if (!str.empty()) {
    n = std::stod(str);
  }

  bool is_print_words = IS_PRINT_WORDS;
  std::cout << "Need to print words(default=" << (is_print_words ? "y" : "n")
            << ")[y/n]: ";
  is_print_words = get_user_variant(is_print_words);

  bool is_print_ditrigrams = IS_PRINT_DITRIGRAMS;
  std::cout << "Need to print digrams/trigrams(default="
            << (is_print_ditrigrams ? "y" : "n") << ")[y/n]: ";
  is_print_ditrigrams = get_user_variant(is_print_ditrigrams);

  while (input_file >> str) {
    str = remove_non_word_characters(str);
    add_digrams_from_word(digram_container, str);
    add_trigrams_from_word(trigram_container, str);
  }
  // remove eof, fail flags
  input_file.clear();
  input_file.seekg(0, std::ifstream::beg);
  while (input_file >> str) {
    str = remove_non_word_characters(str);
    word_weights.insert(std::make_pair(
        str, calculate_index_i(digram_container, trigram_container, str)));
  }

  if (is_print_ditrigrams) {
    std::cout << "digrams:" << std::endl;
    print_ditrigrams(digram_container);
    std::cout << "trigrams:" << std::endl;
    print_ditrigrams(trigram_container);
    std::cout << std::endl;
  }
  if (is_print_words) {
    std::cout << "words: " << std::endl;
    print_words(word_weights, n);
  }
}

bool get_user_variant(bool def) {
  std::string str;
  std::getline(std::cin, str);
  if (!str.empty()) {
    if (std::tolower(str[0]) == 'y') {
      def = true;
    } else if (std::tolower(str[0]) == 'n') {
      def = false;
    } else {
      std::cout << "Let it be default, ok?" << std::endl;
    }
  }
  return def;
}
