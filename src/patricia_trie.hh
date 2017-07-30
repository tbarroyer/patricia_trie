# pragma once

# include <string>
# include <map>
# include <boost/serialization/split_free.hpp>
# include <boost/serialization/map.hpp>

namespace textMining
{
  struct Node
  {
  public:
    Node(size_t, size_t);
    Node();
  public:
    size_t start_;
    size_t len_;
    std::map<char, Node*> childs_;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & start_;
      ar & len_;
      ar & childs_;
    } 
  };

  class PatriciaTrie
  {
  public:
    PatriciaTrie();
    PatriciaTrie(std::string);
    void insert(std::string);
    void reduce();

  private:
    friend class boost::serialization::access;
    std::string reducedNode(Node* node, char b);
    std::string reducedNode(Node* node);
    void reduce(Node* node, char b);
    void reduce(Node* node);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & str_;
      ar & tree_;
      ar & reduced_;
    }

  private:
    std::string str_ = "";
    Node* tree_ = new Node(0, 0);
    bool reduced_ = false;
  };
}
