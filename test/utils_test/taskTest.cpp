#include "taskTest.h"

#include <gtest/gtest.h>

#include <liter/task.h>
#include <vector>
#include <functional>


TEST_F(TaskTest, testGet)
{
    liter::task<int(int)> t([](int i)->int{
            return i;
        });

    EXPECT_EQ(0, t.get(0));
    EXPECT_EQ(5, t.get(5));
    EXPECT_EQ(70, t.get(70));

    liter::task<void(int)> t1([](int i){
        });
}


TEST_F(TaskTest, testRun)
{
    liter::task<int()> t([]()->int{
            return 0;
        });

    t.run();
}

TEST_F(TaskTest, testThen)
{
    int g = 0;
    liter::task<int(int)> t([&](int i){
            g += i;
            return g;
        });

    EXPECT_EQ(6, t.get(6));

    g = 0;
    auto t1 = t.then([&](int i) -> void {
            g += i;
        });
    t1.get(5);

    EXPECT_EQ(g, 10);
}


TEST_F(TaskTest, testGroupRunAndWait)
{
    liter::task_group g;
    g.run(liter::task<int()>([]()->int{
            return 1;
            }));

    g.run(liter::task<void()>([](){}));

    g.wait();
}


TEST_F(TaskTest, testWhenAll)
{
    std::vector<liter::task<int()>> v = {
        liter::task<int()>([]() -> int{return 0;}),
        liter::task<int()>([]() -> int{return 1;}),
    };

    auto v0 = liter::when_all(v);
    auto v00 = v0[0];
    EXPECT_EQ(v00, 0);
    EXPECT_EQ(1, v0[1]);
}
