#include    <unordered_map>
#include    <deque>
#include    <cassert>

using namespace std;
class LRU{
    public:
    LRU(int size) : _size(size){}
    int get(int key){
        auto itr = _entries.find(key);
        if (itr ==_entries.end()){
            return -1;
        }
        mapValue v = itr->second;
        jumpToFront(key, v._indexInList);
        return _entries[key]._value;
    }

    bool put(int key,int value){
        auto itr = _entries.find(key);
        if(itr != _entries.end()) {
            _entries[key]._value = value;
            return true;
        }
        create_space();

        _order.push_front(key);
        mapValue v = {key, _order.begin()};
        _entries[key] = v;
        return true;
    }

    
    private:

    void create_space(){
        if(_order.size() == _size){
            auto key = _order.back();
            _entries.erase(key);
            _order.pop_back();
        }
    }

    void jumpToFront(int key,deque<int>::iterator index){
        _order.erase(index);
        _order.push_front(key);
        _entries[key]._indexInList = _order.begin();
    }

    struct mapValue{
        int _value;
        deque<int>::iterator _indexInList;
    };

    int                    _size;
    deque<int>             _order;        
    unordered_map<int,mapValue> _entries;
};


int main() {
    {
    LRU cache(2);

    cache.put(1, 1);
    assert(cache.get(1) == 1);

    cache.put(2, 2);
    assert(cache.get(2) == 2);

    cache.put(3, 3); // evicts key 1
    assert(cache.get(1) == -1);

    cache.put(2, 4);
    assert(cache.get(2) == 4);

    cache.put(4, 4); // evicts key 3
    assert(cache.get(3) == -1);

    assert(cache.get(5) == -1);
    }
}