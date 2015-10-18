/*!
* brief: 单生产者-多消费者模型实例
*
* date: 2015/10/18
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
    Consumer consumer1(repository, "Consumer1");
    Consumer consumer2(repository, "Consumer2");

    // 分别设定单位工作的耗时
    producer.set_unit_cost(20);
    consumer1.set_unit_cost(30);
    consumer2.set_unit_cost(50);

    // 创建并执行生产和消费线程
    std::thread producer_thread([&producer] { producer.produce(5); });
    std::thread consumer_thread1([&consumer1] { consumer1.consume(2); });
    std::thread consumer_thread2([&consumer2] { consumer2.consume(3); });

    // 等待线程合并
    producer_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();

    return 0;
}