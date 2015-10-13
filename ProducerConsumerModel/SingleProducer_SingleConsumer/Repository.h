#pragma once
#include <mutex>
#include <queue>

class Product;

// 产品仓库
class Repository {
public:
	std::mutex mutex;

	Repository(int storage) :_storage(storage) {}
	std::condition_variable cv; // 条件变量，在库存发生变化时向生产者和消费者发送通知
	bool full() {
		return _products.size() == _storage;
	}
	bool empty() {
		return _products.empty();
	}
	void push(std::shared_ptr<Product>& product) {
		_products.push(product);
		cv.notify_all();
	}
	auto fetch() {
		auto ret = _products.front();
		_products.pop();
		cv.notify_all();
		return ret;
	}
private:
	std::queue<std::shared_ptr<Product>> _products;
	size_t _storage; // 仓库容量
};