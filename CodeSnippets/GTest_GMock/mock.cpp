#include <iostream>
#include "gmock/gmock.h"
#include <gtest/gtest.h>

using namespace testing;


// Let's define a simple interface and a class that implements it.
class Interface {
public:
    virtual ~Interface() {}
    virtual int getValue() const = 0;
    virtual int getOtherValue(int x) = 0;
};

class MyClass : public Interface {
public:
    virtual int getValue() const override { return 42; }
    virtual int getOtherValue(int x) override { return x*10; }
};

// Now let's create a mock class using Google Mock.
class MockClass : public Interface {
public:
	// https://google.github.io/googletest/gmock_cook_book.html#old-style-mock_methodn-macros
    MOCK_CONST_METHOD0(getValue, int());
    MOCK_METHOD1(getOtherValue, int(int x));
};

// Test function that uses the mock.
int testFunction(Interface& obj) {
   return obj.getValue();
}





TEST(MockTest, test_get_value){

 // Create an instance of the mock class.
	MockClass mockObj;

	// Define the expectations using Google Mock.
	EXPECT_CALL(mockObj, getValue())
		.Times(4) // optional. Important for "WillRepeatatly". Without it would be 3
		.WillOnce(::testing::Return(100))
		.WillOnce(::testing::Return(150))
		.WillRepeatedly(::testing::Return(200));

	// Call the function with the mock object.
	int val = testFunction(mockObj);
	EXPECT_EQ(val,100);
	val = testFunction(mockObj);
	EXPECT_EQ(val,150);
	val = testFunction(mockObj);
	EXPECT_EQ(val,200);
	val = testFunction(mockObj);
	EXPECT_EQ(val,200);



};

TEST(MockTest, test_get_other_value){

 // Create an instance of the mock class.
	MockClass mockObj;

	// Define the expectations using Google Mock.
	EXPECT_CALL(mockObj, getOtherValue(Ge(5))).WillOnce(::testing::Return(50));

	// Call the function with the mock object.
	int val = mockObj.getOtherValue(5);

	EXPECT_EQ(
			val,
			50
		);

};


TEST(MockTest, test_get_other_value_between){

 // Create an instance of the mock class.
	MockClass mockObj;

	// Define the expectations using Google Mock.
	EXPECT_CALL(mockObj, getOtherValue(AllOf(Gt(1),Lt(6)))).WillOnce(testing::Return(50));

	// Call the function with the mock object.
	int val = mockObj.getOtherValue(5);

	EXPECT_EQ(
			val,
			50
		);

};

TEST(MockTest, test_sequence){

 // Create an instance of the mock class.
	MockClass mockObj;

	{
	    InSequence seq;

	    EXPECT_CALL(mockObj, getValue()).WillOnce(Return(10));
	    EXPECT_CALL(mockObj, getOtherValue(5)).WillOnce(Return(50));
	    EXPECT_CALL(mockObj, getValue()).WillOnce(Return(50));
	  }

	// Call the function with the mock object.
	int val = mockObj.getValue();
	EXPECT_EQ(val,10);
	val = mockObj.getOtherValue(5);
	EXPECT_EQ(val,50);
	val = mockObj.getValue();
	EXPECT_EQ(val,50);

};




// =================================================

class GPIO {
public:
	int readPin(int pin);
};


class MOCK_GPIO : public GPIO {
public:
	// https://google.github.io/googletest/gmock_cook_book.html#old-style-mock_methodn-macros

	MOCK_METHOD1(readPin, int(int pin));
};

// important function
bool notify_festo_twin(int pinVal){
	if (pinVal == 1)
		// do other stuff
		return true;
	else
		return false;

}


TEST(MockTest, gpioMock){

	MOCK_GPIO gpio;

	EXPECT_CALL(gpio, readPin(5))
		.WillOnce(Return(1))
		.WillOnce(Return(0));


	EXPECT_TRUE(notify_festo_twin(gpio.readPin(5)));
	EXPECT_TRUE(! notify_festo_twin(gpio.readPin(5)));


}
