# include <iostream>
# include <sstream>
# include <cstdlib>
# include <fstream>
# include <boost/archive/binary_oarchive.hpp>

# include "patricia_trie.hh"

namespace textMining
{
  Node::Node(size_t start, size_t len, int freq) :
    start_(start), len_(len), freq_(freq)
  {}

  Node::Node() :
    start_(0), len_(0), freq_(-1)
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
      this->insert(word, freq);
    }

    file.close();
    std::cout << "Inserted" << std::endl;
  }

  void PatriciaTrie::insert(std::string word, int freq)
  {
    if (this->reduced_)
      return;

    auto t = this->tree_;
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
        t->childs_[word[i]] = std::make_shared<Node>();
        t = t->childs_[word[i]];
        i++;
      }
      t->freq_ = freq;
      this->size_++;
    }
    else if (t->freq_ == -1)
    {
      t->freq_ = freq;
      this->size_++;
    }
  }
  
  std::string PatriciaTrie::reducedNode(std::shared_ptr<Node> node, char b)
  {
    if (node->childs_.size() == 1)
      return b + this->reducedNode(node->childs_.begin()->second, node->childs_.begin()->first);
    return std::string(1, b);
  }
  
  std::string PatriciaTrie::reducedNode(std::shared_ptr<Node> node)
  {
    if (node->childs_.size() == 1)
      return this->reducedNode(node->childs_.begin()->second, node->childs_.begin()->first);
    return "";
  }

  void PatriciaTrie::reduce(std::shared_ptr<Node> node, char b)
  {
    std::string red = this->reducedNode(node, b);

    if (red.length() > 1)
    {
      std::string att = red.substr(1, red.length() - 1);
      node->start_ = this->str_.length();
      node->len_ = att.length();
      this->str_ += att;

      auto attach = node->childs_.begin()->second;
      for (size_t i = 0; i < att.length(); ++i)
      {
        if (attach->childs_.size() > 0)
        {
          //delete attach;
          attach = attach->childs_.begin()->second;
        }
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

  void PatriciaTrie::reduce(std::shared_ptr<Node> node)
  {
    std::string red = this->reducedNode(node);

    if (red.length() > 1)
    {
      std::string att = red.substr(1, red.length() - 1);
      node->start_ = this->str_.length();
      node->len_ = att.length();
      this->str_ += att;

      auto attach = node->childs_.begin()->second;
      for (size_t i = 0; i < att.length(); ++i)
      {
        if (attach->childs_.size() > 0)
        {
          //delete attach;
          attach = attach->childs_.begin()->second;
        }
        else
          attach = NULL;
      }

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

    std::cout << "Reduced" << std::endl;
    //std::cout << this->str_ << std::endl;
  }

  void PatriciaTrie::save(std::string path)
  {
    std::ofstream ofs(path);
    {
      boost::archive::binary_oarchive oa(ofs);
      oa << *this;
    }
  }
}
