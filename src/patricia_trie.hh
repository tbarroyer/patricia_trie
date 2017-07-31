# pragma once

# include <string>
# include <map>
# include <boost/serialization/split_free.hpp>
# include <boost/serialization/map.hpp>
# include <boost/serialization/shared_ptr.hpp>
# include <memory>
# include <fstream>

# define BUFF_SIZE 10000

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
    std::map<char, std::shared_ptr<Node>> childs_;

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
    //void reduce();
    void save(std::string path);

  private:
    friend class boost::serialization::access;

    //std::string reducedNode(std::shared_ptr<Node> node, char b);
    //std::string reducedNode(std::shared_ptr<Node> node);
    //void reduce(std::shared_ptr<Node> node, char b);
    //void reduce(std::shared_ptr<Node> node);

    std::string get_data(int pos, size_t len);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      //ar & str_;
      ar & tree_;
      ar & reduced_;
      ar & size_;
      ar & data_size_;
    }

  private:
    std::fstream data_;
    //std::string str_ = "";
    char buff_[BUFF_SIZE];
    int pos_ = -1;
    std::streampos beg_;


    std::shared_ptr<Node> tree_ = std::make_shared<Node>();
    bool reduced_ = false;
    size_t size_ = 0;
    size_t data_size_ = 0;
  };
}
