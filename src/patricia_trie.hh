# pragma once

# include <string>
# include <map>
# include <boost/serialization/split_free.hpp>
# include <boost/serialization/map.hpp>
# include <memory>
# include <fstream>
# include <tuple>
# include <vector>

# define BUFF_SIZE 10

namespace textMining
{
  /*! \struct Node
    \brief A node in the Patricia Trie data structure.
  */
  struct Node
  {
  public:
    //! Constructor.
    /*!
     *  \brief Creates a node with the given parameters.
     *  \param start start location of the word in the local disc.
     *  \param len word length (size).
     *  \param freq word frequency in the database.
    */
    Node(size_t start, size_t len, int freq);
    //! Default constructor.
    /*!
     * Creates a node with 0 as start location, 0 as length and -1 as size.
    */
    Node();

  public:
    size_t start_;  /*!< start location of the word in the local disc */
    size_t len_;  /*!< word length (size) */
    int freq_;  /*!< word frequency in the database */
    std::map<char, Node> childs_;  /*!< word childs */

  private:
    friend class boost::serialization::access;

    /*! \fn void serialize(Archive& ar, const unsigned int version)
     *  \brief Takes a data and a version and serializes it.
     *  \param ar The data to serialize.
     *  \param version The serialization version.
     */
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

  /*! \class Patricia Trie
    \brief The data is represented as a Patricia Trie in the disk.
  */
  class PatriciaTrie
  {
  public:
    PatriciaTrie();
    //! Constructor.
    /*!
     *  \brief takes a path to the words database and creates a PatriciaTrie from this database
     *  Iterates over all the lines in the file, constructs a node from the word and its frequency and inserts it in the tree
     *  \param path The path towrds the words databse
    */
    PatriciaTrie(std::string);
    ~PatriciaTrie();
    /*! \fn void insert(std::string, int freq);
     *  \brief Takes a word and its frequency, creates a node and inserts it in the trie.
     *  \param word The word to insert.
     *  \param freq The word frequency.
     */
    void insert(std::string, int freq);
    /*! \fn void save(std::string path);
     *  \brief Takes a path and saves the serialized data in the disk.
     *  \param path The path to the desired location in the disk.
     */
    void save(std::string path);
    /*! \fn int search(std::string word);
     *  \brief Takes two words and returns its frequency in the database.
     *  \param word The word to search for in the tree.
     *  \return The word frequency if the word is found, -1 if it does not exist.
     */
    int search(std::string word);
    /*! \fn std::vector<std::tuple<std::string, int, int>> approx(std::string word, int dist);
     *  \brief Takes a word and a Damerau-Levenshtein distance and returns the correction results respecting the passed distance.
     *  The returned results are sorted according to the distance, frequency and the lexicographic order.
     *  \param word The word to correct.
     *  \param dist The Damerau-Levenshtein distance to respect when suggesting correction results.
     *  \return A sorted vetor of tuples: 1st elem = word, 2nd elem = word distance from the word passed as argument, 3rd elem = word frequency.
     */
    std::vector<std::tuple<std::string, int, int>> approx(std::string word, int dist);
    /*! \fn std::vector<std::tuple<std::string, int, int>> approx(std::string word, Node* n, int dist, std::string acc);
     *  \brief Iterates over the current node (in the tree) childs and returns the nodes that respects the passed distance.
     *  \param word The word to correct.
     *  \param n The current node in the tree
     *  \param dist The Damerau-Levenshtein distance to respect when suggesting correction results.
     *  \return A sorted vetor of tuples: 1st elem = word, 2nd elem = word distance from the word passed as argument, 3rd elem = word frequency.
     */
    std::vector<std::tuple<std::string, int, int>> approx(std::string word, Node* n, int dist, std::string acc);

  private:
    friend class boost::serialization::access;

    std::string get_data(int pos, size_t len);

     /*! \fn void serialize(Archive& ar, const unsigned int version)
     *  \brief Takes a data and a version and serializes it.
     *  \param ar The data to serialize.
     *  \param version The serialization version.
     */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & tree_;
      ar & size_;
      ar & data_size_;
    }

  private:
    std::fstream data_; /*!< data stream when it is loaded from the specified location */
    char buff_[BUFF_SIZE];
    int pos_ = -1;
    std::streampos beg_;


    Node tree_ = Node(); /*!< start node in the patricia trie */
    size_t size_ = 0; /*!< Patricia trie size, default value = 0 */
    size_t data_size_ = 0; /*!< data size, default value = 0 */
  };
}
