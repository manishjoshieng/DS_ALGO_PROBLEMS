#include <unordered_map>
#include <deque>
#include <cassert>
#include <algorithm>

using namespace std;

class LFUCache{

    public:
        LFUCache(int size) : _size(size){}


        void put(int key,int value){
            auto itr = _entries.find(key);
            if(itr == _entries.end()){
                clearSpace();
                _order.push_front(key);
                mapValue mv;
                mv._freq = 0;
                mv._position = _order.begin();
                mv._value = value;
                _entries[key] = mv;
            } else {
                if(itr->second._value != value){
                    itr->second._value = value;
                }
            }
        }

        int get(int key){
            auto itr = _entries.find(key);
            if(itr != _entries.end()) {
                updateFreqency(key);
                return _entries[key]._value;
            }
            return -1;
        }

    private:

        void clearSpace() {
            if(_size == _entries.size()) {
                auto key = _order.front();
                _entries.erase(key);
                _order.pop_front();
            }
        }

        void updateFreqency(int key){
            _entries[key]._freq++;
            auto itr = _entries[key]._position;

            if(std::next(itr) == _order.end()) return;

            while(itr !=_order.end() && 
               _entries[*itr]._freq <= _entries[key]._freq){
                itr++;
            }
            if(std::prev(itr)!=_entries[key]._position){
                if(itr != _order.end()) itr--;
                auto newItr = _order.insert(itr, key);
                _order.erase(_entries[key]._position);
                _entries[key]._position = newItr;
            }
        }
       
        struct mapValue
        {
            int                                 _freq;
            int                                 _value;
            deque<int>::iterator                _position;
        };
     
        int                                  _size;
        unordered_map<int, mapValue >        _entries;
        deque<int>                           _order;
}; 


int main() {
    LFUCache cache(3);

    cache.put(1,5);
    assert(cache.get(1) == 5);

    cache.put(2,4);
    assert(cache.get(2) == 4);
    assert(cache.get(2) == 4);
    cache.put(3,2);
    assert(cache.get(3)== 2);
    assert(cache.get(3)== 2);
    assert(cache.get(3)== 2);

    assert(cache.get(1) == 5);
    assert(cache.get(1) == 5);
    assert(cache.get(2) == 4);

    cache.put(4,2);
    assert(cache.get(4) == 2);

    assert(cache.get(3) == -1);
    assert(cache.get(2) == 4);

}
