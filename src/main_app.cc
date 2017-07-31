# include <iostream>
# include <cstdlib>
# include <boost/archive/binary_iarchive.hpp>

# include "patricia_trie.hh"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " /path/to/compiled/dict.bin" << std::endl;
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
    std::string app;
    int dist;
    std::string word;
    std::cin >> app >> dist >> word;

    if (app == "approx")
    {
      if (dist == 0)
      {
        std::cout << "[{\"word\":\"" << word <<
          "\",\"freq\":" << trie.search(word) << ",\"distance\":0}]" << std::endl;
      }
    }
  }
  return 0;
}
