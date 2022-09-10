#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;

class CSVParser {
private:
	std::string m_filename;
	std::ifstream m_file;

public:
	CSVParser(const std::string & file_name)
			: m_filename(file_name), m_file(std::ifstream(file_name)) {}

	CSVParser(std::string file_name)
			: m_filename(file_name), m_file(std::ifstream(file_name)) {}

	CSVParser(const char * file_name)
			: m_filename(file_name), m_file(std::ifstream(file_name)) {}

	~CSVParser() { m_file.close(); }

	template<typename... Args>
	std::vector<std::tuple<Args...>> parse_file() {
		std::vector<std::tuple<Args...>> data;
		std::string line;
		while (m_file >> line) {
			std::stringstream ss(line);
			data.push_back(parse_line<Args...>(ss));
		}
		return data;
	}

private:
	template<typename... Args>
	std::tuple<Args...> parse_line(std::stringstream & line_stream) {
		return std::tuple<Args...>{read_argument<Args>(line_stream)...};
	}

	template<typename T, std::enable_if_t<!std::is_convertible_v<T, std::string>,
																				bool> = true>
	T read_argument(std::stringstream & line_stream) {
		T arg;
		line_stream >> arg;
		return arg;
	}

	template<typename T,
					 std::enable_if_t<std::is_convertible_v<T, std::string>, bool> = true>
	T read_argument(std::stringstream & line_stream) {
		T string;
		getline(line_stream, string, ';');
		return string;
	}
}; // CSVParser

// Variant: 2 (С.Петербург -> Житомир)
int main() {
	CSVParser parser("test/in.1");
	auto data = parser.parse_file<std::string, std::string, int>();
	for (auto line : data) {
		auto [from, to, dist] = line;
		cout << from << to << dist << endl;
	}
}
