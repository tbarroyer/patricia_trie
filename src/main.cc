# include <iostream>
# include <cstdlib>

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

# include "patricia_trie.hh"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " /path/to/word/freq.txt /path/to/output/dict.bin" << std::endl;
    std::abort();
  }

  textMining::PatriciaTrie trie(argv[1]);
  std::cout << "inserted" << std::endl;
  //trie.reduce();
  //trie.save(argv[2]);
  std::cout << "saved" << std::endl;
  while (1)
  {
    std::string test;
    std::cin >> test;
    std::cout << trie.search(test) << std::endl;
  }

  /*textMining::PatriciaTrie trie2;
  {
    std::ifstream ifs("bina");
    boost::archive::text_iarchive ia(ifs);
    ia >> trie2;
  }*/
  return 0;
}
