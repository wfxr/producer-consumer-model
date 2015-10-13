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
	Consumer(std::shared_ptr<Repository> repository, double consume_speed)
		: _repository(repository) {
		set_consume_speed(consume_speed);
	}

	void set_consume_speed(double produce_speed) {
		if (produce_speed <= 0.0)
			throw std::logic_error("consume speed should be greater than 0.0");
		_consume_speed = produce_speed;
	}

	// 消费单位产品的用时
	size_t unit_time_cost() {
		return static_cast<size_t>(1000 / _consume_speed);
	}

	// 等待仓库到货
	void waitProductArrival(std::unique_lock<std::mutex>& lock) {
		_repository->cv.wait(lock, [this] {return !_repository->empty(); });
	}

	void consume_one() {
		std::unique_lock<std::mutex> repositoryLock(_repository->mutex);

		// 如果仓库无货则等待到货
		while (_repository->empty()) {
			stream_lock.lock();
			std::cout << "\tConsumer is waiting for products..." << std::endl;
			stream_lock.unlock();
			waitProductArrival(repositoryLock);
		}

		// 开始消费，获得产品后不再占用repository
		auto product = _repository->fetch();
		repositoryLock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(unit_time_cost()));
	}

	void consume(int count) {
		for (auto i = 0; i < count; ++i) {
			consume_one();
			stream_lock.lock();
			std::cout << "\t" << i << " products consumed" << std::endl;
			stream_lock.unlock();
		}
	}
private:
	std::shared_ptr<Repository> _repository;
	double _consume_speed;
};