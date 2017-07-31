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

  PatriciaTrie::~PatriciaTrie()
  {
    this->data_.flush();
    this->data_.close();
  }

  PatriciaTrie::PatriciaTrie()
  {
    this->data_ = std::fstream("tmp", std::fstream::in | std::fstream::out | std::fstream::trunc);

    if (!this->data_.is_open())
    {
      std::cerr << "Unable to open file data" << std::endl;
      std::abort();
    }

    this->beg_ = this->data_.tellg();
  }

  PatriciaTrie::PatriciaTrie(std::string path)
  {
    this->data_ = std::fstream("tmp", std::fstream::in | std::fstream::out | std::fstream::trunc);

    if (!this->data_.is_open())
    {
      std::cerr << "Unable to open file data" << std::endl;
      std::abort();
    }

    this->beg_ = this->data_.tellg();

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
  }
  
  std::string PatriciaTrie::get_data(int pos, size_t len)
  {
    if (pos < 0)
    {
      std::cerr << "Wrong pos getting data" << std::endl;
      std::abort();
    }

    if (len > BUFF_SIZE)
    {
      std::cerr << "Wrong len getting data" << std::endl;
      std::abort();
    }

    if (!(this->pos_ >= 0 && pos > this->pos_ && pos < this->pos_ + BUFF_SIZE))
    {
      this->data_.flush();
      std::streampos old_pos = this->data_.tellg();
      this->data_.seekg(this->beg_);
      this->data_.seekg(pos, std::ios::cur);
      this->data_.read(this->buff_, BUFF_SIZE);
      this->data_.seekg(old_pos);
      this->pos_ = pos;
      this->data_.clear();
    }
    return std::string(this->buff_ + (pos - this->pos_), len);
  }

  bool PatriciaTrie::search(std::string word)
  {
    auto t = &(this->tree_);
    size_t i = 0;
    while (i < word.length() && t->childs_.find(word[i]) != t->childs_.end())
    {
      t = &(t->childs_[word[i]]);
      i++;
      if (t->len_ > 0)
      {
        size_t j = 0;
        while (j < t->len_ && this->get_data(t->start_ + j, 1)[0] == word[i])
        {
          j++;
          i++;
        }
        if (j < t->len_)
          return false;
      }
    }
    if (t->freq_ == -1 || i < word.length())
      return false;
    return true;
  }

  void PatriciaTrie::insert(std::string word, int freq)
  {
    auto t = &(this->tree_);
    size_t i = 0;
    while (i < word.length() && t->childs_.find(word[i]) != t->childs_.end())
    {
      t = &(t->childs_[word[i]]);
      i++;
      if (t->len_ > 0)
      {
        size_t j = 0;
        while (j < t->len_ && this->get_data(t->start_ + j, 1)[0] == word[i])
        {
          j++;
          i++;
        }
        if (j < t->len_)
        {
          char cap = this->get_data(j, 1)[0];

          size_t s = t->len_;
          t->len_ = j;
          t->childs_[cap] = Node();
          t->childs_[cap].len_ = s - j - 1;
          t->childs_[cap].start_ = t->start_ + j + 1;
          t->childs_[cap].freq_ = t->freq_;
          t->freq_ = -1;

          t->childs_[word[i]] = Node();
          if (i < word.length() - 1)
          {
            t->childs_[word[i]].start_ = this->data_size_;
            t->childs_[word[i]].len_ = word.length() - i - 1;
            //this->str_ += word.substr(i + 1, t->childs_[word[i]]->len_);
            this->data_ << word.substr(i + 1, t->childs_[word[i]].len_);
            this->data_size_ += t->childs_[word[i]].len_;
          }
          t->childs_[word[i]].freq_ = freq;
          this->size_++;
          return;
        }
      }
    }
    if (i < word.length())
    {
      t->childs_[word[i]] = Node();
      if (i < word.length() - 1)
      {
        t->childs_[word[i]].start_ = this->data_size_;
        t->childs_[word[i]].len_ = word.length() - i - 1;
        //this->str_ += word.substr(i + 1, t->len_);
        this->data_ << word.substr(i + 1, t->childs_[word[i]].len_);
        this->data_size_ += t->childs_[word[i]].len_;
      }
      t->childs_[word[i]].freq_ = freq;
      this->size_++;
    }
    else if (t->freq_ == -1)
    {
      t->freq_ = freq;
      this->size_++;
    }
  }
  
/*  std::string PatriciaTrie::reducedNode(std::shared_ptr<Node> node, char b)
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
  }*/

  void PatriciaTrie::save(std::string path)
  {
    std::ofstream ofs(path, std::ios::binary);
    {
      boost::archive::binary_oarchive oa(ofs);
      oa << *this;
    }
    ofs.close();
  }
}
