/*!
* brief: 单生产者-单消费者模型实例
*
* date: 2015/10/13
* author: Wenxuan
*/
#include <mutex>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

std::mutex stream_lock; // 全局锁，保证cout的同步

int main() {
	// 创建仓库
	auto repository = std::make_shared<Repository>(10);

	// 创建生产者和消费者
	Producer producer(repository);
	Consumer consumer(repository);

	// 分别设定单位工作的耗时
	producer.set_unit_cost(20);
	consumer.set_unit_cost(30);

	auto count = 50;  // 准备生产和消费的产品数量
	std::thread producer_thread([&producer, count] {producer.produce(count); });
	std::thread consumer_thread([&consumer, count] {consumer.consume(count); });

	// 等待线程合并
	producer_thread.join();
	consumer_thread.join();

	return 0;
}