 #include <iostream>
 #include <gtest/gtest.h>


 using namespace std;

 int main(int argc, char** args) {

 	/* Running google unit tests. */
 	::testing::InitGoogleTest(&argc, args);
 	int result = RUN_ALL_TESTS();

 	/* Conclusion */
 	cout << endl << "RUN_ALL_TESTS() returned " << result << "." << endl << endl;

 	if(result == 0){
 		cout << "Testing done. Will exit..." << endl;
 	} else {
 		cout << "Testing failed!. Will exit..." << endl;
 		return EXIT_FAILURE;
 	}

 	return EXIT_SUCCESS;
 }


