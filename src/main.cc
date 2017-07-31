# include <iostream>
# include <cstdlib>

# include "patricia_trie.hh"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " /path/to/word/freq.txt /path/to/output/dict.bin" << std::endl;
    std::abort();
  }

  textMining::PatriciaTrie trie(argv[1]);
  trie.save(argv[2]);
  /*std::cout << "saved" << std::endl;
  while (1)
  {
    std::string test;
    std::cin >> test;
    std::cout << trie.search(test) << std::endl;
  }*/
  return 0;
}
