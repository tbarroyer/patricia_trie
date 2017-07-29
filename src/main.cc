# include <iostream>
# include <cstdlib>

# include "patricia_trie.hh"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " /path/to/word/freq.txt /path/to/output/dict.bin" << std::endl;
    std::abort();
  }

  textMining::PatriciaTrie trie(argv[1]);
  trie.reduce();

  return 0;
}
