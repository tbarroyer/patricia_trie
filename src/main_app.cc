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
        int s = trie.search(word);
        if (s != -1)
          std::cout << "[{\"word\":\"" << word <<
            "\",\"freq\":" << s << ",\"distance\":0}]" << std::endl;
      }
      else
      {
        auto out = trie.approx(word, 1);
        for (int i = 2; i <= dist; ++i) {
          auto aux = trie.approx(word, i);
          out.insert(out.end(), aux.begin(), aux.end());
        }
        std::string aff = "[";
        int s = trie.search(word);
        if (s != -1) {
          aff += "{\"word\":\"";
          aff += word;
          aff += "\",\"freq\":";
          aff += std::to_string(s);
          aff += ",\"distance\":0}";
          if (out.size() > 0)
            aff += ",";
        }
        for (auto it = out.begin(); it != out.end(); ++it)
        {
          aff += "{\"word\":\"";
          aff += std::get<0>(*it);
          aff += "\",\"freq\":";
          aff += std::to_string(std::get<2>(*it));
          aff += ",\"distance\":";
          aff += std::to_string(std::get<1>(*it));
          aff += "}";
          if (std::next(it) != out.end())
            aff += ",";
        }
        aff += "]";
        std::cout << aff << std::endl;
      }
    }
  }
  return 0;
}
