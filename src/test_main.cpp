#include <gtest/gtest.h>
#include "VariableDatabase.hpp"

TEST(VariableDatabaseTest, SetAndGetVariousTypes) {
    xsmall_hmi::VariableDatabase db;
    
    db.setVariable("integer", 42);
    db.setVariable("float", 3.14f);
    db.setVariable("string", std::string("hello"));
    db.setVariable("boolean", true);
    
    auto int_val = db.getVariableAs<int>("integer");
    EXPECT_TRUE(int_val.has_value());
    EXPECT_EQ(*int_val, 42);
    
    auto float_val = db.getVariableAs<float>("float");
    EXPECT_TRUE(float_val.has_value());
    EXPECT_FLOAT_EQ(*float_val, 3.14f);
    
    auto str_val = db.getVariableAs<std::string>("string");
    EXPECT_TRUE(str_val.has_value());
    EXPECT_EQ(*str_val, "hello");
    
    auto bool_val = db.getVariableAs<bool>("boolean");
    EXPECT_TRUE(bool_val.has_value());
    EXPECT_TRUE(*bool_val);
}

TEST(VariableDatabaseTest, VariableNotFoundAndUpdate) {
    xsmall_hmi::VariableDatabase db;
    
    auto missing = db.getVariable("missing");
    EXPECT_FALSE(missing.has_value());
    
    db.setVariable("counter", 0);
    EXPECT_TRUE(db.hasVariable("counter"));
    
    db.setVariable("counter", 100);
    auto updated = db.getVariableAs<int>("counter");
    EXPECT_TRUE(updated.has_value());
    EXPECT_EQ(*updated, 100);
}

TEST(VariableDatabaseTest, CallbackSystem) {
    xsmall_hmi::VariableDatabase db;
    
    int callback_count = 0;
    std::string last_variable_changed;
    int last_value = 0;
    
    db.subscribe("temperature", [&](const std::string& name, const auto& value) {
        callback_count++;
        last_variable_changed = name;
        
        if (auto* int_val = std::get_if<int>(&value)) {
            last_value = *int_val;
        }
    });
    
    db.setVariable("temperature", 25);
    EXPECT_EQ(callback_count, 1);
    EXPECT_EQ(last_variable_changed, "temperature");
    EXPECT_EQ(last_value, 25);
    
    db.setVariable("temperature", 30);
    EXPECT_EQ(callback_count, 2);
    EXPECT_EQ(last_value, 30);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "Running VariableDatabase tests..." << std::endl;
    std::cout << "================================" << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    if (result == 0) {
        std::cout << "\n✅ All 3 tests passed!" << std::endl;
    } else {
        std::cout << "\n❌ Some tests failed!" << std::endl;
    }
    
    return result;
}