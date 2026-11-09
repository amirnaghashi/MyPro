/**
 *
 */
class DependencyClass{

private:
	int var = 0;

public:
	void baz(){
		/*
		
		Sehr komplexe Logik, mit weiteren Abhängigkeiten. Ganz krasser shit!		

		*/
		this->var = 1; // eigentlich Werte zwischen 1 und 10 zurück
	}
	int getVar(){
		return this->var;
	}
};



class ExampleClass{

public:	

	int foo(){
		return 5;
	}
	bool baa(){
		return false;
	}

	int doIt(DependencyClass *dC){
		dC->baz();
		return dC->getVar();
	}






};



