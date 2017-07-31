# include <iostream>
# include <sstream>
# include <cstdlib>
# include <fstream>
# include <boost/archive/binary_oarchive.hpp>

# include "patricia_trie.hh"
# include "tools.hh"

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
      std::cerr << "File not found: " << path << std::endl;
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
  
  std::vector<std::tuple<std::string, int, int>> PatriciaTrie::approx(std::string word, Node* n, int dist, std::string acc)
  {
    acc += this->get_data(n->start_, n->len_);

    std::vector<std::tuple<std::string, int, int>> out;
    if (n->freq_ != -1)
    {
      int d = Tools::distance(word, acc);
      if (d <= dist)
        out.push_back(std::make_tuple(acc, d, n->freq_));
    }
    
    for (auto it = n->childs_.begin(); it != n->childs_.end(); ++it)
    {
      auto add = this->approx(word, &(it->second), dist, acc + it->first);
      out.insert(out.end(), add.begin(), add.end());
    }
    return out;
  }

  std::vector<std::tuple<std::string, int, int>> PatriciaTrie::approx(std::string word, int dist)
  {
    std::vector<std::tuple<std::string, int, int>> out;
    auto t = &(this->tree_);
    std::string acc = "";
    for (auto it = t->childs_.begin(); it != t->childs_.end(); ++it)
    {
      auto add = this->approx(word, &(it->second), dist, acc + it->first);
      out.insert(out.end(), add.begin(), add.end());
    }
    return out;
  }

  int PatriciaTrie::search(std::string word)
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
          return -1;
      }
    }
    if (t->freq_ == -1 || i < word.length())
      return -1;
    return t->freq_;
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
  
  void PatriciaTrie::save(std::string path)
  {
    std::ofstream ofs(path, std::fstream::binary | std::fstream::out);
    {
      boost::archive::binary_oarchive oa(ofs);
      oa << *this;
    }
    ofs.close();
  }
}
