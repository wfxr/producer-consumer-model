#pragma once

#include <memory>
#include <mutex>
#include <iostream>
#include <thread>

#include "Repository.h"
#include "Product.h"

extern std::mutex stream_lock;

// 生产者
class Producer {
public:
    Producer(std::shared_ptr<Repository>, std::string);

    void set_unit_cost(size_t);

    void wait_repository_vacancy(std::unique_lock<std::mutex>&);

    void produce(int);
private:
    std::shared_ptr<Product> make_product();

    std::shared_ptr<Repository> _repository;
    std::string _name;
    size_t _unit_cost = 1;
};

inline Producer::Producer(std::shared_ptr<Repository> repository, std::string name)
    : _repository(repository), _name(name) { }

inline void Producer::set_unit_cost(size_t unit_cost) {
    _unit_cost = unit_cost;
}

void Producer::wait_repository_vacancy(std::unique_lock<std::mutex>& lock) {
    stream_lock.lock();
    std::cout << _name << " is waiting for an empty slog..." << std::endl;
    stream_lock.unlock();
    _repository->cv.wait(lock, [this] {return !_repository->full(); });
}

void Producer::produce(int count) {
    // 创建一个仓库锁
    std::unique_lock<std::mutex> repositoryLock(_repository->mutex);
    _repository->add_producer();

    for (auto i = 0; i < count; ++i) {
        repositoryLock.unlock();
        auto newProduct = make_product();

        // 如果仓库已满则等待空位
        repositoryLock.lock();
        if (_repository->full())
            wait_repository_vacancy(repositoryLock);

        _repository->push(newProduct);
    }
    _repository->remove_producer();
}

std::shared_ptr<Product> Producer::make_product() {
    // 模拟生产耗时
    std::this_thread::sleep_for(std::chrono::milliseconds(_unit_cost));

    auto product = std::make_shared<Product>();
    stream_lock.lock();
    std::cout << _name << ": [" << product->get_id() << "] produced." << std::endl;
    stream_lock.unlock();
    return product;
}
