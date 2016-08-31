/*!
* brief: 单生产者-多消费者模型实例
*
* date: 2015/10/18
* author: Wenxuan
*/
#include <mutex>
#include <vector>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"
#include <future>

std::mutex stream_lock; // 全局锁，保证cout的同步

int main() {
    // 创建仓库
    auto repository = std::make_shared<Repository>(10);

    // 创建生产者和消费者
    Producer producer1(repository, "Producer 1");
    Producer producer2(repository, "Producer 2");
    Consumer consumer1(repository, "Consumer 1");
    Consumer consumer2(repository, "Consumer 2");
    Consumer consumer3(repository, "Consumer 3");

    // 分别设定单位工作的耗时
    producer1.set_unit_cost(100);
    producer2.set_unit_cost(50);
    consumer1.set_unit_cost(80);
    consumer2.set_unit_cost(100);
    consumer3.set_unit_cost(150);

    // 异步执行生产和消费任务
    auto p1 = std::async([&producer1] { producer1.produce(20); });
    auto p2 = std::async([&producer2] { producer2.produce(30); });
    auto c1 = std::async([&consumer1] { consumer1.consume(20); });
    auto c2 = std::async([&consumer2] { consumer2.consume(20); });
    auto c3 = std::async([&consumer3] { consumer3.consume(10); });

    return 0;
}
