//UNN
//Litvyakov D. D.
//

#include "table.h"
#include <gtest.h>



class table_fixture : public ::testing::Test {
protected:
    void SetUp() {
        table1 = new UnsortedTable<double>;
        table2 = new SortedTable<double>;
        table3 = new HashTable<double>(10);
        sz = new size_t(100);
        for (int i = (*sz-1); i >= 0; --i) {
            table1->insert(i, double(i * 2));
            table2->insert(i, double(i * 2));
            table3->insert(i, double(i * 2));
        }
    }
    void TearDown() {
        delete table1;
        delete table2;
        delete table3;
        delete sz;
    }
    UnsortedTable<double>* table1;
    SortedTable<double>* table2;
    HashTable<double>* table3;
    size_t* sz;
};


class table_fixture2 : public ::testing::Test {
protected:
    void SetUp() {
        table1 = new UnsortedTable<double>;
        table2 = new SortedTable<double>;
        table3 = new HashTable<double>(100);
        sz = new size_t(10000);
        for (int i = (*sz - 1); i >= 0; --i) {
            table1->insert(i, double(i * 2));
            table2->insert(i, double(i * 2));
            table3->insert(i, double(i * 2));
        }
    }
    void TearDown() {
        delete table1;
        delete table2;
        delete table3;
        delete sz;
    }
    UnsortedTable<double>* table1;
    SortedTable<double>* table2;
    HashTable<double>* table3;
    size_t* sz;
};

//for size = 100
TEST_F(table_fixture, can_get_length)
{
    EXPECT_EQ(*sz, table1->size());
    EXPECT_EQ(*sz, table2->size());
    EXPECT_EQ(*sz, table3->size());
}

TEST_F(table_fixture, check_value) {
    for (size_t i = 0; i < *sz; ++i) {
        EXPECT_EQ(double(table1->find(i).value()), double(i * 2));
        EXPECT_EQ((table2->find(i).value()), double(i*2));
        EXPECT_EQ((*(table3->find(i))).second, double(i * 2));
    }
}

TEST_F(table_fixture, check_erase) {
    for (size_t i = 0; i < *sz; ++i) {
        table1->erase(i);
        table2->erase(i);
        table3->erase(i);
        EXPECT_EQ(table1->size(), *sz - 1 - i);
        EXPECT_EQ(table2->size(), *sz - 1 - i);
        EXPECT_EQ(table3->size(), *sz - 1 - i);
    }
    EXPECT_EQ(table1->empty(), true);
    EXPECT_EQ(table2->empty(), true);
    EXPECT_EQ(table3->empty(), true);
}

TEST_F(table_fixture, check_clear) {
    table1->clear();
    table2->clear();
    table3->clear();
    EXPECT_EQ(table1->empty(), true);
    EXPECT_EQ(table2->empty(), true);
    EXPECT_EQ(table3->empty(), true);
}

TEST_F(table_fixture, can_reinsertion)
{
    for (int i = (*sz - 1); i >= 0; --i) {
        table1->insert(i, double(i * 2));
        table2->insert(i, double(i * 2));
        table3->insert(i, double(i * 2));
    }
    //is the elements inserted twice ?
    EXPECT_EQ(*sz, table1->size());
    EXPECT_EQ(*sz, table2->size());
    EXPECT_EQ(*sz, table3->size());
}

//for size = 10000
TEST_F(table_fixture2, can_get_length)
{
    EXPECT_EQ(*sz, table1->size());
    EXPECT_EQ(*sz, table2->size());
    EXPECT_EQ(*sz, table3->size());
}

TEST_F(table_fixture2, check_value) {
    for (size_t i = 0; i < *sz; ++i) {
        EXPECT_EQ(double(table1->find(i).value()), double(i * 2));
        EXPECT_EQ((table2->find(i).value()), double(i * 2));
        EXPECT_EQ((*(table3->find(i))).second, double(i * 2));
    }
}

TEST_F(table_fixture2, check_erase) {
    for (size_t i = 0; i < *sz; ++i) {
        table1->erase(i);
        table2->erase(i);
        table3->erase(i);
        EXPECT_EQ(table1->size(), *sz - 1 - i);
        EXPECT_EQ(table2->size(), *sz - 1 - i);
        EXPECT_EQ(table3->size(), *sz - 1 - i);
    }
    EXPECT_EQ(table1->empty(), true);
    EXPECT_EQ(table2->empty(), true);
    EXPECT_EQ(table3->empty(), true);
}

TEST_F(table_fixture2, check_clear) {
    table1->clear();
    table2->clear();
    table3->clear();
    EXPECT_EQ(table1->empty(), true);
    EXPECT_EQ(table2->empty(), true);
    EXPECT_EQ(table3->empty(), true);
}

TEST_F(table_fixture2, can_reinsertion)
{
    for (int i = (*sz - 1); i >= 0; --i) {
        table1->insert(i, double(i * 2));
        table2->insert(i, double(i * 2));
        table3->insert(i, double(i * 2));
    }
    //is the elements inserted twice ?
    EXPECT_EQ(*sz, table1->size());
    EXPECT_EQ(*sz, table2->size());
    EXPECT_EQ(*sz, table3->size());
}



