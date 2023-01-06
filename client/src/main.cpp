#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace po = boost::program_options;

constexpr int DEFAULT_PORT = 6543;
const std::string DEFAULT_DBFILE = "tmp/db.bin";

void run_client(const std::string &dbfile, const int port) {
  std::string buf;
  std::string line;

  std::cout << "Enter DB command:" << std::endl;
  std::cout << "> ";
  while (getline(std::cin, line)) {
    buf.append(line);
    buf.append("\n");
    // Check if command end
    std::string line_cp = boost::trim_right_copy(line);
    size_t cp_size = line_cp.size();
    if (cp_size && line_cp[cp_size - 1] == ';') {
      // TODO: send command to server
      buf.clear();
      std::cout << "> ";
    }
  }
}

int main(int argc, char *argv[]) {

  po::options_description desc("Client options");

  desc.add_options()("help,h", "Show help")("port,p", po::value<int>(), "Server port")(
      "database,d", po::value<std::string>(), "Database file");

  po::variables_map vm;
  try {
    po::parsed_options parsed =
        po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);
    // Process command arguments
    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }
    const int port = !vm.count("port") ? DEFAULT_PORT : vm["port"].as<int>();
    const std::string dbfile =
        !vm.count("database") ? DEFAULT_DBFILE : vm["database"].as<std::string>();

    run_client(dbfile, port);

  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
  }
  return 0;
}
