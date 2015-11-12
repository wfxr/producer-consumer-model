#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

class Product;

// 产品仓库
class Repository {
public:
    std::mutex mutex;

    Repository(int storage) :_storage(storage) {}
    bool full();
    bool empty();
    void push(std::shared_ptr<Product>& product);
    std::shared_ptr<Product> fetch();

    std::condition_variable cv; // 条件变量，在库存发生变化时向调用者发送通知
private:
    std::queue<std::shared_ptr<Product>> _products; // 存储的产品
    size_t _storage; // 仓库容量
};

inline bool Repository::full() {
    return _products.size() == _storage;
}

inline bool Repository::empty() {
    return _products.empty();
}

inline void Repository::push(std::shared_ptr<Product>& product) {
    if (full())
        throw std::overflow_error("repository is already full!");
    _products.push(product);
    cv.notify_all();
}

inline std::shared_ptr<Product> Repository::fetch() {
    if (empty()) 
        throw std::underflow_error("repository is empty!");
    auto ret = _products.front();
    _products.pop();
    cv.notify_all();
    return ret;
}
