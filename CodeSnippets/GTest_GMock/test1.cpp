 #include "ExampleClass.cpp"

 #include <gtest/gtest.h>
 #include <iostream>

// == Without Test Class ==

int sqaured(int n) {return n*n;};  // Returns the factorial of n


TEST(SquaredTest, ZeroInput) {
  EXPECT_EQ(sqaured(0), 0);
}



// == Classes  ==


 using namespace std;

 class Test1: public ::testing::Test {
 protected:
 	ExampleClass* ec;
 	ExampleClass* ec2;


 	virtual void SetUp() {
 		this->ec = new ExampleClass();
 		this->ec2 = new ExampleClass();
 	}

 	virtual void TearDown() {
 		delete this->ec;
 	}
 };


// usage of a "fixture" -> the test class
 TEST_F(Test1, foo_test){
 	int value = this->ec->foo();

 	EXPECT_EQ(
 			value,
 			5
 		);


 };

 TEST_F(Test1, baa_test){
 	bool value = this->ec->baa();

 	EXPECT_TRUE(value);
 	EXPECT_EQ(typeid(*ec).name(), typeid(*ec2).name());



 };



