# include <iostream>
# include <cstdlib>

#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

# include "patricia_trie.hh"
# include "tools.hh"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " /path/to/word/freq.txt /path/to/output/dict.bin" << std::endl;
    std::abort();
  }

  textMining::PatriciaTrie trie(argv[1]);
  //trie.reduce();
  trie.save(argv[2]);
  //while (1);

  textMining::PatriciaTrie trie2;
  {
    std::ifstream ifs("bina");
    boost::archive::binary_iarchive ia(ifs);
    ia >> trie2;
  }
  return 0;
}
