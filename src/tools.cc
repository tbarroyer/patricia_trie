#include "tools.hh"

namespace textMining
{
  unsigned short Tools::distance(const std::string s1, const std::string s2)
  {
    size_t n1 = s1.length();
    size_t n2 = s2.length();

    size_t d[n1 + 1][n2 + 1];
    for (size_t i = 0; i <= n1; ++i)
      d[i][0] = i;
    for (size_t i = 0; i <= n2; ++i)
      d[0][i] = i;
    int cost;
    for (size_t i = 1; i <= n1; i++)
    {
      for (size_t j = 1; j <= n2; j++)
      {
        cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
        d[i][j] = std::min(std::min(d[i - 1][j] + 1, d[i][j - 1] + 1), d[i - 1][j - 1] + cost);

        if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1])
        {
          d[i][j] = std::min(d[i][j], d[i - 2][j - 2] + cost);
        }
      }
    }
    return d[n1][n2];
  }
}
