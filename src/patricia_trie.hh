# pragma once

# include <string>
# include <map>

namespace textMining
{
  struct Node
  {
  public:
    Node(size_t, size_t);
  public:
    size_t start_;
    size_t len_;
    std::map<char, Node*> childs_;
  };

  class PatriciaTrie
  {
  public:
    PatriciaTrie();
    PatriciaTrie(std::string);
    void insert(std::string);
    void reduce();
  private:
    std::string reducedNode(Node* node, char b);
    std::string reducedNode(Node* node);
    void reduce(Node* node, char b);
    void reduce(Node* node);

  private:
    std::string str_ = "";
    Node* tree_ = new Node(0, 0);
    bool reduced_ = false;
  };
}
