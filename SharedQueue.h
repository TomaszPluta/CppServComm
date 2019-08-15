#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


template <class T>
class SharedQueue{
private:
    std::condition_variable _dataReady;
    std::mutex _m;
    std::queue<T> _q;
    
public:
    void Push(const T item){
    std::unique_lock<std::mutex> qLock(_m);
    _q.push(item);
    _dataReady.notify_one();
     qLock.unlock();
     }
     
     void Pop (void){
         std::unique_lock<std::mutex>qLock(_m);
         while (_q.empty()){
            _dataReady.wait(qLock);
         }
         _q.pop();
     }
     
         T& Front (void){
         std::unique_lock<std::mutex>qLock(_m);
         while (_q.empty()){
            _dataReady.wait(qLock);
         }
         return _q.front();
     }
    
};