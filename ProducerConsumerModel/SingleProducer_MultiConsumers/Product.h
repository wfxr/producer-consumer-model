#pragma once

#include <cstdlib>

// ²úÆ·
class Product {
public:
    Product() : _id(_next_id++) { }
    static size_t next_id() { return _next_id; }
    size_t get_id() { return _id; }
private:
    size_t _id;
    static size_t _next_id;
};

size_t Product::_next_id = 0;
