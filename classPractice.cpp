# include <iostream>

using namespace std;

class Integer{
	private:
		int number;
	
	public:
		
		Integer() : number(0){

		}
		
		Integer(int num): number(num){

		}

		void displayInteger(){
			cout<<number;
		}

		void changeInteger(int num){
			number = num;
		}

};


int main(){
	
	Integer i;
	
	i.displayInteger();
	i.changeInteger(10);
	i.displayInteger();



	return 0;
}