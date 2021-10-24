/*
 * 如项目名，这个是一个学习C++的地方，用极其简单的small cases去学习某个feature的用法 
 * 单独作为头文件的feature相对复杂，小的都会放在misc.h文件中。
 * 注释中打引号""的地方，摘抄自我认为可以帮助理解feature核心或设计理念的句子，来源很广
 */

#include "TemplateClass.h"
#include "Type.h"

class book;

class person {
public:
	typedef book mybook;
	void Printname(const book& b);
	mybook IhaveABook(book& b);
private:
	mybook* bookmy = nullptr;
};

class book {
	friend void PrintBookName(book b);
	friend void person::Printname(const book& b);
	friend void PrintBookNameInside(book b) {
		std::cout << b.name << std::endl;
	}

public:
	book(std::string bookName = "N/A", std::string _author = "N/A") : name(bookName), author(_author) { ++numBooks; }
	~book() {};

	static void printNumBooks() {
		std::cout << numBooks << std::endl;
	}
private:
	std::string name;
	std::string author;

	static unsigned short int numBooks;			//Non-const static members are initialized outside the class body
	static constexpr int noLettersOfBook = 50;	//const static members are initialized inside the class body
												//HOWEVER!!!!!!!! Even so, it's of the best interest to define const static members once outside the body.
};

unsigned short int book::numBooks = 0;
constexpr int noLettersOfBook = 50;

class comicBook : book {
public:
	comicBook(std::string _type = "N/A") : book(), type(_type) {};
	void printType() const { std::cout << type << std::endl; }
private:
	std::string type;
};

person::mybook person::IhaveABook(book& b) {
	bookmy = &b;
	return *bookmy;
}

void person::Printname(const book& b) {
	std::cout << b.name << std::endl;
}


void PrintBookName(book b) {
	std::cout << b.name << std::endl;
}

void PrintBookNameInside(book b);

static std::vector<std::string> vec = { "1", "2", "3" ,"4" ,"5","6","7","8","9","0"};

//void submain() {
//	int val = 42; // value we'll look for
//// result will denote the element we want if it's in vec, or vec.cend() if not
//	auto result = std::accumulate(vec.cbegin(), vec.cend(), 0);
//	// report the result
//	std::cout << "The value " << result << std::endl;
//	return;
//}

void LambdaTest() {
	// Wow, this is some advanced thing

	// [capture list] (parameter list) -> return type {function body}
	/*
	[capture list] = [], [name], [&], [=], [&, name], [=, &name]
	parameter list, just like usual function
	return, most times can be inferred from the function body, however, there's mutable type to change variables captured by values. Also, one has to specify the return type if function body has more than a single statement of return, example is donw below
	*/

	//int val = 50;
	//auto l = [&val]()->bool {if (--val > 0) return false; else return true; };
	//while (val > 0)
	//{
	//	std::cout << (l() ? "Yes ": "false ") << val << std::endl;
	//}

	// To print each entry inside vector, look what Ive done!
	for (auto i = vec.begin(); i != vec.end(); ++i) {
		[](const auto& i){std::cout << *i << std::endl; }(i);
	}	
	
}

template <typename T, typename D>
auto add(const T& a, const D& b) {
	return a + b;
}

//Help you learn pointers!
void PointersInANutshell() {

	int a{ 5 }, b = 1;

	int& c = a;			// Reference Type, don't forget it.
	int* stack_b = &a;  // Pointer type, as you've already known.

	int** double_stack_b = &stack_b;

	int* heap_b = new int(500);

	int** ptr_to_ptr_b;
	ptr_to_ptr_b = &heap_b;

	int** double_heap_b = new int*;
	*double_heap_b = heap_b;

	delete heap_b;
	heap_b = NULL;
	delete double_heap_b;
	double_heap_b = NULL;
}

int main()
{
	//book a;
	//book wrong(); //Given parentheses, this statement declaras function wrong() with return type of class book, which apparently isn't an object!
	//PrintBookName(a);

	//a.printNumBooks();
	//book::printNumBooks();
	//LambdaTest();
	//int (*ftp)(int, int) = &add;
	//std::cout << add("Hgg", "Haaa") << std::endl;
	//test();
	TypeofAndDecltype();

	return 0;
}