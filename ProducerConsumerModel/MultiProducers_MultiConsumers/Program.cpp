/*!
* brief: ��������-��������ģ��ʵ��
*
* date: 2015/10/18
* author: Wenxuan
*/
#include <mutex>
#include <vector>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

std::mutex stream_lock; // ȫ��������֤cout��ͬ��

int main() {
    // �����ֿ�
    auto repository = std::make_shared<Repository>(10);

    // ���������ߺ�������
    Producer producer1(repository, "Producer 1");
    Producer producer2(repository, "Producer 2");
    Consumer consumer1(repository, "Consumer 1");
    Consumer consumer2(repository, "Consumer 2");
    Consumer consumer3(repository, "Consumer 3");

    // �ֱ��趨��λ�����ĺ�ʱ
    producer1.set_unit_cost(100);
    producer2.set_unit_cost(50);
    consumer1.set_unit_cost(80);
    consumer2.set_unit_cost(100);
    consumer3.set_unit_cost(150);

    // ������ִ�������������߳�
    std::vector<std::thread> threads;
    threads.emplace_back([&producer1] { producer1.produce(20); });
    threads.emplace_back([&producer2] { producer2.produce(30); });
    threads.emplace_back([&consumer1] { consumer1.consume(20); });
    threads.emplace_back([&consumer2] { consumer2.consume(20); });
    threads.emplace_back([&consumer3] { consumer3.consume(10); });

    // �ȴ��̺߳ϲ�
    for (auto& th : threads) th.join();

    return 0;
}