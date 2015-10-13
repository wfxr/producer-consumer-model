/*!
* brief: 单生产者-单消费者模型实例
*
* date: 2015/10/13
* author: Wenxuan
*/
#include <mutex>
#include <iostream>
#include <queue>
#include <functional>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

std::mutex stream_lock; // 全局锁，保证cout的同步

int main() {
	auto repository = std::make_shared<Repository>(10);

	Producer producer(repository, 1000);
	Consumer consumer(repository, 500);

	auto count = 50;  // 生产和消费的产品数量
	std::thread produce_thread(std::bind(&Producer::produce, &producer, count));
	std::thread consumer_thread(std::bind(&Consumer::consume, &consumer, count));

	produce_thread.join();
	consumer_thread.join();

	return 0;
}