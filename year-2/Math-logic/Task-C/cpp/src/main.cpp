#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include <chrono>

#include "expression/expression.h"
#include "expression/implication.h"
#include "compute/axiom.h"
#include "compute/proof.h"
#include "compute/hipothesis.h"
#include "util/trim.h"

#include "./parser/expression.tab.h"
#include "./parser/expression.lexer.h"

using namespace std;


struct theorem_output;


static struct theorem_output parse_theorem_string(string str);

static vector<string> split_hipothesis(string str);

static void theorem_output_write(const struct theorem_output &s, ostream &out);

static void do_everything(theorem_output &output, list<proof *> &result_proof, istream &ifile, ostream &ofile);


extern expression *parse_result;

struct theorem_output {
    vector<string> hipothesis;
    string output;
};


int yywrap() {
    return 1;
}

void yyerror(const char *error) {
    cerr << error;
}


int main(int argc, char *argv[]) {

//    ifstream ifile("input.txt");
//    ofstream ofile("output.txt");
//
//    if (argc == 3) {
//        ifile.close();
//        ofile.close();
//        ifile.open(argv[1]);
//        ofile.open(argv[2]);
//    }
//
//    if (!ifile.is_open()) return -1;
//
    theorem_output output;
    list<proof *> result_proof;

    do_everything(output, result_proof, cin, cout);
//    do_everything(output, result_proof, ifile, ofile);

    // closing files
//    ifile.close();
//    ofile.close();

    return 0;
}

/*
static void theorem_output_write(const struct theorem_output& s, ostream &out) {
    out << "Output: " << s.output << endl;
    size_t cnt = 0;
    for (auto &e : s.hipothesis) {
        out << static_cast<unsigned long>(++cnt) << " " << e << endl;
    }
}
*/

static void do_everything(theorem_output &output, list<proof *> &result_proof, istream &ifile, ostream &ofile) {
    // hipo -> proof
    list<implication *> proof_needed;
    list<expression *> hipothesis;

    struct cmpexpr {
        bool operator()(const expression *s1, const expression *s2) const {
            if (s1->hashcode() < s2->hashcode()) return false;
            else if (s1->hashcode() > s2->hashcode()) return true;
            else return strcmp(s1->to_string().c_str(), s2->to_string().c_str()) < 0;
        };
    };
    // (A -> B -> C), C - key, (A -> B -> C) - value
    // allows only those elements that starts with (A -> ...). complexity(log)
    multimap<expression *, expression *, cmpexpr> map_mp_first;
    typedef multimap<expression *, expression *, cmpexpr>::iterator map_first_iterator;
    // (A -> B), B - key. if key is not present than there is no element. complexity(log)
    map<expression *, expression *, cmpexpr> map_mp_second;

    // initial string (from first line)
    string theorem;
    getline(ifile, theorem);
    output = parse_theorem_string(theorem);

    // parsing hipothesis
    for (const auto &h: output.hipothesis) {
        yy_scan_string(h.c_str());
        yyparse();
        hipothesis.push_back(parse_result);
    }

    // reading and parsing proofs
    {
        string line;
        while (getline(ifile, line)) {
            yy_scan_string(line.c_str());
            yyparse();
            // last-hipo -> proof
            proof_needed.push_back(new implication(hipothesis.back(), parse_result));
        }
    }

    // algorithm calculate
//    auto start = std::chrono::high_resolution_clock::now();

    // line 1 is theorem, so from line 2 output proofs begin
#define print_line result_proof.size() + 2
#define need_newline false

    for (const auto &needed: proof_needed) {
        // case 1
        if (needed->get_left()->equals(needed->get_right())) {
            auto *axiom_first = new implication(needed->get_left(), needed);
            auto *axiom_first2 = new implication(needed->get_left(), new implication(needed, needed->get_left()));
            auto *axiom_two = new implication(axiom_first, new implication(axiom_first2, needed));
            // MP axiom_first, axiom_two
            auto *mp = axiom_two->get_right();
            // MP axiom_first2, mp
            auto *mp2 = ((implication *) mp)->get_right();

            // adding to output
            result_proof.push_back(new proof(axiom_first, print_line));
            result_proof.push_back(new proof(axiom_first2, print_line));
            result_proof.push_back(new proof(axiom_two, print_line));
            result_proof.push_back(new proof(mp, print_line));
            result_proof.push_back(new proof(mp2, print_line, need_newline));

            // pushing in two maps
            auto *mp2_right = ((implication *) (mp2))->get_right();
            map_mp_second.insert({mp2_right, mp2});
            if (mp2_right->get_type() == expr_type::IMPLICATION) {
                map_mp_first.insert({((implication *) mp2_right)->get_right(), mp2});
            }
        }
            // case 2 (removed if is_in_proof because it increases complexity
        else if (find_axiom_number(needed->get_right()) || is_in_hipothesis_vector(&hipothesis, needed->get_right())) {
            auto *axiom = needed->get_right();
            auto *axiom_first = new implication(needed->get_right(), needed);
            auto *mp = needed;

            result_proof.push_back(new proof(axiom, print_line));
            result_proof.push_back(new proof(axiom_first, print_line));
            result_proof.push_back(new proof(mp, print_line, need_newline));

            // pushing in two maps
            auto *mp_right = ((implication *) (mp))->get_right();
            map_mp_second.insert({mp_right, mp});
            if (mp_right->get_type() == expr_type::IMPLICATION) {
                map_mp_first.insert({((implication *) mp_right)->get_right(), mp});
            }
        }
            // case else - find in loop the expression that can be proved using second axiom
        else {
            pair<map_first_iterator, map_first_iterator> rez_first = map_mp_first.equal_range(needed->get_right());
            for (auto it = rez_first.first; it != rez_first.second; ++it) {
                auto *first = ((implication *) it->second);

                auto itr = map_mp_second.find(((implication *) first->get_right())->get_left());
                if (itr != map_mp_second.end()) {
                    // resolving
                    auto axiom_2 = new implication(itr->second, new implication(first, needed));
                    auto *mp1 = axiom_2->get_right();
                    auto *mp2 = needed;

                    // writing
                    result_proof.push_back(new proof(axiom_2, print_line));
                    result_proof.push_back(new proof(mp1, print_line));
                    result_proof.push_back(new proof(mp2, print_line, need_newline));

                    // pushing in two maps
                    auto *mp2_right = ((implication *) (mp2))->get_right();
                    map_mp_second.insert({mp2_right, mp2});
                    if (mp2_right->get_type() == expr_type::IMPLICATION) {
                        map_mp_first.insert({((implication *) mp2_right)->get_right(), mp2});
                    }
                    break;
                }
            }
        }
    }
#undef print_line

    // finish timer
//    auto finish = std::chrono::high_resolution_clock::now();
//    std::cout << "Execution time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()
//              << "ns\n";

    // writing first line
    for (size_t i = 0; i < (output.hipothesis.size() - 2) && (output.hipothesis.size() >= 2); ++i) {
        ofile << output.hipothesis[i] << ", ";
    }
    ofile << (output.hipothesis.size() < 2 ? "" : output.hipothesis[output.hipothesis.size() - 2]);
    ofile << "|-" << output.output << (need_newline ? "\n" : "") << endl;
#undef need_newline
    // writing proofs
    for (const auto &sol: result_proof) {
        ofile << sol->get_expr()->to_string() << endl;
        ofile << (sol->is_last_in_proof() ? "\n" : "");
    }
}

static struct theorem_output parse_theorem_string(string str) {
    // trimming the string
    trim(str);

    size_t index = str.find("|-");
    string right = str.substr(index + 2, str.length() - index - 2);
    string left = str.substr(0, index);
    size_t comma_index = left.find_last_of(',');

    struct theorem_output output = {};

    if (comma_index < 0) {
        yy_scan_string(left.c_str());
        yyparse();
        output.output = parse_result->to_string() + "->" + trim(right);
    } else {
        string l_s = left.substr(comma_index + 1, left.length() - comma_index - 1);
        yy_scan_string(l_s.c_str());
        yyparse();
        output.output = parse_result->to_string() + "->" + trim(right);
    }

    output.hipothesis = split_hipothesis(left);

    return output;
}

static vector<string> split_hipothesis(string str) {
    string delimeter = ",";
    // trim string
    trim(str);

    vector<string> tokens;
    size_t pos = 0;
    // adding up to the last comma
    while ((pos = str.find(delimeter)) != string::npos) {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimeter.length());
    }
    // adding the last one
    if (str.length() != 0) {
        tokens.push_back(str);
    }

    return tokens;
}
