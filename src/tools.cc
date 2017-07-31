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
    for (size_t i = 0; i <= n1; i++)
    {
      for (size_t j = 0; j <= n2; j++)
      {
        cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1 ;
        if ( (i > 1) && (j > 1) && (s1[i] == s2[j - 1]) && (s1[i - 1] == s2[j]))
        {
          size_t a = std::min(d[i - 1][j], d[i][j - 1] + 1);
          size_t b = std::min(d[i][j] + cost, d[i - 2][j - 2]);
          d[i][j] = std::min(a, b);
        }
        else
        {
          d[i][j] = std::min(std::min(d[i][j -1] + 1, d[i - 1][j] + 1), d[i - 1][j - 1] + cost);
        }
      }
    }
    return d[n1][n2];
  }
}
