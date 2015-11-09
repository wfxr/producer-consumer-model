#pragma once
#include <mutex>
#include <queue>

class Product;

// 产品仓库
class Repository {
public:
    std::mutex mutex;

    Repository(int storage) : _storage(storage), _producer_count(0) {}
    bool full();
    bool empty();
    bool hasProducer();
    void push(std::shared_ptr<Product> &product);
    std::shared_ptr<Product> fetch();
    void add_producer();
    void remove_producer();

    std::condition_variable cv; // 条件变量，在库存发生变化时向调用者发送通知
private:
    std::queue<std::shared_ptr<Product>> _products; // 存储的产品
    size_t _storage;                                // 仓库容量
    size_t _producer_count; // 已订阅的生产者的数量
};

inline bool Repository::full() { return _products.size() == _storage; }

inline bool Repository::empty() { return _products.empty(); }

inline void Repository::push(std::shared_ptr<Product> &product) {
    if (full()) throw std::overflow_error("repository is already full!");
    _products.push(product);
    cv.notify_all();
}

inline std::shared_ptr<Product> Repository::fetch() {
    if (empty()) throw std::underflow_error("repository is empty!");
    auto ret = _products.front();
    _products.pop();
    cv.notify_all();
    return ret;
}

inline void Repository::add_producer() { ++_producer_count; }

inline void Repository::remove_producer() { --_producer_count; }

inline bool Repository::hasProducer() { return _producer_count != 0; }
