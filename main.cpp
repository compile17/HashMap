#include <unordered_map>
#include <iostream>
#include <forward_list>
#include <vector>

template <typename Key,typename Val>
class HashTable
{
    using node = std::forward_list<std::pair<Key, Val>>;

    static unsigned hash(const Key &id)         {std::hash<Key> hash_fn; return hash_fn(id)%17;}
    std::vector<node>& table()                  {return mLinks;}

    std::vector<node> mLinks;
    int size;
public:
    HashTable() {mLinks.resize(17), size = 0;}

    void insert(const Key& key, const Val& val);
    void erase(const Key& key);
    Val operator[](const Key& key);

    int Size() {return size;}

    class iterator;
    iterator begin()
    {
        auto firstList = mLinks.begin();
        auto firstPair = mLinks[0].begin();
        for (; firstList != mLinks.end(); ++firstList)
            if (!(*firstList).empty())
            {
                firstPair = (*firstList).begin();
                break;
            }

        return iterator(firstList, firstPair, *this);
    }

    iterator end()
    {
        auto firstList = --mLinks.end();
        auto firstPair = mLinks[0].begin();
        for (; firstList != mLinks.begin(); --firstList)
            if (!(*firstList).empty())
            {
                firstPair = (*firstList).end();
                break;
            }

        return iterator(firstList, firstPair, *this);
    }

    class iterator: public std::iterator<std::forward_iterator_tag, std::pair<Key, Val>>
    {
        std::vector<node> basket;
        std::pair<Key, Val> *mLink;
        typename std::vector<node>::iterator it1;
        typename node::iterator it2;
        int size;
        int index;

    public:
        iterator()                                                      {}
        iterator(typename std::vector<node>::iterator itVec, typename node::iterator itFL, HashTable ht)
            : basket(ht.table()), mLink(&*itFL), it1(itVec), it2(itFL), size(ht.Size()), index (0)  {}

        bool operator==(iterator const& other)   const {return mLink->mKey == other.mLink->mKey;}
        bool operator!=(iterator const& other)   const {return mLink != other.mLink && index < size;}
        typename iterator::reference operator*() const {return *mLink;}
        iterator& operator++()
        {
            if (++it2 == (*it1).end())
            {
                // функия поиска следующего узла
                while (1)
                {
                    ++it1;
                    if (!(*it1).empty()) break;
                }
                it2 = (*it1).begin();
            }

            ++index;
            mLink = &*it2;

            return *this;
        }
    };
};

template<typename Key, typename Val>
void HashTable<Key, Val>::insert(const Key &key, const Val &val)
{
    size_t place = HashTable::hash(key);
    std::pair<Key, Val> pair(key, val);
    mLinks[place].push_front(pair);
    ++size;
}

template<typename Key, typename Val>
void HashTable<Key, Val>::erase(const Key &key)
{
    size_t place = hash(key);
    auto begin = mLinks[place].begin(), prev = mLinks[place].before_begin();

    if (std::next(begin) == mLinks[place].end())
        mLinks[place].clear();
    else {
        while(begin != mLinks[place].end())
        {
            auto s = *begin;
            if(s.first == key) break;

            ++begin;
            ++prev;
        }

        --size;
        mLinks[place].erase_after(prev);
    }
}

template<typename Key, typename Val>
Val HashTable<Key, Val>::operator[](const Key &key)
{
    size_t place = hash(key);
    for (std::pair<Key, Val> p : mLinks[place])
        if (p.first == key)
            return p.second;
    return Val();
}

int main(int argc, char *argv[])
{
    HashTable<int, int> hT;
    hT.insert(17, 17);
    hT.insert(51, 31);
    hT.insert(34, 49);
    hT.insert(5, 22);
    hT.insert(22, 39);
    hT.insert(39, 58);
    hT.insert(1, 18);
    hT.insert(18, 35);
    hT.insert(35, 52);

//    hT.erase(17);
//    hT.erase(22);
//    hT.erase(35);

    std::cout << hT[17] << " "
              << hT[22] << " "
              << hT[35] << std::endl;

    HashTable<std::string, std::string> ht1;
    ht1.insert("sad", "boy");        // [2]
    ht1.insert("ocean", "eyes");     // [0]
    ht1.insert("green", "grass");    // [0]
    ht1.insert("vodka", "tequila");  // [13]
    ht1.insert("rome", "italy");     // [13]
    ht1.insert("white", "black");    // [5]
    ht1.insert("Jobs", "Steve");     // [8]
    ht1.insert("West", "Kanye");     // [3]
    ht1.insert("GaGa", "Lady");      // [8]
    ht1.insert("Warhol", "Andy");    // [4]
    ht1.insert("A$AP", "Rocky");     // [10]
    ht1.insert("Hardy", "Tom");      // [13]
    ht1.insert("Holland", "Tom");    // [14]
    ht1.insert("Venom", "Carnage");  // [8]

//    ht1.erase("rome");
//    ht1.erase("Venom");
//    ht1.erase("ocean");
//    ht1.erase("sad");

    std::cout << ht1["rome"]     << " "
              << ht1["Venom"]    << " "
              << ht1["ocean"]    << " "
              << ht1["A$AP"]     << std::endl;

    HashTable<double, double> ht2;
    ht2.insert(3.14, 1.0);      // [5]
    ht2.insert(58.3,2.0);       // [3]
    ht2.insert(17.7,3.0);       // [6]
    ht2.insert(33.1,4.0);       // [13]
    ht2.insert(36.5,5.0);       // [3]
    ht2.insert(49.2,6.0);       // [2]
    ht2.insert(42.0,7.0);       // [3]
    ht2.insert(2.21,8.0);       // [5]
    ht2.insert(35.17,9.0);      // [10]

//    ht2.erase(2.21);
//    ht2.erase(36.5);
//    ht2.erase(35.17);

    std::cout << ht2[35.17] << " "
              << ht2[49.2]  << " "
              << ht2[35.17] << std::endl;

    std::cout << ht2[36.5] << " "
              << ht2[2.21]  << " "
              << ht2[35.17] << std::endl;

    for (std::pair<double, double> l : ht2)
        std::cout << l.first << " " << l.second << std::endl;

    return 0;
}
