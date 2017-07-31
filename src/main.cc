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

  {
    std::fstream f("tmp", std::fstream::in | std::fstream::out | std::fstream::trunc);
    f.close();
  }

  textMining::PatriciaTrie trie(argv[1]);
  trie.save(argv[2]);
  return 0;
}
