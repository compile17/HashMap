#include <iostream>
#include <forward_list>
#include <vector>

template<typename Key,typename Val>
struct LinkedHashEntry {
    LinkedHashEntry(const Key& key, const Val& val, size_t hash)
        : mKey(key), mHash(hash), mValue(val)                   {}

    const Key       mKey;
    const size_t      mHash;
    Val             mValue;
};

template <typename Key,typename Val>
class HashTable
{
    using Link = LinkedHashEntry<Key, Val> ;

    unsigned hash(const Key &id)       {std::hash<Key> hash_fn; return hash_fn(id)%17;}

    std::vector<std::forward_list<Link*>> mLinks;
public:
    HashTable() {mLinks.resize(17);}

    void insert(const Key& key, const Val& val);
    void erase(const Key& key);
    Val operator[](const Key& key);

    std::vector<std::forward_list<Link*>> table() {return mLinks;}

    class iterator;
    iterator begin()
    {
        for (size_t i = 0; i < mLinks.size(); ++i)
            if (!mLinks[i].empty())
                return iterator(mLinks[i].front(), *this);
        return iterator(nullptr, *this);
    }

    iterator end()
    {
        Link *buf = nullptr;
        for (size_t i = 0; i < mLinks.size(); ++i)
            if (!mLinks[i].empty())
//                buf = *(mLinks[i].end());
                for (auto& j : mLinks[i])
                    buf = j;
        return iterator(buf, *this);
    }


    class iterator: public std::iterator<std::forward_iterator_tag, Link>
    {
    public:
        iterator(Link *link, HashTable ht)  : basket(ht.table()), mLink(link), index(0), right(-1)    {notZero();}
        iterator(const iterator &it)        : mLink(it.mLink)                                         {}

        bool operator==(iterator const& other)   const {return mLink->mKey == other.mLink->mKey;}
        bool operator!=(iterator const& other)   const {return mLink->mKey != other.mLink->mKey;}
        typename iterator::reference operator*() const {return *mLink;}
        iterator& operator++()
        {
            it2 = basket[right].begin();
            std::advance(it2,index);
            if (++it2 == basket[right].end())
            {
                notZero();
                it2 = basket[right].begin();
                index = 0;
            }
            else index++;

            mLink = *it2;

            return *this;
        }
    private:
        void notZero()
        {
            for (int i = right+1; i < basket.size();++i)
                if (!basket[i].empty())
                {
                    right = i;
                    break;
                }
        }

        std::vector<std::forward_list<Link*>> basket;
        Link* mLink;
        size_t index; // индекс текущего положения в vector
        int right; // индекс текущего элемента в списке
        typename std::forward_list<Link*>::iterator it2;
    };
};

template<typename Key, typename Val>
void HashTable<Key, Val>::insert(const Key &key, const Val &val)
{
    Link *subLink = new Link(key, val, hash(key));
    size_t place = subLink->mHash;
    mLinks[place].push_front(subLink);
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
            if(s->mKey == key) break;

            ++begin;
            ++prev;
        }

        mLinks[place].erase_after(prev);
    }
}

template<typename Key, typename Val>
Val HashTable<Key, Val>::operator[](const Key &key)
{
    size_t place = hash(key);
    for (const Link* l : mLinks[place])
        if (l->mKey == key)
            return l->mValue;
    return Val();
}

int main()
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

//        hT.erase(17);
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

    ht2.erase(2.21);
    ht2.erase(36.5);
    ht2.erase(35.17);


    std::cout << ht2[35.17] << " "
              << ht2[49.2]  << " "
              << ht2[35.17] << std::endl;

    std::cout << ht2[36.5] << " "
              << ht2[2.21]  << " "
              << ht2[35.17] << std::endl;

    for (LinkedHashEntry<double, double> l : ht2)
        std::cout << l.mValue << " ";

    return 0;
}
