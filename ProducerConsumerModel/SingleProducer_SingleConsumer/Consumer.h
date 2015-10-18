#pragma once

#include <memory>
#include <mutex>
#include <iostream>

#include "Repository.h"
#include "Product.h"

extern std::mutex stream_lock;

// 消费者
class Consumer {
public:
	Consumer(std::shared_ptr<Repository>);

	void set_unit_cost(size_t);

	void wait_product_arrival(std::unique_lock<std::mutex>&);

	void consume();

	void consume(int count);
private:
	std::shared_ptr<Repository> _repository;
	size_t _unit_cost = 1;
};
inline Consumer::Consumer(std::shared_ptr<Repository> repository)
	: _repository(repository) {
}

inline void Consumer::set_unit_cost(size_t unit_cost) {
	_unit_cost = unit_cost;
}


inline void Consumer::wait_product_arrival(std::unique_lock<std::mutex>& lock) {
	_repository->cv.wait(lock, [this] {return !_repository->empty(); });
}

inline void Consumer::consume() {
	std::unique_lock<std::mutex> repositoryLock(_repository->mutex);

	// 如果仓库无货则等待到货
	if (_repository->empty()) {
		stream_lock.lock();
		std::cout << "\tConsumer is waiting for products..." << std::endl;
		stream_lock.unlock();
		wait_product_arrival(repositoryLock);
	}

	// 开始消费，获得产品后不再占用repository
	auto product = _repository->fetch();
	repositoryLock.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(_unit_cost));
}

inline void Consumer::consume(int count) {
	for (auto i = 0; i < count; ++i) {
		consume();
		stream_lock.lock();
		std::cout << "\t" << i + 1 << " products consumed" << std::endl;
		stream_lock.unlock();
	}
}
