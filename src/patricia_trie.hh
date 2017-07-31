# pragma once

# include <string>
# include <map>
# include <boost/serialization/split_free.hpp>
# include <boost/serialization/map.hpp>
# include <memory>
# include <fstream>
# include <tuple>
# include <vector>

# define BUFF_SIZE 1000

namespace textMining
{
  struct Node
  {
  public:
    Node(size_t start, size_t len, int freq);
    Node();

  public:
    size_t start_;
    size_t len_;
    int freq_;
    std::map<char, Node> childs_;

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & start_;
      ar & len_;
      ar & freq_;
      ar & childs_;
    } 
  };

  class PatriciaTrie
  {
  public:
    PatriciaTrie();
    PatriciaTrie(std::string);
    ~PatriciaTrie();
    void insert(std::string, int freq);
    void save(std::string path);
    int search(std::string word);
    std::vector<std::tuple<std::string, int, int>> approx(std::string word, int dist);
    std::vector<std::tuple<std::string, int, int>> approx(std::string word, Node* n, int dist, std::string acc);

  private:
    friend class boost::serialization::access;

    std::string get_data(int pos, size_t len);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & tree_;
      ar & size_;
      ar & data_size_;
    }

  private:
    std::fstream data_;
    char buff_[BUFF_SIZE];
    int pos_ = -1;
    std::streampos beg_;


    Node tree_ = Node();
    size_t size_ = 0;
    size_t data_size_ = 0;
  };
}
