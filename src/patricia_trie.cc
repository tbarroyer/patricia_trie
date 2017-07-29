# include <fstream>
# include <iostream>
# include <sstream>
# include <cstdlib>

# include "patricia_trie.hh"

namespace textMining
{
  Node::Node(size_t start, size_t len) :
    start_(start), len_(len)
  {}

  PatriciaTrie::PatriciaTrie()
  {}

  PatriciaTrie::PatriciaTrie(std::string path)
  {
    std::ifstream file(path);
    if (!file.is_open())
    {
      std::cerr << "Unable to open file" << std::endl;
      std::abort();
    }

    std::string line;
    std::string word;
    int freq;

    while (std::getline(file, line))
    {
      std::stringstream(line) >> word >> freq;
      this->insert(word);
    }

    (void) freq;

    file.close();
  }

  void PatriciaTrie::insert(std::string word)
  {
    if (this->reduced_)
      return;

    Node* t = this->tree_;
    size_t i = 0;
    while (i < word.length() && t->childs_.find(word[i]) != t->childs_.end())
    {
      t = t->childs_[word[i]];
      i++;
    }
    if (i < word.length())
    {
      while (i < word.length())
      {
        t->childs_[word[i]] = new Node(0, 0);
        t = t->childs_[word[i]];
        i++;
      }
    }
  }
  
  std::string PatriciaTrie::reducedNode(Node* node, char b)
  {
    if (node->childs_.size() == 1)
      return b + this->reducedNode(node->childs_.begin()->second, node->childs_.begin()->first);
    return std::string(1, b);
  }
  
  std::string PatriciaTrie::reducedNode(Node* node)
  {
    if (node->childs_.size() == 1)
      return this->reducedNode(node->childs_.begin()->second, node->childs_.begin()->first);
    return "";
  }

  void PatriciaTrie::reduce(Node* node, char b)
  {
    std::string red = this->reducedNode(node, b);

    if (red.length() > 1)
    {
      std::string att = red.substr(1, red.length() - 1);
      node->start_ = this->str_.length();
      node->len_ = att.length();
      this->str_ += att;

      Node* attach = node;
      for (size_t i = 0; i < att.length() + 1; ++i)
      {
        if (attach->childs_.size() > 0)
          attach = attach->childs_.begin()->second;
        else
          attach = NULL;
      }
      if (attach != NULL)
        node->childs_[red[0]] = attach;
      else
        node->childs_.clear();
    }

    for (auto it=node->childs_.begin(); it != node->childs_.end(); ++it)
      this->reduce(it->second, it->first);
  }

  void PatriciaTrie::reduce(Node* node)
  {
    std::string red = this->reducedNode(node);

    if (red.length() > 1)
    {
      std::string att = red.substr(1, red.length() - 1);
      node->start_ = this->str_.length();
      node->len_ = att.length();
      this->str_ += att;

      Node* attach = node;
      for (size_t i = 0; i < att.length() + 1; ++i)
        attach = attach->childs_.begin()->second;

      if (attach != NULL)
        node->childs_[red[0]] = attach;
      else
        node->childs_.clear();
    }

    for (auto it = node->childs_.begin(); it != node->childs_.end(); ++it)
      this->reduce(it->second, it->first);
  }

  void PatriciaTrie::reduce()
  {
    if (this->reduced_)
      return;

    reduced_ = true;

    this->reduce(this->tree_);
  }
}
