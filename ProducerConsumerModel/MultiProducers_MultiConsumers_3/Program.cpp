/*!
* brief: 单生产者-多消费者模型实例
*
* date: 2015/10/18
* author: Wenxuan
*/
#include <mutex>
#include <vector>
#include <memory>
using std::shared_ptr;

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

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
    producer1.set_unit_cost(900);
    producer2.set_unit_cost(500);
    consumer1.set_unit_cost(800);
    consumer2.set_unit_cost(1000);
    consumer3.set_unit_cost(1500);

    // 创建并执行生产和消费线程
    std::vector<std::thread> threads;
    threads.emplace_back([&producer1] { producer1.produce(); });
    threads.emplace_back([&producer2] { producer2.produce(); });
    threads.emplace_back([&consumer1] { consumer1.consume(); });
    threads.emplace_back([&consumer2] { consumer2.consume(); });
    threads.emplace_back([&consumer3] { consumer3.consume(); });

    // 等待线程合并
    for (auto &th : threads)
        th.join();

    return 0;
}
