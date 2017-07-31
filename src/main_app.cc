# include <iostream>
# include <cstdlib>
# include <boost/archive/binary_iarchive.hpp>

# include "patricia_trie.hh"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " /path/to/output/dict.bin" << std::endl;
    std::abort();
  }

  std::ifstream ifs(argv[1], std::fstream::binary | std::fstream::in);
  textMining::PatriciaTrie trie;
  {
    boost::archive::binary_iarchive ia(ifs);
    ia >> trie;
  }
  ifs.close();

  while (1)
  {
    std::string test;
    std::cin >> test;
    std::cout << trie.search(test) << std::endl;
  }
  return 0;
}
