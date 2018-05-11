#include <boost/filesystem.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "USAGE: " << argv[0] << " {sym} {rsrc}" << std::endl
              << std::endl
              << "  Creates {sym}.cpp from the contents of {rsrc}" << std::endl;
    return EXIT_FAILURE;
  }

  boost::filesystem::path dst{argv[1]};
  boost::filesystem::path src{argv[2]};

  std::string sym = src.filename().string();
  std::replace(sym.begin(), sym.end(), '.', '_');
  std::replace(sym.begin(), sym.end(), '-', '_');

  boost::filesystem::create_directories(dst.parent_path());

  std::ofstream ofs(dst.generic_string());

  ofs << "#include <cstdlib>" << std::endl << std::endl;
  ofs << "extern const unsigned char _resource_" << sym << "[] = {" << std::endl;

  std::ifstream ifs(src.generic_string(), std::ios::binary);
  ifs.unsetf(std::ios::skipws);

  constexpr size_t buffer_size = 10;
  while (!ifs.eof()) {
    // Read the data
    char buffer[buffer_size];
    ifs.read(buffer, buffer_size);
    if (ifs.gcount()) {
      // Indent the line
      ofs << "  ";
      // Print the data
      for (int i = 0; i < ifs.gcount(); ++i) {
        ofs << "0x" << std::setfill('0') << std::setw(2) << std::hex
            << (0xff & buffer[i]) << ", ";
      }
      ofs << std::endl;
    }
  }

  ofs << "};" << std::endl << std::endl;
  ofs << "extern const size_t _resource_" << sym << "_len = sizeof(_resource_" << sym
      << ");" << std::endl << std::endl;

  return EXIT_SUCCESS;
}
