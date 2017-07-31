# pragma once

# include <string>

namespace textMining
{

  class Tools
  {
  public:
  	/*! \fn static unsigned short distance(const std::string, const std::string);
     *  \brief Takes two words and returns their Damerau-Levenshtein distance.
     *  \param s1 the first word.
     *  \param s2 the second word.
     *  \return unsigned short that represents the Damerau-Levenshtein distance of the two words.
     */
      static unsigned short distance(const std::string, const std::string);
  };
}
